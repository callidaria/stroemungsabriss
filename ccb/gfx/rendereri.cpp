#include "rendereri.h"

/*
	constructor()
	purpose: purpose: create renderer object to subsequently add instances to and draw them
*/
RendererI::RendererI()
{ buffer.add_buffer(); }

/*
	add(vec2,float,float,const char*) -> uint16_t
	p: origin position of indexed 2D object
	w: x-axis scaling resulting in initial object width
	h: y-axis scaling resulting in initial object height
	t: path to file holding the indexed objects texture
	purpose: creating and adding an indexable 2D object to render memory
	returns: memory index the created instanceable object is to referred by when drawing
*/
uint16_t RendererI::add(glm::vec2 p,float w,float h,const char* t)
{
	il.push_back(Instance(p,w,h,t));
	return il.size()-1;
}

/*
	add(vec2,float,float,const char*,uint8_t,uint8_t,uint8_t,uint8_t) -> uint16_t
	overloads: previous add()
	row: amount of rows contained in the animation texture sprite sheet
	col: amount of columns contained in the animation texture sprite sheet
	itn: total number of subtexture states contained in the sprite sheet
	f: amount of frames the animation should take to complete one full circle
	purpose: creates an indexable 2D animation object to render later
	returns: memory index the created instanceable animation object is to referred by when drawing
*/
uint16_t RendererI::add(glm::vec2 p,float w,float h,const char* t,uint8_t row,uint8_t col,
		uint8_t itn,uint8_t f)
{
	ial.push_back(InstancedAnim(p,w,h,t,row,col,itn,f));
	return ial.size()-1;
}

/*
	load() -> void
	purpose: combine vertex upload and texturing and compile shader
*/
void RendererI::load()
{
	// write all object vertices to master array
	std::vector<float> v;
	for (int i=0;i<il.size();i++) v.insert(v.end(),il[i].v.begin(),il[i].v.end());
	for (int i=0;i<ial.size();i++) v.insert(v.end(),ial[i].v.begin(),ial[i].v.end());

	// upload to buffer
	buffer.bind();
	buffer.upload_vertices(v);

	// compile classical instance shader program
	sI.compile2d("shader/vertex_inst.shader","shader/fragment_inst.shader");
	buffer.bind_index();
	sI.def_indexF(buffer.get_indices(),"offset",2,0,INSTANCE_REPEAT);
	sI.def_indexF(buffer.get_indices(),"rotation_sin",1,2,INSTANCE_REPEAT);
	sI.def_indexF(buffer.get_indices(),"rotation_cos",1,3,INSTANCE_REPEAT);
	sI.def_indexF(buffer.get_indices(),"i_tex",2,4,INSTANCE_REPEAT);
	// ??maybe find a different way of representing instanced rotation??
	// precalculating sine & cosine for a matrix 2D seems like the most performant way of doing this
	// ??uploading i_tex for all instances using this shader leaves a lot of 0s for single textures
	// we could make instanced_anim the only instanced object or find a different solution??

	// texture
	for (int i=0;i<il.size();i++) il[i].texture();
	for (int i=0;i<ial.size();i++) ial[i].texture();
	sI.upload_int("tex",0);

	// coordinate system
	Camera2D cam2D = Camera2D(1280.0f,720.0f);
	sI.upload_camera(cam2D);
}

/*
	prepare(float) -> void
	dtime: delta time for animation updates
	purpose: enable shader program and bind buffer to use its data
*/
void RendererI::prepare(float dtime)
{
	// gl settings
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// prepare shader & buffer
	sI.enable();
	buffer.bind();

	// update instance animations
	for (int i=0;i<ial.size();i++) ial[i].update(dtime);
}

/*
	reset_anim_ticks(uint16_t,uint16_t) -> void
	cluster: index identification of added cluster
	idx: memory list index for animation update ticks
	purpose: reset given tick within given cluster
*/
void RendererI::reset_anim_tick(uint16_t cluster,uint16_t idx)
{ ial[cluster].reset_tick(idx); }

/*
	render(uint16_t,uint16_t) -> void
	i: memory index of previously added indexable object
	amt: amount of identical objects to render from indexed source
	purpose: draw object the target amount after uploading the position transforms to index buffer
*/
void RendererI::render(uint16_t i,uint16_t amt)
{
	// setup
	il.at(i).setup();
	buffer.upload_indices(il[i].o,sizeof(float)*INSTANCE_VALUES);

	// render instanced
	glDrawArraysInstanced(GL_TRIANGLES,i*6,6,amt);
}
 
/*
	render(uint16_t,uint16_t,vec2) -> void
	overloads: previous render()
	i_tex: texture index vector indicating subtexture position on spritesheet raster
	purpose: render specific animation frame of previously added instanced animation object
*/
void RendererI::render(uint16_t i,uint16_t amt,glm::vec2 i_tex)
{
	// load texture & index buffer data
	ial[i].setup(&sI);
	buffer.upload_indices(ial[i].i,sizeof(float)*IANIMATION_VALUES);
	sI.upload_vec2("i_tex",i_tex);

	// draw
	glDrawArraysInstanced(GL_TRIANGLES,(il.size()+i)*6,6,amt);
}

/*
	render_anim(uint16_t,uint16_t) -> void
	i: memory index of animated instanced object to render
	amt: amount of duplicates to render from instanced animation object
	purpose: render instanced animation objects with interpolated animation updates
*/
void RendererI::render_anim(uint16_t i,uint16_t amt)
{
	// load texture & index buffer data
	ial[i].setup(&sI);
	buffer.upload_indices(ial[i].i,sizeof(float)*IANIMATION_VALUES);

	// draw
	glDrawArraysInstanced(GL_TRIANGLES,(il.size()+i)*6,6,amt);
}

/*
	PARAMETER DEFINITIONS:
	i: memory index of indexable object
	j: memory index of position transform vector to change
*/

/*
	get_offset(uint16_t,uint16_t) -> vec2
	returns: vectorized offset direction and length as derived from index upload list
*/
glm::vec2 RendererI::get_offset(uint16_t i,uint16_t j)
{ return glm::vec2(il[i].o[j*INSTANCE_REPEAT],il[i].o[j*INSTANCE_REPEAT+1]); }

/*
	get_aOffset(uint16_t i,uint16_t j) -> vec2
	variation of: get_offset()
	returns: vectorized offset direction and length as derived from index upload list of animation
*/
glm::vec2 RendererI::get_aOffset(uint16_t i,uint16_t j)
{ return glm::vec2(ial[i].i[j*INSTANCE_REPEAT],ial[i].i[j*INSTANCE_REPEAT+1]); }

/*
	set_offset(uint16_t,uint16_t,vec2) -> void
	o: vector value to set the selected position transform vector to
	purpose: set a certain position transform vector of a specific object
*/
void RendererI::set_offset(uint16_t i,uint16_t j,glm::vec2 o)
{
	il[i].o[j*INSTANCE_REPEAT] = o.x;
	il[i].o[j*INSTANCE_REPEAT+1] = o.y;
}

/*
	set_aOffset(uint16_t,uint16_t,vec2) -> void
	variation of: set_offset()
	purpose: set a certain position transform vector of a specific animation object
*/
void RendererI::set_aOffset(uint16_t i,uint16_t j,glm::vec2 o)
{
	ial[i].i[j*INSTANCE_REPEAT] = o.x;
	ial[i].i[j*INSTANCE_REPEAT+1] = o.y;
}

/*
	set_rotation(uint16_t,uint16_t,float) -> void
	r: radians rotation of referenced instance in upload
	purpose: set radians rotation of referenced instance in upload list to given value
*/
void RendererI::set_rotation(uint16_t i,uint16_t j,float r)
{
	il[i].o[j*INSTANCE_REPEAT+2] = glm::sin(r);
	il[i].o[j*INSTANCE_REPEAT+3] = glm::cos(r);
}

/*
	set_aRotation(uint16_t,uint16_t,float) -> void
	r: radians rotation of referenced animated instance in upload
	purpose: set radians rotation of referenced animated instance in upload list to given value
*/
void RendererI::set_aRotation(uint16_t i,uint16_t j,float r)
{
	ial[i].i[j*IANIMATION_REPEAT+2] = glm::sin(r);
	ial[i].i[j*IANIMATION_REPEAT+3] = glm::cos(r);
}

/*
	add_offset(uint16_t,uint16_t,vec2) -> void
	dv: vector to change the current offset vector by
	purpose: changes current, indexed offset by given distance and direction
*/
void RendererI::add_offset(uint16_t i,uint16_t j,glm::vec2 dv)
{
	il[i].o[j*INSTANCE_REPEAT] += dv.x;
	il[i].o[j*INSTANCE_REPEAT+1] += dv.y;
}

/*
	add_aOffset(uint16_t i,uint16_t j,glm::vec2 dv) -> void
	variation of: add_offset
	purpose: changes current, indexed and animated offset by given distance and direction
*/
void RendererI::add_aOffset(uint16_t i,uint16_t j,glm::vec2 dv)
{
	ial[i].i[j*INSTANCE_REPEAT] += dv.x;
	ial[i].i[j*INSTANCE_REPEAT+1] += dv.y;
}

/*
	get_next_instindex() -> uint16_t
	returns: memory index of instance, that will be uploaded next
*/
uint16_t RendererI::get_next_instindex()
{ return il.size(); }

/*
	get_next_animindex() -> uint16_t
	returns: memory index of animated instance, that will be uploaded next
*/
uint16_t RendererI::get_next_animindex()
{ return ial.size(); }