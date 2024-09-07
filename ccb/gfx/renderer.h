#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../fcn/config.h"
#include "../mat/math.h"
#include "../fcn/buffer.h"
#include "../frm/frame.h"

#include "shader.h"
#include "texture.h"


// upload structures
// per instance upload datastructure for duplicate shader
struct SpriteInstanceUpload
{
	glm::vec2 offset = glm::vec2(0);
	glm::vec2 scale = glm::vec2(1);
	float rotation = .0f;
	glm::vec2 atlas_index = glm::vec2(0);
};
constexpr uint8_t SPRITE_INSTANCE_UPLOAD_REPEAT = sizeof(SpriteInstanceUpload)/sizeof(float);

// mesh upload struction
struct MeshUpload
{
	glm::vec3 position;
	glm::vec2 uv_coord;
	glm::vec3 normal;
	glm::vec3 tangent;
};


// sprite data
// texture information for sprites
struct SpriteTextureTuple
{
	Texture texture;
	uint8_t rows,columns;
	uint16_t frames;
};

// render entry connected to loaded sprite and individually transformed
struct Sprite
{
	uint16_t texture_id;
	Transform2D transform;
	glm::vec2 atlas_index = glm::vec2(0);
};

// animation entry connected to sprite render entry, holding state of animation
struct SpriteAnimation
{
	uint16_t id;
	uint8_t cycle_duration;
	float frame_duration;
	float anim_progression = .0f;
};

// data structure to represent a full set of duplicates
constexpr uint16_t SPRITE_INSTANCE_CAPACITY = 4096;
struct SpriteInstance
{
	uint16_t texture_id;
	Transform2D transform;
	SpriteInstanceUpload upload[SPRITE_INSTANCE_CAPACITY];
	uint16_t active_range = 0;
};

// animation entry connected to duplicate render entry, holding state of animation
struct SpriteAnimationInstance
{
	uint16_t id;
	uint8_t cycle_duration;
	float frame_duration;
	float anim_progressions[SPRITE_INSTANCE_CAPACITY] = { .0f };
};


// mesh data
// mesh texture cluster
struct MeshTextureTuple
{
	Texture colours;
	Texture normals;
	Texture materials;
	Texture emission;
};

// mesh geometry
struct Mesh
{
	Transform3D transform;
	uint32_t vertex_offset,vertex_range;
	std::string path;
};


// lighting
struct LightDirectional
{
	glm::vec3 position;
	glm::vec3 colour;
	float intensity;
};

struct LightPoint
{
	glm::vec3 position;
	glm::vec3 colour;
	float intensity;
	float c0,c1,c2;
};

struct LightSpot
{
	glm::vec3 position;
	glm::vec3 colour;
	glm::vec3 direction;
	float intensity;
	float cut_inner,cut_outer;
};

struct Lighting
{
	// utility
	void upload(ShaderPipeline& pipeline,uint8_t& dir_offset,uint8_t& point_offset,uint8_t& spot_offset);

	// data
	std::vector<LightDirectional> directional_lights;
	std::vector<LightPoint> point_lights;
	std::vector<LightSpot> spot_lights;
	uint8_t offset_directional,offset_point,offset_spot;
};


// batches
// batch data to seperately load and display to other buffers
struct RenderBatch
{
	// utility
	// sprite creation
	uint16_t add_sprite(std::string path,uint8_t rows=1,uint8_t cols=1,uint8_t frames=1);
	void register_sprite(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt);
	void register_sprite(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,uint8_t dur);
	void register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt);
	void register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,uint8_t dur);

	// mesh creation
	void add_mesh(std::string obj,std::string tex,std::string norm,std::string mats,std::string emit,
			glm::vec3 pos,float scl=1.f,glm::vec3 rot=glm::vec3(0));

	// spawners
	void spawn_sprite_instance(uint16_t inst_id,
			glm::vec2 ofs,glm::vec2 scl=glm::vec2(1),float rot=.0f,glm::vec2 subtex=glm::vec2(0));

	// loader
	void load_mesh(Mesh& mesh);

	// update
	void update_sprites();
	void update_duplicates();

	// data
	// sprites
	std::vector<SpriteTextureTuple> sprite_textures;
	std::vector<Sprite> sprites;
	std::vector<SpriteAnimation> sprite_animations;

	// instanced sprites
	std::vector<SpriteInstance> duplicates;
	std::vector<SpriteAnimationInstance> duplicate_animations;

	// meshes
	Buffer mesh_buffer;
	ShaderPipeline mesh_pipeline;
	std::vector<MeshTextureTuple> mesh_textures;
	std::vector<Mesh> meshes;
	std::vector<MeshUpload> mesh_vertices;

	// lighting
	Lighting lighting;

	// load progression
	uint16_t sprite_upload_head = 0;
	uint16_t mesh_upload_head = 0,mesh_upload_subhead = 0;

	// load flags
	bool selected = false;
	bool sprite_ready = false;
	bool mesh_ready = false;

	// attributes
	std::string path;
};
constexpr uint8_t RENDERER_BATCHES_COUNT = 4;


class Renderer
{
public:

	Renderer();
	~Renderer() {  }

	RenderBatch* load(const std::string& path);
	void update();
	void update_lighting();

private:

	// draw
	void render_sprites(RenderBatch* batch);
	void render_duplicates(RenderBatch* batch);
	void render_meshes(RenderBatch* batch);

public:

	RenderBatch batches[RENDERER_BATCHES_COUNT];
	std::vector<RenderBatch*> gpu_update_pointers;

private:

	// buffers
	Buffer m_SpriteBuffer;
	Buffer m_CanvasBuffer;

	// shader pipelines
	ShaderPipeline m_SpritePipeline;
	ShaderPipeline m_DuplicatePipeline;
	ShaderPipeline m_DeferredPipeline;

	// rendertarget
	FrameBuffer m_GBuffer = FrameBuffer(4);
};

inline Renderer g_Renderer = Renderer();
namespace RendererUtils
{
	inline void register_batch_pointer(RenderBatch* batch) { g_Renderer.gpu_update_pointers.push_back(batch); }
};

#endif
