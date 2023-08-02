#ifndef CCB_GRAPHICS_MESH_ANIMATION
#define CCB_GRAPHICS_MESH_ANIMATION

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../mat/toolbox.h"
#include "shader.h"

// #define LIGHT_SELFIMPLEMENTATION_COLLADA_LOAD

constexpr uint8_t BONE_INFLUENCE_STACK_RANGE = 4;

// rotary joints for animation information
struct ColladaJoint {
	std::string id;
	glm::mat4 trans = glm::mat4(1),btrans = glm::mat4(1);
	std::vector<uint16_t> children;
};
// TODO: correlate string ids with equivalent integers after read

// structure of keys throughout animations
struct JointKeys {
	uint16_t joint_id;
	std::vector<double> dur_positions,dur_scales,dur_rotations;
	std::vector<glm::vec3> key_positions,key_scales;
	std::vector<glm::quat> key_rotations;
};

// structure of animation data
struct ColladaAnimationData {
	double duration;
	std::vector<JointKeys> joints;
	std::vector<uint16_t> crr_position,crr_scale,crr_rotation;
};

class MeshAnimation
{
public:

	// construction
	MeshAnimation(const char* path,const char* itex_path,uint32_t &mofs);
	~MeshAnimation() {  }

	// load
	inline void texture() { Toolbox::load_texture_repeat(tex,tex_path,true); }
	void upload_interpolation(Shader* shader);

	// update
	inline void set_animation(uint16_t anim_id) { current_anim = anim_id; }
	void interpolate(float dt);

	// system
	friend std::ostream &operator<<(std::ostream &os,const MeshAnimation& obj);

private:

#ifdef LIGHT_SELFIMPLEMENTATION_COLLADA_LOAD

	// read helpers
	std::vector<float> extract_array_data(std::vector<std::string> raw_data);
	std::vector<std::string> parameters_from_line(std::string line);

	// specialized recursion
	static ColladaJoint rc_assemble_joint_hierarchy(std::ifstream &file);
#else
	uint16_t rc_get_joint_count(aiNode* joint);
	void rc_assemble_joint_hierarchy(aiNode* joint,uint16_t &joint_count);

#endif

	// standard recursion
	uint16_t rc_get_joint_id(std::string jname,ColladaJoint cjoint,bool &found);
	void rc_transform_interpolation(ColladaJoint* cjoint,glm::mat4 gtrans,uint16_t &id);

	// conversion
	static inline glm::vec3 glmify(aiVector3D ivec3) { return glm::vec3(ivec3.x,ivec3.y,ivec3.z); }
	static inline glm::quat glmify(aiQuaternion iquat)
		{ return glm::quat(iquat.w,iquat.x,iquat.y,iquat.z); }
	static inline glm::mat4 glmify(aiMatrix4x4 imat4)
		{ return glm::transpose(glm::make_mat4(&imat4.a1)); }

	// animation
	float advance_animation(uint16_t &crr_index,std::vector<double> key_indices);

	// system
	static void rc_print_joint_tree(std::ostream &os,std::vector<ColladaJoint> joints,uint16_t jid,
			uint8_t depth);

public:

	// vertex data
	std::vector<float> verts;
	std::vector<uint32_t> elems;
	uint32_t tex;
	uint32_t ofs,size = 0;

	// transformation
	glm::mat4 model = glm::mat4(1.0f);

private:

	// vertex information
	const char* tex_path;
	std::vector<float> vaddress;

	// rigging data
	std::vector<ColladaJoint> joints;
	std::vector<ColladaAnimationData> anims;
	std::vector<glm::mat4> bone_offsets;
	uint16_t current_anim;

	// timing
	float avx = 0;
};

#endif