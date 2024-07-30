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
	float vertices[] = { -.5f,.5f,.0f,.0f, .5f,.5f,1.f,.0f, .5f,-.5f,1.f,1.f, -.5f,-.5f,.0f,1.f };
	uint32_t elements[] = { 0,2,1, 2,0,3 };

	// setup vertex & element buffer for sprites
	spr_buffer.add_buffer();
	spr_buffer.bind();
	spr_buffer.upload_vertices(vertices,PATTERN_SPRITE_VERTEX_REPEAT*sizeof(float));
	spr_buffer.upload_elements(elements,PATTERN_SPRITE_ELEMENT_REPEAT*sizeof(uint32_t));

	// setup sprite shader
	COMM_LOG("2D shader preparation");
	spr_shader.compile2d("./shader/obj/sprite.vs","./shader/standard/direct.fs");
	spr_shader.upload_int("tex",0);
	spr_shader.upload_camera();

	COMM_SCC("renderer ready");
}


/**
 *	TODO
*/

// loader definition command list
const std::string gfxcmd[RENDERER_INTERPRETER_COMMAND_COUNT] = { "texture","sprite" };

/*
	TODO
*/
void interpreter_logic_texture(uint8_t batch_id,std::vector<std::string>& args)
{
	// arguments to variables
	const char* path = args[1].c_str();
	uint8_t rows = 1, cols = 1, frames = 1;

	// check for texture atlas
	if (args.size()>2) rows = stoi(args[2]), cols = stoi(args[3]), frames = stoi(args[4]);

	// write texture
	g_Renderer.add_sprite(batch_id,path,rows,cols,frames);
}

/*
	TODO
*/
void interpreter_logic_sprite(uint8_t batch_id,std::vector<std::string>& args)
{
	// arguments to variables
	uint16_t texture_id = stoi(args[1]);
	glm::vec2 position = glm::vec2(stof(args[2]),stof(args[3]));
	float width = stof(args[4]), height = stof(args[5]);

	// register sprite or animation based on argument length
	if (args.size()>6)
	{
		uint8_t duration = stoi(args[6]);
		g_Renderer.register_animation(batch_id,texture_id,position,width,height,duration);
	}
	else g_Renderer.register_sprite(batch_id,texture_id,position,width,height);
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
	interpreter_logic_texture,interpreter_logic_sprite,
	interpreter_logic_syntax_error
};

/*
	TODO
*/
void Renderer::compile(const char* path)
{
	COMM_LOG("renderer: reading load definition file \"%s\"",path);

	// find available batch
	// TODO
	uint8_t batch_id = 0;

	// open file
	std::ifstream file(path,std::ios::in);
	std::string cmd_line;
	while (getline(file,cmd_line)) {

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
	file.close();
	sprite_batches[batch_id].state = RBFR_LOAD;

	COMM_SCC("interpretation completed");
}


/*
	!O(1)m /+load -> (public)
	// TODO: add purpose
	\param bfr_id: id of sprite buffer to write animation to
	\param texpath: path to file containing spritesheet
	\param r (default=1): rows on spritesheet
	\param c (default=1): columns on spritesheet
	\param f (default=1): number of frames held by spritesheet
	\param s (default=0): frames the animation takes to fully iterate through all textures
	\returns: memory index the spritesheet can be referenced by later
*/
uint16_t Renderer::add_sprite(uint8_t bfr_id,const char* texpath,uint8_t r,uint8_t c,uint8_t f)
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
	sprite_batches[bfr_id].textures.push_back(tuple);
	return sprite_batches[bfr_id].textures.size()-1;
}

/*
	!O(1)m /+load -> (public)
	\param bfr_id: id of sprite buffer to write animation to
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

		// transform component
		.transform = {
			.position = p,
			.width = w,
			.height = h,
		},

		// link sprite to texture
		.texture_id = tex_id,
	};

	// transform and write
	sprite.transform.to_origin();
	sprite_batches[batch_id].sprites.push_back(sprite);
}

/*
	TODO
*/
void Renderer::register_animation(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h,uint8_t dur)
{
	// add animation data
	sprite_batches[batch_id].animations.push_back({
			.id = sprite_batches[batch_id].sprites.size(),
			.cycle_duration = dur,
			.frame_duration = (float)dur/sprite_batches[batch_id].textures[tex_id].frames
		});

	// register sprite data
	register_sprite(batch_id,tex_id,p,w,h);
}


/**
 *	TODO
*/

typedef void (*sprite_buffer_routine)(SpriteBatch&,Shader&);

/*
	TODO
*/
void sprite_buffer_idle(SpriteBatch& sb,Shader& shader)
{
	// TODO: what even do here? maintaining checks, idle update, can this be skipped?
	//		buffers definitely will be in this state and the update has to be handled accordingly (no ee)
}

/*
	TODO
*/
void sprite_buffer_load(SpriteBatch& sb,Shader& shader)
{
	COMM_AWT("streaming %li textures",sb.textures.size());

	for (SpriteTextureTuple& t : sb.textures)
	{
		t.texture.gpu_upload();
		Texture::set_texture_parameter_clamp_to_edge();
		Texture::set_texture_parameter_linear_mipmap();
		t.texture.generate_mipmap();
		t.texture.cleanup();
	}
	sb.state = RBFR_RENDER;

	COMM_CNF();
}

/*
	TODO
*/
void sprite_buffer_render(SpriteBatch& sb,Shader& shader)
{
	// iterate animation updates
	for (SpriteAnimation& ta : sb.animations)
	{
		Sprite& ts = sb.sprites[ta.id];
		SpriteTextureTuple& tt = sb.textures[ts.texture_id];

		// calculate current frame
		bool inc_anim = ta.anim_progression<ta.cycle_duration;
		ta.anim_progression += inc_anim-ta.anim_progression*!inc_anim;
		// TODO: make this update depend on frame update delta
		//		test with unlocked frames to make sure this is actually working

		// calculate spritesheet location
		int index = ta.anim_progression/ta.frame_duration;
		ts.atlas_index = glm::vec2(index%tt.columns,index/tt.columns);
	}

	// iterate sprites
	for (uint16_t i=0;i<sb.sprites.size();i++)
	{
		Sprite& ts = sb.sprites[i];
		SpriteTextureTuple& tt = sb.textures[ts.texture_id];

		// upload sprite attributes
		shader.upload_int("row",tt.rows);
		shader.upload_int("col",tt.columns);
		shader.upload_vec2("i_tex",ts.atlas_index);
		shader.upload_matrix("model",ts.transform.model);

		// draw sprite
		sb.textures[sb.sprites[i].texture_id].texture.bind();
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)0);
	}
}

// behaviours mapped towards bufferstate enumeration
sprite_buffer_routine routine_sbuffers[RBFR_STATE_COUNT] = {
	sprite_buffer_idle,
	sprite_buffer_load,
	sprite_buffer_render
};


/*
	TODO
*/
void Renderer::update()
{
	// setup sprite buffers
	spr_shader.enable();
	spr_buffer.bind();

	// iterate sprite buffer
	for (uint8_t i=0;i<RENDERER_BATCHES_SPRITE_COUNT;i++)
	{
		SpriteBatch& bfr = sprite_batches[i];
		routine_sbuffers[bfr.state](bfr,spr_shader);
	}
}

/*
	TODO
*/
void Renderer::close()
{
	COMM_MSG(LOG_DESTRUCTION,"closing renderer...");
	COMM_MSG(LOG_SNITCH,"deprecated function called! not useful right now.");

	// TODO

	COMM_SCC("done");
}
