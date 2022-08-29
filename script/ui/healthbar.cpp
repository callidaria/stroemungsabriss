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
Healthbar::Healthbar(glm::vec2 pos,uint16_t width,uint16_t height,std::vector<int> phases,
		std::vector<int> hp)
    : max_width(width)
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
	hpbuffer.add_buffer();

	// compile hpbar shader
	shp.compile("shader/fbv_healthbar.shader","shader/fbf_healthbar.shader");
	shp.def_attributeF("position",2,0,3);
	shp.def_attributeF("edge_id",1,2,3);

	// hpbar indexing
	hpbuffer.bind_index();
	shp.def_indexF(hpbuffer.get_indices(),"ofs",1,0,2);
	shp.def_indexF(hpbuffer.get_indices(),"wdt",1,1,2);

	// 2D projection hpbar
	shp.upload_matrix("view",tc2d.view2D);
	shp.upload_matrix("proj",tc2d.proj2D);

	// vertices border
	float brdverts[] = {
		pos.x,pos.y-2,0, pos.x,pos.y+height+2,1,
		pos.x,pos.y+height+2,1, pos.x,pos.y+height+2,3,
		pos.x,pos.y+height+2,3, pos.x,pos.y-2,4,
		pos.x,pos.y-2,4, pos.x,pos.y-2,0,
	};
	brdbuffer.bind();
	brdbuffer.upload_vertices(brdverts,sizeof(brdverts));
	brdbuffer.add_buffer();

	// compile border shader
	sborder.compile("shader/fbv_hpborder.shader","shader/fbf_hpborder.shader");
	sborder.def_attributeF("position",2,0,3);
	sborder.def_attributeF("edge_id",1,2,3);

	// border indexing
	brdbuffer.bind_index();
	sborder.def_indexF(brdbuffer.get_indices(),"ofs",1,0,2);
	sborder.def_indexF(brdbuffer.get_indices(),"wdt",1,1,2);

	// 2D projection border
	sborder.upload_matrix("view",tc2d.view2D);
	sborder.upload_matrix("proj",tc2d.proj2D);  // TODO: write camera upload in shader

	// vertices indexed splice
	float splcverts[] = { pos.x,pos.y-6.0f, pos.x,pos.y+height+12.0f, };
	splcbuffer.bind();
	splcbuffer.upload_vertices(splcverts,sizeof(splcverts));
	splcbuffer.add_buffer();

	// compile splice shader
	ssplice.compile("shader/fbv_hpsplice.shader","shader/fbf_hpsplice.shader");
	ssplice.def_attributeF("position",2,0,2);

	// splice indexing
	splcbuffer.bind_index();
	ssplice.def_indexF(splcbuffer.get_indices(),"ofs",1,0,2);
	ssplice.def_indexF(splcbuffer.get_indices(),"wdt",1,1,2);

	// 2D projection splice
	ssplice.upload_matrix("view",tc2d.view2D);
	ssplice.upload_matrix("proj",tc2d.proj2D);

	// calculate render specifications
	uint8_t t_index = 0;
	for (int i=0;i<phases.size();i++) {

		// calculate current phases combined hp
		float target_hp = 0;
		for (int j=0;j<phases[i];j++)
			target_hp += hp[t_index+j];

		// calculate positions and widths
		std::vector<float> t_pos,t_wdt;
		float draw_cursor = 0;
		for (int j=0;j<phases[i];j++) {
			t_pos.push_back(draw_cursor);
			float t_width = (hp[t_index+j]/target_hp)*width;
			t_wdt.push_back(t_width);
			draw_cursor += t_width;
		}

		// save list to upload space
		hpswap.dest_pos.push_back(t_pos);
		hpswap.dest_wdt.push_back(t_wdt);

		// increase index above phase counter
		t_index += phases[i];
	}
} Healthbar::~Healthbar() {  }

/*
	render() -> void
	purpose: visualize current health status by rendering health bar
*/
void Healthbar::render()
{
	// switch healthbar handlers
	fill_switch[frdy](frdy,hpswap);

	// setup & draw hpbar
	shp.enable();
	hpbuffer.bind();
	hpbuffer.bind_index();
	hpbuffer.upload_indices(hpswap.upload);
	glDrawArraysInstanced(GL_TRIANGLES,0,6,hpswap.upload.size()/2);

	// setup & draw border
	sborder.enable();
	brdbuffer.bind();
	brdbuffer.bind_index();
	brdbuffer.upload_indices(hpswap.upload);  // FIXME: remove and use hpbuffers index buffer
	glDrawArraysInstanced(GL_LINES,0,8,hpswap.upload.size()/2);

	// setup & draw splice
	ssplice.enable();
	splcbuffer.bind();
	splcbuffer.bind_index();
	splcbuffer.upload_indices(std::vector<float>(hpswap.upload.begin()+2,hpswap.upload.end()));
	glDrawArraysInstanced(GL_LINES,0,2,hpswap.upload.size()/2-1);
	// FIXME: remove and use hpbuffers index buffer
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

void Healthbar::fill_hpbar(bool &frdy,HPBarSwap &hpswap)
{
	// load targets for upload vector
	for (int i=0+1000*(hpswap.upload_target.size()>0);i<hpswap.dest_pos[0].size();i++) {
		hpswap.upload.push_back(hpswap.dest_pos[0][i]);
		hpswap.upload_target.push_back(hpswap.dest_wdt[0][i]);
		hpswap.upload.push_back(0);
	}

	// filling upload until target
	uint8_t itr = hpswap.target_itr;
	bool full_bar = hpswap.upload[itr*2+1]>=hpswap.upload_target[itr];
	hpswap.target_itr += full_bar;
	hpswap.upload[itr*2+1] += 4;
	hpswap.upload[itr*2+1] = hpswap.upload_target[itr]*full_bar+hpswap.upload[itr*2+1]*!full_bar;

	// signal if bar has been filled
	frdy = hpswap.target_itr>=hpswap.upload_target.size();
}

void Healthbar::ready_hpbar(bool &frdy,HPBarSwap &hpswap)
{
	// TODO: handle healthbar while fight
}