#include "mesh_anim.h"

/*
	constructor(const char*,const char*,uint32_t&)
	purpose: load animation file, holding object mesh and animation key data
	\param path: path to collada (.dae) file, holding mesh and animation key data
	\param itex_path: path to animation mesh texture
	\param mofs: self increasing offset, which saves the buffer offset when draw calling
	TODO: extend documentation
*/
MeshAnimation::MeshAnimation(const char* path,const char* ipcol,const char* ipnorm,const char* ipmat,
			const char* ipemit,uint32_t &mofs)
	: path_colour(ipcol),path_normals(ipnorm),path_materials(ipmat),path_emission(ipemit)
{
	// texture generation
	glGenTextures(1,&t_colour);
	glGenTextures(1,&t_normals);
	glGenTextures(1,&t_material);
	glGenTextures(1,&t_emission);

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
	const aiScene* dae_file = importer.ReadFile(path,aiProcess_CalcTangentSpace|aiProcess_Triangulate
			|aiProcess_JoinIdenticalVertices);
	aiMesh* cmesh = dae_file->mMeshes[0];

	// extract animation nodes
	uint16_t joint_count = 0;
	joints = std::vector<ColladaJoint>(rc_get_joint_count(dae_file->mRootNode),ColladaJoint());
	rc_assemble_joint_hierarchy(dae_file->mRootNode,joint_count);

	// extract bone armature offset
	uint16_t armature_offset = get_joint_id(cmesh->mBones[0]->mName.C_Str());
	bone_offsets = std::vector<glm::mat4>(joint_count,glm::mat4());

	// assemble bone influence weights
	uint8_t veindex[cmesh->mNumVertices] = { 0 };
	float vbindex[cmesh->mNumVertices][BONE_INFLUENCE_STACK_RANGE] = { 0 };
	float vweight[cmesh->mNumVertices][BONE_INFLUENCE_STACK_RANGE] = { 0 };
	for (uint16_t i=0;i<cmesh->mNumBones;i++) {
		aiBone* cbone = cmesh->mBones[i];
		bone_offsets[i+armature_offset] = glmify(cbone->mOffsetMatrix);

		// map bone weights onto vertices
		for (uint32_t j=0;j<cbone->mNumWeights;j++) {
			aiVertexWeight cweight = cbone->mWeights[j];

			// store indices & weights until overflow
			if (veindex[cweight.mVertexId]<BONE_INFLUENCE_STACK_RANGE) {
				uint8_t cindex = veindex[cweight.mVertexId]++;
				vbindex[cweight.mVertexId][cindex] = i+armature_offset;
				vweight[cweight.mVertexId][cindex] = cweight.mWeight;

			// handle weight overflow
			} else {
				uint8_t cwidx = 0;
				float cwproc = vweight[cweight.mVertexId][0];
				for (uint8_t ci=1;ci<4;ci++) {
					if (cwproc>vweight[cweight.mVertexId][ci]) {
						cwproc = vweight[cweight.mVertexId][ci];
						cwidx = ci;
					}
				} if (cweight.mWeight>vweight[cweight.mVertexId][cwidx])
					vweight[cweight.mVertexId][cwidx] = cweight.mWeight;
			}
		}

	// assemble vertex array
	} for (uint32_t i=0;i<cmesh->mNumVertices;i++) {

		// extract vertex positions
		aiVector3D position = cmesh->mVertices[i];
		verts.push_back(position.x),verts.push_back(position.y),verts.push_back(position.z);

		// extract texture coordinates
		aiVector3D tex_coords = cmesh->mTextureCoords[0][i];
		verts.push_back(tex_coords.x),verts.push_back(tex_coords.y);

		// extract normals
		aiVector3D normals = cmesh->mNormals[i];
		verts.push_back(normals.x),verts.push_back(normals.y),verts.push_back(normals.z);

		// tangent & bitangent
		aiVector3D tangent = cmesh->mTangents[i],bitangent = cmesh->mBitangents[i];
		verts.push_back(tangent.x),verts.push_back(tangent.y),verts.push_back(tangent.z);
		verts.push_back(bitangent.x),verts.push_back(bitangent.y),verts.push_back(bitangent.z);

		// correct weight array after simplification
		glm::vec4 vrip_weight = glm::vec4(vweight[i][0],vweight[i][1],vweight[i][2],vweight[i][3]);
		glm::normalize(vrip_weight);

		// insert influencing bone indices & relating weights
		verts.push_back(vbindex[i][0]),verts.push_back(vbindex[i][1]),
				verts.push_back(vbindex[i][2]),verts.push_back(vbindex[i][3]);
		verts.push_back(vrip_weight.x),verts.push_back(vrip_weight.y),
				verts.push_back(vrip_weight.z),verts.push_back(vrip_weight.w);

	// assemble element array
	} for (uint32_t i=0;i<cmesh->mNumFaces;i++) {
		aiFace face = cmesh->mFaces[i];
		for (uint32_t j=0;j<face.mNumIndices;j++) elems.push_back(face.mIndices[j]);

	// extract animations
	} for (uint32_t i=0;i<dae_file->mNumAnimations;i++) {
		aiAnimation* canim = dae_file->mAnimations[i];
		ColladaAnimationData proc;
		proc.duration = canim->mDuration/canim->mTicksPerSecond;

		// process all related bone keys
		for (uint32_t j=0;j<dae_file->mAnimations[i]->mNumChannels;j++) {
			aiNodeAnim* cnanim = canim->mChannels[j];
			JointKeys cjkey;

			// correlate bone string id with object from joint tree
			bool found = false;
			cjkey.joint_id = get_joint_id(cnanim->mNodeName.C_Str());

			// add key information
			for (uint32_t k=0;k<cnanim->mNumPositionKeys;k++) {
				cjkey.dur_positions.push_back(cnanim->mPositionKeys[k].mTime/canim->mTicksPerSecond);
				cjkey.key_positions.push_back(glmify(cnanim->mPositionKeys[k].mValue));
			} for (uint32_t k=0;k<cnanim->mNumScalingKeys;k++) {
				cjkey.dur_scales.push_back(cnanim->mScalingKeys[k].mTime/canim->mTicksPerSecond);
				cjkey.key_scales.push_back(glmify(cnanim->mScalingKeys[k].mValue));
			} for (uint32_t k=0;k<cnanim->mNumRotationKeys;k++) {
				cjkey.dur_rotations.push_back(cnanim->mRotationKeys[k].mTime/canim->mTicksPerSecond);
				cjkey.key_rotations.push_back(glmify(cnanim->mRotationKeys[k].mValue));
			}

			// add joint to animation & add animation to list
			proc.joints.push_back(cjkey);
		}

		// initialize keyframe counter lists and store animation data
		proc.crr_position = std::vector<uint16_t>(proc.joints.size(),0);
		proc.crr_scale = std::vector<uint16_t>(proc.joints.size(),0);
		proc.crr_rotation = std::vector<uint16_t>(proc.joints.size(),0);
		anims.push_back(proc);
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
{
	Toolbox::load_texture_repeat(t_colour,path_colour,true);
	Toolbox::load_texture_repeat(t_normals,path_normals);
	Toolbox::load_texture_repeat(t_material,path_materials);
	Toolbox::load_texture_repeat(t_emission,path_emission);
}

/*
	upload_interpolation(Shader*) -> void !O(n)
	purpose: upload joint interpolation data to animation shader
	\param shader: animation shader to upload interpolation data to
*/
void MeshAnimation::upload_interpolation(Shader* shader)
{ 
	for (uint16_t i=0;i<joints.size();i++)
		shader->upload_matrix(joints[i].uniform_location.c_str(),joints[i].btrans);
}

/*
	interpolate(float) -> void !O(2n) => O(n)
	purpose: calculate current bone transformations, interpolate between keys and recurse influence
	\param dt: time passed since last interpolation
*/
void MeshAnimation::interpolate(double dt)
{
	// interpolation delta
	avx += dt;
	avx = fmod(avx,anims[current_anim].duration);

	// iterate all joints for local transformations
	ColladaAnimationData* anim = &anims[current_anim];
	for (uint16_t i=0;i<anim->joints.size();i++) {
		auto joint = anim->joints[i];

		// determine transformation keyframes
		float pprog = advance_animation(anim->crr_position[i],joint.dur_positions);
		float rprog = advance_animation(anim->crr_rotation[i],joint.dur_rotations);
		float sprog = advance_animation(anim->crr_scale[i],joint.dur_scales);

		// smooth interpolation between keyframes
		glm::vec3 tip = glm::mix(joint.key_positions[anim->crr_position[i]],
				joint.key_positions[anim->crr_position[i]+1],pprog);
		glm::quat rip = glm::slerp(joint.key_rotations[anim->crr_rotation[i]],
				joint.key_rotations[anim->crr_rotation[i]+1],rprog);
		glm::vec3 sip = glm::mix(joint.key_scales[anim->crr_scale[i]],
				joint.key_scales[anim->crr_scale[i]+1],sprog);

		// combine translation matrices
		joints[joint.joint_id].trans = glm::translate(glm::mat4(1),tip)*glm::toMat4(rip)
				* glm::scale(glm::mat4(1),sip);
	}

	// kickstart transformation matrix recursion
	uint16_t tree_id = 0;
	rc_transform_interpolation(&joints[0],glm::mat4(1),tree_id);
}

/*
	operator<<(std::ostream&,MeshAnimation& const) -> std::ostream& (friend)
	purpose: make animation object information console printable
	conforming to: operator definition
*/
std::ostream &operator<<(std::ostream &os,const MeshAnimation& obj)
{
	os << "---------------------< MeshAnimation >-----------------------\n";
	os << "vertex array size: " << obj.verts.size() << '\n';
	os << "vertex drawcall count: " << obj.size << "    drawcall offset: " << obj.ofs << '\n';
	os << obj.joints.size() << " joints found -> joint tree:\n";
	MeshAnimation::rc_print_joint_tree(os,obj.joints,0,0);
	return os;
}

#ifdef LIGHT_SELFIMPLEMENTATION_COLLADA_LOAD

// experimental method
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

// experimental method
std::vector<std::string> MeshAnimation::parameters_from_line(std::string line)
{
	uint8_t lstart = line.find('<')+1;
	std::stringstream pline(line.substr(lstart,line.length()-lstart));
	std::vector<std::string> out;
	std::string rdata;
	while (std::getline(pline,rdata,' ')) out.push_back(rdata);
	return out;
}

// experimental method
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
	rc_get_joint_count(aiNode*) -> uint16_t (private,static,recursive) !O(n)
	purpose: used by constructor to analyse how much memory has to be allocated by joint list
	\param joint: root node in assimp node format, to start depthsearch count from
	\returns total number of nodes in joint tree
*/
uint16_t MeshAnimation::rc_get_joint_count(aiNode* joint)
{
	uint16_t out = 1;
	for (uint16_t i=0;i<joint->mNumChildren;i++) out += rc_get_joint_count(joint->mChildren[i]);
	return out;
}

/*
	rc_assemble_joint_hierarchy(aiNode*,uint16_t&) -> void (private,recursive) !O(n)
	purpose: used by constructor to extract joint information from tree
	\param joint: root node in assimp node format, to start processing extraction from
	\joint_count: current progression of id count to identify joint's place in memory by
*/
void MeshAnimation::rc_assemble_joint_hierarchy(aiNode* joint,uint16_t &joint_count)
{
	// get joint name & convert initial transformation matrix
	ColladaJoint out;
	out.id = joint->mName.C_Str();
	out.trans = glmify(joint->mTransformation);

	// save joints place in memory and increase
	uint16_t memory_id = joint_count;
	out.uniform_location = "joint_transform["+std::to_string(memory_id)+"]";
	joint_count++;

	// recursively process children joints & output results
	for (uint16_t i=0;i<joint->mNumChildren;i++) {
		out.children.push_back(joint_count);
		rc_assemble_joint_hierarchy(joint->mChildren[i],joint_count);
	} joints[memory_id] = out;
}

#endif

/*
	rc_transform_interpolation(ColladaJoint*,mat4,uint16t&) -> void (private,recursive) !O(n)
	purpose: recursion through joint tree, calculating transforms with chained parent influence
	\param cjoint: rootnode of joint tree
	\param gtrans: previous transformation influence until this joint within the chain
	\param id: current joint index
*/
void MeshAnimation::rc_transform_interpolation(ColladaJoint* cjoint,glm::mat4 gtrans,uint16_t &id)
{
	glm::mat4 lgtrans = gtrans*cjoint->trans;
	cjoint->btrans = lgtrans*bone_offsets[id++];
	for (auto child : cjoint->children) rc_transform_interpolation(&joints[child],lgtrans,id);
}

/*
	get_joint_id(string) -> uint16_t ~O(n)
	purpose: used to link joints, referred to by name, to their actual objects
	\param jname: name of joint, which has to be found
	\returns memory index of equinamed joint
*/
uint16_t MeshAnimation::get_joint_id(std::string jname)
{
	uint16_t i = 0;
	while (jname!=joints[i].id) i++;
	return i;
}

/*
	advance_animation(uint16_t&,vector<double>) -> float (private) !O(1)
	purpose: update animation key index based on animation time advancement
	\param crr_index: current key index, to be advanced by this method
	\param keys: key timing list for advancement comparison & interpolation
	\returns 0 <= n <= 1, where n describes the interpolation mixing between current & next key
*/
float MeshAnimation::advance_animation(uint16_t &crr_index,std::vector<double> keys)
{
	while (keys[crr_index+1]<avx) crr_index++;
	crr_index *= crr_index<keys.size()&&keys[crr_index]<avx;
	return (avx-keys[crr_index])/(keys[crr_index+1]-keys[crr_index]);
}

/*
	rc_print_joint_tree(ostream&,vector<ColladaJoint>,uint16_t,uint8_t)
			-> void (private,static recursive) !O(n)
	purpose: paint joint tree to console output
	\param os: ostream to print output to
	\param joints: list of joints
	\param jid: current id of node to print depthsearch output from
	\param depth: current depth of recursion
*/
void MeshAnimation::rc_print_joint_tree(std::ostream &os,std::vector<ColladaJoint> joints,
		uint16_t jid,uint8_t depth)
{
	for (auto joint : joints[jid].children) {
		for (uint8_t i=0;i<depth;i++) os << "--";
		os << "> " << joints[joint].id << '\n';
		rc_print_joint_tree(os,joints,joint,++depth);
	}
}