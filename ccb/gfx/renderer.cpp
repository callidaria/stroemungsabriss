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
	SpriteTextureTuple tuple = {

		// source
		.texture = Texture(path),

		// spritesheet segmentation
		.rows = rows,
		.columns = cols,
		.frames = frames
	};

	// write and return reference id
	sprite_textures.push_back(tuple);
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
	Sprite sprite = {

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
	sprite.transform.to_origin();
	sprites.push_back(sprite);
}

/*
	TODO
*/
void RenderBatch::register_sprite(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,uint8_t dur)
{
	// add animation data
	SpriteAnimation anim = {
		.id = (uint16_t)sprites.size(),
		.cycle_duration = dur,
		.frame_duration = (float)dur/sprite_textures[tex_id].frames,
	};

	// register sprite & animation
	anim_sprites.push_back(anim);
	register_sprite(tex_id,pos,wdt,hgt);
}

/*
	TODO
*/
void RenderBatch::register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt)
{
	SpriteInstance instance = {

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
	instance.transform.to_origin();
	duplicates.push_back(instance);
}

/*
	TODO
*/
void RenderBatch::register_duplicates(uint16_t tex_id,glm::vec2 pos,float wdt,float hgt,uint8_t dur)
{
	// add animation data
	SpriteAnimationInstance anim = {
		.id = (uint16_t)duplicates.size(),
		.cycle_duration = dur,
		.frame_duration = (float)dur/sprite_textures[tex_id].frames,
	};

	// register duplicate & animation
	anim_duplicates.push_back(anim);
	register_duplicates(tex_id,pos,wdt,hgt);
}

/*
	TODO
*/
void RenderBatch::add_mesh(std::string obj,std::string tex,std::string norm,std::string mats,std::string emit,
		glm::vec3 pos,float scl,glm::vec3 rot)
{
	// TODO
}

/*
	TODO
*/
void RenderBatch::spawn_sprite_instance(uint16_t inst_id,glm::vec2 ofs,glm::vec2 scl,float rot,glm::vec2 subtex)
{
	SpriteInstance& t_instance = duplicates[inst_id];
	SpriteInstanceUpload& t_upload = t_instance.upload[t_instance.active_range];

	// setup transform
	t_upload.offset = ofs;
	t_upload.scale = scl;
	t_upload.rotation = rot;
	t_upload.atlas_index = subtex;

	// increase spawn counter
	t_instance.active_range++;
}

/*
	TODO
*/
void RenderBatch::update_sprites()
{
	for (SpriteAnimation& ta : anim_sprites)
	{
		Sprite& ts = sprites[ta.id];
		SpriteTextureTuple& tt = sprite_textures[ts.texture_id];

		// calculate current frame
		bool inc_anim = ta.anim_progression<ta.cycle_duration;
		ta.anim_progression += inc_anim-ta.anim_progression*!inc_anim;
		// TODO: make this update depend on frame update delta
		//		test with unlocked frames to make sure this is actually working
		//		-> also do the same for sprite instance animation update

		// calculate spritesheet location
		int index = ta.anim_progression/ta.frame_duration;
		ts.atlas_index = glm::vec2(index%tt.columns,index/tt.columns);
	}
}

/*
	TODO
*/
void RenderBatch::update_duplicates()
{
	for (SpriteAnimationInstance& ta : anim_duplicates)
	{
		SpriteInstance& ti = duplicates[ta.id];
		SpriteTextureTuple& tt = sprite_textures[ti.texture_id];

		// calculate frames for all active instances
		for (uint16_t i=0;i<ti.active_range;i++)
		{
			bool inc_anim = ta.anim_progressions[i]<ta.cycle_duration;
			ta.anim_progressions[i] += inc_anim-ta.anim_progressions[i]*!inc_anim;
			int index = ta.anim_progressions[i]/ta.frame_duration;
			ti.upload[i].atlas_index = glm::vec2(index%tt.columns,index/tt.columns);
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

	// sprite initialization
	// generate sprite vertex data
	COMM_LOG("pre-loading basic sprite geometry");
	float sprite_vertices[] = {
		-.5f,.5f,.0f,.0f, .5f,-.5f,1.f,1.f, .5f,.5f,1.f,.0f,
		.5f,-.5f,1.f,1.f, -.5f,.5f,.0f,.0f, -.5f,-.5f,.0f,1.f
	};
	spr_buffer.bind();
	spr_buffer.upload_vertices(sprite_vertices,PATTERN_SPRITE_TRIANGLE_REPEAT*sizeof(float));

	// setup sprite shader
	COMM_LOG("compile shader for sprites");
	spr_shader.compile2d("./shader/obj/sprite.vs","./shader/standard/direct.fs");
	spr_shader.upload_int("tex",0);
	spr_shader.upload_camera();

	// compile classical instance shader program
	COMM_LOG("compile shader for sprite duplicates");
	dpl_shader.compile2d("./shader/obj/duplicate.vs","./shader/standard/direct.fs");
	spr_buffer.add_buffer();
	spr_buffer.bind_index();
	dpl_shader.def_indexF("offset",2,0,SPRITE_INSTANCE_UPLOAD_REPEAT);
	dpl_shader.def_indexF("scale",2,2,SPRITE_INSTANCE_UPLOAD_REPEAT);
	dpl_shader.def_indexF("rotation",1,4,SPRITE_INSTANCE_UPLOAD_REPEAT);
	dpl_shader.def_indexF("i_tex",2,5,SPRITE_INSTANCE_UPLOAD_REPEAT);

	// sprite duplication shader initial attributes
	dpl_shader.upload_int("tex",0);
	dpl_shader.upload_camera();

	// screen space
	// setup gbuffer
	COMM_LOG("setup material buffer and generate its components");
	m_gbuffer.bind();
	m_gbuffer.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight);
	m_gbuffer.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight,true,1);
	m_gbuffer.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight,true,2);
	m_gbuffer.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight,true,3);
	m_gbuffer.add_depth_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight);
	m_gbuffer.combine_attachments();  // FIXME: move to construction and remove
	FrameBuffer::unbind();

	// generate canvas vertex data
	COMM_LOG("pre-loading basic canvas geometry");
	float canvas_vertices[] = {
		-1.f,1.f,.0f,.0f, 1.f,-1.f,1.f,1.f, 1.f,1.f,1.f,.0f,
		1.f,-1.f,1.f,1.f, -1.f,1.f,.0f,.0f, -1.f,-1.f,.0f,1.f
	};
	m_canvas_buffer.bind();
	m_canvas_buffer.upload_vertices(canvas_vertices,PATTERN_SPRITE_TRIANGLE_REPEAT*sizeof(float));

	// setup deferred lighting shader
	COMM_LOG("compile shader for deferred lighting systems");
	m_deferred_shader.compile2d("./shader/standard/framebuffer.vs","./shader/lighting/pbr.fs");
	m_deferred_shader.upload_int("gbuffer_colour",0);
	m_deferred_shader.upload_int("gbuffer_position",1);
	m_deferred_shader.upload_int("gbuffer_normals",2);
	m_deferred_shader.upload_int("gbuffer_materials",3);

	// setup framebuffer for deferred shading
	m_deferred.bind();
	m_deferred.add_colour_component(g_Config.vFrameResolutionWidth,g_Config.vFrameResolutionHeight);
	FrameBuffer::unbind();

	COMM_SCC("renderer ready");
}

/*
	TODO
*/
RenderBatch* Renderer::load(std::string path)
{
	// find available batch by unix approach: last batch will be overwritten if no idle
	uint8_t batch_id = 0;
	while (batch_id<(RENDERER_BATCHES_COUNT-1)&&g_Renderer.batches[batch_id].state!=RBFR_IDLE) batch_id++;

	// store path in free batch & signal batch load
	RenderBatch* batch = &batches[batch_id];
	batch->path = path;

	// communicate selection
	COMM_LOG("batch %i selected for writing",batch_id);
	COMM_ERR_COND(batch->state!=RBFR_IDLE,"CAREFUL! batch not in idle, overflow buffer selected!");

	// activate and return batch
	batch->state = RBFR_LOAD;
	return batch;
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

// loader definition command list
constexpr uint8_t RENDERER_INTERPRETER_COMMAND_COUNT = 5;
const std::string gfxcmd[RENDERER_INTERPRETER_COMMAND_COUNT] = {
	"texture","sprite","duplicate","mesh","spawn"
};

/*
	TODO
*/
void interpreter_logic_texture(RenderBatch* batch,std::vector<std::string>& args)
{
	enum Args : uint8_t { Command,TexPath,Rows,Columns,Frames };

	COMM_LOG_COND(
			args.size()>Args::TexPath,
			"request %s: %s",(args.size()>Args::Frames) ? "spritesheet" : "texture",args[Args::TexPath].c_str()
		)
		COMM_ERR_FALLBACK("texture request: not enough arguments provided");

	// arguments to variables
	uint8_t rows = 1, cols = 1, frames = 1;

	// check for texture atlas information
	if (args.size()>Args::Frames)
	{
		rows = stoi(args[Args::Rows]);
		cols = stoi(args[Args::Columns]);
		frames = stoi(args[Args::Frames]);
	}

	// write texture
	batch->add_sprite(args[Args::TexPath],rows,cols,frames);
}

/*
	TODO
*/
void interpreter_logic_sprite(RenderBatch* batch,std::vector<std::string>& args)
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
	uint16_t texture_id = stoi(args[Args::TexID]);
	glm::vec2 position = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
	float width = stof(args[Args::Width]), height = stof(args[Args::Height]);
	// TODO: error messaging when conversion fails

	// register sprite or animation based on argument length
	if (args.size()>Args::AnimDuration)
	{
		uint8_t duration = stoi(args[Args::AnimDuration]);
		batch->register_sprite(texture_id,position,width,height,duration);
		return;
	}
	batch->register_sprite(texture_id,position,width,height);
}

/*
	TODO
*/
void interpreter_logic_instanced_sprite(RenderBatch* batch,std::vector<std::string>& args)
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
	uint16_t texture_id = stoi(args[Args::TexID]);
	glm::vec2 position = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
	float width = stof(args[Args::Width]), height = stof(args[Args::Height]);

	// register animated sprite instance based on argument length
	if (args.size()>Args::AnimDuration)
	{
		uint8_t duration = stoi(args[Args::AnimDuration]);
		batch->register_duplicates(texture_id,position,width,height,duration);
		return;
	}
	batch->register_duplicates(texture_id,position,width,height);
}
// FIXME: code duplications

/*
	TODO
*/
void interpreter_logic_mesh(RenderBatch* batch,std::vector<std::string>& args)
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
	glm::vec3 position = glm::vec3(stof(args[Args::PosX]),stof(args[Args::PosY]),stof(args[Args::PosZ]));
	float scale = 1;
	glm::vec3 rotation = glm::vec3(0);

	// check if scale and rotation is specified before adding mesh
	if (args.size()>Args::RotZ)
	{
		scale = stof(args[Args::Scl]);
		rotation = glm::vec3(stof(args[Args::RotX]),stof(args[Args::RotY]),stof(args[Args::RotZ]));
	}
	batch->add_mesh(
			args[Args::ObjPath],args[Args::TexPath],
			args[Args::NormPath],args[Args::MatPath],args[Args::EmitPath],
			position,scale,rotation
		);
}

/*
	TODO
*/
void interpreter_logic_spawn_instanced(RenderBatch* batch,std::vector<std::string>& args)
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
		uint16_t inst_id = stoi(args[Args::InstID]);
		glm::vec2 offset = glm::vec2(stof(args[Args::PosX]),stof(args[Args::PosY]));
		glm::vec2 scale = glm::vec2(1.f);
		float rot = .0f;
		glm::vec2 subtex = glm::vec2(0);

		// optionally extract rotation and spritesheet index & spawn
		if (args.size()>Args::SubtexRow)
		{
			scale = glm::vec2(stof(args[Args::SclX]),stof(args[Args::SclY]));
			rot = stof(args[Args::Rotation]);
			subtex = glm::vec2(stoi(args[Args::SubtexCol]),stoi(args[Args::SubtexRow]));
		}
		batch->spawn_sprite_instance(inst_id,offset,scale,rot,subtex);
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

/*
	TODO
*/
void interpreter_logic_syntax_error(RenderBatch* batch,std::vector<std::string>& args)
{
	COMM_ERR("syntax error while interpreting %s: \"%s\" not a valid command",
			batch->path.c_str(),args[0].c_str());
}

typedef void (*gfx_interpreter_logic)(RenderBatch*,std::vector<std::string>&);
const gfx_interpreter_logic cmd_handler[RENDERER_INTERPRETER_COMMAND_COUNT+1] = {
	interpreter_logic_texture,
	interpreter_logic_sprite,
	interpreter_logic_instanced_sprite,
	interpreter_logic_mesh,
	interpreter_logic_spawn_instanced,
	interpreter_logic_syntax_error
};


/**
 *	TODO
*/

/*
	TODO
*/
void bgr_load_batch(RenderBatch* batch)
{
	// interpret load file
	COMM_LOG("renderer: reading load definition file \"%s\"",batch->path.c_str());

	// open file
	COMM_MSG(LOG_BLUE,"-> interpretation start");
	std::ifstream file(batch->path,std::ios::in);
	std::string cmd_line;
	while (getline(file,cmd_line))
	{
		// split command and its arguments, interrupt when empty command
		std::vector<std::string> args = Toolbox::split_string(cmd_line,' ');
		if (!args.size()) continue;

		// correlate command
		uint8_t i = 0;
		while (i<RENDERER_INTERPRETER_COMMAND_COUNT&&args[0]!=gfxcmd[i]) i++;

		// call command
		cmd_handler[i](batch,args);
	}

	// close file and ready batch load
	file.close();
	COMM_MSG(LOG_BLUE,"-> interpretation end");

	// load textures
	COMM_AWT("streaming %li textures",batch->sprite_textures.size());
	for (SpriteTextureTuple& t : batch->sprite_textures) t.texture.load();
	COMM_CNF();

	// unlock batch & reset
	batch->load_semaphore = false;
	batch->upload_head = 0;
}

/*
	TODO
*/
void batch_load(RenderBatch& batch)
{
	// set semaphore & signal texture gpu upload when ready
	batch.load_semaphore = true;
	batch.state = RBFR_UPLOAD;

	// load pixel data in background
	std::thread ld_thread(&bgr_load_batch,&batch);
	ld_thread.detach();
}

/*
	TODO
*/
void batch_upload(RenderBatch& batch)
{
	// postpone load, when texture data not ready
	if (batch.load_semaphore) return;

	// load textures
	COMM_AWT("attempting to upload %li textures to gpu",batch.sprite_textures.size());
	std::chrono::steady_clock::time_point stime = std::chrono::steady_clock::now();
	while (batch.upload_head<batch.sprite_textures.size())
	{
		// check timing for stall until next frame when upload takes too long
		if ((std::chrono::steady_clock::now()-stime).count()*CONVERSION_MULT_MILLISECONDS>1.f)
		{
			COMM_CNF();
			return;
		}

		// upload texture to gpu
		SpriteTextureTuple& t = batch.sprite_textures[batch.upload_head];
		t.texture.upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		Texture::generate_mipmap();
		batch.upload_head++;
	}

	// proceed to update state
	batch.state = RBFR_READY;
	g_Renderer.draw_pointers.push_back(&batch);

	COMM_CNF();
}
// TODO: automatically allocate time budget based on current workload
//		background texture streaming while render

/*
	TODO
*/
void batch_precalculation(RenderBatch& batch)
{
	batch.update_sprites();
	batch.update_duplicates();
}

// update behaviours mapped towards bufferstate enumeration
typedef void (*batch_routine)(RenderBatch&);
batch_routine batch_update[RBFR_STATE_COUNT] = { batch_load,batch_upload,batch_precalculation };

/*
	TODO
*/
void Renderer::update()
{
	// iterate batch updates
	for (RenderBatch& batch : batches)
	{
		if (batch.state==RBFR_IDLE) continue;
		batch_update[batch.state-1](batch);
	}
	// FIXME: remove idle state

	// rendering 3D geometry
	// deferred 3D setup
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// opening rendertarget
	m_gbuffer.bind();

	// draw 3D geometry
	// TODO

	// post processing
	// 2D setup
	FrameBuffer::unbind();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// deferred shading
	//m_deferred.bind();
	m_canvas_buffer.bind();
	m_deferred_shader.enable();
	m_gbuffer.upload_components();
	glDrawArrays(GL_TRIANGLES,0,6);
	//FrameBuffer::unbind();

	// render 2D geometry
	// iterate sprite render
	spr_buffer.bind();
	spr_shader.enable();
	for (RenderBatch* batch : draw_pointers) render_sprites(batch);

	// iterate duplicate render
	spr_buffer.bind_index();
	dpl_shader.enable();
	for (RenderBatch* batch : draw_pointers) render_duplicates(batch);
}

/*
	TODO
*/
void Renderer::render_sprites(RenderBatch* batch)
{
	// iterate registered sprites
	for (uint16_t i=0;i<batch->sprites.size();i++)
	{
		Sprite& ts = batch->sprites[i];
		SpriteTextureTuple& tt = batch->sprite_textures[ts.texture_id];

		// upload sprite attributes
		spr_shader.upload_int("row",tt.rows);
		spr_shader.upload_int("col",tt.columns);
		spr_shader.upload_vec2("i_tex",ts.atlas_index);
		spr_shader.upload_matrix("model",ts.transform.model);

		// draw sprite
		batch->sprite_textures[ts.texture_id].texture.bind();
		glDrawArrays(GL_TRIANGLES,0,6);
	}
}

/*
	TODO
*/
void Renderer::render_duplicates(RenderBatch* batch)
{
	// iterate registered duplicates
	for (uint16_t i=0;i<batch->duplicates.size();i++)
	{
		SpriteInstance& ti = batch->duplicates[i];
		SpriteTextureTuple& tt = batch->sprite_textures[ti.texture_id];

		// upload instance attributes & data
		spr_buffer.upload_indices(ti.upload,SPRITE_INSTANCE_CAPACITY*sizeof(SpriteInstanceUpload));
		dpl_shader.upload_int("row",tt.rows);
		dpl_shader.upload_int("col",tt.columns);
		dpl_shader.upload_matrix("model",ti.transform.model);

		// draw instances
		batch->sprite_textures[ti.texture_id].texture.bind();
		glDrawArraysInstanced(GL_TRIANGLES,0,6,ti.active_range);
	}
}
