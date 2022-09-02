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
		std::vector<int> hp,const char* boss_name)
{
	// save position and width to swap structure
	hpswap.position = pos;
	hpswap.max_width = width;

	// projection into coordinate system
	Camera2D tc2d = Camera2D(1280.0f,720.0f);

	// vertices hpbar
	float hpverts[] = {
		pos.x,pos.y,0, pos.x,pos.y+height,1, pos.x,pos.y+height,3,
		pos.x,pos.y+height,3, pos.x,pos.y,2, pos.x,pos.y,0,
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
	shp.def_indexF(hpbuffer.get_indices(),"ofs",1,0,PT_REPEAT);
	shp.def_indexF(hpbuffer.get_indices(),"wdt",1,1,PT_REPEAT);
	shp.def_indexF(hpbuffer.get_indices(),"dmg",1,2,PT_REPEAT);

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
	sborder.def_indexF(brdbuffer.get_indices(),"ofs",1,0,PT_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"wdt",1,1,PT_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"dmg",1,2,PT_REPEAT);

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
	ssplice.def_indexF(splcbuffer.get_indices(),"ofs",1,0,PT_REPEAT);
	ssplice.def_indexF(splcbuffer.get_indices(),"wdt",1,1,PT_REPEAT);
	ssplice.def_indexF(splcbuffer.get_indices(),"dmg",1,2,PT_REPEAT);

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

	// set name and phase counter
	Font hbfont = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",TEXT_SIZE,TEXT_SIZE);
	hpswap.phcnt = Text(hbfont);hpswap.phname = Text(hbfont);
	hpswap.phname.add(boss_name,glm::vec2(pos.x+TEXT_MV,pos.y+TEXT_DV));
	hpswap.phname.load_wcam(&tc2d);
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
	glDrawArraysInstanced(GL_TRIANGLES,0,6,hpswap.upload.size()/PT_REPEAT);

	// setup & draw border
	sborder.enable();
	brdbuffer.bind();
	brdbuffer.bind_index();
	brdbuffer.upload_indices(hpswap.upload);  // FIXME: remove and use hpbuffers index buffer
	glDrawArraysInstanced(GL_LINES,0,8,hpswap.upload.size()/PT_REPEAT);

	// setup & draw splice
	ssplice.enable();
	splcbuffer.bind();
	splcbuffer.bind_index();
	splcbuffer.upload_indices(hpswap.upload_splice);
	glDrawArraysInstanced(GL_LINES,0,2,hpswap.upload_splice.size()/PT_REPEAT);

	// render name and phase counter
	hpswap.phname.prepare();
	hpswap.phname.render(MAX_CHAR_NAME,TEXT_COL);
	hpswap.phcnt.prepare();
	hpswap.phcnt.render(3,TEXT_COL);
}

/*
	register_damage(uint16_t) -> void
	dmg: damage value to register to health bar (reduce the maximum hp by)
	purpose: reducing health points by given value
*/
void Healthbar::register_damage(uint16_t dmg)
{
	hpswap.dmg_threshold += dmg*(frdy==3);
}

/*
	fill_hpbar(uint8_t&,HPBarSwap&) -> void
	conforming to: fill_switch
	purpose: animate nano healthbar filling and capping value at max
*/
void Healthbar::fill_hpbar(uint8_t &frdy,HPBarSwap &hpswap)
{
	// load targets for upload vector
	uint8_t ihp = hpswap.hpbar_itr;
	for (int i=0+1000*(hpswap.upload_target.size()>0);i<hpswap.dest_pos[ihp].size();i++) {
		hpswap.upload.push_back(hpswap.dest_pos[ihp][i]);
		hpswap.upload_target.push_back(hpswap.dest_wdt[ihp][i]);
		hpswap.upload.push_back(0);
		hpswap.upload.push_back(0);
	}

	// filling upload until target
	int8_t itr = hpswap.target_itr;
	bool full_bar = hpswap.upload[itr*PT_REPEAT+1]>=hpswap.upload_target[itr];
	hpswap.target_itr += full_bar;
	hpswap.upload[itr*PT_REPEAT+1] += 4;
	hpswap.upload[itr*PT_REPEAT+1]
			= hpswap.upload_target[itr]*full_bar+hpswap.upload[itr*PT_REPEAT+1]*!full_bar;
	hpswap.upload[itr*PT_REPEAT+2] += 4;
	hpswap.upload[itr*PT_REPEAT+2]
			= hpswap.upload_target[itr]*full_bar+hpswap.upload[itr*PT_REPEAT+2]*!full_bar;

	// signal ready to splice if bars full
	frdy += hpswap.target_itr>=hpswap.upload_target.size();
	hpswap.target_itr -= frdy;
}  // TODO: transform geometry randomized

/*
	splice_hpbar(uint8_t&,HPBarSwap&) -> void
	conforming to: fill_switch
	purpose: insert cut geometry between nano healthbars
*/
void Healthbar::splice_hpbar(uint8_t &frdy,HPBarSwap &hpswap)
{
	// defining splice index upload
	hpswap.upload_splice = std::vector<float>(hpswap.upload.begin()+PT_REPEAT,hpswap.upload.end());
	frdy++;
}  // TODO: animate healthbar splicing

/*
	count_phases(uint8_t&,HPBarSwap&) -> void
	conforming to: fill_switch
	purpose: animated phase count up from 0 towards full amount to increase tension
*/
void Healthbar::count_phases(uint8_t &frdy,HPBarSwap &hpswap)
{
	// skip animation if not in first phase
	hpswap.anim_tick += POT*(hpswap.hpbar_itr>0);

	// increase phase counter
	Camera2D tc2d = Camera2D(1280.0f,720.0f);
	uint8_t split_ticks = POT/(hpswap.dest_pos.size()+2);  // ticks after scaling is reversed
	uint8_t aprog = hpswap.dest_pos.size()*((hpswap.anim_tick+split_ticks)/POT);
	hpswap.phcnt.clear();
	hpswap.phcnt.add((/*std::to_string(hpswap.hpbar_itr+1)+'/'+*/std::to_string(aprog)).c_str(),
			glm::vec2(0,0));
	hpswap.phcnt.load_wcam(&tc2d);

	// zoom scroll phase counter at increment
	uint8_t mticks = hpswap.anim_tick%split_ticks;		// animation ticks after last revert
	bool upscale = (hpswap.anim_tick/split_ticks)%2;	// distiguishing up and downscale phases
	float zmscale = .5f*(((float)mticks/split_ticks)*(-2*upscale+1)+upscale);

	// upload scroll towards expectation
	glm::mat4 mdl_trans = glm::translate(glm::mat4(1.0f),
			glm::vec3(hpswap.position.x+hpswap.max_width-TEXT_MV,hpswap.position.y+TEXT_DV,0));
	mdl_trans = glm::scale(mdl_trans,glm::vec3(1+zmscale,1+zmscale,1));
	hpswap.phcnt.set_scroll(mdl_trans);

	// signal hpbar readiness
	hpswap.anim_tick++;
	bool anim_ready = hpswap.anim_tick>POT;
	frdy += anim_ready;
	hpswap.anim_tick -= hpswap.anim_tick*anim_ready;
}  // TODO: tick-up zoomout when counting up phases

/*
	ready_hpbar(uint8_t&,HPBarSwap&) -> void
	conforming to: fill_switch
	purpose: checks if current nano healthbar is depleted and if so, changes to the previous one.
		if all bars are empty, the method signals a refill.
		if the last batch of bars exceeds the maximum amount of phases, a clearstate is signaled.
*/
void Healthbar::ready_hpbar(uint8_t &frdy,HPBarSwap &hpswap)
{
	// subtract nanobar hp by damage in threshold
	hpswap.upload[hpswap.target_itr*PT_REPEAT+2] -= hpswap.dmg_threshold;
	hpswap.dmg_threshold = 0;

	// decrease target iteration if nanobar is empty
	hpswap.target_itr -= hpswap.upload[hpswap.target_itr*PT_REPEAT+2]<=0;

	// signal refill if all bars empty
	bool hpbar_empty = hpswap.target_itr<0;
	hpswap.target_itr += hpbar_empty;
	hpswap.hpbar_itr += hpbar_empty;
	frdy += hpbar_empty+(hpswap.hpbar_itr>=hpswap.dest_pos.size());
}

/*
	reset_hpbar(uint8_t&,HPBarSwap&) -> void
	conforming to: fill_switch
	purpose: reset hpswap upload and target lists to later refill them with new information
*/
void Healthbar::reset_hpbar(uint8_t &frdy,HPBarSwap &hpswap)
{
	// ready hpswap lists for refill
	hpswap.upload_target.clear();
	hpswap.upload.clear();
	hpswap.upload_splice.clear();

	// signal refill
	frdy = 0;
}

/*
	signal_clear(uint8_t&,HPBarSwap&) -> void
	conforming to: fill_switch
	purpose: represent boss clearstate with the healthbar
*/
void Healthbar::signal_clear(uint8_t &frdy,HPBarSwap &hpswap)
{
	// TODO: visualize clear
}