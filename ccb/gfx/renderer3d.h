#pragma once

#include <iostream>

#include "shader.h"
#include "mesh.h"

#include "../mat/toolbox.h"
#include "../mat/camera3d.h"

#include "../frm/frame.h"
#include "../fcn/buffer.h"

constexpr uint8_t R3D_INDEX_REPEAT = 9;

class Renderer3D
{
public:

	// construction
	Renderer3D();
	~Renderer3D() {  }

	// creation
	uint16_t add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 p,float s,glm::vec3 r,bool cast_shadow=false);
	uint16_t add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 p,float s,glm::vec3 r,uint16_t dcap,bool cast_shadow=false);
	void create_shadow(glm::vec3 pos,glm::vec3 center,float mwidth,float mheight,
			float fdiv,uint16_t res);

	// loaders
	void load(Camera3D cam3d);

	// preparations
	void prepare();
	void prepare(Camera3D cam3d);
	void prepare_inst();
	void prepare_inst(Camera3D cam3d);
	void prepare_shadow();
	void register_geometry(ShadowGeometry* geometry);

	// close process
	void close_shadow(uint16_t w_res,uint16_t h_res);
	void clear_memory();

	// shadow
	void render_mesh_shadow();
	void render_instance_shadow();
	void render_geometry_shadow();

	// draw
	void render_mesh(uint16_t b,uint16_t e);
	void render_inst(uint16_t i);

	// uploads
	void upload_shadow();

	// setters
	void inst_position(uint8_t id,uint8_t mid,glm::vec3 pos);
	void inst_rotation(uint8_t id,uint8_t mid,glm::vec3 rot);

private:

	uint32_t mofs = 0,imofs = 0;

public:

	// cascabel
	Buffer buffer,ibuffer;
	Shader s3d,shs,is3d;

	// object information upload lists
	std::vector<Mesh> ml,iml;
	std::vector<std::vector<float>> mesh_indices;

	// shadow
	uint16_t shadow_res;
	std::vector<uint16_t> scast_mesh_ids;
	std::vector<uint16_t> scast_instance_ids;
	std::vector<ShadowGeometry*> shadow_geometry;
	GLuint depth_fbo,shadow_map;
	glm::mat4 shadow_proj,shadow_view,scam_projection;
};