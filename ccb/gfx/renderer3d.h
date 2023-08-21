#ifndef CCB_GRAPHICS_RENDERER3D
#define CCB_GRAPHICS_RENDERER3D

#include <iostream>

#include "shader.h"
#include "mesh.h"
#include "mesh_anim.h"
#include "physical_mesh.h"

#include "../mat/toolbox.h"
#include "../mat/camera3d.h"

#include "../frm/frame.h"
#include "../frm/framebuffer.h"
#include "../frm/gbuffer.h"
#include "../fcn/buffer.h"

constexpr uint8_t R3D_INDEX_REPEAT = 9;
constexpr uint8_t R3D_ANIMATION_MAP_REPEAT = 16;

struct Target3D {
	GBuffer gbuffer;
	FrameBuffer cbuffer;
};

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
	uint16_t add(const char* a,const char* t,glm::vec3 p,float s,bool cast_shadow=false);
	uint16_t add_physical(const char* m,const char* t,const char* nm,const char* mm,const char* em,
			glm::vec3 p,float s,glm::vec3 r,bool cast_shadow=false);
	void create_shadow(glm::vec3 pos,glm::vec3 center,float mwidth,float mheight,
			float fdiv,uint16_t res);
	uint8_t add_target(Frame* frame);

	// loaders
	void load(Camera3D cam3d,float &progress,float pseq);

	// targets
	inline void start_target(uint8_t id) { rtargets[id].gbuffer.bind(); }
	void prepare_target(uint8_t id,Camera3D cam3D);

	// preparations
	void prepare();
	void prepare(Camera3D cam3d);
	void prepare_inst();
	void prepare_inst(Camera3D cam3d);
	void prepare_anim();
	void prepare_anim(Camera3D cam3d);
	void prepare_pmesh();
	void prepare_pmesh(Camera3D cam3d);
	void prepare_shadow();
	void register_geometry(ShadowGeometry* geometry);

	// close process
	void close_shadow(uint16_t w_res,uint16_t h_res);
	void clear_memory();

	// update
	void update_animations(double dt);
	void update_shadows(float swidth,float sheight);

	// shadow
	void render_mesh_shadow();
	void render_instance_shadow();
	void render_animation_shadow();
	void render_physical_shadow();
	void render_geometry_shadow();

	// draw
	void render_mesh(uint16_t b,uint16_t e);
	void render_inst(uint16_t i);
	void render_anim(uint16_t i);
	void render_pmsh(uint16_t i);

	// setters
	void inst_position(uint8_t id,uint8_t mid,glm::vec3 pos);
	void inst_rotation(uint8_t id,uint8_t mid,glm::vec3 rot);
	inline void start_animation(uint16_t id) { update_animation_ids.push_back(id); }
	inline void stop_animation(uint16_t id)
		{ std::remove(update_animation_ids.begin(),update_animation_ids.end(),id); }

private:

	uint32_t mofs = 0,imofs = 0,pmofs = 0,amofs = 0;

public:

	// cascabel
	Buffer buffer,ibuffer,abuffer,pbuffer;
	Shader s3d,is3d,as3d,pbms,shs;
	std::vector<Target3D> rtargets;

	// object information upload lists
	std::vector<Mesh> ml,iml;
	std::vector<MeshAnimation> mal;
	std::vector<PhysicalMesh> pml;
	std::vector<std::vector<float>> mesh_indices;

	// update
	std::vector<uint16_t> scast_mesh_ids,scast_instance_ids,scast_animation_ids,
			update_animation_ids,scast_physical_ids;
	std::vector<ShadowGeometry*> shadow_geometry;

	// shadow
	glm::vec3 slight_pos;
	uint16_t shadow_res;
	GLuint depth_fbo,shadow_map;
	glm::mat4 shadow_proj,shadow_view,scam_projection;
};

#endif