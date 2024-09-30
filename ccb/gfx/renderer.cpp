#include "renderer.h"


/**
 *	TODO
*/

/*
	TODO
*/
float advance_keys(std::vector<double>& durations,uint16_t& crr,double progress)
{
	while (durations[crr+1]<progress) crr++;
	crr *= crr<durations.size()&&durations[crr]<progress;
	return (progress-durations[crr])/(durations[crr+1]-durations[crr]);
}

void AnimatedMesh::update_animation()
{
	// interpolation delta
	progress += g_Frame.time_delta;
	progress = fmod(progress,animations[current_animation].duration);

	// iterate joints for local animation transformations
	MeshAnimation& p_Animation = animations[current_animation];
	for (MeshJoint& p_Joint : p_Animation.joints)
	{
		// determine transformation keyframes
		float __TransformProgress = advance_keys(p_Joint.position_durations,p_Joint.crr_position,progress);
		float __ScalingProgress = advance_keys(p_Joint.scaling_durations,p_Joint.crr_scale,progress);
		float __RotationProgress = advance_keys(p_Joint.rotation_durations,p_Joint.crr_rotation,progress);

		// interpolation between keyframes
		glm::vec3 __TransformInterpolation = glm::mix(
				p_Joint.position_keys[p_Joint.crr_position],
				p_Joint.position_keys[p_Joint.crr_position+1],
				__TransformProgress
			);
		glm::vec3 __ScalingInterpolation = glm::mix(
				p_Joint.scaling_keys[p_Joint.crr_scale],
				p_Joint.scaling_keys[p_Joint.crr_scale+1],
				__ScalingProgress
			);
		glm::quat __RotationInterpolation = glm::slerp(
				p_Joint.rotation_keys[p_Joint.crr_rotation],
				p_Joint.rotation_keys[p_Joint.crr_rotation+1],
				__RotationProgress
			);

		// translation
		joints[p_Joint.id].transform
				= glm::translate(glm::mat4(1.f),__TransformInterpolation)
				* glm::scale(glm::mat4(1.f),__ScalingInterpolation)
				* glm::toMat4(__RotationInterpolation);
	}

	// calculate transform after parent influence
	rc_transform_interpolation(joints[0],glm::mat4(1.f));
	// FIXME: it's unclear if joints[0] is always root node
}

void AnimatedMesh::rc_transform_interpolation(AnimationJoint& joint,glm::mat4 const& parent_transform)
{
	glm::mat4 __LocalTransform = parent_transform*joint.transform;
	joint.recursive_transform = __LocalTransform*joint.offset;
	for (uint16_t child : joint.children) rc_transform_interpolation(joints[child],__LocalTransform);
}


/**
 *	TODO
*/

/*
	TODO
*/
void Lighting::upload(ShaderPipeline& pipeline,uint8_t& dir_offset,uint8_t& point_offset,uint8_t& spot_offset)
{
	// upload the many suns
	offset_directional = dir_offset;
	for (LightDirectional& light : directional_lights)
	{
		std::string __ArrayLocation = "sunlight["+std::to_string(dir_offset)+"].";
		pipeline.upload_vec3((__ArrayLocation+"position").c_str(),light.position);
		pipeline.upload_vec3((__ArrayLocation+"colour").c_str(),light.colour);
		pipeline.upload_float((__ArrayLocation+"intensity").c_str(),light.intensity);
		dir_offset++;
	}

	// upload pointlights
	offset_point = point_offset;
	for (LightPoint& light : point_lights)
	{
		std::string __ArrayLocation = "pointlight["+std::to_string(point_offset)+"].";
		pipeline.upload_vec3((__ArrayLocation+"position").c_str(),light.position);
		pipeline.upload_vec3((__ArrayLocation+"colour").c_str(),light.colour);
		pipeline.upload_float((__ArrayLocation+"instensity").c_str(),light.intensity);
		pipeline.upload_float((__ArrayLocation+"constant").c_str(),light.c0);
		pipeline.upload_float((__ArrayLocation+"linear").c_str(),light.c1);
		pipeline.upload_float((__ArrayLocation+"quadratic").c_str(),light.c2);
		point_offset++;
	}

	// upload spotlights
	offset_spot = spot_offset;
	for (LightSpot& light : spot_lights)
	{
		std::string __ArrayLocation = "spotlight["+std::to_string(spot_offset)+"].";
		pipeline.upload_vec3((__ArrayLocation+"position").c_str(),light.position);
		pipeline.upload_vec3((__ArrayLocation+"colour").c_str(),light.colour);
		pipeline.upload_vec3((__ArrayLocation+"direction").c_str(),light.direction);
		pipeline.upload_float((__ArrayLocation+"intensity").c_str(),light.intensity);
		pipeline.upload_float((__ArrayLocation+"cut_in").c_str(),light.cut_inner);
		pipeline.upload_float((__ArrayLocation+"cut_out").c_str(),light.cut_outer);
		spot_offset++;
	}
}


/**
 *	TODO
*/

/*
	!O(1)m /+load -> (public)
	// TODO: add purpose
	\param path: path to file containing spritesheet
	\param rows (default=1): rows on spritesheet
	\param cols (default=1): columns on spritesheet
	\param frames (default=1): number of frames held by spritesheet
	\returns: memory index the spritesheet can be referenced by later
*/
uint16_t RenderBatch::add_sprite(std::string path,uint8_t rows,uint8_t cols,uint8_t frames)
{
	SpriteTextureTuple __Tuple = {
		.texture = Texture(path),
		.rows = rows,
		.columns = cols,
		.frames = frames
	};
	sprite_textures.push_back(__Tuple);
	return sprite_textures.size()-1;
}

/*
	!O(1)m /+load -> (public)
	\param tex_id: id of spritesheet texture to link
	\param pos: origin position of added sprite
	\param wdt: width of added sprite
	\param hgt: height of added sprite
	TODO extend param description
*/
void RenderBatch::register_sprite(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt)
{
	Sprite __Sprite = {
		.texture_id = tex_id,
		.transform = {
			.position = pos,
			.width = wdt,
			.height = hgt,
		},
	};
	__Sprite.transform.to_origin();
	sprites.push_back(__Sprite);
}

/*
	TODO
*/
void RenderBatch::register_sprite(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,uint8_t dur)
{
	// add animation data
	SpriteAnimation __Animation = {
		.id = (uint16_t)sprites.size(),
		.cycle_duration = dur,
		.frame_duration = (float)dur/sprite_textures[tex_id].frames,
	};

	// register sprite & animation
	sprite_animations.push_back(__Animation);
	register_sprite(tex_id,pos,wdt,hgt);
}

/*
	TODO
*/
void RenderBatch::register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt)
{
	SpriteInstance __Instance = {
		.texture_id = tex_id,
		.transform = {
			.position = pos,
			.width = wdt,
			.height = hgt,
		},
	};
	__Instance.transform.to_origin();
	duplicates.push_back(__Instance);
}

/*
	TODO
*/
void RenderBatch::register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,uint8_t dur)
{
	// add animation data
	SpriteAnimationInstance __Animation = {
		.id = (uint16_t)duplicates.size(),
		.cycle_duration = dur,
		.frame_duration = (float)dur/sprite_textures[tex_id].frames,
	};

	// register duplicate & animation
	duplicate_animations.push_back(__Animation);
	register_duplicates(tex_id,pos,wdt,hgt);
}

/*
	TODO
*/
void RenderBatch::add_mesh(std::string obj,std::string tex,std::string norm,std::string mats,std::string emit,
		glm::vec3 pos,float scl,glm::vec3 rot)
{
	// store mesh vertex data
	Mesh __Mesh = {
		.transform = {
			.position = pos,
			.scaling = scl,
			.rotation = rot
		},
		.path = obj
	};
	__Mesh.transform.to_origin();
	meshes.push_back(__Mesh);

	// store texture information
	MeshTextureTuple __Texture = {
		.colours = Texture(tex,true),
		.normals = Texture(norm),
		.materials = Texture(mats),
		.emission = Texture(emit),
	};
	mesh_textures.push_back(__Texture);
}

/*
	TODO
*/
void RenderBatch::add_animation(std::string dae,std::string tex,
		std::string norm,std::string mats,std::string emit,glm::vec3 pos,float scl,glm::vec3 rot)
{
	// store mesh animation vertex data
	AnimatedMesh __Animation = {
		.mesh = {
			.transform = {
				.position = pos,
				.scaling = scl,
				.rotation = rot
			},
			.path = dae
		}
	};
	__Animation.mesh.transform.to_origin();
	animated_meshes.push_back(__Animation);

	// store texture information
	MeshTextureTuple __Texture = {
		.colours = Texture(tex,true),
		.normals = Texture(norm),
		.materials = Texture(mats),
		.emission = Texture(emit),
	};
	animation_textures.push_back(__Texture);
}

/*
	TODO
*/
void RenderBatch::spawn_sprite_instance(uint16_t inst_id,glm::vec2 ofs,glm::vec2 scl,float rot,glm::vec2 subtex)
{
	SpriteInstance& p_Instance = duplicates[inst_id];
	SpriteInstanceUpload& p_Upload = p_Instance.upload[p_Instance.active_range];

	// setup transform
	p_Upload.offset = ofs;
	p_Upload.scale = scl;
	p_Upload.rotation = rot;
	p_Upload.atlas_index = subtex;

	// increase spawn counter
	p_Instance.active_range++;
}

/*
	TODO
*/
void RenderBatch::load_mesh(Mesh& mesh)
{
	// create storage for file contents
	std::vector<glm::vec3> __Positions;
	std::vector<glm::vec2> __UVCoordinates;
	std::vector<glm::vec3> __Normals;
	std::vector<uint32_t> __PositionIndices,__UVIndices,__NormalIndices;

	// open source file
	FILE* __OBJFile = fopen(mesh.path.c_str(),"r");
	if (__OBJFile==NULL)
	{
		COMM_ERR("object loader -> file %s could not be found!",mesh.path.c_str());
		return;
	}

	// iterate file contents
	char __Command[128];
	while (fscanf(__OBJFile,"%s",__Command)!=EOF)
	{
		// process value prefix
		// position prefix
		if (!strcmp(__Command,"v"))
		{
			glm::vec3 __Position;
			uint32_t _eof = fscanf(__OBJFile,"%f %f %f\n",&__Position.x,&__Position.y,&__Position.z);
			__Positions.push_back(__Position);
		}

		// uv coordinate prefix
		else if (!strcmp(__Command,"vt"))
		{
			glm::vec2 __UVCoordinate;
			uint32_t _eof = fscanf(__OBJFile,"%f %f\n",&__UVCoordinate.x,&__UVCoordinate.y);
			__UVCoordinates.push_back(__UVCoordinate);
		}

		// normal prefix
		else if (!strcmp(__Command,"vn"))
		{
			glm::vec3 __Normal;
			uint32_t _eof = fscanf(__OBJFile,"%f %f %f\n",&__Normal.x,&__Normal.y,&__Normal.z);
			__Normals.push_back(__Normal);
		}

		// face prefix
		else if (!strcmp(__Command,"f"))
		{
			// extract face indices
			uint32_t __PositionIndex[3],__UVIndex[3],__NormalIndex[3];
			uint32_t _eof = fscanf(
					__OBJFile,"%u/%u/%u %u/%u/%u %u/%u/%u\n",
					&__PositionIndex[0],&__UVIndex[0],&__NormalIndex[0],
					&__PositionIndex[1],&__UVIndex[1],&__NormalIndex[1],
					&__PositionIndex[2],&__UVIndex[2],&__NormalIndex[2]
				);

			// insert face indices
			for (uint8_t i=0;i<3;i++) __PositionIndices.push_back(__PositionIndex[i]);
			for (uint8_t i=0;i<3;i++) __UVIndices.push_back(__UVIndex[i]);
			for (uint8_t i=0;i<3;i++) __NormalIndices.push_back(__NormalIndex[i]);
		}
	}
	fclose(__OBJFile);

	// set offset and allocate memory
	mesh.vertex_offset = mesh_vertices.size();
	mesh.vertex_range = __PositionIndices.size();
	mesh_vertices.reserve(mesh.vertex_offset+mesh.vertex_range);

	// iterate faces & write vertex
	for (uint32_t i=0;i<__PositionIndices.size();i+=3)
	{
		for (uint8_t j=0;j<3;j++)
		{
			uint32_t n = i+j;
			MeshUpload __VertexComponent = {
				.position = __Positions[__PositionIndices[n]-1],
				.uv_coord = __UVCoordinates[__UVIndices[n]-1],
				.normal = __Normals[__NormalIndices[n]-1]
			};
			mesh_vertices.push_back(__VertexComponent);
		}

		// precalculate tangent for normal mapping
		// setup values
		uint32_t n = mesh.vertex_offset+i;
		glm::vec3 __EdgeDelta0 = mesh_vertices[n+1].position-mesh_vertices[n].position;
		glm::vec3 __EdgeDelta1 = mesh_vertices[n+2].position-mesh_vertices[n].position;
		glm::vec2 __UVDelta0 = mesh_vertices[n+1].uv_coord-mesh_vertices[n].uv_coord;
		glm::vec2 __UVDelta1 = mesh_vertices[n+2].uv_coord-mesh_vertices[n].uv_coord;
		float __Factor = 1.f/(__UVDelta0.x*__UVDelta1.y-__UVDelta0.y*__UVDelta1.x);

		// calculate tangent & store in vertex
		glm::mat2x3 __CombinedEdges = glm::mat2x3(__EdgeDelta0,__EdgeDelta1);
		glm::vec2 __CombinedUVs = glm::vec2(__UVDelta1.y,-__UVDelta0.y);
		glm::vec3 __Tangent = __Factor*(__CombinedEdges*__CombinedUVs);
		__Tangent = glm::normalize(__Tangent);
		for (uint8_t j=0;j<3;j++) mesh_vertices[n+j].tangent = __Tangent;
	}
}
// FIXME: it was written [-NAS] a long time ago, there are some things to optimize here

/*
	TODO
*/
uint16_t rc_get_joint_count(aiNode* root)
{
	uint16_t __Result = 1;
	for (uint16_t i=0;i<root->mNumChildren;i++) __Result += rc_get_joint_count(root->mChildren[i]);
	return __Result;
}

void rc_assemble_joint_hierarchy(std::vector<AnimationJoint>& joints,aiNode* root)
{
	// translate root joint
	uint16_t __MemoryID = joints.size();
	AnimationJoint __Joint = {
		.id = root->mName.C_Str(),
		.uniform_location = "joint_transform["+std::to_string(__MemoryID)+"]",
		.transform = Toolbox::assimp_to_mat4(root->mTransformation),
		.children = std::vector<uint16_t>(root->mNumChildren)
	};
	joints.push_back(__Joint);

	// recursively process children joints
	for (uint16_t i=0;i<root->mNumChildren;i++)
	{
		joints[__MemoryID].children[i] = joints.size();
		rc_assemble_joint_hierarchy(joints,root->mChildren[i]);
	}
}

uint16_t get_joint_id(std::vector<AnimationJoint>& joints,std::string id)
{
	uint16_t i = 0;
	while (id!=joints[i].id) i++;
	return i;
}

void RenderBatch::load_animation(AnimatedMesh& mesh)
{
	// load collada file
	Assimp::Importer __Importer;
	aiScene const* __DAEFile = __Importer.ReadFile(mesh.mesh.path.c_str(),
			aiProcess_CalcTangentSpace|aiProcess_Triangulate|aiProcess_JoinIdenticalVertices);

	// extract joints
	uint16_t __JointCount = rc_get_joint_count(__DAEFile->mRootNode);
	mesh.joints.reserve(__JointCount);
	rc_assemble_joint_hierarchy(mesh.joints,__DAEFile->mRootNode);

	// load mesh
	// extract bone armature offset
	aiMesh* __Mesh = __DAEFile->mMeshes[0];
	uint16_t __RootOffset = get_joint_id(mesh.joints,__Mesh->mBones[0]->mName.C_Str());
	// TODO: allow for all the meshes in the file to be added one by one. not only the first one!

	// extract bone influence weights
	uint8_t __WriteCount[__Mesh->mNumVertices] = { 0 };
	float __BoneIndices[__Mesh->mNumVertices][ANIMATION_INFLUENCE_STACK_RANGE] = { 0 };
	float __Weights[__Mesh->mNumVertices][ANIMATION_INFLUENCE_STACK_RANGE] = { 0 };
	// TODO: join these into one data structure
	for (uint16_t i=0;i<__Mesh->mNumBones;i++)
	{
		aiBone* __Bone = __Mesh->mBones[i];
		uint16_t __JointIndex = __RootOffset+i;
		mesh.joints[__JointIndex].offset = Toolbox::assimp_to_mat4(__Bone->mOffsetMatrix);
		// FIXME: questionable placement of offset extraction

		// map bone weights onto vertices
		for (uint32_t j=0;j<__Bone->mNumWeights;j++)
		{
			aiVertexWeight& __Weight = __Bone->mWeights[j];

			// store indices & weights until overflow
			if (__WriteCount[__Weight.mVertexId]<ANIMATION_INFLUENCE_STACK_RANGE)
			{
				uint8_t k = __WriteCount[__Weight.mVertexId]++;
				__BoneIndices[__Weight.mVertexId][k] = __JointIndex;
				__Weights[__Weight.mVertexId][k] = __Weight.mWeight;
			}

			// priority store in case of weight overflow
			else
			{
				// iterate to find least influential weight
				uint8_t __ProcIndex = 0;
				float __ProcWeight = __Weights[__Weight.mVertexId][0];  // TODO: remove
				for (uint8_t k=1;k<ANIMATION_INFLUENCE_STACK_RANGE;k++)
				{
					if (__ProcWeight>__Weights[__Weight.mVertexId][k])
					{
						__ProcIndex = k;
						__ProcWeight = __Weights[__Weight.mVertexId][k];
					}
				}

				// overwrite most insignificant weight if current weight is important enought
				if (__Weight.mWeight>__Weights[__Weight.mVertexId][__ProcIndex])
					__Weights[__Weight.mVertexId][__ProcIndex] = __Weight.mWeight;
			}
		}
	}

	// compose vertex data
	// assemble vertex array
	mesh.mesh.vertex_offset = animation_vertices.size();
	animation_vertices.reserve(mesh.mesh.vertex_offset+__Mesh->mNumVertices);
	for (uint32_t i=0;i<__Mesh->mNumVertices;i++)
	{
		AnimationUpload __Vertex = {
			.position = Toolbox::assimp_to_vec3(__Mesh->mVertices[i]),
			.uv_coord = Toolbox::assimp_to_vec2(__Mesh->mTextureCoords[0][i]),
			.normal = Toolbox::assimp_to_vec3(__Mesh->mNormals[i]),
			.tangent = Toolbox::assimp_to_vec3(__Mesh->mTangents[i]),
			.bone_index
					= glm::vec4(__BoneIndices[i][0],__BoneIndices[i][1],__BoneIndices[i][2],__BoneIndices[i][3]),
			.bone_weight = glm::vec4(__Weights[i][0],__Weights[i][1],__Weights[i][2],__Weights[i][3])
		};
		animation_vertices.push_back(__Vertex);
	}

	// allocate memory for element array
	mesh.mesh.vertex_range = 0;
	for (uint32_t i=0;i<__Mesh->mNumFaces;i++) mesh.mesh.vertex_range += __Mesh->mFaces[i].mNumIndices;
	animation_elements.reserve(animation_elements.size()+mesh.mesh.vertex_range);

	// assemble element array
	for (uint32_t i=0;i<__Mesh->mNumFaces;i++)
	{
		for (uint32_t j=0;j<__Mesh->mFaces[i].mNumIndices;j++)
			animation_elements.push_back(mesh.mesh.vertex_offset+__Mesh->mFaces[i].mIndices[j]);
	}

	// extract animations
	// allocate memory & iterate animations
	size_t __AnimationOffset = mesh.animations.size();
	mesh.animations.reserve(__AnimationOffset+__DAEFile->mNumAnimations);
	for (uint32_t i=0;i<__DAEFile->mNumAnimations;i++)
	{
		aiAnimation* __Animation = __DAEFile->mAnimations[i];

		// birth of a new animation
		MeshAnimation __MeshAnimation = {
			.joints = std::vector<MeshJoint>(__Animation->mNumChannels),
			.duration = __Animation->mDuration/__Animation->mTicksPerSecond
		};
		mesh.animations.push_back(__MeshAnimation);

		// process animation channels
		for (uint32_t j=0;j<__Animation->mNumChannels;j++)
		{
			aiNodeAnim* __Node = __Animation->mChannels[j];
			MeshJoint& __Joint = mesh.animations.back().joints[j];

			// process channel keys for related joint
			__Joint = {
				.id = get_joint_id(mesh.joints,__Node->mNodeName.C_Str()),
				.position_keys = std::vector<glm::vec3>(__Node->mNumPositionKeys),
				.scaling_keys = std::vector<glm::vec3>(__Node->mNumScalingKeys),
				.rotation_keys = std::vector<glm::quat>(__Node->mNumRotationKeys),
				.position_durations = std::vector<double>(__Node->mNumPositionKeys),
				.scaling_durations = std::vector<double>(__Node->mNumScalingKeys),
				.rotation_durations = std::vector<double>(__Node->mNumRotationKeys)
			};

			// extract position keys
			for (uint32_t k=0;k<__Node->mNumPositionKeys;k++)
			{
				__Joint.position_keys[k] = Toolbox::assimp_to_vec3(__Node->mPositionKeys[k].mValue);
				__Joint.position_durations[k] = __Node->mPositionKeys[k].mTime/__Animation->mTicksPerSecond;
			}

			// extract scaling keys
			for (uint32_t k=0;k<__Node->mNumScalingKeys;k++)
			{
				__Joint.scaling_keys[k] = Toolbox::assimp_to_vec3(__Node->mScalingKeys[k].mValue);
				__Joint.scaling_durations[k] = __Node->mScalingKeys[k].mTime/__Animation->mTicksPerSecond;
			}

			// extract rotation keys
			for (uint32_t k=0;k<__Node->mNumRotationKeys;k++)
			{
				__Joint.rotation_keys[k] = Toolbox::assimp_to_quat(__Node->mRotationKeys[k].mValue);
				__Joint.rotation_durations[k] = __Node->mRotationKeys[k].mTime/__Animation->mTicksPerSecond;
			}
		}
	}
}

/*
	TODO
*/
void RenderBatch::update_sprites()
{
	for (SpriteAnimation& p_Animation : sprite_animations)
	{
		Sprite& p_Sprite = sprites[p_Animation.id];
		SpriteTextureTuple& p_Texture = sprite_textures[p_Sprite.texture_id];

		// calculate current frame
		bool __IncAnim = p_Animation.anim_progression<p_Animation.cycle_duration;
		p_Animation.anim_progression += __IncAnim*g_Frame.time_delta-p_Animation.anim_progression*!__IncAnim;

		// calculate spritesheet location
		int __Index = p_Animation.anim_progression/p_Animation.frame_duration;
		p_Sprite.atlas_index = glm::vec2(__Index%p_Texture.columns,__Index/p_Texture.columns);
	}
}

/*
	TODO
*/
void RenderBatch::update_duplicates()
{
	for (SpriteAnimationInstance& p_Animation : duplicate_animations)
	{
		SpriteInstance& p_Instance = duplicates[p_Animation.id];
		SpriteTextureTuple& p_Texture = sprite_textures[p_Instance.texture_id];

		// calculate frames for all active instances
		for (uint16_t i=0;i<p_Instance.active_range;i++)
		{
			bool __IncAnim = p_Animation.anim_progressions[i]<p_Animation.cycle_duration;
			p_Animation.anim_progressions[i] += __IncAnim*g_Frame.time_delta
					- p_Animation.anim_progressions[i]*!__IncAnim;
			int __Index = p_Animation.anim_progressions[i]/p_Animation.frame_duration;
			p_Instance.upload[i].atlas_index = glm::vec2(__Index%p_Texture.columns,__Index/p_Texture.columns);
		}
		// FIXME: code repitition
		// FIXME: a lot of division for update code
	}
}


/**
 *	TODO: expand
*/

/*
	TODO
*/
Renderer::Renderer()
{
	COMM_MSG(LOG_HEADINGS,"renderer setup...");
	COMM_LOG("compile shaders");

	// vertex shaders
	Shader __SpriteVertexShader = Shader("./shader/obj/sprite.vs",GL_VERTEX_SHADER);
	Shader __DuplicateVertexShader = Shader("./shader/obj/duplicate.vs",GL_VERTEX_SHADER);
	Shader __MeshVertexShader = Shader("./shader/obj/mesh.vs",GL_VERTEX_SHADER);
	Shader __AnimationVertexShader = Shader("./shader/obj/animation.vs",GL_VERTEX_SHADER);
	Shader __FramebufferVertexShader = Shader("./shader/standard/framebuffer.vs",GL_VERTEX_SHADER);

	// fragment shaders
	Shader __DirectFragmentShader = Shader("./shader/standard/direct.fs",GL_FRAGMENT_SHADER);
	Shader __MeshFragmentShader = Shader("./shader/obj/mesh.fs",GL_FRAGMENT_SHADER);
	Shader __DeferredFragmentShader = Shader("./shader/lighting/pbr.fs",GL_FRAGMENT_SHADER);

	COMM_LOG("assemble shader pipelines");
	m_SpritePipeline.assemble(__SpriteVertexShader,__DirectFragmentShader);
	m_DuplicatePipeline.assemble(__DuplicateVertexShader,__DirectFragmentShader);
	for (uint8_t i=0;i<RENDERER_BATCHES_COUNT;i++)
	{
		// mesh
		batches[i].mesh_pipeline.assemble(__MeshVertexShader,__MeshFragmentShader);
		batches[i].mesh_pipeline.enable();
		batches[i].mesh_pipeline.upload_int("colour_map",0);
		batches[i].mesh_pipeline.upload_int("normal_map",1);
		batches[i].mesh_pipeline.upload_int("material_map",2);
		batches[i].mesh_pipeline.upload_int("emission_map",3);

		// animation
		batches[i].animation_buffer.add_buffer();
		batches[i].animation_pipeline.assemble(__AnimationVertexShader,__MeshFragmentShader);
		batches[i].animation_pipeline.enable();
		batches[i].animation_pipeline.upload_int("colour_map",0);
		batches[i].animation_pipeline.upload_int("normal_map",1);
		batches[i].animation_pipeline.upload_int("material_map",2);
		batches[i].animation_pipeline.upload_int("emission_map",3);
	}
	m_DeferredPipeline.assemble(__FramebufferVertexShader,__DeferredFragmentShader);

	// geometry
	COMM_LOG("pre-loading basic sprite geometry");
	float __SpriteVertices[] = {
		-.5f,.5f,.0f,.0f, .5f,-.5f,1.f,1.f, .5f,.5f,1.f,.0f,
		.5f,-.5f,1.f,1.f, -.5f,.5f,.0f,.0f, -.5f,-.5f,.0f,1.f
	};
	m_SpriteBuffer.bind();
	m_SpriteBuffer.upload_vertices(__SpriteVertices,PATTERN_SPRITE_TRIANGLE_REPEAT*sizeof(float));
	m_SpritePipeline.enable();
	m_SpritePipeline.point_buffer2D();
	m_SpritePipeline.upload_int("tex",0);
	m_SpritePipeline.upload_camera();

	COMM_LOG("setup sprite duplication");
	m_SpriteBuffer.add_buffer();
	m_DuplicatePipeline.enable();
	m_DuplicatePipeline.point_buffer2D();
	m_SpriteBuffer.bind_index();
	m_DuplicatePipeline.def_indexF("offset",2,0,SPRITE_INSTANCE_UPLOAD_REPEAT);
	m_DuplicatePipeline.def_indexF("scale",2,2,SPRITE_INSTANCE_UPLOAD_REPEAT);
	m_DuplicatePipeline.def_indexF("rotation",1,4,SPRITE_INSTANCE_UPLOAD_REPEAT);
	m_DuplicatePipeline.def_indexF("i_tex",2,5,SPRITE_INSTANCE_UPLOAD_REPEAT);
	m_DuplicatePipeline.upload_int("tex",0);
	m_DuplicatePipeline.upload_camera();

	// screen space post processing
	COMM_LOG("setup material buffer and generate its components");
	m_GBuffer.bind();
	m_GBuffer.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight);
	m_GBuffer.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight,true,1);
	m_GBuffer.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight,true,2);
	m_GBuffer.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight,true,3);
	m_GBuffer.add_depth_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight);
	m_GBuffer.combine_attachments();
	FrameBuffer::unbind();

	COMM_LOG("pre-loading basic canvas geometry");
	float __CanvasVertices[] = {
		-1.f,1.f,.0f,1.f, 1.f,-1.f,1.f,.0f, 1.f,1.f,1.f,1.f,
		1.f,-1.f,1.f,.0f, -1.f,1.f,.0f,1.f, -1.f,-1.f,.0f,.0f
	};
	m_CanvasBuffer.bind();
	m_CanvasBuffer.upload_vertices(__CanvasVertices,PATTERN_SPRITE_TRIANGLE_REPEAT*sizeof(float));

	COMM_LOG("setup shader pipeline for deferred lighting system");
	m_DeferredPipeline.enable();
	m_DeferredPipeline.point_buffer2D();
	m_DeferredPipeline.upload_int("gbuffer_colour",0);
	m_DeferredPipeline.upload_int("gbuffer_position",1);
	m_DeferredPipeline.upload_int("gbuffer_normals",2);
	m_DeferredPipeline.upload_int("gbuffer_materials",3);
	m_DeferredPipeline.upload_int("shadow_map",4);

	COMM_LOG("setup shadow projection");
	m_ShadowFrameBuffer.bind();
	m_ShadowFrameBuffer.add_depth_component(RENDERER_SHADOW_RESOLUTION,RENDERER_SHADOW_RESOLUTION);
	Texture::set_texture_parameter_clamp_to_border();
	Texture::set_texture_border_colour(glm::vec4(1));
	FrameBuffer::unbind();
	// TODO: setup geometry-only render pipeline for shadow projection

	COMM_SCC("renderer ready");
}


/**
 *			Load Interpretation: Loader Language Definition
 *
 *	TODO describe formatting
 *
 *	request a texture from memory:
 *		-> texture string(texture_path)|
 *		-> ( int(number_of_rows) int(number_of_columns) int(number_of_subtextures))+e|
 *
 *	register sprite to draw:
 *		-> sprite int(texture_id) float(pos_x) float(pos_y) float(width) float(height)|
 *		-> ( int(animation_duration_in_frames))+e|
 *
 *	register instance groups:
 *		-> duplicate int(texture_id) float(pos_x) float(pos_y) float(width) float(height)|
 *		-> ( int(animation_duration_in_frames))+e|
 *
 *	register meshes:
 *		-> mesh string(object_path) string(texture_path)|
 *		-> string(normal_map) string(material_map) string(emission_map)|
 *		-> float(pos_x) float(pos_y) float(pos_z)( float(scale) float(rot_x) float(rot_y) float(rot_z))+e|
 *
 *	TODO register instances mesh groups
 *
 *	TODO register animations
 *
 *	TODO emit particles
 *
 *	TODO register free structures
 *
 *	spawn instances:
 *		-> spawn (sprite int(index) float(ofs_x) float(ofs_y)|
 *		-> ( float(scale_x) float(scale_y) float(rotation) int(atlas_index_x) int(atlas_index_y))+e)|
 *		-> +(mesh int(index) float(ofs_x) float(ofs_y) float(ofs_z)|
 *		-> ( float(rotation_x) float(rotation_y) float(rotation_z))+e)|
*/
// TODO: remove naming bloat, command can be reduced to single character format

void interpreter_logic_texture(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,TexPath,Rows,Columns,Frames };

	COMM_LOG_COND(
			args.size()>Args::TexPath,
			"request %s: %s",(args.size()>Args::Frames) ? "spritesheet" : "texture",args[Args::TexPath].c_str()
		)
		COMM_ERR_FALLBACK("texture request: not enough arguments provided");

	// check for texture atlas information
	uint8_t __Rows = 1,__Cols = 1,__Frames = 1;
	if (args.size()>Args::Frames)
	{
		__Rows = stoi(args[Args::Rows]);
		__Cols = stoi(args[Args::Columns]);
		__Frames = stoi(args[Args::Frames]);
	}

	// write texture
	batch->add_sprite(args[Args::TexPath],__Rows,__Cols,__Frames);
}

void interpreter_logic_sprite(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,TexID,PosX,PosY,Width,Height,AnimDuration };

	COMM_LOG_COND(
			args.size()>Args::Height,
			"register %s of texture %s: pos -> (%s,%s), dim -> %sx%s",
			(args.size()>Args::AnimDuration) ? "animation" : "sprite",
			args[Args::TexID].c_str(),
			args[Args::PosX].c_str(),args[Args::PosY].c_str(),
			args[Args::Width].c_str(),args[Args::Height].c_str()
		)
		COMM_ERR_FALLBACK("sprite registration: not enough arguments provided");

	// arguments to variables
	uint16_t __TextureID = stoi(args[Args::TexID]);
	glm::vec2 __Position = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
	float __Width = stof(args[Args::Width]),__Height = stof(args[Args::Height]);
	// TODO: error messaging when conversion fails

	// register sprite or animation based on argument length
	if (args.size()>Args::AnimDuration)
	{
		uint8_t __Duration = stoi(args[Args::AnimDuration]);
		batch->register_sprite(__TextureID,__Position,__Width,__Height,__Duration);
		return;
	}
	batch->register_sprite(__TextureID,__Position,__Width,__Height);
}

void interpreter_logic_instanced_sprite(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,TexID,PosX,PosY,Width,Height,AnimDuration };

	COMM_LOG_COND(
			args.size()>Args::Height,
			"register instanced sprite%s of texture %s: pos -> (%s,%s), dim -> %sx%s",
			(args.size()>AnimDuration) ? " animation" : "",
			args[Args::TexID].c_str(),
			args[Args::PosX].c_str(),args[Args::PosY].c_str(),
			args[Args::Width].c_str(),args[Args::Height].c_str()
		)
		COMM_ERR_FALLBACK("duplicate registration: not enough arguments provided");

	// arguments to variables
	uint16_t __TextureID = stoi(args[Args::TexID]);
	glm::vec2 __Position = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
	float __Width = stof(args[Args::Width]),__Height = stof(args[Args::Height]);

	// register animated sprite instance based on argument length
	if (args.size()>Args::AnimDuration)
	{
		uint8_t __Duration = stoi(args[Args::AnimDuration]);
		batch->register_duplicates(__TextureID,__Position,__Width,__Height,__Duration);
		return;
	}
	batch->register_duplicates(__TextureID,__Position,__Width,__Height);
}
// FIXME: code duplications

void interpreter_logic_mesh(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,ObjPath,TexPath,NormPath,MatPath,EmitPath,PosX,PosY,PosZ,Scl,RotX,RotY,RotZ };

	COMM_LOG_COND(
			args.size()>Args::PosZ,
			"register mesh %s, textured with %s: pos -> (%s,%s,%s)",
			args[Args::ObjPath].c_str(),args[Args::TexPath].c_str(),
			args[Args::PosX].c_str(),args[Args::PosY].c_str(),args[Args::PosZ].c_str()
		)
		COMM_ERR_FALLBACK("mesh registration: not enough arguments provided");

	// arguments to variables
	glm::vec3 __Position = glm::vec3(stof(args[Args::PosX]),stof(args[Args::PosY]),stof(args[Args::PosZ]));
	float __Scale = 1;
	glm::vec3 __Rotation = glm::vec3(0);

	// check if scale and rotation is specified before adding mesh
	if (args.size()>Args::RotZ)
	{
		__Scale = stof(args[Args::Scl]);
		__Rotation = glm::vec3(stof(args[Args::RotX]),stof(args[Args::RotY]),stof(args[Args::RotZ]));
	}
	batch->add_mesh(
			args[Args::ObjPath],args[Args::TexPath],
			args[Args::NormPath],args[Args::MatPath],args[Args::EmitPath],
			__Position,__Scale,__Rotation
		);
}

void interpreter_logic_animation(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,DaePath,TexPath,NormPath,MatPath,EmitPath,PosX,PosY,PosZ,Scl,RotX,RotY,RotZ };

	COMM_LOG_COND(
			args.size()>Args::PosZ,
			"register mesh animation %s, textured with %s: pos -> (%s,%s,%s)",
			args[Args::DaePath].c_str(),args[Args::TexPath].c_str(),
			args[Args::PosX].c_str(),args[Args::PosY].c_str(),args[Args::PosZ].c_str()
		)
		COMM_ERR_FALLBACK("mesh animation registration: not enough arguments provided");

	// arguments to variables
	glm::vec3 __Position = glm::vec3(stof(args[Args::PosX]),stof(args[Args::PosY]),stof(args[Args::PosZ]));
	float __Scale = 1;
	glm::vec3 __Rotation = glm::vec3(0);

	// check if scale and rotation is specified before adding mesh
	if (args.size()>Args::RotZ)
	{
		__Scale = stof(args[Args::Scl]);
		__Rotation = glm::vec3(stof(args[Args::RotX]),stof(args[Args::RotY]),stof(args[Args::RotZ]));
	}
	batch->add_animation(
			args[Args::DaePath],args[Args::TexPath],
			args[Args::NormPath],args[Args::MatPath],args[Args::EmitPath],
			__Position,__Scale,__Rotation
		);
}

void interpreter_logic_spawn_instanced(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,Type,InstID,PosX,PosY,SclX,SclY,Rotation,SubtexCol,SubtexRow };

	COMM_LOG_COND(
			args.size()>Args::PosY,
			"attempting to spawn %s instance of %s: pos -> (%s,%s)",
			args[Args::Type].c_str(),args[Args::InstID].c_str(),args[Args::PosX].c_str(),args[Args::PosY].c_str()
		)
		COMM_ERR_FALLBACK("spawning instances: not enough arguments provided");

	// process sprite instance spawn request
	if (args[Args::Type]=="sprite")
	{
		// setup attributes
		uint16_t __InstanceID = stoi(args[Args::InstID]);
		glm::vec2 __Offset = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
		glm::vec2 __Scale = glm::vec2(1.f);
		float __Rotation = .0f;
		glm::vec2 __SubTexture = glm::vec2(0);

		// optionally extract rotation and spritesheet index & spawn
		if (args.size()>Args::SubtexRow)
		{
			__Scale = glm::vec2(stof(args[Args::SclX]),stof(args[Args::SclY]));
			__Rotation = stof(args[Args::Rotation]);
			__SubTexture = glm::vec2(stoi(args[Args::SubtexCol]),stoi(args[Args::SubtexRow]));
		}
		batch->spawn_sprite_instance(__InstanceID,__Offset,__Scale,__Rotation,__SubTexture);
	}

	// process mesh instance spawn request
	else if (args[Args::Type]=="mesh")
	{
		// TODO
	}

	COMM_ERR_FALLBACK("cannot spawn instance of %s",args[Args::Type].c_str());
}

void interpreter_logic_light(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t
	{
		Command,Type,PosX,PosY,PosZ,Intensity,ColR,ColG,ColB,
		Coeff0,Coeff1,Coeff2,  // alternate: represent direction for spotlight
		Inner,Outer
	};

	COMM_LOG_COND(
			args.size()>Args::PosZ,
			"emitting %slight from position -> (%s,%s,%s)",
			args[Args::Type].c_str(),args[Args::PosX].c_str(),args[Args::PosY].c_str(),args[Args::PosZ].c_str()
		)
		COMM_ERR_FALLBACK("emitting lights: not enough arguments provided");

	// extracting basic parameters
	glm::vec3 __Position = glm::vec3(stof(args[Args::PosX]),stof(args[Args::PosY]),stof(args[Args::PosZ]));
	float __Intensity = stof(args[Args::Intensity]);
	glm::vec3 __Colour = glm::vec3(1.f);
	if (args.size()>Args::ColB)
		glm::vec3 __Colour = glm::vec3(stof(args[Args::ColR]),stof(args[Args::ColG]),stof(args[Args::ColB]));

	// process sunlight emission request
	if (args[Args::Type]=="sun")
	{
		LightDirectional __Result = {
			.position = __Position,
			.colour = __Colour,
			.intensity = __Intensity
		};
		batch->lighting.directional_lights.push_back(__Result);
		return;
	}

	// process pointlight emission request
	if (args[Args::Type]=="point")
	{
		LightPoint __Result = {
			.position = __Position,
			.colour = __Colour,
			.intensity = __Intensity,
			.c0 = stof(args[Args::Coeff0]),
			.c1 = stof(args[Args::Coeff1]),
			.c2 = stof(args[Args::Coeff2])
		};
		batch->lighting.point_lights.push_back(__Result);
		return;
	}

	if (args[Args::Type]=="spot")
	{
		LightSpot __Result = {
			.position = __Position,
			.colour = __Colour,
			.direction = glm::vec3(stof(args[Args::Coeff0]),stof(args[Args::Coeff1]),stof(args[Args::Coeff2])),
			.intensity = __Intensity,
			.cut_inner = stof(args[Args::Inner]),
			.cut_outer = stof(args[Args::Outer])
		};
		batch->lighting.spot_lights.push_back(__Result);
		return;
	}

	COMM_ERR("cannot emit specified source of %slight, type not known",args[Args::Type].c_str());
}

void interpreter_logic_shadow(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,PosX,PosY,PosZ };

	COMM_LOG_COND(
			args.size()>PosZ,
			"projecting shadow from position -> (%s,%s,%s)",
			args[Args::PosX].c_str(),args[Args::PosY].c_str(),args[Args::PosZ].c_str()
		)
		COMM_ERR_FALLBACK("shadow projection: not enough arguments provided");

	g_Renderer.set_shadow(glm::vec3(stof(args[Args::PosX]),stof(args[Args::PosY]),stof(args[Args::PosZ])));
}

void interpreter_logic_syntax_error(RenderBatch* batch,const std::vector<std::string>& args)
{
	COMM_ERR("syntax error while interpreting %s: \"%s\" not a valid command",
			batch->path.c_str(),args[0].c_str());
}

constexpr uint8_t RENDERER_INTERPRETER_COMMAND_COUNT = 8;
const std::string gfx_command_correlation[RENDERER_INTERPRETER_COMMAND_COUNT] = {
	"texture","sprite","duplicate","mesh","animation","spawn","light","shadow"
};
typedef void (*gfx_interpreter_logic)(RenderBatch*,const std::vector<std::string>&);
gfx_interpreter_logic cmd_handler[RENDERER_INTERPRETER_COMMAND_COUNT+1] = {
	interpreter_logic_texture,
	interpreter_logic_sprite,
	interpreter_logic_instanced_sprite,
	interpreter_logic_mesh,
	interpreter_logic_animation,
	interpreter_logic_spawn_instanced,
	interpreter_logic_light,
	interpreter_logic_shadow,
	interpreter_logic_syntax_error
};


/*
	TODO
*/
void bgr_load_batch(RenderBatch* batch)
{
	COMM_LOG("renderer: reading load definition file \"%s\"",batch->path.c_str());

	// open file
	COMM_MSG(LOG_BLUE,"-> interpretation start");
	std::ifstream __CCBFile(batch->path,std::ios::in);
	std::string __CommandLine;
	while (getline(__CCBFile,__CommandLine))
	{
		// split command and its arguments, interrupt when empty command
		std::vector<std::string> __Args = Toolbox::split_string(__CommandLine,' ');
		if (!__Args.size()) continue;

		// correlate command
		uint8_t i = 0;
		while (i<RENDERER_INTERPRETER_COMMAND_COUNT&&__Args[0]!=gfx_command_correlation[i]) i++;

		// call command
		cmd_handler[i](batch,__Args);
	}
	// FIXME: reading FILE* by fscan pattern could be a lot more useful in this case

	// close file and ready batch load
	__CCBFile.close();
	COMM_MSG(LOG_BLUE,"-> interpretation end");

	// load geometry
	COMM_AWT("loading geometry of %li meshes",batch->meshes.size());
	for (Mesh& m : batch->meshes) batch->load_mesh(m);
	for (AnimatedMesh& a : batch->animated_meshes) batch->load_animation(a);
	COMM_CNF();

	// load sprite textures
	COMM_AWT("sprites: streaming %li textures",batch->sprite_textures.size());
	for (SpriteTextureTuple& __Texture : batch->sprite_textures) __Texture.texture.load();
	COMM_CNF();

	// load mesh textures
	COMM_AWT("meshes: streaming %li textures",batch->mesh_textures.size()*4);
	for (MeshTextureTuple& __Texture : batch->mesh_textures)
	{
		__Texture.colours.load();
		__Texture.normals.load();
		__Texture.materials.load();
		__Texture.emission.load();
	}
	COMM_CNF();

	COMM_AWT("animations: streaming %li textures",batch->animation_textures.size()*4);
	for (MeshTextureTuple& __Texture : batch->animation_textures)
	{
		__Texture.colours.load();
		__Texture.normals.load();
		__Texture.materials.load();
		__Texture.emission.load();
	}
	COMM_CNF();

	g_Renderer.gpu_update_pointers.push_back(batch);
}

RenderBatch* Renderer::load(const std::string& path)
{
	// find available batch by unix approach: last batch will be overwritten if no idle
	uint8_t __BatchID = 0;
	while (__BatchID<(RENDERER_BATCHES_COUNT-1)&&batches[__BatchID].selected) __BatchID++;

	// store path in free batch & signal batch load
	RenderBatch* r_Batch = &batches[__BatchID];
	r_Batch->path = path;

	// communicate selection
	COMM_LOG("batch %i selected for writing",__BatchID);
	COMM_ERR_COND(r_Batch->selected,"CAREFUL! batch not in idle, overflow buffer selected!");
	r_Batch->selected = true;

	// start background loading
	std::thread __LoadThread(&bgr_load_batch,r_Batch);
	__LoadThread.detach();
	return r_Batch;
}


/*
	TODO
*/
void sprite_upload(RenderBatch* batch,ShaderPipeline* pipeline,double& stream_time)
{
	COMM_AWT("attempting to upload %li sprite textures to gpu",batch->sprite_textures.size());

	// load textures
	std::chrono::steady_clock::time_point __StreamTime = std::chrono::steady_clock::now();
	while (batch->sprite_upload_head<batch->sprite_textures.size())
	{
		// check timing for stall until next frame when upload takes too long
		stream_time += (std::chrono::steady_clock::now()-__StreamTime).count()*CONVERSION_MULT_MILLISECONDS;
		if (stream_time>1.f)
		{
			COMM_CNF();
			return;
		}
		// TODO: maybe order uploads towards end of render to measure a remaining time budget for streaming

		// upload texture to gpu
		SpriteTextureTuple& __Texture = batch->sprite_textures[batch->sprite_upload_head];
		__Texture.texture.upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		batch->sprite_upload_head++;
	}

	batch->sprite_ready = true;
	COMM_CNF();
}
// TODO: automatically allocate time budget based on current workload
//		background texture streaming while render

void sprite_update(RenderBatch* batch,ShaderPipeline* pipeline,double& stream_time)
{
	// update registered animations
	batch->update_sprites();

	// iterate registered sprites
	for (Sprite& p_Sprite : batch->sprites)
	{
		SpriteTextureTuple& p_Texture = batch->sprite_textures[p_Sprite.texture_id];

		// upload sprite attributes
		pipeline->upload_int("row",p_Texture.rows);
		pipeline->upload_int("col",p_Texture.columns);
		pipeline->upload_vec2("i_tex",p_Sprite.atlas_index);
		pipeline->upload_matrix("model",p_Sprite.transform.model);

		// draw sprite
		p_Texture.texture.bind();
		glDrawArrays(GL_TRIANGLES,0,6);
	}
}

typedef void (*sprite_update_routine)(RenderBatch*,ShaderPipeline*,double&);
sprite_update_routine update_sprites[2] = { sprite_upload,sprite_update };


/*
	TODO
*/
void mesh_upload(RenderBatch* batch,double& stream_time)
{
	COMM_AWT("attempting to upload %li mesh textures to gpu",batch->mesh_textures.size()*4);

	// upload textures
	std::chrono::steady_clock::time_point __StreamTime = std::chrono::steady_clock::now();
	while (batch->mesh_upload_head<batch->mesh_textures.size())
	{
		while (batch->mesh_upload_subhead<4)
		{
			// check timing and stall texture upload until next frame when necessary for framerate reasons
			stream_time += (std::chrono::steady_clock::now()-__StreamTime).count()
					* CONVERSION_MULT_MILLISECONDS;
			if (stream_time>1.f)
			{
				COMM_CNF();
				return;
			}

			// move to address of texture to upload
			Texture* p_Texture = &batch->mesh_textures[batch->mesh_upload_head].colours;
			p_Texture = p_Texture+batch->mesh_upload_subhead;

			// upload texture and move on
			p_Texture->upload();
			Texture::set_texture_parameter_clamp_to_edge();
			Texture::set_texture_parameter_linear_mipmap();
			batch->mesh_upload_subhead++;
		}

		// reset texture index within tuple and increment tuple index
		batch->mesh_upload_subhead = 0;
		batch->mesh_upload_head++;
	}

	// ready mesh buffer
	batch->mesh_buffer.upload_vertices(batch->mesh_vertices);
	batch->mesh_pipeline.point_buffer3D();

	g_Renderer.update_lighting();
	batch->mesh_ready = true;
	COMM_CNF();
}

void mesh_update(RenderBatch* batch,double& stream_time)
{
	// upload camera transform & iterate meshes
	for (uint16_t i=0;i<batch->meshes.size();i++)
	{
		Mesh& p_Mesh = batch->meshes[i];
		MeshTextureTuple& p_Texture = batch->mesh_textures[i];

		// upload attributes
		batch->mesh_pipeline.upload_matrix("model",p_Mesh.transform.model);
		p_Texture.colours.bind();
		glActiveTexture(GL_TEXTURE1);
		p_Texture.normals.bind();
		glActiveTexture(GL_TEXTURE2);
		p_Texture.materials.bind();
		glActiveTexture(GL_TEXTURE3);
		p_Texture.emission.bind();
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glDrawArrays(GL_TRIANGLES,p_Mesh.vertex_offset,p_Mesh.vertex_range);
	}
}

typedef void (*mesh_update_routine)(RenderBatch*,double&);
mesh_update_routine update_meshes[2] = { mesh_upload,mesh_update };

/*
	TODO
*/
void animation_upload(RenderBatch* batch,double& stream_time)
{

	COMM_AWT("attempting to upload %li animation textures to gpu",batch->animation_textures.size()*4);

	// upload textures
	std::chrono::steady_clock::time_point __StreamTime = std::chrono::steady_clock::now();
	while (batch->animation_upload_head<batch->animation_textures.size())
	{
		while (batch->animation_upload_subhead<4)
		{
			// check timing and stall texture upload until next frame when necessary for framerate reasons
			stream_time += (std::chrono::steady_clock::now()-__StreamTime).count()
					* CONVERSION_MULT_MILLISECONDS;
			if (stream_time>1.f)
			{
				COMM_CNF();
				return;
			}

			// move to address of texture to upload
			Texture* p_Texture = &batch->animation_textures[batch->animation_upload_head].colours;
			p_Texture = p_Texture+batch->animation_upload_subhead;

			// upload texture and move on
			p_Texture->upload();
			Texture::set_texture_parameter_clamp_to_edge();
			Texture::set_texture_parameter_linear_mipmap();
			batch->animation_upload_subhead++;
		}

		// reset texture index within tuple and increment tuple index
		batch->animation_upload_subhead = 0;
		batch->animation_upload_head++;
	}

	// setup vertices
	batch->animation_buffer.upload_vertices(batch->animation_vertices);
	batch->animation_buffer.upload_elements(batch->animation_elements);
	batch->animation_pipeline.def_attributeF("position",3,0,ANIMATION_UPLOAD_REPEAT);
	batch->animation_pipeline.def_attributeF("texCoords",2,3,ANIMATION_UPLOAD_REPEAT);
	batch->animation_pipeline.def_attributeF("normals",3,5,ANIMATION_UPLOAD_REPEAT);
	batch->animation_pipeline.def_attributeF("tangent",3,8,ANIMATION_UPLOAD_REPEAT);
	batch->animation_pipeline.def_attributeF("boneIndex",4,11,ANIMATION_UPLOAD_REPEAT);
	batch->animation_pipeline.def_attributeF("boneWeight",4,15,ANIMATION_UPLOAD_REPEAT);
	// TODO: find a way to handle these offset parameters. this is very unsafe (handle by shader/offsetof)
	batch->anim_ready = true;

	COMM_CNF();
}

void animation_update(RenderBatch* batch,double& stream_time)
{
	for (uint16_t i=0;i<batch->animated_meshes.size();i++)
	{
		AnimatedMesh& p_Animation = batch->animated_meshes[i];
		MeshTextureTuple& p_Texture = batch->animation_textures[i];

		// update
		p_Animation.update_animation();
		for (AnimationJoint& joint : p_Animation.joints)
			batch->animation_pipeline.upload_matrix(joint.uniform_location.c_str(),joint.recursive_transform);

		// upload attributes
		batch->animation_pipeline.upload_matrix("model",p_Animation.mesh.transform.model);
		p_Texture.colours.bind();
		glActiveTexture(GL_TEXTURE1);
		p_Texture.normals.bind();
		glActiveTexture(GL_TEXTURE2);
		p_Texture.materials.bind();
		glActiveTexture(GL_TEXTURE3);
		p_Texture.emission.bind();
		glActiveTexture(GL_TEXTURE0);

		// draw animation
		glDrawElements(GL_TRIANGLES,p_Animation.mesh.vertex_range,GL_UNSIGNED_INT,
				(void*)(p_Animation.mesh.vertex_offset*sizeof(uint32_t)));
	}
}

typedef void (*anim_update_routine)(RenderBatch*,double&);
anim_update_routine update_animations[2] = { animation_upload,animation_update };

/*
	TODO
*/
void Renderer::update()
{
	// rendering 3D geometry
	// 3D setup
	double __StreamTime = .0;
	glDisable(GL_BLEND);

	// render geometry for shadow projection
	glCullFace(GL_FRONT);
	glViewport(0,0,RENDERER_SHADOW_RESOLUTION,RENDERER_SHADOW_RESOLUTION);
	m_ShadowFrameBuffer.bind();
	Frame::clear();
	for (RenderBatch* batch : gpu_update_pointers)
	{
		if (!batch->mesh_ready||!batch->anim_ready) continue;

		// mesh
		batch->mesh_buffer.bind();
		batch->mesh_pipeline.enable();
		batch->mesh_pipeline.upload_camera(m_Shadow.shadow_view);
		update_meshes[1](batch,__StreamTime);

		// animation
		batch->animation_buffer.bind();
		batch->animation_pipeline.enable();
		batch->animation_pipeline.upload_camera(m_Shadow.shadow_view);
		update_animations[1](batch,__StreamTime);
	}
	FrameBuffer::unbind();
	glViewport(0,0,g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight);
	glCullFace(GL_BACK);
	// FIXME: basic render call is way too heavy for a shadow projection!!

	// render geometry for deferred shading
	m_GBuffer.bind();
	Frame::clear();
	for (RenderBatch* batch : gpu_update_pointers)
	{
		// mesh
		batch->mesh_buffer.bind();
		batch->mesh_pipeline.enable();
		batch->mesh_pipeline.upload_camera(g_Camera3D);
		update_meshes[batch->mesh_ready](batch,__StreamTime);

		// animation
		batch->animation_buffer.bind();
		batch->animation_pipeline.enable();
		batch->animation_pipeline.upload_camera(g_Camera3D);
		update_animations[batch->anim_ready](batch,__StreamTime);
	}
	FrameBuffer::unbind();

	// post processing
	// 2D setup
	glEnable(GL_BLEND);

	// deferred shading
	m_CanvasBuffer.bind();
	m_DeferredPipeline.enable();
	m_GBuffer.upload_colour_component(0);
	glActiveTexture(GL_TEXTURE1);
	m_GBuffer.upload_colour_component(1);
	glActiveTexture(GL_TEXTURE2);
	m_GBuffer.upload_colour_component(2);
	glActiveTexture(GL_TEXTURE3);
	m_GBuffer.upload_colour_component(3);
	glActiveTexture(GL_TEXTURE4);
	m_ShadowFrameBuffer.upload_depth_component();
	glActiveTexture(GL_TEXTURE0);
	m_DeferredPipeline.upload_vec3("view_pos",g_Camera3D.position);
	m_DeferredPipeline.upload_vec3("light_position",m_Shadow.source_position);
	m_DeferredPipeline.upload_matrix("shadow_matrix",m_Shadow.shadow_matrix);
	glDrawArrays(GL_TRIANGLES,0,6);

	// render 2D geometry
	// iterate sprite render
	m_SpriteBuffer.bind();
	m_SpritePipeline.enable();
	for (RenderBatch* batch : gpu_update_pointers)
		update_sprites[batch->sprite_ready](batch,&m_SpritePipeline,__StreamTime);

	// iterate duplicate render
	m_SpriteBuffer.bind_index();
	m_DuplicatePipeline.enable();
	for (RenderBatch* batch : gpu_update_pointers) render_duplicates(batch);
}

/*
	TODO
*/
void Renderer::update_lighting()
{
	m_DeferredPipeline.enable();
	uint8_t __DirOffset = 0,__PointOffset = 0,__SpotOffset = 0;
	for (RenderBatch* batch : gpu_update_pointers)
		batch->lighting.upload(m_DeferredPipeline,__DirOffset,__PointOffset,__SpotOffset);
	m_DeferredPipeline.upload_int("sunlight_count",__DirOffset);
	m_DeferredPipeline.upload_int("pointlight_count",__PointOffset);
	m_DeferredPipeline.upload_int("spotlight_count",__SpotOffset);
}

/*
	TODO
*/
void Renderer::set_shadow(glm::vec3 source)
{
	m_Shadow.shadow_view = Camera3D(source,glm::vec3(.0f),RENDERER_SHADOW_RANGE,RENDERER_SHADOW_RANGE);
	m_Shadow.shadow_matrix = m_Shadow.shadow_view.combine_matrices();
	m_Shadow.source_position = source;
	// TODO: specify centerpoint and add vector to source and target respectively
}

/*
	TODO
*/
void Renderer::render_duplicates(RenderBatch* batch)
{
	// skip render if sprite textures are not ready yet
	if (!batch->sprite_ready) return;
	batch->update_duplicates();

	// iterate registered duplicates
	for (SpriteInstance& p_Instance : batch->duplicates)
	{
		SpriteTextureTuple& p_Texture = batch->sprite_textures[p_Instance.texture_id];

		// upload instance attributes & data
		m_SpriteBuffer.upload_indices(p_Instance.upload,SPRITE_INSTANCE_CAPACITY*sizeof(SpriteInstanceUpload));
		m_DuplicatePipeline.upload_int("row",p_Texture.rows);
		m_DuplicatePipeline.upload_int("col",p_Texture.columns);
		m_DuplicatePipeline.upload_matrix("model",p_Instance.transform.model);

		// draw duplicates
		p_Texture.texture.bind();
		glDrawArraysInstanced(GL_TRIANGLES,0,6,p_Instance.active_range);
	}
}
