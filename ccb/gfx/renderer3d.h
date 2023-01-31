#pragma once

#include <iostream>
#include "shader.h"
#include "mesh.h"
#include "../mat/camera3d.h"
#include "../fcn/buffer.h"

constexpr uint8_t R3D_INDEX_REPEAT = 9;

const std::string shadow_matrix_location = "light_trans";

class Renderer3D
{
public:

	// construction
	Renderer3D();
	~Renderer3D() {  }

	// creation
	uint16_t add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 p,float s,glm::vec3 r);
	uint16_t add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 p,float s,glm::vec3 r,uint16_t dcap);

	// loaders
	void load(Camera3D* cam3d);

	// preparations
	void prepare();
	void prepare(Camera3D* cam3d);
	void prepare_inst(Camera3D* cam3d);

	// draw
	void render_mesh(uint16_t b,uint16_t e);
	void render_inst(uint16_t i,uint16_t c);

	// uploads
	void upload_shadow(glm::mat4 m);  // TODO: implement this, it's important

	// setters
	void inst_position(uint8_t id,uint8_t mid,glm::vec3 pos);
	void inst_rotation(uint8_t id,uint8_t mid,glm::vec3 rot);

private:

	uint32_t mofs = 0,imofs = 0;

public:

	Buffer buffer,ibuffer;
	Shader s3d,shs,is3d;
	std::vector<Mesh> ml;
	std::vector<Mesh> iml;
	std::vector<std::vector<float>> mesh_indices;
};