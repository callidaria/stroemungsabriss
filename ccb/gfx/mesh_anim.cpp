#include "mesh_anim.h"

/*
	constructor(const char*,const char*,const char*,const char*,const char*,uint32_t&)
			elements:!O(n)
	purpose: load animation file, holding object mesh and animation key data
	\param path: path to collada (.dae) file, holding mesh and animation key data
	\param ipcol: path to animation mesh texture
	\param ipnorm: path to animation mesh normal map
	\param ipmat: path to animation mesh material map
	\param ipemit: path to animation mesh emission texture
	\param vl: vertex array to save loaded vertex data to
	\param el: element array to save loaded elements to
	\param mofs: self increasing offset, which saves the buffer offset for later draw calls
*/
MeshAnimation::MeshAnimation(const char* path,const char* ipcol,const char* ipnorm,const char* ipmat,
			const char* ipemit,std::vector<float> &vl,std::vector<uint32_t> &el,uint32_t &mofs)
	: path_colour(ipcol),path_normals(ipnorm),path_materials(ipmat),path_emission(ipemit),ofs(mofs)
{
	// texture generation
	glGenTextures(1,&t_colour),glGenTextures(1,&t_normals),
			glGenTextures(1,&t_material),glGenTextures(1,&t_emission);
	// TODO: test differences between this and glGenTextures(4,[]);

	// load collada file
	Assimp::Importer importer;
	const aiScene* dae_file = importer.ReadFile(path,aiProcess_CalcTangentSpace
			|aiProcess_Triangulate|aiProcess_JoinIdenticalVertices);
	aiMesh* cmesh = dae_file->mMeshes[0];

	// extract animation nodes
	uint16_t joint_count = 0;
	joints = std::vector<ColladaJoint>(rc_get_joint_count(dae_file->mRootNode),ColladaJoint());
	rc_assemble_joint_hierarchy(dae_file->mRootNode,joint_count);

	// extract bone armature offset
	uint16_t armature_offset = get_joint_id(cmesh->mBones[0]->mName.C_Str());
	bone_offsets = std::vector<glm::mat4>(joint_count,glm::mat4());

	// calculate element array offset before pushing to vertex list
	uint32_t eoffset = vl.size()/ANIM_VERTEX_REPEAT;

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
	}

	// assemble vertex array
	size_t t_vls = vl.size();
	vl.resize(t_vls+cmesh->mNumVertices*ANIM_VERTEX_REPEAT);
	for (uint32_t i=0;i<cmesh->mNumVertices;i++) {
		uint32_t r = t_vls+i*ANIM_VERTEX_REPEAT;

		// extract vertex positions
		aiVector3D position = cmesh->mVertices[i];
		vl[r+ANIM_POSITION_X] = position.x;
		vl[r+ANIM_POSITION_Y] = position.y;
		vl[r+ANIM_POSITION_Z] = position.z;

		// extract texture coordinates
		aiVector3D tex_coords = cmesh->mTextureCoords[0][i];
		vl[r+ANIM_TCOORD_X] = tex_coords.x;
		vl[r+ANIM_TCOORD_Y] = tex_coords.y;

		// extract normals
		aiVector3D normals = cmesh->mNormals[i];
		vl[r+ANIM_NORMALS_X] = normals.x;
		vl[r+ANIM_NORMALS_Y] = normals.y;
		vl[r+ANIM_NORMALS_Z] = normals.z;

		// tangent for normal mapping
		aiVector3D tangent = cmesh->mTangents[i];
		vl[r+ANIM_TANGENT_X] = tangent.x;
		vl[r+ANIM_TANGENT_Y] = tangent.y;
		vl[r+ANIM_TANGENT_Z] = tangent.z;

		// correct weight array after simplification
		glm::vec4 vrip_weight = glm::vec4(vweight[i][0],vweight[i][1],vweight[i][2],vweight[i][3]);
		glm::normalize(vrip_weight);

		// insert influencing bone indices & relating weights
		vl[r+ANIM_BONE0] = vbindex[i][0], vl[r+ANIM_BONE1] = vbindex[i][1],
				vl[r+ANIM_BONE2] = vbindex[i][2], vl[r+ANIM_BONE3] = vbindex[i][3];
		vl[r+ANIM_WEIGHT0] = vrip_weight.x, vl[r+ANIM_WEIGHT1] = vrip_weight.y,
				vl[r+ANIM_WEIGHT2] = vrip_weight.z, vl[r+ANIM_WEIGHT3] = vrip_weight.w;
	}

	// allocate memory for element array
	size_t mem_size = 0;
	for (uint32_t i=0;i<cmesh->mNumFaces;i++) {
		for (uint32_t j=0;j<cmesh->mFaces[i].mNumIndices;j++) mem_size++;
	}
	size_t mem_index = el.size();
	el.resize(mem_index+mem_size);

	// assemble element array
	for (uint32_t i=0;i<cmesh->mNumFaces;i++) {
		aiFace face = cmesh->mFaces[i];
		for (uint32_t j=0;j<face.mNumIndices;j++) el[mem_index++] = eoffset+face.mIndices[j];
		size += face.mNumIndices;
	}

	// extract animations
	size_t t_size_anims = anims.size();
	anims.resize(t_size_anims+dae_file->mNumAnimations);
	for (uint32_t i=0;i<dae_file->mNumAnimations;i++) {
		aiAnimation* canim = dae_file->mAnimations[i];

		// memory allocation for new animation
		anims[t_size_anims+i] = {
			.duration = canim->mDuration/canim->mTicksPerSecond,
			.joints = std::vector<Joint>(dae_file->mAnimations[i]->mNumChannels)
		};

		// process all related bone keys
		for (uint32_t j=0;j<dae_file->mAnimations[i]->mNumChannels;j++) {
			aiNodeAnim* cnanim = canim->mChannels[j];
			Joint& cjkey = anims[t_size_anims+i].joints[j];

			// correlate bone string id & allocate memory
			cjkey = {
				.joint_id = get_joint_id(cnanim->mNodeName.C_Str()),
				.position_keys = std::vector<AnimationPositionKey>(cnanim->mNumPositionKeys),
				.scale_keys = std::vector<AnimationScaleKey>(cnanim->mNumScalingKeys),
				.rotation_keys = std::vector<AnimationRotationKey>(cnanim->mNumRotationKeys)
			};

			// add position key informations
			for (uint32_t k=0;k<cnanim->mNumPositionKeys;k++) {
				cjkey.position_keys[k].position = glmify(cnanim->mPositionKeys[k].mValue);
				cjkey.position_keys[k].duration = cnanim->mPositionKeys[k].mTime/canim->mTicksPerSecond;
			}

			// add scaling key informations
			for (uint32_t k=0;k<cnanim->mNumScalingKeys;k++) {
				cjkey.scale_keys[k].scale = glmify(cnanim->mScalingKeys[k].mValue);
				cjkey.scale_keys[k].duration = cnanim->mScalingKeys[k].mTime/canim->mTicksPerSecond;
			}

			// add rotation key informations
			for (uint32_t k=0;k<cnanim->mNumRotationKeys;k++) {
				cjkey.rotation_keys[k].rotation = glmify(cnanim->mRotationKeys[k].mValue);
				cjkey.rotation_keys[k].duration = cnanim->mRotationKeys[k].mTime/canim->mTicksPerSecond;
			}
		}
	}

	// vertex offset
	mofs += size;
}

/*
	texture() -> void !O(1)
	purpose: load textures for animated meshes
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
	interpolate(float) -> void !O(2n) => !O(n)
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
		// advance position keys
		while (joint.position_keys[joint.crr_position+1].duration<avx) joint.crr_position++;
		joint.crr_position *= joint.crr_position<joint.position_keys.size()
				&& joint.position_keys[joint.crr_position].duration<avx;
		float pprog = (avx-joint.position_keys[joint.crr_position].duration)
				/ (joint.position_keys[joint.crr_position+1].duration
				- joint.position_keys[joint.crr_position].duration);

		// advance scaling keys
		while (joint.scale_keys[joint.crr_scale+1].duration<avx) joint.crr_scale++;
		joint.crr_scale *= joint.crr_scale<joint.scale_keys.size()
				&& joint.scale_keys[joint.crr_scale].duration<avx;
		float sprog = (avx-joint.scale_keys[joint.crr_scale].duration)
				/ (joint.scale_keys[joint.crr_scale+1].duration
				- joint.scale_keys[joint.crr_scale].duration);

		// advance rotation keys
		while (joint.rotation_keys[joint.crr_rotation+1].duration<avx) joint.crr_rotation++;
		joint.crr_rotation *= joint.crr_rotation<joint.rotation_keys.size()
				&& joint.rotation_keys[joint.crr_rotation].duration<avx;
		float rprog = (avx-joint.rotation_keys[joint.crr_rotation].duration)
				/ (joint.rotation_keys[joint.crr_rotation+1].duration
				- joint.rotation_keys[joint.crr_rotation].duration);
		// TODO: code repitition, this can be improved i believe

		// smooth interpolation between keyframes
		glm::vec3 tip = glm::mix(joint.position_keys[joint.crr_position].position,
				joint.position_keys[joint.crr_position+1].position,pprog);
		glm::vec3 sip = glm::mix(joint.scale_keys[joint.crr_scale].scale,
				joint.scale_keys[joint.crr_scale+1].scale,sprog);
		glm::quat rip = glm::slerp(joint.rotation_keys[joint.crr_rotation].rotation,
				joint.rotation_keys[joint.crr_rotation+1].rotation,rprog);

		// combine translation matrices
		joints[joint.joint_id].trans = glm::translate(glm::mat4(1),tip)*glm::toMat4(rip)
				* glm::scale(glm::mat4(1),sip);
	}

	// kickstart transformation matrix recursion
	uint16_t tree_id = 0;
	rc_transform_interpolation(&joints[0],glm::mat4(1),tree_id);
}

/*
	operator<<(std::ostream&,MeshAnimation& const) -> std::ostream& (friend) joints:!O(n)
	purpose: make animation object information console printable
	conforming to: operator definition
*/
std::ostream &operator<<(std::ostream &os,const MeshAnimation& obj)
{
	os << "---------------------< MeshAnimation >-----------------------\n";
	os << "faces: " << obj.size << '\n';
	os << "vertex drawcall count: " << obj.size << "    drawcall offset: " << obj.ofs << '\n';
	os << obj.joints.size() << " joints found -> joint tree:\n";
	MeshAnimation::rc_print_joint_tree(os,obj.joints,0,0);
	return os;
}

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
	// save joints place in memory and increase
	uint16_t memory_id = joint_count++;

	// get joint name & convert initial transformation matrix
	joints[memory_id] = {
		.id = joint->mName.C_Str(),
		.uniform_location = "joint_transform["+std::to_string(memory_id)+"]",
		.trans = glmify(joint->mTransformation),
		.children = std::vector<uint16_t>(joint->mNumChildren)
	};

	// recursively process children joints
	for (uint16_t i=0;i<joint->mNumChildren;i++) {
		joints[memory_id].children[i] = joint_count;
		rc_assemble_joint_hierarchy(joint->mChildren[i],joint_count);
	}
}

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
