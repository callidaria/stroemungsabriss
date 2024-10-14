#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../fcn/config.h"
#include "../mat/math.h"
#include "../fcn/buffer.h"
#include "../frm/frame.h"

#include "shader.h"
#include "texture.h"


constexpr uint16_t RENDERER_SHADOW_RESOLUTION = 4096;
constexpr uint16_t RENDERER_SHADOW_RANGE = 25;
constexpr uint8_t ANIMATION_INFLUENCE_STACK_RANGE = 4;


// upload structures
struct SpriteInstanceUpload
{
	glm::vec2 offset = glm::vec2(0);
	glm::vec2 scale = glm::vec2(1);
	float rotation = .0f;
	glm::vec2 atlas_index = glm::vec2(0);
};
constexpr uint8_t SPRITE_INSTANCE_UPLOAD_REPEAT = sizeof(SpriteInstanceUpload)/sizeof(float);

struct MeshUpload
{
	glm::vec3 position;
	glm::vec2 uv_coord;
	glm::vec3 normal;
	glm::vec3 tangent;
};

struct AnimationUpload
{
	glm::vec3 position;
	glm::vec2 uv_coord;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec4 bone_index;
	glm::vec4 bone_weight;
};
constexpr uint8_t ANIMATION_UPLOAD_REPEAT = sizeof(AnimationUpload)/sizeof(float);


// sprite data
struct SpriteTextureTuple
{
	Texture texture;
	uint8_t rows,columns;
	uint16_t frames;
};

struct Sprite
{
	uint16_t texture_id;
	Transform2D transform;
	glm::vec2 atlas_index = glm::vec2(0);
};

struct SpriteAnimation
{
	uint16_t id;
	float cycle_duration;
	float frame_duration;
	float anim_progression = .0f;
};

constexpr uint16_t SPRITE_INSTANCE_CAPACITY = 4096;
struct SpriteInstance
{
	uint16_t texture_id;
	Transform2D transform;
	SpriteInstanceUpload upload[SPRITE_INSTANCE_CAPACITY];
	uint16_t active_range = 0;
};

struct SpriteAnimationInstance
{
	uint16_t id;
	float cycle_duration;
	float frame_duration;
	float anim_progressions[SPRITE_INSTANCE_CAPACITY] = { .0f };
};


// text data
struct Text
{
	// utility
	void render();

	// data
	std::string data;
	glm::vec2 position;
};


// mesh data
struct MeshTextureTuple
{
	Texture colours;
	Texture normals;
	Texture materials;
	Texture emission;
};

struct Mesh
{
	Transform3D transform;
	uint32_t vertex_offset,vertex_range;
	std::string path;
};


// animation data
struct AnimationJoint
{
	std::string id;
	std::string uniform_location;
	glm::mat4 offset;
	glm::mat4 transform = glm::mat4(1.f);
	glm::mat4 recursive_transform = glm::mat4(1.f);
	std::vector<uint16_t> children;
};

struct MeshJoint
{
	uint16_t id;
	uint16_t crr_position = 0,crr_scale = 0,crr_rotation = 0;
	std::vector<glm::vec3> position_keys;
	std::vector<glm::vec3> scaling_keys;
	std::vector<glm::quat> rotation_keys;
	std::vector<double> position_durations;
	std::vector<double> scaling_durations;
	std::vector<double> rotation_durations;
};

struct MeshAnimation
{
	std::vector<MeshJoint> joints;
	double duration;
};

struct AnimatedMesh
{
	// utility
	void update_animation();
	void rc_transform_interpolation(AnimationJoint& joint,glm::mat4 const& parent_transform);

	// data
	Mesh mesh;
	std::vector<AnimationJoint> joints;
	std::vector<MeshAnimation> animations;
	uint16_t current_animation = 0;
	double progress = .0;
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

struct Shadow
{
	Camera3D shadow_view;
	glm::mat4 shadow_matrix;
	glm::vec3 source_position;
};


// batches
// batch data to seperately load and display to other buffers
constexpr uint8_t RENDERER_BATCHES_COUNT = 4;
struct RenderBatch
{
	// utility
	// sprite creation
	uint16_t add_sprite(std::string path,uint8_t rows=1,uint8_t cols=1,uint8_t frames=1);
	void register_sprite(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt);
	void register_sprite(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,float dur);
	void register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt);
	void register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,float dur);

	// mesh creation
	void add_mesh(std::string obj,std::string tex,std::string norm,std::string mats,std::string emit,
			glm::vec3 pos,float scl=1.f,glm::vec3 rot=glm::vec3(0));
	void add_animation(std::string dae,std::string tex,std::string norm,std::string mats,std::string emit,
			glm::vec3 pos,float scl=1.f,glm::vec3 rot=glm::vec3(0));

	// spawners
	void spawn_sprite_instance(uint16_t inst_id,
			glm::vec2 ofs,glm::vec2 scl=glm::vec2(1),float rot=.0f,glm::vec2 subtex=glm::vec2(0));

	// loader
	void load_mesh(Mesh& mesh);
	void load_animation(AnimatedMesh& animation);

	// update
	void update_sprites();
	void update_duplicates();
	void update_animations();

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

	// animations
	Buffer animation_buffer;
	ShaderPipeline animation_pipeline;
	std::vector<MeshTextureTuple> animation_textures;
	std::vector<AnimatedMesh> animated_meshes;
	std::vector<AnimationUpload> animation_vertices;
	std::vector<uint32_t> animation_elements;
	// TODO: just like sprite textures, make mesh textures reusable

	// lighting
	Lighting lighting;

	// load progression
	uint16_t sprite_upload_head = 0;
	uint16_t mesh_upload_head = 0,mesh_upload_subhead = 0;
	uint16_t animation_upload_head = 0,animation_upload_subhead = 0;

	// load flags
	bool selected = false;
	bool sprite_ready = false;
	bool mesh_ready = false;
	bool anim_ready = false;

	// attributes
	std::string path;
};


class Renderer
{
public:

	Renderer();
	~Renderer() {  }

	RenderBatch* load(const std::string& path,int16_t target_batch=-1);
	void update();
	void vanish();

	// text
	void add_font(const char* path,uint16_t size);

	// lighting
	void update_lighting();
	void set_shadow(glm::vec3 source);

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

	// render targets
	FrameBuffer m_GBuffer = FrameBuffer(4);
	FrameBuffer m_ShadowFrameBuffer = FrameBuffer();

	// text
	FT_Library m_FreetypeLibrary;
	Glyph m_Font[128];
	std::vector<Text> m_Texts;

	// lighting
	Shadow m_Shadow;
};
// TODO: make data static to prevent problems with possible underuse of renderer global

inline Renderer g_Renderer = Renderer();

#endif
