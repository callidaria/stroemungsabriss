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
constexpr uint8_t MESH_UPLOAD_REPEAT = sizeof(MeshUpload)/sizeof(float);


// batch datastructure definitions
// description of current buffer state, is it rendering, is it loading, should it be ignored?
enum BatchState
{
	RBFR_IDLE,
	RBFR_LOAD,
	RBFR_UPLOAD,
	RBFR_READY,
	RBFR_STATE_COUNT
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
	std::string path;
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
	void load_mesh(std::string& path);

	// update
	void update_sprites();
	void update_duplicates();

	// data
	// sprites
	std::vector<SpriteTextureTuple> sprite_textures;
	std::vector<Sprite> sprites;
	std::vector<SpriteAnimation> anim_sprites;

	// meshes
	//Buffer mesh_buffer;
	std::vector<MeshTextureTuple> mesh_textures;
	std::vector<Mesh> meshes;
	std::vector<MeshUpload> mesh_vertices;

	// instanced sprites
	std::vector<SpriteInstance> duplicates;
	std::vector<SpriteAnimationInstance> anim_duplicates;

	// attributes
	BatchState state = RBFR_IDLE;
	std::string path;

	// loading
	volatile bool load_semaphore = false;
	uint16_t upload_head = 0;
};
constexpr uint8_t RENDERER_BATCHES_COUNT = 4;


class Renderer
{
public:

	// construction
	Renderer();
	~Renderer() {  }

	// interpretation
	RenderBatch* load(std::string path);

	// stages
	void update();

private:

	void render_sprites(RenderBatch* batch);
	void render_duplicates(RenderBatch* batch);
	void render_meshes(RenderBatch* batch);

public:

	// batches
	RenderBatch batches[RENDERER_BATCHES_COUNT];
	std::vector<RenderBatch*> draw_pointers;

private:

	// buffers
	Buffer spr_buffer;
	Buffer mesh_buffer;

	// shaders
	Shader vs_sprite = Shader("./shader/obj/sprite.vs",GL_VERTEX_SHADER);
	Shader vs_duplicate = Shader("./shader/obj/duplicate.vs",GL_VERTEX_SHADER);
	Shader fs_direct = Shader("./shader/standard/direct.fs",GL_FRAGMENT_SHADER);
	Shader fs_mesh = Shader("./shader/obj/mesh.fs",GL_FRAGMENT_SHADER);

	// shader pipelines
	ShaderPipeline sp_sprite;
	ShaderPipeline sp_duplicate;
	ShaderPipeline sp_mesh;

	// rendertarget
	Buffer m_canvas_buffer;
	//Shader m_deferred_shader;
	FrameBuffer m_gbuffer = FrameBuffer(4);
	FrameBuffer m_deferred = FrameBuffer(1);
};

inline Renderer g_Renderer = Renderer();

#endif
