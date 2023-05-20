#include "mesh_anim.h"

/*
	TODO
*/
MeshAnimation::MeshAnimation(const char* path,uint32_t &mofs)
{
	// iterate collada file
	std::vector<std::vector<float>> farrs;
	std::ifstream file(path);
	std::string line;
	while(std::getline(file,line)) {

		// split lines attribute
		uint8_t lstart = line.find('<')+1;
		std::stringstream pline(line.substr(lstart,line.length()-lstart));
		std::vector<std::string> raw_data;
		std::string rdata;
		while (std::getline(pline,rdata,' ')) raw_data.push_back(rdata);

		// check bracket type information for float array
		if (raw_data[0]=="float_array") {

			// extract & filter first value
			bool nfound = true;
			int8_t vstart = -1;
			size_t findex;
			while (nfound) {
				vstart++;
				findex = raw_data[vstart].find('>');
				nfound = findex==std::string::npos;
			} raw_data[vstart] = raw_data[vstart].substr(findex+1);

			// filter last value
			raw_data.back() = raw_data.back().substr(0,raw_data.back().find('<'));

			// write values to list
			std::vector<float> carr;
			for (uint16_t i=vstart;i<raw_data.size();i++) carr.push_back(atof(raw_data[i].c_str()));
			farrs.push_back(carr);
		}

		// check for vertex element list
		else if (raw_data[0][0]=='p'&&raw_data[0][1]=='>') {

			// filter first & last value, also write to vertex element data
			raw_data[0] = raw_data[0].substr(2);
			raw_data.back() = raw_data.back().substr(0,raw_data.back().find('<'));
			for (auto rdata : raw_data) vaddress.push_back(atof(rdata.c_str()));
		}
	} file.close();

	// insert float array information into vertex array
	for (uint32_t i=0;i<vaddress.size();i+=8) {
		verts.push_back(farrs[0][vaddress[i]]),verts.push_back(farrs[0][vaddress[i+1]]),
			verts.push_back(farrs[0][vaddress[i+2]]);
		verts.push_back(farrs[2][vaddress[i+6]]),verts.push_back(farrs[2][vaddress[i+7]]);
		verts.push_back(farrs[1][vaddress[i+3]]),verts.push_back(farrs[1][vaddress[i+4]]),
			verts.push_back(farrs[1][vaddress[i+5]]);
		size++;
	}

	// vertex size & offset
	ofs = mofs;
	mofs += size;
}