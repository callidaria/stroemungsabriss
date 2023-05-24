#include "mesh_anim.h"

/*
	TODO
*/
MeshAnimation::MeshAnimation(const char* path,const char* itex_path,uint32_t &mofs)
	: tex_path(itex_path)
{

	// texture generation
	glGenTextures(1,&tex);

#ifdef LIGHT_SELFIMPLEMENTATION_COLLADA_LOAD

	// iterate collada file
	std::vector<std::vector<float>> farrs;
	std::vector<float> weight_count,weight_elements;
	std::ifstream file(path);
	std::string line;
	uint8_t stride = 0;
	uint8_t farri_weights;
	bool stride_increment = false;
	while(std::getline(file,line)) {

		// split lines attribute & accumulate stride value
		std::vector<std::string> raw_data = parameters_from_line(line);
		stride += stride_increment;

		// read node definitions
		if (raw_data[0]=="visual_scene") jroot.children.push_back(rc_assemble_joint_hierarchy(file));

		// check bracket type information for float array & save its data
		else if (raw_data[0]=="float_array") farrs.push_back(extract_array_data(raw_data));

		// check for vertex element list
		else if (raw_data[0][0]=='p'&&raw_data[0][1]=='>') {
			vaddress = extract_array_data(raw_data);

			// switch increment & correct stride value
			stride_increment = false;
			stride--;
		}

		// extract weight linking information
		else if (raw_data[0][0]=='v'&&raw_data[0][1]=='c')
			weight_count = extract_array_data(raw_data);
		else if (raw_data[0][0]=='v'&&raw_data[0][1]=='>')
			weight_elements = extract_array_data(raw_data);

		// check for triangle element input definition start
		else if (raw_data[0]=="triangles") stride_increment = true;

		// saving float array indices
		else if (raw_data[0]=="library_controllers>") farri_weights = farrs.size();
	} file.close();

	// insert float array information into vertex array
	uint32_t eprog = 0;
	std::cout << vaddress.size() << "    " << weight_count.size() << '\n';
	for (uint32_t i=0;i<vaddress.size();i+=stride) {

		// basic geometry information
		verts.push_back(farrs[0][vaddress[i]*3]),verts.push_back(farrs[0][vaddress[i]*3+1]),
			verts.push_back(farrs[0][vaddress[i]*3+2]);
		verts.push_back(farrs[2][vaddress[i+2]*2]),verts.push_back(farrs[2][vaddress[i+2]*2+1]);
		verts.push_back(farrs[1][vaddress[i+1]*3]),verts.push_back(farrs[1][vaddress[i+1]*3+1]),
			verts.push_back(farrs[1][vaddress[i+1]*3+2]);

		// joint weights colouring
		const uint32_t joint_count = weight_count[size];
		uint16_t wjoint[joint_count];
		float jweight[joint_count];
		for (uint16_t i=0;i<joint_count;i++) {

			// write joint index & get weight value from element index, also increment progression
			/*wjoint[i] = weight_elements[eprog*2];
			jweight[i] = farrs[farri_weights+1][weight_elements[eprog*2+1]];*/
			eprog++;
		} std::cout << eprog << '\n';

		// increment vertex size
		size++;
	}

	// vertex size & offset
	ofs = mofs;
	mofs += size;

#else

	// load collada file
	Assimp::Importer importer;
	const aiScene* dae_scene = importer.ReadFile(path,aiProcess_CalcTangentSpace|aiProcess_Triangulate
			|aiProcess_JoinIdenticalVertices|aiProcess_SortByPType);

	// extract animation nodes
	jroot = rc_assemble_joint_hierarchy(dae_scene->mRootNode);

	// assemble vertex array
	aiMesh* cmesh = dae_scene->mMeshes[0];
	for (uint32_t i=0;i<cmesh->mNumVertices;i++) {

		// extract vertex positions
		aiVector3D position = cmesh->mVertices[i];
		verts.push_back(position.x),verts.push_back(position.y),verts.push_back(position.z);

		// extract texture coordinates
		aiVector3D tex_coords = cmesh->mTextureCoords[0][i];
		verts.push_back(tex_coords.x),verts.push_back(tex_coords.y);

		// extract normals
		aiVector3D normals = cmesh->mNormals[i];
		verts.push_back(normals.x),verts.push_back(normals.y),verts.push_back(normals.z);
	}

	// assemble element array
	for (uint32_t i=0;i<cmesh->mNumFaces;i++) {
		aiFace face = cmesh->mFaces[i];
		for (uint32_t j=0;j<face.mNumIndices;j++) elems.push_back(face.mIndices[j]);
	}

	// vertex size & offset
	size = elems.size();
	ofs = mofs;
	mofs += size;

#endif

}

/*
	TODO
*/
void MeshAnimation::texture()
{ Toolbox::load_texture_repeat(tex,tex_path,true); }

/*
	TODO
*/
std::ostream &operator<<(std::ostream &os,const MeshAnimation& obj)
{
	os << "---------------------< MeshAnimation >-----------------------\n";
	os << "vertex array size: " << obj.verts.size() << '\n';
	os << "vertex drawcall count: " << obj.size << "    drawcall offset: " << obj.ofs << '\n';
	os << "joint tree:\n";
	MeshAnimation::rc_print_joint_tree(os,obj.jroot,0);
	return os;
}

/*
	TODO
*/
std::vector<float> MeshAnimation::extract_array_data(std::vector<std::string> raw_data)
{
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
	std::vector<float> out;
	for (uint16_t i=vstart;i<raw_data.size();i++) out.push_back(atof(raw_data[i].c_str()));
	return out;
}

/*
	TODO
*/
std::vector<std::string> MeshAnimation::parameters_from_line(std::string line)
{
	uint8_t lstart = line.find('<')+1;
	std::stringstream pline(line.substr(lstart,line.length()-lstart));
	std::vector<std::string> out;
	std::string rdata;
	while (std::getline(pline,rdata,' ')) out.push_back(rdata);
	return out;
}

#ifdef LIGHT_SELFIMPLEMENTATION_COLLADA_LOAD

/*
	TODO
*/
ColladaJoint MeshAnimation::rc_assemble_joint_hierarchy(std::ifstream &file)
{
	// extract node information from file
	ColladaJoint out;
	std::string line;
	while (std::getline(file,line)) {
		std::vector<std::string> raw_data = parameters_from_line(line);

		// check for information end bracket
		if (raw_data[0]=="/node>"||raw_data[0]=="/visual_scene>") break;

		// get translation matrix
		else if (raw_data[0]=="matrix") {
			std::vector<float> mvalues = extract_array_data(raw_data);
			for (uint8_t y=0;y<4;y++) {
				for (uint8_t x=0;x<4;x++)
					out.trans[x][y] = mvalues[y*4+x];
			}
		}

		// recursive children accumulation
		else if (raw_data[0]=="node") {

			// add child recursively
			uint16_t i = out.children.size();
			out.children.push_back(rc_assemble_joint_hierarchy(file));

			// add node information
			out.children[i].id = raw_data[1].substr(4,raw_data[1].length()-5);
		}
	} return out;
}

#else

/*
	TODO
*/
ColladaJoint MeshAnimation::rc_assemble_joint_hierarchy(aiNode* joint)
{
	// get joint name
	ColladaJoint out;
	out.id = joint->mName.C_Str();

	// translation matrices
	aiMatrix4x4 rt = joint->mTransformation;
	float arr_trans[16] = {
		rt.a1,rt.a2,rt.a3,rt.a4,
		rt.b1,rt.b2,rt.b3,rt.b4,
		rt.c1,rt.c2,rt.c3,rt.c4,
		rt.d1,rt.d2,rt.d3,rt.d4,
	}; out.trans = glm::make_mat4(arr_trans);
	out.inv_trans = glm::inverse(out.trans);

	// recursively process children joints
	for (uint16_t i=0;i<joint->mNumChildren;i++)
		out.children.push_back(rc_assemble_joint_hierarchy(joint->mChildren[i]));

	// output results
	return out;
}

#endif

/*
	TODO
*/
void MeshAnimation::rc_print_joint_tree(std::ostream &os,ColladaJoint cjoint,uint8_t depth)
{
	for (auto joint : cjoint.children) {
		for (uint8_t i=0;i<depth;i++) os << "--";
		os << "> " << joint.id << '\n';
		rc_print_joint_tree(os,joint,depth+1);
	}
}