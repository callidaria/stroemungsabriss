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
uint16_t RenderBatch::add_sprite(const char* path,uint8_t rows,uint8_t cols,uint8_t frames)
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
	textures.push_back(tuple);
	return textures.size()-1;
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
		.id = sprites.size(),
		.cycle_duration = dur,
		.frame_duration = (float)dur/textures[tex_id].frames,
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
		.id = duplicates.size(),
		.cycle_duration = dur,
		.frame_duration = (float)dur/textures[tex_id].frames,
	};

	// register duplicate & animation
	anim_duplicates.push_back(anim);
	register_duplicates(tex_id,pos,wdt,hgt);
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


/**
 * TODO: expand
*/

/*
	TODO
*/
Renderer::Renderer()
{
	COMM_MSG(LOG_HEADINGS,"renderer setup...");

	// preload sprite data
	// generate sprite vertex data
	COMM_LOG("pre-loading basic geometry");
	float vertices[] = {
		-.5f,.5f,.0f,.0f, .5f,-.5f,1.f,1.f, .5f,.5f,1.f,.0f,
		.5f,-.5f,1.f,1.f, -.5f,.5f,.0f,.0f, -.5f,-.5f,.0f,1.f
	};

	// setup vertex & element buffer for sprites
	spr_buffer.bind();
	spr_buffer.upload_vertices(vertices,PATTERN_SPRITE_TRIANGLE_REPEAT*sizeof(float));

	// setup sprite shader
	COMM_LOG("shader preparation for sprites");
	spr_shader.compile2d("./shader/obj/sprite.vs","./shader/standard/direct.fs");
	spr_shader.upload_int("tex",0);
	spr_shader.upload_camera();

	// compile classical instance shader program
	// TODO: ??maybe find a different way of representing instanced rotation??
	// precalculating sine & cosine for a matrix 2D seems like the most performant way of doing this
	// ??uploading i_tex for all instances using this shader leaves a lot of 0s for single textures
	// we could make instanced_anim the only instanced object or find a different solution??
	COMM_LOG("shader preparation for sprite duplicates");
	dpl_shader.compile2d("./shader/obj/duplicate.vs","./shader/standard/direct.fs");

	// sprite duplication shader upload pattern
	spr_buffer.add_buffer();
	spr_buffer.bind_index();
	dpl_shader.def_indexF("offset",2,0,INSTANCE_SHADER_UPLOAD_REPEAT);
	dpl_shader.def_indexF("scale",2,2,INSTANCE_SHADER_UPLOAD_REPEAT);
	dpl_shader.def_indexF("rotation",1,4,INSTANCE_SHADER_UPLOAD_REPEAT);
	dpl_shader.def_indexF("i_tex",2,5,INSTANCE_SHADER_UPLOAD_REPEAT);

	// sprite duplication shader initial attributes
	dpl_shader.upload_int("tex",0);
	dpl_shader.upload_camera();

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
 *	TODO register meshes
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
constexpr uint8_t RENDERER_INTERPRETER_COMMAND_COUNT = 4;
const std::string gfxcmd[RENDERER_INTERPRETER_COMMAND_COUNT] = { "texture","sprite","duplicate","spawn" };

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
	const char* path = args[Args::TexPath].c_str();
	uint8_t rows = 1, cols = 1, frames = 1;

	// check for texture atlas information
	if (args.size()>Args::Frames)
	{
		rows = stoi(args[Args::Rows]);
		cols = stoi(args[Args::Columns]);
		frames = stoi(args[Args::Frames]);
	}

	// write texture
	batch->add_sprite(path,rows,cols,frames);
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
	interpreter_logic_texture,interpreter_logic_sprite,interpreter_logic_instanced_sprite,
	interpreter_logic_spawn_instanced,
	interpreter_logic_syntax_error
};


/**
 *	TODO
*/

typedef void (*batch_routine)(RenderBatch&);

/*
	TODO
*/
void batch_idle(RenderBatch& batch)
{
	// TODO: what even do here? maintaining checks, idle update, can this be skipped?
	//		buffers definitely will be in this state and the update has to be handled accordingly (no ee)
}

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
	COMM_AWT("streaming %li textures",batch->textures.size());
	for (SpriteTextureTuple& t : batch->textures) t.texture.load();
	COMM_CNF();

	// unlock batch
	batch->load_semaphore = false;
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
	COMM_AWT("uploading %li textures to gpu",batch.textures.size());
	for (SpriteTextureTuple& t : batch.textures)
	{
		t.texture.upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		Texture::generate_mipmap();
	}
	COMM_CNF();

	// proceed to render state
	batch.state = RBFR_RENDER;
}
// TODO: automatically allocate time budget based on current workload
//		background texture streaming while render

/*
	TODO
*/
void batch_render(RenderBatch& batch)
{
	// update processing
	// iterate sprite animation updates
	for (SpriteAnimation& ta : batch.anim_sprites)
	{
		Sprite& ts = batch.sprites[ta.id];
		SpriteTextureTuple& tt = batch.textures[ts.texture_id];

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

	// iterate duplicate animation update
	for (SpriteAnimationInstance& ta : batch.anim_duplicates)
	{
		SpriteInstance& ti = batch.duplicates[ta.id];
		SpriteTextureTuple& tt = batch.textures[ti.texture_id];

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

	// rendering geometry
	// enter transparency section
	glEnable(GL_BLEND);
	// FIXME: join transparent/deferred batch sections together to switch only once per frame

	// ready sprite buffer & shader
	g_Renderer.spr_buffer.bind();
	g_Renderer.spr_shader.enable();

	// iterate sprites
	for (uint16_t i=0;i<batch.sprites.size();i++)
	{
		Sprite& ts = batch.sprites[i];
		SpriteTextureTuple& tt = batch.textures[ts.texture_id];

		// upload sprite attributes
		g_Renderer.spr_shader.upload_int("row",tt.rows);
		g_Renderer.spr_shader.upload_int("col",tt.columns);
		g_Renderer.spr_shader.upload_vec2("i_tex",ts.atlas_index);
		g_Renderer.spr_shader.upload_matrix("model",ts.transform.model);

		// draw sprite
		batch.textures[ts.texture_id].texture.bind();
		glDrawArrays(GL_TRIANGLES,0,6);
	}

	// ready instance shader, instances will be using sprite buffer due to data overlap
	g_Renderer.spr_buffer.bind_index();
	g_Renderer.dpl_shader.enable();

	// iterate duplicates
	for (uint16_t i=0;i<batch.duplicates.size();i++)
	{
		SpriteInstance& ti = batch.duplicates[i];
		SpriteTextureTuple& tt = batch.textures[ti.texture_id];

		// upload instance attributes & data
		g_Renderer.spr_buffer.upload_indices(ti.upload,INSTANCE_CAPACITY*sizeof(SpriteInstanceUpload));
		g_Renderer.dpl_shader.upload_int("row",tt.rows);
		g_Renderer.dpl_shader.upload_int("col",tt.columns);
		g_Renderer.dpl_shader.upload_matrix("model",ti.transform.model);

		// draw instances
		batch.textures[ti.texture_id].texture.bind();
		glDrawArraysInstanced(GL_TRIANGLES,0,6,ti.active_range);
	}
}

// behaviours mapped towards bufferstate enumeration
batch_routine batch_routines[RBFR_STATE_COUNT] = { batch_idle,batch_load,batch_upload,batch_render };

/*
	TODO
*/
void Renderer::update()
{
	// setup sprite buffers
	spr_shader.enable();
	spr_buffer.bind();

	// iterate sprite buffer
	for (uint8_t i=0;i<RENDERER_BATCHES_COUNT;i++)
	{
		RenderBatch& batch = batches[i];
		batch_routines[batch.state](batch);
	}
}
