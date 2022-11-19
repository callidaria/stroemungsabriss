#include "healthbar.h"

/*
	constructor(vec2,uint16_t,uint16_t)
	pos: down left position of the health bar on the screen
	width: maximum width of the health bar if health points are at maximum value
	height: height of the health bar
	phases: list of phases per health bar refill
	hp: (also current hp will initially be set to minimum)
	boss_name: self explanatory
	purpose: creates an healthbar to save and visualize health stati with
*/
Healthbar::Healthbar(glm::vec2 pos,uint16_t width,uint16_t height,std::vector<int> phases,
		std::vector<int> hp,const char* boss_name)
{
	// save position and width to swap structure
	hpswap.position = pos;
	hpswap.max_width = width;
	hpswap.max_height = height;

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
	shp.def_indexF(hpbuffer.get_indices(),"edg_trans[0]",1,3,PT_REPEAT);
	shp.def_indexF(hpbuffer.get_indices(),"edg_trans[1]",1,4,PT_REPEAT);
	shp.def_indexF(hpbuffer.get_indices(),"edg_trans[2]",1,5,PT_REPEAT);
	shp.def_indexF(hpbuffer.get_indices(),"edg_trans[3]",1,6,PT_REPEAT);
	shp.def_indexF(hpbuffer.get_indices(),"flt",2,7,PT_REPEAT);
	shp.def_indexF(hpbuffer.get_indices(),"target",1,9,PT_REPEAT);

	// 2D projection hpbar
	shp.upload_matrix("view",tc2d.view2D);
	shp.upload_matrix("proj",tc2d.proj2D);

	// vertices border
	float brdverts[] = {
		pos.x,pos.y,0, pos.x,pos.y+height,1,
		pos.x,pos.y+height,1, pos.x,pos.y+height,3,
		pos.x,pos.y+height,3, pos.x,pos.y,2,
		pos.x,pos.y,2, pos.x,pos.y,0,
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
	sborder.def_indexF(brdbuffer.get_indices(),"ofs",1,0,BRD_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"wdt",1,1,BRD_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"dmg",1,2,BRD_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"edg_trans[0]",1,3,BRD_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"edg_trans[1]",1,4,BRD_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"edg_trans[2]",1,5,BRD_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"edg_trans[3]",1,6,BRD_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"flt",2,7,BRD_REPEAT);
	sborder.def_indexF(brdbuffer.get_indices(),"target",1,9,BRD_REPEAT);

	// 2D projection border
	sborder.upload_matrix("view",tc2d.view2D);
	sborder.upload_matrix("proj",tc2d.proj2D);  // TODO: write camera upload in shader

	// vertices indexed splice
	float splcverts[] = { pos.x,pos.y,0, pos.x,pos.y+height,1, };
	splcbuffer.bind();
	splcbuffer.upload_vertices(splcverts,sizeof(splcverts));
	splcbuffer.add_buffer();

	// compile splice shader
	ssplice.compile("shader/fbv_hpsplice.shader","shader/fbf_hpsplice.shader");
	ssplice.def_attributeF("position",2,0,3);
	ssplice.def_attributeF("edge_id",1,2,3);

	// splice indexing
	splcbuffer.bind_index();
	ssplice.def_indexF(splcbuffer.get_indices(),"ofs[0]",2,0,SL_REPEAT);
	ssplice.def_indexF(splcbuffer.get_indices(),"ofs[1]",2,2,SL_REPEAT);
	ssplice.def_indexF(splcbuffer.get_indices(),"spread",1,4,SL_REPEAT);

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
	hpswap.phcnt = Text(hbfont),hpswap.phname = Text(hbfont);
	hpswap.phname.add(boss_name,glm::vec2(pos.x+TEXT_MV,pos.y+TEXT_DV));
	hpswap.phname.load(&tc2d);
} Healthbar::~Healthbar() {  }

/*
	render() -> void
	purpose: visualize current health status by rendering health bar
*/
void Healthbar::render()
{
	// switch healthbar handlers
	fill_switch[frdy](frdy,hpswap);

	// animate momentum
	floating_nanobars();

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
	brdbuffer.upload_indices(hpswap.upload);
	sborder.upload_int("cnt_height",hpswap.max_height);
	sborder.upload_int("brd_space",BORDER_CLEARING);
	glDrawArraysInstanced(GL_LINES,0,8,hpswap.upload.size()/BRD_REPEAT);

	// setup & draw splice
	ssplice.enable();
	splcbuffer.bind();
	splcbuffer.bind_index();
	splcbuffer.upload_indices(hpswap.upload_splice);
	sborder.upload_int("cnt_height",hpswap.max_height);
	glDrawArraysInstanced(GL_LINES,0,2,hpswap.upload_splice.size()/SL_REPEAT);

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
	hpswap.dmg_threshold += dmg*(frdy==HBState::READY);
}

/*
	floating_nanobars() -> void (private)
	purpose: let nanobars float slightly based on the slicing directions
*/
void Healthbar::floating_nanobars()
{
	for (int i=0+1000*((uint8_t)frdy<2);i<hpswap.mntm.size();i++) {

		// apply momentum to position
		hpswap.upload[i*PT_REPEAT+7] += hpswap.mntm[i].x;
		hpswap.upload[i*PT_REPEAT+8] += hpswap.mntm[i].y;

		// mellow momentum through appearance of resistance
		hpswap.mntm[i] *= glm::vec2(NBMOMENTUM_RESISTANCE);
	}
}

/*
	fill_hpbar(HBState&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: animate nano healthbar filling and capping value at max
*/
void Healthbar::fill_hpbar(HBState &frdy,HPBarSwap &hpswap)
{
	// setup for upload creation
	uint8_t ihp = hpswap.hpbar_itr;		// readability of healthbar iteration
	int32_t rnd_edge_dwn = 0,rnd_edge_up = 0;

	// load upload vector and its targets
	for (int i=0+1000*(hpswap.upload.size()>0);i<hpswap.dest_pos[ihp].size();i++) {

		// basis for upload and targets
		hpswap.upload.push_back(hpswap.dest_pos[ihp][i]);			// x-axis position offset
		hpswap.upload.push_back(0);		// current hp in healthbar
		hpswap.upload.push_back(0);		// current damage to health

		// left edge transformation for current nanobar
		hpswap.upload.push_back(rnd_edge_dwn);	// randomized left lower edge modification
		hpswap.upload.push_back(rnd_edge_up);	// randomized left upper edge modification

		// generate new healthbar cut if not at last nanobar
		bool inner_edge = i!=(hpswap.dest_pos[ihp].size()-1);
		rnd_edge_dwn = (rand()%30-15)*inner_edge,rnd_edge_up = (rand()%30-15)*inner_edge;

		// right edge transformation for current nanobar
		hpswap.upload.push_back(rnd_edge_dwn);	// randomized right lower edge modification
		hpswap.upload.push_back(rnd_edge_up);	// randomized right upper edge modification

		// vertex information placeholders for floating effect
		hpswap.upload.push_back(0);
		hpswap.upload.push_back(0);

		// upload tail specifying width target after fill
		hpswap.upload.push_back(hpswap.dest_wdt[ihp][i]);

		// memory reservation for leftest nanobar momentum
		hpswap.mntm.push_back(glm::vec2(0));
	}

	// filling width & dmg until target
	uint8_t ritr = hpswap.target_itr*PT_REPEAT;		// rasterization of one dimensional list
	bool full_bar = hpswap.upload[ritr+1]>=hpswap.upload[ritr+9];
	hpswap.target_itr += full_bar;
	hpswap.upload[ritr+1] += 4;
	hpswap.upload[ritr+1] = hpswap.upload[ritr+9]*full_bar+hpswap.upload[ritr+1]*!full_bar;
	hpswap.upload[ritr+2] += 4;
	hpswap.upload[ritr+2] = hpswap.upload[ritr+9]*full_bar+hpswap.upload[ritr+2]*!full_bar;

	// cap upload values to ideal once target is full
	hpswap.upload[ritr+1] = hpswap.upload[ritr+9]*full_bar+hpswap.upload[ritr+1]*!full_bar;
	hpswap.upload[ritr+2] = hpswap.upload[ritr+9]*full_bar+hpswap.upload[ritr+2]*!full_bar;

	// signal ready to splice if bars full
	frdy = (HBState)((uint8_t)frdy+hpswap.target_itr>=(hpswap.upload.size()/PT_REPEAT));
	hpswap.target_itr -= (uint8_t)frdy;
}

/*
	splice_hpbar(HBState&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: insert cut geometry between nano healthbars
*/
void Healthbar::splice_hpbar(HBState &frdy,HPBarSwap &hpswap)
{
	// defining splice index upload
	for (int i=1+1000*(hpswap.upload_splice.size()>0);i<hpswap.upload.size()/PT_REPEAT;i++) {

		// calculate vector continuation
		uint8_t ridx = i*PT_REPEAT+3;
		glm::vec2 splice_dwn = glm::vec2(hpswap.upload[i*PT_REPEAT]+hpswap.upload[ridx],0);
		glm::vec2 splice_up =
				glm::vec2(hpswap.upload[i*PT_REPEAT]+hpswap.upload[ridx+1],hpswap.max_height);
		glm::vec2 splice_dir = glm::normalize(splice_up-splice_dwn);
		glm::vec2 upload_dwn = splice_dwn-splice_dir*glm::vec2(SPLICE_ELONGATION),
				upload_up = glm::vec2(splice_up.x,0)+splice_dir*glm::vec2(SPLICE_ELONGATION);

		// upload splice continuations
		hpswap.upload_splice.push_back(upload_dwn.x);
		hpswap.upload_splice.push_back(upload_dwn.y);
		hpswap.upload_splice.push_back(upload_up.x);
		hpswap.upload_splice.push_back(upload_up.y);
		hpswap.upload_splice.push_back(0);

		// calculate nanobar momentum based on the direction of the splices
		int8_t dir_mulid = (splice_dir.x<=0)-(splice_dir.x>0);
		hpswap.mntm[i].y += SLICE_BLOWBACK*dir_mulid;
		hpswap.mntm[i-1].y += SLICE_BLOWBACK*-dir_mulid;
	}

	// animate splicing
	uint8_t amt_splice = hpswap.upload_splice.size()/SL_REPEAT;
	float per_splice = 1.0f/(SPLICE_TICKS/amt_splice);

	// all index spreads get updated
	uint8_t index = 0;
	while (index<amt_splice) {

		// add length to splice when incomplete
		uint8_t j = index*SL_REPEAT+4;
		bool ncomplete = hpswap.upload_splice[j]<1;
		hpswap.upload_splice[j] += per_splice*ncomplete;

		// halt at index if incomplete
		index += !ncomplete;
		index += (amt_splice+1)*ncomplete;
	}

	// signal splice readiness
	uint8_t ifrdy = (uint8_t)frdy+(index==amt_splice);
	frdy = (HBState)ifrdy;
}

/*
	count_phases(HBState&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: animated phase count up from 0 towards full amount to increase tension
*/
void Healthbar::count_phases(HBState &frdy,HPBarSwap &hpswap)
{
	// skip animation if not in first phase
	hpswap.anim_tick += POT*(hpswap.hpbar_itr>0);

	// increase phase counter
	Camera2D tc2d = Camera2D(1280.0f,720.0f);
	uint8_t split_ticks = POT/(hpswap.dest_pos.size()+2);  // ticks after scaling is reversed
	uint8_t aprog = hpswap.dest_pos.size()*((hpswap.anim_tick+split_ticks)/POT);
	std::string pprefix = (hpswap.anim_tick>=POT)?std::to_string(hpswap.hpbar_itr+1)+'/':"";
	hpswap.phcnt.clear();
	hpswap.phcnt.add((pprefix+std::to_string(aprog)).c_str(),glm::vec2(0,0));
	hpswap.phcnt.load(&tc2d);
	// TODO: split both counters in different text objects

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
	frdy = (HBState)((uint8_t)frdy+anim_ready);
	hpswap.anim_tick -= hpswap.anim_tick*anim_ready;
}

/*
	ready_hpbar(HBState&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: checks if current nano healthbar is depleted and if so, changes to the previous one.
		if all bars are empty, the method signals a refill.
		if the last batch of bars exceeds the maximum amount of phases, a clearstate is signaled.
*/
void Healthbar::ready_hpbar(HBState &frdy,HPBarSwap &hpswap)
{
	// subtract nanobar hp by damage in threshold
	hpswap.upload[hpswap.target_itr*PT_REPEAT+2] -= hpswap.dmg_threshold;
	hpswap.dmg_threshold = 0;

	// decrease target iteration if nanobar is empty
	hpswap.target_itr -= hpswap.upload[hpswap.target_itr*PT_REPEAT+2]<=0;

	// signal refill if all bars are empty
	bool hpbar_empty = hpswap.target_itr<0;
	hpswap.target_itr += hpbar_empty;
	hpswap.hpbar_itr += hpbar_empty;
	frdy = (HBState)((uint8_t)frdy+hpbar_empty+(hpswap.hpbar_itr>=hpswap.dest_pos.size()));
}

/*
	reset_hpbar(HBState&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: reset hpswap upload and target lists to later refill them with new information
*/
void Healthbar::reset_hpbar(HBState &frdy,HPBarSwap &hpswap)
{
	// ready hpswap lists for refill
	hpswap.upload.clear();
	hpswap.upload_splice.clear();
	hpswap.mntm.clear();

	// signal refill
	frdy = HBState::FILLING;
}

/*
	signal_clear(HBState&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: represent boss clearstate with the healthbar
*/
void Healthbar::signal_clear(HBState &frdy,HPBarSwap &hpswap)
{
	// TODO: visualize clear
}