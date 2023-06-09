#ifndef CCB_GRAPHICS_RENDERER3D
#define CCB_GRAPHICS_RENDERER3D

#include <iostream>

#include "shader.h"
#include "mesh.h"
#include "physical_mesh.h"

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
	uint16_t add_physical(const char* m,const char* t,const char* nm,const char* mm,const char* em,
			glm::vec3 p,float s,glm::vec3 r,bool cast_shadow=false);
	void create_shadow(glm::vec3 pos,glm::vec3 center,float mwidth,float mheight,
			float fdiv,uint16_t res);

	// loaders
	void load(Camera3D cam3d);

	// preparations
	void prepare();
	void prepare(Camera3D cam3d);
	void prepare_inst();
	void prepare_inst(Camera3D cam3d);
	void prepare_pmesh();
	void prepare_pmesh(Camera3D cam3d);
	void prepare_shadow();
	void register_geometry(ShadowGeometry* geometry);

	// close process
	void close_shadow(uint16_t w_res,uint16_t h_res);
	void clear_memory();

	// shadow
	void render_mesh_shadow();
	void render_instance_shadow();
	void render_physical_shadow();
	void render_geometry_shadow();

	// draw
	void render_mesh(uint16_t b,uint16_t e);
	void render_inst(uint16_t i);
	void render_pmsh(uint16_t i);

	// setters
	void inst_position(uint8_t id,uint8_t mid,glm::vec3 pos);
	void inst_rotation(uint8_t id,uint8_t mid,glm::vec3 rot);

private:

	uint32_t mofs = 0,imofs = 0,pmofs = 0;

public:

	// cascabel
	Buffer buffer,ibuffer,pbuffer;
	Shader s3d,is3d,pbms,shs;

	// object information upload lists
	std::vector<Mesh> ml,iml;
	std::vector<PhysicalMesh> pml;
	std::vector<std::vector<float>> mesh_indices;

	// shadow
	glm::vec3 slight_pos;
	uint16_t shadow_res;
	std::vector<uint16_t> scast_mesh_ids,scast_instance_ids,scast_physical_ids;
	std::vector<ShadowGeometry*> shadow_geometry;
	GLuint depth_fbo,shadow_map;
	glm::mat4 shadow_proj,shadow_view,scam_projection;
};

#endif