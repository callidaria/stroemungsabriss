#ifndef CCB_GRAPHICS_MESH_ANIMATION
#define CCB_GRAPHICS_MESH_ANIMATION

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	glm::mat4 trans,inv_trans,gtrans;
	std::vector<ColladaJoint> children;
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
	double delta_ticks;
	std::vector<JointKeys> joints;
};

class MeshAnimation
{
public:

	// construction
	MeshAnimation(const char* path,const char* itex_path,uint32_t &mofs);
	~MeshAnimation() {  }

	// load
	void texture();

	// update
	void set_animation(uint16_t anim_id);
	void interpolate(Shader* shader,uint8_t i);

	// system
	friend std::ostream &operator<<(std::ostream &os,const MeshAnimation& obj);

private:

	// read helpers
	std::vector<float> extract_array_data(std::vector<std::string> raw_data);
	std::vector<std::string> parameters_from_line(std::string line);

	// recursion
#ifdef LIGHT_SELFIMPLEMENTATION_COLLADA_LOAD
	static ColladaJoint rc_assemble_joint_hierarchy(std::ifstream &file);
#else
	static ColladaJoint rc_assemble_joint_hierarchy(aiNode* joint);
	static uint16_t rc_get_joint_id(std::string jname,ColladaJoint cjoint,bool &found);
	ColladaJoint* rc_get_joint_object(ColladaJoint* cjoint,uint16_t anim_id,uint16_t &curr_id);
	static void rc_upload_joint(Shader* shader,ColladaJoint cjoint,uint16_t id);
#endif

	// conversion
	static glm::vec3 glmify(aiVector3D ivec3);
	static glm::quat glmify(aiQuaternion iquat);

	// system
	static void rc_print_joint_tree(std::ostream &os,ColladaJoint cjoint,uint8_t depth);

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
	ColladaJoint jroot;
	std::vector<ColladaAnimationData> anims;
	uint16_t current_anim;
	float anim_progression = 0;
};

#endif