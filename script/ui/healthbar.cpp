#include "healthbar.h"

/*
	constructor(vec2,uint16_t,uint16_t)
	pos: down left position of the health bar on the screen
	width: maximum width of the health bar if health points are at maximum value
	height: height of the health bar
	phases:
	hp: (also current hp will initially be set to minimum)
	purpose: creates an healthbar to save and visualize health stati with
*/
Healthbar::Healthbar(glm::vec2 pos,uint16_t width,uint16_t height,std::vector<uint8_t> phases,
		std::vector<uint16_t> hp)
    : max_width(width),hb_phases(phases),hp_list(hp)
{
	// projection into coordinate system
	Camera2D tc2d = Camera2D(1280.0f,720.0f);

	// vertices hpbar
	float hpverts[] = {
		pos.x,pos.y,0, pos.x,pos.y+height,0, pos.x,pos.y+height,1,
		pos.x,pos.y+height,1, pos.x,pos.y,1, pos.x,pos.y,0,
	};
	hpbuffer.bind();
	hpbuffer.upload_vertices(hpverts,sizeof(hpverts));

	// compile hpbar shader
	shp.compile("shader/fbv_healthbar.shader","shader/fbf_healthbar.shader");
	shp.def_attributeF("position",2,0,3);
	shp.def_attributeF("edge_id",1,2,3);

	// 2D projection hpbar
	shp.upload_matrix("view",tc2d.view2D);
	shp.upload_matrix("proj",tc2d.proj2D);

	// vertices border
	float brdverts[] = {
		pos.x-2,pos.y-2, pos.x-2,pos.y+height+2,
		pos.x-2,pos.y+height+2, pos.x+width+2,pos.y+height+2,
		pos.x+width+2,pos.y+height+2, pos.x+width+2,pos.y-2,
		pos.x+width+2,pos.y-2, pos.x-2,pos.y-2,
	};
	brdbuffer.bind();
	brdbuffer.upload_vertices(brdverts,sizeof(brdverts));

	// compile border shader
	sborder.compile("shader/fbv_hpborder.shader","shader/fbf_hpborder.shader");
	sborder.def_attributeF("position",2,0,2);

	// 2D projection border
	sborder.upload_matrix("view",tc2d.view2D);
	sborder.upload_matrix("proj",tc2d.proj2D);  // TODO: write camera upload in shader

	// vertices indexed splice
	float splcverts[] = { pos.x,pos.y-6.0f, pos.x,pos.y+height+12.0f, };
	splcbuffer.bind();
	splcbuffer.upload_vertices(splcverts,sizeof(splcverts));

	// compile splice shader
	ssplice.compile("shader/fbv_hpsplice.shader","shader/fbf_hpsplice.shader");
	ssplice.def_attributeF("position",2,0,2);

	// splice indexing
	splcbuffer.bind_index();
	ssplice.def_indexF(splcbuffer.get_indices(),"ofs",1,0,1);

	// 2D projection splice
	ssplice.upload_matrix("view",tc2d.view2D);
	ssplice.upload_matrix("proj",tc2d.proj2D);
} Healthbar::~Healthbar() {  }

/*
	render() -> void
	purpose: visualize current health status by rendering health bar
*/
void Healthbar::render()
{
	// check for empty bar and maximum
	bool nohp = !curr_hp;
	max_hp = combine_hp();

	// fill if empty and not final bar
	fill_cooldown *= !nohp;
	bool filling = fill_cooldown<241;
	fill_cooldown += filling;
	curr_hp = (curr_hp*!filling)+(max_hp*(fill_cooldown/240.0f)*filling);

	// setup hpbar
	shp.enable();
	hpbuffer.bind();

	// fill bar by status
	float dist = ((float)max_width/max_hp)*curr_hp;
	shp.upload_float("fill_width",dist);

	// draw hpbar
	glDrawArrays(GL_TRIANGLES,0,6);

	// setup border
	sborder.enable();
	brdbuffer.bind();

	// draw border
	glDrawArrays(GL_LINES,0,8);

	// setup splice
	ssplice.enable();
	splcbuffer.bind();
	splcbuffer.upload_indices(ofs);

	// draw splice
	glDrawArraysInstanced(GL_LINES,0,2,hb_phases[chb]);
}

/*
	register_damage(uint16_t) -> void
	dmg: damage value to register to health bar (reduce the maximum hp by)
	purpose: reducing health points by given value
*/
void Healthbar::register_damage(uint16_t dmg)
{
	curr_hp -= dmg;
}

/*
	combine_hp() -> uint16_t
	TODO: outside main loop
*/
uint16_t Healthbar::combine_hp()
{
	uint16_t out;
	ofs.clear();
	for (int i=0;i<hb_phases[chb];i++) {
		out += hp_list[cphase+i];
		ofs.push_back(out);
	} return out;
}  // FIXME: should return exactely 0 if last bar has been emptied