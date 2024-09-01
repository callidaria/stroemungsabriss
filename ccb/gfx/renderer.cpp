#include "renderer.h"


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
	// create sprite source
	SpriteTextureTuple t_Tuple = {

		// source
		.texture = Texture(path),

		// spritesheet segmentation
		.rows = rows,
		.columns = cols,
		.frames = frames
	};

	// write and return reference id
	sprite_textures.push_back(t_Tuple);
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
	// information setup
	Sprite t_Sprite = {

		// link sprite to texture
		.texture_id = tex_id,

		// transform component
		.transform = {
			.position = pos,
			.width = wdt,
			.height = hgt,
		},
	};

	// transform and write
	t_Sprite.transform.to_origin();
	sprites.push_back(t_Sprite);
}

/*
	TODO
*/
void RenderBatch::register_sprite(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,uint8_t dur)
{
	// add animation data
	SpriteAnimation t_Animation = {
		.id = (uint16_t)sprites.size(),
		.cycle_duration = dur,
		.frame_duration = (float)dur/sprite_textures[tex_id].frames,
	};

	// register sprite & animation
	sprite_animations.push_back(t_Animation);
	register_sprite(tex_id,pos,wdt,hgt);
}

/*
	TODO
*/
void RenderBatch::register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt)
{
	SpriteInstance t_Instance = {

		// link sprites to texture
		.texture_id = tex_id,

		// tranformation
		.transform = {
			.position = pos,
			.width = wdt,
			.height = hgt,
		},
	};

	// transform and write
	t_Instance.transform.to_origin();
	duplicates.push_back(t_Instance);
}

/*
	TODO
*/
void RenderBatch::register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,uint8_t dur)
{
	// add animation data
	SpriteAnimationInstance t_Animation = {
		.id = (uint16_t)duplicates.size(),
		.cycle_duration = dur,
		.frame_duration = (float)dur/sprite_textures[tex_id].frames,
	};

	// register duplicate & animation
	duplicate_animations.push_back(t_Animation);
	register_duplicates(tex_id,pos,wdt,hgt);
}

/*
	TODO
*/
void RenderBatch::add_mesh(std::string obj,std::string tex,std::string norm,std::string mats,std::string emit,
		glm::vec3 pos,float scl,glm::vec3 rot)
{
	// store mesh vertex data
	Mesh t_Mesh = {
		// TODO: transform
		.path = obj
	};
	meshes.push_back(t_Mesh);

	// store texture information
	MeshTextureTuple t_Texture = {
		.colours = Texture(tex),
		.normals = Texture(norm),
		.materials = Texture(mats),
		.emission = Texture(emit),
	};
	mesh_textures.push_back(t_Texture);
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
void RenderBatch::load_mesh(const std::string& path)
{
	// create storage for file contents
	std::vector<glm::vec3> t_Positions;
	std::vector<glm::vec2> t_UVCoordinates;
	std::vector<glm::vec3> t_Normals;
	std::vector<uint32_t> t_PositionIndices,t_UVIndices,t_NormalIndices;

	// open source file
	FILE* t_OBJFile = fopen(path.c_str(),"r");
	if (t_OBJFile==NULL)
	{
		COMM_ERR("object loader -> file %s could not be found!",path.c_str());
		return;
	}

	// iterate file contents
	char t_Command[128];
	while (fscanf(t_OBJFile,"%s",t_Command)!=EOF)
	{
		// process value prefix
		// position prefix
		if (!strcmp(t_Command,"v"))
		{
			glm::vec3 t_Position;
			uint32_t __eof = fscanf(t_OBJFile,"%f %f %f\n",&t_Position.x,&t_Position.y,&t_Position.z);
			t_Positions.push_back(t_Position);
		}

		// uv coordinate prefix
		else if (!strcmp(t_Command,"vt"))
		{
			glm::vec2 t_UVCoordinate;
			uint32_t __eof = fscanf(t_OBJFile,"%f %f\n",&t_UVCoordinate.x,&t_UVCoordinate.y);
			t_UVCoordinates.push_back(t_UVCoordinate);
		}

		// normal prefix
		else if (!strcmp(t_Command,"vn"))
		{
			glm::vec3 t_Normal;
			uint32_t __eof = fscanf(t_OBJFile,"%f %f %f\n",&t_Normal.x,&t_Normal.y,&t_Normal.z);
			t_Normals.push_back(t_Normal);
		}

		// face prefix
		else if (!strcmp(t_Command,"f"))
		{
			// extract face indices
			uint32_t t_PositionIndex[3],t_UVIndex[3],t_NormalIndex[3];
			uint32_t __eof = fscanf(
					t_OBJFile,"%u/%u/%u %u/%u/%u %u/%u/%u\n",
					&t_PositionIndex[0],&t_UVIndex[0],&t_NormalIndex[0],
					&t_PositionIndex[1],&t_UVIndex[1],&t_NormalIndex[1],
					&t_PositionIndex[2],&t_UVIndex[2],&t_NormalIndex[2]
				);

			// insert face indices
			for (uint8_t i=0;i<3;i++) t_PositionIndices.push_back(t_PositionIndex[i]);
			for (uint8_t i=0;i<3;i++) t_UVIndices.push_back(t_UVIndex[i]);
			for (uint8_t i=0;i<3;i++) t_NormalIndices.push_back(t_NormalIndex[i]);
		}
	}
	fclose(t_OBJFile);

	// iterate faces & write vertex
	mesh_vertices.reserve(mesh_vertices.size()+t_PositionIndices.size());
	for (uint32_t i=0;i<t_PositionIndices.size();i+=3)
	{
		for (uint8_t j=0;j<3;j++)
		{
			uint32_t n = i+j;
			MeshUpload t_VertexComponent = {
				.position = t_Positions[t_PositionIndices[n]-1],
				.uv_coord = t_UVCoordinates[t_UVIndices[n]-1],
				.normal = t_Normals[t_NormalIndices[n]-1]
			};
			mesh_vertices.push_back(t_VertexComponent);
		}

		// precalculate tangent for normal mapping
		// setup values
		glm::vec3 t_e0 = mesh_vertices[i+1].position-mesh_vertices[i].position;
		glm::vec3 t_e1 = mesh_vertices[i+2].position-mesh_vertices[i].position;
		glm::vec2 t_duv0 = mesh_vertices[i+1].uv_coord-mesh_vertices[i].uv_coord;
		glm::vec2 t_duv1 = mesh_vertices[i+2].uv_coord-mesh_vertices[i].uv_coord;
		float t_Factor = 1.f/(t_duv0.y*t_duv1.y-t_duv1.x*t_duv0.y);

		// calculate tangent & store in vertex
		glm::vec3 t_Tangent = glm::vec3(
				t_Factor*(t_duv1.y*t_e0.x-t_duv0.y*t_e1.x),
				t_Factor*(t_duv1.y*t_e0.y-t_duv0.y*t_e1.y),
				t_Factor*(t_duv1.y*t_e0.z-t_duv0.y*t_e1.z)
			);
		t_Tangent = glm::normalize(t_Tangent);
		for (uint8_t j=0;j<3;j++) mesh_vertices[i+j].tangent = t_Tangent;
		// FIXME: how the hell is the tangent creating a memory leak?!? it's a mathematical operation!
		//		could be an old version, test on a more modern system (no offense, computer i'm writing this on)
		// TODO: using a matrix calculation might be significantly faster
	}
}
// FIXME: it was written [-NAS] a long time ago, there are some things to optimize here

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
		bool t_IncAnim = p_Animation.anim_progression<p_Animation.cycle_duration;
		p_Animation.anim_progression += t_IncAnim-p_Animation.anim_progression*!t_IncAnim;
		// TODO: make this update depend on frame update delta
		//		test with unlocked frames to make sure this is actually working
		//		-> also do the same for sprite instance animation update

		// calculate spritesheet location
		int t_Index = p_Animation.anim_progression/p_Animation.frame_duration;
		p_Sprite.atlas_index = glm::vec2(t_Index%p_Texture.columns,t_Index/p_Texture.columns);
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
			bool t_IncAnim = p_Animation.anim_progressions[i]<p_Animation.cycle_duration;
			p_Animation.anim_progressions[i] += t_IncAnim-p_Animation.anim_progressions[i]*!t_IncAnim;
			int t_Index = p_Animation.anim_progressions[i]/p_Animation.frame_duration;
			p_Instance.upload[i].atlas_index = glm::vec2(t_Index%p_Texture.columns,t_Index/p_Texture.columns);
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
	Shader t_SpriteVertexShader = Shader("./shader/obj/sprite.vs",GL_VERTEX_SHADER);
	Shader t_DuplicateVertexShader = Shader("./shader/obj/duplicate.vs",GL_VERTEX_SHADER);
	Shader t_MeshVertexShader = Shader("./shader/obj/mesh.vs",GL_VERTEX_SHADER);
	Shader t_FramebufferVertexShader = Shader("./shader/standard/framebuffer.vs",GL_VERTEX_SHADER);

	// fragment shaders
	Shader t_DirectFragmentShader = Shader("./shader/standard/direct.fs",GL_FRAGMENT_SHADER);
	Shader t_MeshFragmentShader = Shader("./shader/obj/mesh.fs",GL_FRAGMENT_SHADER);
	Shader t_DeferredFragmentShader = Shader("./shader/lighting/pbr.fs",GL_FRAGMENT_SHADER);

	COMM_LOG("assemble shader pipelines");
	m_SpritePipeline.assemble(t_SpriteVertexShader,t_DirectFragmentShader);
	m_DuplicatePipeline.assemble(t_DuplicateVertexShader,t_DirectFragmentShader);
	for (uint8_t i=0;i<RENDERER_BATCHES_COUNT;i++)
	{
		batches[i].mesh_pipeline.assemble(t_MeshVertexShader,t_MeshFragmentShader);
		batches[i].mesh_pipeline.enable();
		batches[i].mesh_pipeline.upload_int("colour_map",0);
		batches[i].mesh_pipeline.upload_int("normal_map",1);
		batches[i].mesh_pipeline.upload_int("material_map",2);
		batches[i].mesh_pipeline.upload_int("emission_map",3);
	}
	m_DeferredPipeline.assemble(t_FramebufferVertexShader,t_DeferredFragmentShader);

	COMM_LOG("pre-loading basic sprite geometry");
	float t_SpriteVertices[] = {
		-.5f,.5f,.0f,.0f, .5f,-.5f,1.f,1.f, .5f,.5f,1.f,.0f,
		.5f,-.5f,1.f,1.f, -.5f,.5f,.0f,.0f, -.5f,-.5f,.0f,1.f
	};
	m_SpriteBuffer.bind();
	m_SpriteBuffer.upload_vertices(t_SpriteVertices,PATTERN_SPRITE_TRIANGLE_REPEAT*sizeof(float));
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
	float t_CanvasVertices[] = {
		-1.f,1.f,.0f,1.f, 1.f,-1.f,1.f,.0f, 1.f,1.f,1.f,1.f,
		1.f,-1.f,1.f,.0f, -1.f,1.f,.0f,1.f, -1.f,-1.f,.0f,.0f
	};
	m_CanvasBuffer.bind();
	m_CanvasBuffer.upload_vertices(t_CanvasVertices,PATTERN_SPRITE_TRIANGLE_REPEAT*sizeof(float));

	COMM_LOG("setup shader pipeline for deferred lighting system");
	m_DeferredPipeline.enable();
	m_DeferredPipeline.point_buffer2D();
	m_DeferredPipeline.upload_int("gbuffer_colour",0);
	m_DeferredPipeline.upload_int("gbuffer_position",1);
	m_DeferredPipeline.upload_int("gbuffer_normals",2);
	m_DeferredPipeline.upload_int("gbuffer_materials",3);

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
	uint8_t t_Rows = 1,t_Cols = 1,t_Frames = 1;
	if (args.size()>Args::Frames)
	{
		t_Rows = stoi(args[Args::Rows]);
		t_Cols = stoi(args[Args::Columns]);
		t_Frames = stoi(args[Args::Frames]);
	}

	// write texture
	batch->add_sprite(args[Args::TexPath],t_Rows,t_Cols,t_Frames);
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
	uint16_t t_TextureID = stoi(args[Args::TexID]);
	glm::vec2 t_Position = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
	float t_Width = stof(args[Args::Width]),t_Height = stof(args[Args::Height]);
	// TODO: error messaging when conversion fails

	// register sprite or animation based on argument length
	if (args.size()>Args::AnimDuration)
	{
		uint8_t t_Duration = stoi(args[Args::AnimDuration]);
		batch->register_sprite(t_TextureID,t_Position,t_Width,t_Height,t_Duration);
		return;
	}
	batch->register_sprite(t_TextureID,t_Position,t_Width,t_Height);
}

void interpreter_logic_instanced_sprite(RenderBatch* batch,const std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,TexID,PosX,PosY,Width,Height,AnimDuration };

	COMM_LOG_COND(
			args.size()>Args::Height,
			"register instanced sprite%s of texture %s: pos -> (%s,%s),dim -> %sx%s",
			(args.size()>AnimDuration) ? " animation" : "",
			args[Args::TexID].c_str(),
			args[Args::PosX].c_str(),args[Args::PosY].c_str(),
			args[Args::Width].c_str(),args[Args::Height].c_str()
		)
		COMM_ERR_FALLBACK("duplicate registration: not enough arguments provided");

	// arguments to variables
	uint16_t t_TextureID = stoi(args[Args::TexID]);
	glm::vec2 t_Position = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
	float t_Width = stof(args[Args::Width]),t_Height = stof(args[Args::Height]);

	// register animated sprite instance based on argument length
	if (args.size()>Args::AnimDuration)
	{
		uint8_t t_Duration = stoi(args[Args::AnimDuration]);
		batch->register_duplicates(t_TextureID,t_Position,t_Width,t_Height,t_Duration);
		return;
	}
	batch->register_duplicates(t_TextureID,t_Position,t_Width,t_Height);
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
	glm::vec3 t_Position = glm::vec3(stof(args[Args::PosX]),stof(args[Args::PosY]),stof(args[Args::PosZ]));
	float t_Scale = 1;
	glm::vec3 t_Rotation = glm::vec3(0);

	// check if scale and rotation is specified before adding mesh
	if (args.size()>Args::RotZ)
	{
		t_Scale = stof(args[Args::Scl]);
		t_Rotation = glm::vec3(stof(args[Args::RotX]),stof(args[Args::RotY]),stof(args[Args::RotZ]));
	}
	batch->add_mesh(
			args[Args::ObjPath],args[Args::TexPath],
			args[Args::NormPath],args[Args::MatPath],args[Args::EmitPath],
			t_Position,t_Scale,t_Rotation
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
		uint16_t t_InstanceID = stoi(args[Args::InstID]);
		glm::vec2 t_Offset = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
		glm::vec2 t_Scale = glm::vec2(1.f);
		float t_Rotation = .0f;
		glm::vec2 t_SubTexture = glm::vec2(0);

		// optionally extract rotation and spritesheet index & spawn
		if (args.size()>Args::SubtexRow)
		{
			t_Scale = glm::vec2(stof(args[Args::SclX]),stof(args[Args::SclY]));
			t_Rotation = stof(args[Args::Rotation]);
			t_SubTexture = glm::vec2(stoi(args[Args::SubtexCol]),stoi(args[Args::SubtexRow]));
		}
		batch->spawn_sprite_instance(t_InstanceID,t_Offset,t_Scale,t_Rotation,t_SubTexture);
	}

	// process mesh instance spawn request
	else if (args[Args::Type]=="mesh")
	{
		// TODO
	}

	else
	{
		COMM_ERR("cannot spawn instance of %s",args[Args::Type].c_str());
	}
}
// TODO use argument enumeration

void interpreter_logic_syntax_error(RenderBatch* batch,const std::vector<std::string>& args)
{
	COMM_ERR("syntax error while interpreting %s: \"%s\" not a valid command",
			batch->path.c_str(),args[0].c_str());
}

constexpr uint8_t RENDERER_INTERPRETER_COMMAND_COUNT = 5;
const std::string gfx_command_correlation[RENDERER_INTERPRETER_COMMAND_COUNT] = {
	"texture","sprite","duplicate","mesh","spawn"
};
typedef void (*gfx_interpreter_logic)(RenderBatch*,const std::vector<std::string>&);
const gfx_interpreter_logic cmd_handler[RENDERER_INTERPRETER_COMMAND_COUNT+1] = {
	interpreter_logic_texture,
	interpreter_logic_sprite,
	interpreter_logic_instanced_sprite,
	interpreter_logic_mesh,
	interpreter_logic_spawn_instanced,
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
	std::ifstream t_CCBFile(batch->path,std::ios::in);
	std::string t_CommandLine;
	while (getline(t_CCBFile,t_CommandLine))
	{
		// split command and its arguments, interrupt when empty command
		std::vector<std::string> t_Args = Toolbox::split_string(t_CommandLine,' ');
		if (!t_Args.size()) continue;

		// correlate command
		uint8_t i = 0;
		while (i<RENDERER_INTERPRETER_COMMAND_COUNT&&t_Args[0]!=gfx_command_correlation[i]) i++;

		// call command
		cmd_handler[i](batch,t_Args);
	}
	// FIXME: reading FILE* by fscan pattern could be a lot more useful in this case

	// close file and ready batch load
	t_CCBFile.close();
	COMM_MSG(LOG_BLUE,"-> interpretation end");

	// load geometry
	COMM_AWT("loading geometry of %li meshes",batch->meshes.size());
	for (Mesh& m : batch->meshes) batch->load_mesh(m.path);
	COMM_CNF();

	// load sprite textures
	COMM_AWT("sprites: streaming %li textures",batch->sprite_textures.size());
	for (SpriteTextureTuple& t_Texture : batch->sprite_textures) t_Texture.texture.load();
	COMM_CNF();

	// load mesh textures
	COMM_AWT("meshes: streaming %li textures",batch->mesh_textures.size()*4);
	for (MeshTextureTuple& t_Texture : batch->mesh_textures)
	{
		t_Texture.colours.load();
		t_Texture.normals.load();
		t_Texture.materials.load();
		t_Texture.emission.load();
	}

	g_Renderer.gpu_update_pointers.push_back(batch);
	COMM_CNF();
}

RenderBatch* Renderer::load(const std::string& path)
{
	// find available batch by unix approach: last batch will be overwritten if no idle
	uint8_t t_BatchID = 0;
	while (t_BatchID<(RENDERER_BATCHES_COUNT-1)&&g_Renderer.batches[t_BatchID].selected) t_BatchID++;

	// store path in free batch & signal batch load
	RenderBatch* r_Batch = &batches[t_BatchID];
	r_Batch->path = path;

	// communicate selection
	COMM_LOG("batch %i selected for writing",t_BatchID);
	COMM_ERR_COND(r_Batch->selected,"CAREFUL! batch not in idle, overflow buffer selected!");
	r_Batch->selected = true;

	// start background loading
	std::thread t_LoadThread(&bgr_load_batch,r_Batch);
	t_LoadThread.detach();
	return r_Batch;
}


/*
	TODO
*/
void sprite_upload(RenderBatch* batch,ShaderPipeline* pipeline)
{
	COMM_AWT("attempting to upload %li sprite textures to gpu",batch->sprite_textures.size());
	
	// load textures
	std::chrono::steady_clock::time_point t_StreamTime = std::chrono::steady_clock::now();
	while (batch->sprite_upload_head<batch->sprite_textures.size())
	{
		// check timing for stall until next frame when upload takes too long
		if ((std::chrono::steady_clock::now()-t_StreamTime).count()*CONVERSION_MULT_MILLISECONDS>1.f)
		{
			COMM_CNF();
			return;
		}

		// upload texture to gpu
		SpriteTextureTuple& t_Texture = batch->sprite_textures[batch->sprite_upload_head];
		t_Texture.texture.upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		Texture::generate_mipmap();
		batch->sprite_upload_head++;
	}

	batch->sprite_ready = true;
	COMM_CNF();
}
// TODO: automatically allocate time budget based on current workload
//		background texture streaming while render

void sprite_update(RenderBatch* batch,ShaderPipeline* pipeline)
{
	// update registered animations
	batch->update_sprites();

	// iterate registered sprites
	for (Sprite& t_Sprite : batch->sprites)
	{
		SpriteTextureTuple& t_Texture = batch->sprite_textures[t_Sprite.texture_id];

		// upload sprite attributes
		pipeline->upload_int("row",t_Texture.rows);
		pipeline->upload_int("col",t_Texture.columns);
		pipeline->upload_vec2("i_tex",t_Sprite.atlas_index);
		pipeline->upload_matrix("model",t_Sprite.transform.model);

		// draw sprite
		t_Texture.texture.bind();
		glDrawArrays(GL_TRIANGLES,0,6);
	}
}

typedef void (*sprite_update_routine)(RenderBatch*,ShaderPipeline*);
sprite_update_routine update_sprites[2] = { sprite_upload,sprite_update };


/*
	TODO
*/
void mesh_upload(RenderBatch* batch)
{
	COMM_AWT("attempting to upload %li mesh textures to gpu",batch->mesh_textures.size()*4);

	// ready mesh buffer
	batch->mesh_buffer.upload_vertices(batch->mesh_vertices);
	batch->mesh_pipeline.point_buffer3D();

	// upload textures
	for (MeshTextureTuple& t_Texture : batch->mesh_textures)
	{
		t_Texture.colours.upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		Texture::generate_mipmap();
		t_Texture.normals.upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		Texture::generate_mipmap();
		t_Texture.materials.upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		Texture::generate_mipmap();
		t_Texture.emission.upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		Texture::generate_mipmap();
	}
	batch->mesh_ready = true;
	// TODO: stall texture upload

	COMM_CNF();
}

void mesh_update(RenderBatch* batch)
{
	// upload camera transform & iterate meshes
	batch->mesh_pipeline.upload_camera(g_Camera3D);
	for (uint16_t i=0;i<batch->meshes.size();i++)
	{
		Mesh& t_Mesh = batch->meshes[i];
		MeshTextureTuple& t_Texture = batch->mesh_textures[i*4];

		// upload attributes
		batch->mesh_pipeline.upload_matrix("model",t_Mesh.transform.model);

		// upload textures
		t_Texture.colours.bind();
		glActiveTexture(GL_TEXTURE1);
		t_Texture.normals.bind();
		glActiveTexture(GL_TEXTURE2);
		t_Texture.materials.bind();
		glActiveTexture(GL_TEXTURE3);
		t_Texture.emission.bind();
		glActiveTexture(GL_TEXTURE0);

		// draw meshes
		glDrawArrays(GL_TRIANGLES,0,batch->mesh_vertices.size());
	}
}

typedef void (*mesh_update_routine)(RenderBatch*);
mesh_update_routine update_meshes[2] = { mesh_upload,mesh_update };


/*
	TODO
*/
void Renderer::update()
{
	// rendering 3D geometry
	// deferred 3D setup
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	m_GBuffer.bind();

	// draw 3D geometry
	Frame::clear();
	for (RenderBatch* batch : gpu_update_pointers)
	{
		batch->mesh_buffer.bind();
		batch->mesh_pipeline.enable();
		update_meshes[batch->mesh_ready](batch);
	}

	// post processing
	// 2D setup
	FrameBuffer::unbind();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// deferred shading
	m_CanvasBuffer.bind();
	m_DeferredPipeline.enable();
	m_GBuffer.upload_components();
	glDrawArrays(GL_TRIANGLES,0,6);

	// render 2D geometry
	// iterate sprite render
	m_SpriteBuffer.bind();
	m_SpritePipeline.enable();
	for (RenderBatch* batch : gpu_update_pointers) update_sprites[batch->sprite_ready](batch,&m_SpritePipeline);

	// iterate duplicate render
	m_SpriteBuffer.bind_index();
	m_DuplicatePipeline.enable();
	for (RenderBatch* batch : gpu_update_pointers) render_duplicates(batch);
}

/*
	TODO
*/
void Renderer::render_duplicates(RenderBatch* batch)
{
	// skip render if sprite textures are not ready yet
	if (!batch->sprite_ready) return;

	// iterate registered duplicates
	for (SpriteInstance& t_Instance : batch->duplicates)
	{
		SpriteTextureTuple& t_Texture = batch->sprite_textures[t_Instance.texture_id];

		// upload instance attributes & data
		m_SpriteBuffer.upload_indices(t_Instance.upload,SPRITE_INSTANCE_CAPACITY*sizeof(SpriteInstanceUpload));
		m_DuplicatePipeline.upload_int("row",t_Texture.rows);
		m_DuplicatePipeline.upload_int("col",t_Texture.columns);
		m_DuplicatePipeline.upload_matrix("model",t_Instance.transform.model);

		// draw duplicates
		t_Texture.texture.bind();
		glDrawArraysInstanced(GL_TRIANGLES,0,6,t_Instance.active_range);
	}
}
