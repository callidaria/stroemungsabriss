#include "renderer.h"


/**
 * TODO: expand
*/

/*
	TODO
*/
void RTransform2D::transform(glm::vec2 p,float w,float h,float r)
{
	rotate(r);
	scale(w,h);
	translate(p);
}

/*
	TODO
*/
void RTransform2D::transform(glm::vec2 p,float w,float h,float r,glm::vec2 a)
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
void RTransform2D::scale(float w,float h,glm::vec2 a)
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
void RTransform2D::rotate(float r,glm::vec2 a)
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

void sprite_load_thread(SpriteLoadInstrData* data,ThreadState* state)
{
	// create shared context & setup loader
	//Core::gFrame.make_window_context_current(data->context);
	data->context = (SDL_GLContext)SDL_GL_GetCurrentContext();

	// load routine
	while (true) {

		// wait until active loading or termination
		Toolbox::thread_detached_stop(th_ldsprite_data);
		if (!state->active) break;

		// sprite loading
		while (!data->ldbfr.empty()) {
			std::cout << "sync loading\n";
			SpriteBuffer* bfr = data->ldbfr.front();
			for (RTextureTuple& t : bfr->textures) t.load();
			bfr->attribs.state = (BufferState)(bfr->attribs.state+bfr->attribs.auto_stateswitch);
			data->ldbfr.pop();
		}
	}

	// context destruction
	SDL_GL_DeleteContext(*data->context);
}


/**
 * TODO: expand
*/

/*
	TODO
*/
Renderer::Renderer()
{
	// preload sprite data
	// generate sprite vertex data
	float vertices[] = { -.5f,.5f,.0f,.0f, .5f,.5f,1.f,.0f, .5f,-.5f,1.f,1.f, -.5f,-.5f,.0f,1.f };
	uint32_t elements[] = { 0,2,1, 2,0,3 };

	// setup vertex & element buffer for sprites
	spr_buffer.add_buffer();
	spr_buffer.bind();
	spr_buffer.upload_vertices(vertices,PATTERN_SPRITE_VERTEX_REPEAT*sizeof(float));
	spr_buffer.upload_elements(elements,PATTERN_SPRITE_ELEMENT_REPEAT*sizeof(uint32_t));

	// setup sprite shader
	spr_shader.compile2d("./shader/obj/sprite.vs","./shader/standard/direct.fs");
	spr_shader.upload_int("tex",0);
	spr_shader.upload_camera();

	// setup loading thread
	th_inst_sprite_data = { .context = &Core::gFrame.load_context, };
	th_sprite_loader = std::thread(sprite_load_thread,&th_inst_sprite_data,&th_ldsprite_data);
	th_sprite_loader.detach();
}

/*
	!O(1)m /+load -> (public)
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
	RTextureTuple tex = {

		// source
		.path = texpath,

		// spritesheet segmentation
		.rows = r,
		.columns = c,
		.frames = f
	};

	// write and return reference id
	bfr_sprite[bfr_id].textures.push_back(tex);
	return bfr_sprite[bfr_id].textures.size()-1;
}

/*
	!O(1)m /+load -> (public)
	purpose: add new spritesheet to renderer
	\param bfr_id: id of sprite buffer to write animation to
	\param tex_id: id of spritesheet texture to link
	\param p: origin position of added sprite
	\param w: width of added sprite
	\param h: height of added sprite
	TODO extend param description
*/
void Renderer::register_sprite(uint8_t bfr_id,uint16_t tex_id,glm::vec2 p,float w,float h,bool animate,uint8_t s)
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
	bfr_sprite[bfr_id].sprites.push_back(sprite);

	// register animation if requested
	if (animate)
		bfr_sprite[bfr_id].animations.push_back({
			.id = bfr_sprite[bfr_id].sprites.size()-1,
			.cycle_duration = s,
			.frame_duration = (float)s/bfr_sprite[bfr_id].textures[tex_id].frames
		});
}


/**
 *	TODO
*/

typedef void (*sprite_buffer_routine)(SpriteBuffer&,Shader&);

/*
	TODO
*/
void sprite_buffer_idle(SpriteBuffer& sb,Shader& shader)
{
	// TODO: what even do here? maintaining checks, idle update, can this be skipped?
	//		buffers definitely will be in this state and the update has to be handled accordingly (no ee)
}

/*
	TODO
*/
void sprite_buffer_load(SpriteBuffer& sb,Shader& shader)
{
	/*for (RTextureTuple& t : sb.textures) t.load();
	sb.attribs.state = (BufferState)(sb.attribs.state+sb.attribs.auto_stateswitch);*/
	std::cout << "load stage calling\n";
	th_inst_sprite_data.ldbfr.push(&sb);
	Toolbox::thread_detached_continue(th_ldsprite_data);
}

/*
	TODO
*/
void sprite_buffer_render(SpriteBuffer& sb,Shader& shader)
{
	// iterate animation updates
	for (SpriteAnimation& ta : sb.animations) {
		Sprite& ts = sb.sprites[ta.id];
		RTextureTuple& tt = sb.textures[ts.texture_id];

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
	for (uint16_t i=0;i<sb.sprites.size();i++) {
		Sprite& ts = sb.sprites[i];
		RTextureTuple& tt = sb.textures[ts.texture_id];

		// upload sprite attributes
		shader.upload_int("row",tt.rows);
		shader.upload_int("col",tt.columns);
		shader.upload_vec2("i_tex",ts.atlas_index);
		shader.upload_matrix("model",ts.transform.model);

		// draw sprite
		glBindTexture(GL_TEXTURE_2D,sb.textures[sb.sprites[i].texture_id].texture);
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
	for (uint8_t i=0;i<RENDERER_BUFFERS_SPRITE_COUNT;i++) {
		SpriteBuffer& bfr = bfr_sprite[i];
		routine_sbuffers[bfr.attribs.state](bfr,spr_shader);
	}
}

/*
	TODO
*/
void Renderer::close()
{
	th_ldsprite_data.active = false;
	Toolbox::thread_detached_continue(th_ldsprite_data);
}
