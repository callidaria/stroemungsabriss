#include "renderer.h"


/**
 * TODO: expand
*/

/*
	TODO
*/
void Transform2D::transform(glm::vec2 p,float w,float h,float r)
{
	rotate(r);
	scale(w,h);
	translate(p);
}

/*
	TODO
*/
void Transform2D::transform(glm::vec2 p,float w,float h,float r,glm::vec2 a)
{
	// reset towards arbitrary origin
	model = glm::mat4(1.f);
	translate(a-position);

	// execute transformation
	rotate(r);
	scale(w,h);
	translate(position+p-a);

	// save current rotation
	rot = r;
}

/*
	TODO
*/
void Transform2D::to_origin()
{
	translate(position);
	scale(width,height);
}

/*
	TODO
*/
void Transform2D::scale(float w,float h,glm::vec2 a)
{
	// reset towards arbitrary origin
	model = glm::mat4(1.f);
	translate(a-position);

	// scale from arbitrary position & reconstruct
	rotate(rot);
	scale(w,h);
	translate(position-a);
}

/*
	TODO
*/
void Transform2D::rotate(float r,glm::vec2 a)
{
	// reset towards arbitrary origin
	float t_wfac = model[0][0], t_hfac = model[1][1];
	model = glm::mat4(1.f);
	translate(a-position);

	// change rotation and reset scaling and previous position
	rotate(r);
	scale(t_wfac,t_hfac);

	// reconstruct valid transform state
	translate(position-a);
	rot = r;
}


/*
	TODO
*/
void SpriteInstanceUpload::set_rotation(float dg_rot)
{
	float rd_rot = glm::radians(dg_rot);
	rotation_sin = glm::sin(rd_rot), rotation_cos = glm::cos(rd_rot);
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
	dpl_shader.def_indexF("rotation_sin",1,2,INSTANCE_SHADER_UPLOAD_REPEAT);
	dpl_shader.def_indexF("rotation_cos",1,3,INSTANCE_SHADER_UPLOAD_REPEAT);
	dpl_shader.def_indexF("i_tex",2,4,INSTANCE_SHADER_UPLOAD_REPEAT);

	// sprite duplication shader initial attributes
	dpl_shader.upload_int("tex",0);
	dpl_shader.upload_camera();

	COMM_SCC("renderer ready");
}


/**
 *			Loader Language Definition
 *
 *	TODO describe formatting
 *
 *	request a texture from memory:
 *		-> texture string(texture_path)|
 *		-> ( int(number_of_rows) int(number_of_columns) int(number_of_subtextures))+e|
 *
 *	register sprite to draw:
 *		-> sprite int(texture_id) int(pos_x) int(pos_y) int(width) int(height)|
 *		-> ( int(animation_duration_in_frames))+e
 *
 *	register instance groups:
 *		-> duplicate int(texture_id) int(pos_x) int(pos_y) int(width) int(height)|
 *		-> ( int(animation_duration_in_frames))+e
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
 *	TODO spawn instances
*/
// TODO: remove naming bloat, command can be reduced to single character format

// loader definition command list
const std::string gfxcmd[RENDERER_INTERPRETER_COMMAND_COUNT] = { "texture","sprite","duplicate" };

/*
	TODO
*/
void interpreter_logic_texture(uint8_t batch_id,std::vector<std::string>& args)
{
	COMM_LOG_COND(
			args.size()>1,
			"request texture: %s",args[1].c_str()
		)
		COMM_ERR_FALLBACK("texture request: not enough arguments provided");

	// arguments to variables
	const char* path = args[1].c_str();
	uint8_t rows = 1, cols = 1, frames = 1;

	// check for texture atlas information
	if (args.size()>2) rows = stoi(args[2]), cols = stoi(args[3]), frames = stoi(args[4]);

	// write texture
	g_Renderer.add_sprite(batch_id,path,rows,cols,frames);
}

/*
	TODO
*/
void interpreter_logic_sprite(uint8_t batch_id,std::vector<std::string>& args)
{
	COMM_LOG_COND(
			args.size()>5,
			"register %s of texture %s: pos -> (%s,%s), dim -> %sx%s",
			(args.size()>6) ? "animation" : "sprite",
			args[1].c_str(),args[2].c_str(),args[3].c_str(),args[4].c_str(),args[5].c_str()
		)
		COMM_ERR_FALLBACK("sprite registration: not enough arguments provided");

	// arguments to variables
	uint16_t texture_id = stoi(args[1]);
	glm::vec2 position = glm::vec2(stof(args[2]),stof(args[3]));
	float width = stof(args[4]), height = stof(args[5]);

	// register sprite or animation based on argument length
	if (args.size()>6)
	{
		uint8_t duration = stoi(args[6]);
		g_Renderer.register_sprite(batch_id,texture_id,position,width,height,duration);
	}
	else g_Renderer.register_sprite(batch_id,texture_id,position,width,height);
}

/*
	TODO
*/
void interpreter_logic_instanced_sprite(uint8_t batch_id,std::vector<std::string>& args)
{
	// TODO
}

/*
	TODO
*/
void interpreter_logic_syntax_error(uint8_t batch_id,std::vector<std::string>& args)
{
	COMM_ERR("syntax error while writing to batch %i: \"%s\" not a valid command",batch_id,args[0].c_str());
}

typedef void (*gfx_interpreter_logic)(uint8_t,std::vector<std::string>&);
const gfx_interpreter_logic cmd_handler[RENDERER_INTERPRETER_COMMAND_COUNT+1] = {
	interpreter_logic_texture,interpreter_logic_sprite,interpreter_logic_instanced_sprite,
	interpreter_logic_syntax_error
};

/*
	TODO
*/
void bgr_compile_process(const char* path)
{
	COMM_LOG("renderer: reading load definition file \"%s\"",path);

	// find available batch by unix approach: last batch will be overwritten if no idle
	uint8_t batch_id = 0;
	while (batch_id<(RENDERER_BATCHES_COUNT-1)&&g_Renderer.batches[batch_id].state!=RBFR_IDLE) batch_id++;
	COMM_LOG("batch %i selected for writing",batch_id);
	COMM_ERR_COND(
			g_Renderer.batches[batch_id].state!=RBFR_IDLE,
			"CAREFUL! batch not in idle, overflow buffer selected!"
		);

	// open file
	COMM_MSG(LOG_BLUE,"-> interpretation start");
	std::ifstream file(path,std::ios::in);
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
		cmd_handler[i](batch_id,args);
	}

	// close file and ready batch load
	COMM_MSG(LOG_BLUE,"-> interpretation end");
	file.close();
	g_Renderer.batches[batch_id].state = RBFR_LOAD;

	COMM_SCC("interpretation completed");
}

/*
	TODO
*/
void Renderer::compile(const char* path)
{
	std::thread compiler_thread(&bgr_compile_process,path);
	compiler_thread.join();
}


/*
	!O(1)m /+load -> (public)
	// TODO: add purpose
	\param batch_id: id of sprite buffer to write animation to
	\param texpath: path to file containing spritesheet
	\param r (default=1): rows on spritesheet
	\param c (default=1): columns on spritesheet
	\param f (default=1): number of frames held by spritesheet
	\param s (default=0): frames the animation takes to fully iterate through all textures
	\returns: memory index the spritesheet can be referenced by later
*/
uint16_t Renderer::add_sprite(uint8_t batch_id,const char* texpath,uint8_t r,uint8_t c,uint8_t f)
{
	// create sprite source
	SpriteTextureTuple tuple = {

		// source
		.texture = Texture(texpath),

		// spritesheet segmentation
		.rows = r,
		.columns = c,
		.frames = f
	};

	// write and return reference id
	batches[batch_id].textures.push_back(tuple);
	return batches[batch_id].textures.size()-1;
}

/*
	!O(1)m /+load -> (public)
	\param batch_id: id of sprite buffer to write animation to
	\param tex_id: id of spritesheet texture to link
	\param p: origin position of added sprite
	\param w: width of added sprite
	\param h: height of added sprite
	TODO extend param description
*/
void Renderer::register_sprite(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h)
{
	// information setup
	Sprite sprite = {

		// link sprite to texture
		.texture_id = tex_id,

		// transform component
		.transform = {
			.position = p,
			.width = w,
			.height = h,
		},
	};

	// transform and write
	sprite.transform.to_origin();
	batches[batch_id].sprites.push_back(sprite);
}

/*
	TODO
*/
void Renderer::register_sprite(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h,uint8_t dur)
{
	// add animation data
	batches[batch_id].anim_sprites.push_back({
			.id = batches[batch_id].sprites.size(),
			.cycle_duration = dur,
			.frame_duration = (float)dur/batches[batch_id].textures[tex_id].frames,
		});

	// register sprite
	register_sprite(batch_id,tex_id,p,w,h);
}

/*
	TODO
*/
void Renderer::register_duplicate(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h)
{
	SpriteInstance instance = {

		// link sprites to texture
		.texture_id = tex_id,

		// tranformation
		.transform = {
			.position = p,
			.width = w,
			.height = h,
		},
	};

	// transform and write
	instance.transform.to_origin();
	batches[batch_id].duplicates.push_back(instance);
}

/*
	TODO
*/
void Renderer::register_duplicate(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h,uint8_t dur)
{
	// add animation data
	batches[batch_id].anim_duplicates.push_back({
			.id = batches[batch_id].duplicates.size(),
			.cycle_duration = dur,
			.frame_duration = (float)dur/batches[batch_id].textures[tex_id].frames,
		});

	// register duplicate
	register_duplicate(batch_id,tex_id,p,w,h);
}


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
void bgr_process_textures(RenderBatch* batch)
{
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
	std::thread ld_thread(&bgr_process_textures,&batch);
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
