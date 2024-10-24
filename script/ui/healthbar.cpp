#include "healthbar.h"

typedef void (*healthbar_state)(uint8_t&,HPBarSwap&);

/*
	!O(n)m .healthbar destinations /function -> healthbar_state (local,static)
	purpose: memory allocation and data preparation for healthbar filling animation.
		this will be executed once and transitioned immediately into fill_hpbar for the next loop cycle.
*/
void prepare_hpbar(uint8_t& frdy,HPBarSwap& hpswap)
{
	// setup for upload creation
	uint8_t ihp = hpswap.hpbar_itr;
	int32_t rnd_edge_dwn = 0,rnd_edge_up = 0;

	// memory allocation
	size_t iterations = hpswap.dest[ihp].size();
	hpswap.upload = std::vector<HBarIndexUpload>(iterations);
	hpswap.mntm = std::vector<glm::vec2>(iterations,glm::vec2(0));

	// load upload vector and its targets
	for (int i=0;i<iterations;i++) {

		// positioning
		hpswap.upload[i].offset_x = hpswap.dest[ihp][i].position;

		// left edge transformation for current nanobar
		hpswap.upload[i].edgemod_left_lower = rnd_edge_dwn;
		hpswap.upload[i].edgemod_left_upper = rnd_edge_up;

		// generate new healthbar cut if not at last nanobar
		bool inner_edge = i!=(hpswap.dest[ihp].size()-1);
		rnd_edge_dwn = (rand()%30-15)*inner_edge,rnd_edge_up = (rand()%30-15)*inner_edge;

		// right edge transformation for current nanobar
		hpswap.upload[i].edgemod_right_lower = rnd_edge_dwn;
		hpswap.upload[i].edgemod_right_upper = rnd_edge_up;

		// upload tail specifying width target after fill
		hpswap.upload[i].target_width = hpswap.dest[ihp][i].width;
	}
	frdy++;
}

/*
	fill_hpbar(uint8_t&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: animate nano healthbar filling and capping value at max
*/
void fill_hpbar(uint8_t& frdy,HPBarSwap& hpswap)
{
	// filling width & dmg until target
	uint8_t ritr = hpswap.target_itr;
	bool full_bar = hpswap.upload[ritr].bar_value>=hpswap.upload[ritr].target_width;
	hpswap.target_itr += full_bar;
	hpswap.upload[ritr].bar_value += 4;
	hpswap.upload[ritr].bar_value
			= hpswap.upload[ritr].target_width*full_bar+hpswap.upload[ritr].bar_value*!full_bar;
	hpswap.upload[ritr].damage += 4;
	hpswap.upload[ritr].damage
			= hpswap.upload[ritr].target_width*full_bar+hpswap.upload[ritr].damage*!full_bar;

	// cap upload values to ideal once target is full
	hpswap.upload[ritr].bar_value
			= hpswap.upload[ritr].target_width*full_bar+hpswap.upload[ritr].bar_value*!full_bar;
	hpswap.upload[ritr].damage
			= hpswap.upload[ritr].target_width*full_bar+hpswap.upload[ritr].damage*!full_bar;

	// signal ready to splice if bars full
	frdy += hpswap.target_itr>=hpswap.upload.size();
	hpswap.target_itr -= (frdy-1);
}

/*
	!O(n)m .amount of splits between nanobars /function -> healthbar_state (local,static)
	purpose: allocate memory and prepare data for nanobar splice animation
*/
void prepare_splices(uint8_t& frdy,HPBarSwap& hpswap)
{
	// defining splice index upload
	hpswap.upload_splice = std::vector<HBarSpliceIndexUpload>(hpswap.upload.size()-1);
	for (int i=1;i<hpswap.upload.size();i++) {

		// calculate vector continuation
		glm::vec2 splice_dwn = glm::vec2(
				hpswap.upload[i].offset_x+hpswap.upload[i].edgemod_left_lower,
				0
			);
		glm::vec2 splice_up = glm::vec2(
				hpswap.upload[i].offset_x+hpswap.upload[i].edgemod_left_upper,
				hpswap.max_height
			);
		glm::vec2 splice_dir = glm::normalize(splice_up-splice_dwn);
		glm::vec2 upload_dwn = splice_dwn-splice_dir*glm::vec2(SPLICE_ELONGATION),
				upload_up = glm::vec2(splice_up.x,0)+splice_dir*glm::vec2(SPLICE_ELONGATION);

		// upload splice continuations
		hpswap.upload_splice[i-1] = {
			.start = upload_dwn,
			.end = upload_up
		};

		// calculate nanobar momentum based on the direction of the splices
		int8_t dir_mulid = (splice_dir.x<=0)-(splice_dir.x>0);
		hpswap.mntm[i].y += SLICE_BLOWBACK*dir_mulid;
		hpswap.mntm[i-1].y += SLICE_BLOWBACK*-dir_mulid;
	}
	frdy++;
}

/*
	splice_hpbar(uint8_t&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: insert cut geometry between nano healthbars
*/
void splice_hpbar(uint8_t& frdy,HPBarSwap& hpswap)
{
	// animate splicing
	uint8_t amt_splice = hpswap.upload_splice.size();
	float per_splice = 1.0f/(SPLICE_TICKS/amt_splice);

	// all index spreads get updated
	uint8_t index = 0;
	while (index<amt_splice) {

		// add length to splice when incomplete
		bool ncomplete = hpswap.upload_splice[index].spread<1;
		hpswap.upload_splice[index].spread += per_splice*ncomplete;

		// halt at index if incomplete
		index += !ncomplete;
		index += (amt_splice+1)*ncomplete;
	}

	// signal splice readiness
	frdy += index==amt_splice;
}

/*
	count_phases(uint8_t&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: animated phase count up from 0 towards full amount to increase tension
*/
void count_phases(uint8_t& frdy,HPBarSwap& hpswap)
{
	// skip animation if not in first phase
	hpswap.anim_tick += POT*(hpswap.hpbar_itr>0);

	// increase phase counter
	uint8_t split_ticks = POT/(hpswap.dest.size()+2);  // ticks after scaling is reversed
	uint8_t aprog = hpswap.dest.size()*((hpswap.anim_tick+split_ticks)/POT);
	std::string pprefix = (hpswap.anim_tick>=POT) ? std::to_string(hpswap.hpbar_itr+1)+'/' : "";
	hpswap.phcnt.clear();
	hpswap.phcnt.add((pprefix+std::to_string(aprog)).c_str(),glm::vec2(0,0));
	hpswap.phcnt.load();
	// TODO: split both counters in different text objects

	// zoom scroll phase counter at increment
	uint8_t mticks = hpswap.anim_tick%split_ticks;		// animation ticks after last revert
	bool upscale = (hpswap.anim_tick/split_ticks)&1;	// distiguishing up and downscale phases
	float zmscale = .5f*(((float)mticks/split_ticks)*(-2*upscale+1)+upscale);

	// upload scroll towards expectation
	glm::mat4 mdl_trans = glm::translate(glm::mat4(1.0f),
			glm::vec3(hpswap.position.x+hpswap.max_width-TEXT_MV,hpswap.position.y+TEXT_DV,0));
	mdl_trans = glm::scale(mdl_trans,glm::vec3(1+zmscale,1+zmscale,1));
	hpswap.phcnt.set_scroll(mdl_trans);

	// signal hpbar readiness
	hpswap.anim_tick++;
	bool anim_ready = hpswap.anim_tick>POT;
	frdy = frdy+anim_ready;
	hpswap.anim_tick -= hpswap.anim_tick*anim_ready;
}

/*
	ready_hpbar(uint8_t&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: checks if current nano healthbar is depleted and if so, changes to the previous one.
		if all bars are empty, the method signals a refill.
		if the last batch of bars exceeds the maximum amount of phases, a clearstate is signaled.
*/
void ready_hpbar(uint8_t& frdy,HPBarSwap& hpswap)
{
	// accelerate healthbar momentum when empty
	for (int i=0;i<hpswap.mntm.size();i++) {

		// get splice direction
		glm::vec2 splice_dwn = glm::vec2(
				hpswap.upload[i].offset_x+hpswap.upload[i].edgemod_left_lower,
				0
			);
		glm::vec2 splice_up = glm::vec2(
				hpswap.upload[i].offset_x+hpswap.upload[i].edgemod_left_upper,
				hpswap.max_height
			);
		glm::vec2 splice_dir = glm::normalize(splice_up-splice_dwn);
		// FIXME: repeat code. reduce!

		// accellerate if empty
		bool not_empty = hpswap.upload[i].damage>0;
		hpswap.mntm[i] += splice_dir*glm::vec2(ACC_CLEAREDBAR*!not_empty);

		// despawn slices between empty nanobars
		uint8_t curr_splice = (i-1)*(i!=0);
		hpswap.upload_splice[curr_splice].spread -= RED_DISCONSPLC*!not_empty;
		hpswap.upload_splice[curr_splice].spread += -hpswap.upload_splice[curr_splice].spread
				* (hpswap.upload_splice[curr_splice].spread<0);
	}

	// subtract nanobar hp by damage in threshold
	hpswap.upload[hpswap.target_itr].damage -= hpswap.dmg_threshold;
	hpswap.dmg_threshold = 0;

	// decrease target iteration if nanobar is empty
	hpswap.target_itr -= hpswap.upload[hpswap.target_itr].damage<=0;

	// signal refill if all bars are empty
	bool hpbar_empty = hpswap.target_itr<0;
	hpswap.target_itr += hpbar_empty;
	hpswap.hpbar_itr += hpbar_empty;
	frdy += hpbar_empty+(hpswap.hpbar_itr>=hpswap.dest.size());
}

/*
	reset_hpbar(uint8_t&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: reset hpswap upload and target lists to later refill them with new information
*/
void reset_hpbar(uint8_t& frdy,HPBarSwap& hpswap)
{
	// ready hpswap lists for refill
	hpswap.upload.clear();
	hpswap.upload_splice.clear();
	hpswap.mntm.clear();

	// signal refill
	frdy = HBState::PREPARATION;
}

/*
	signal_clear(uint8_t&,HPBarSwap&) -> void (private,static)
	conforming to: fill_switch
	purpose: represent boss clearstate with the healthbar
*/
void signal_clear(uint8_t& frdy,HPBarSwap& hpswap)
{
	// TODO: visualize clear
}

healthbar_state fill_switch[] = {
	prepare_hpbar,
	fill_hpbar,
	prepare_splices,
	splice_hpbar,
	count_phases,
	ready_hpbar,
	reset_hpbar,
	signal_clear
};


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

	// vertices hpbar
	float hpverts[] = {
		pos.x,pos.y,0, pos.x,pos.y+height,3, pos.x,pos.y+height,1,
		pos.x,pos.y+height,3, pos.x,pos.y,0, pos.x,pos.y,2,
	};
	hpbuffer.bind();
	hpbuffer.upload_vertices(hpverts,sizeof(hpverts));
	hpbuffer.add_buffer();

	// compile hpbar shader
	shp.compile("./shader/healthbar/bar.vs","./shader/healthbar/bar.fs");
	shp.def_attributeF("position",2,0,3);
	shp.def_attributeF("edge_id",1,2,3);

	// hpbar indexing
	size_t vsize = sizeof(HBarIndexUpload);
	hpbuffer.bind_index();
	shp.def_irregular_indexF("ofs",1,vsize,offsetof(HBarIndexUpload,offset_x));
	shp.def_irregular_indexF("wdt",1,vsize,offsetof(HBarIndexUpload,bar_value));
	shp.def_irregular_indexF("dmg",1,vsize,offsetof(HBarIndexUpload,damage));
	shp.def_irregular_indexF("edg_trans[0]",1,vsize,offsetof(HBarIndexUpload,edgemod_left_lower));
	shp.def_irregular_indexF("edg_trans[1]",1,vsize,offsetof(HBarIndexUpload,edgemod_left_upper));
	shp.def_irregular_indexF("edg_trans[2]",1,vsize,offsetof(HBarIndexUpload,edgemod_right_lower));
	shp.def_irregular_indexF("edg_trans[3]",1,vsize,offsetof(HBarIndexUpload,edgemod_right_upper));
	shp.def_irregular_indexF("flt",2,vsize,offsetof(HBarIndexUpload,floating));
	shp.def_irregular_indexF("target",1,vsize,offsetof(HBarIndexUpload,target_width));

	// 2D projection hpbar
	shp.upload_camera();

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
	sborder.compile("./shader/healthbar/border.vs","./shader/healthbar/border.fs");
	sborder.def_attributeF("position",2,0,3);
	sborder.def_attributeF("edge_id",1,2,3);

	// border indexing
	brdbuffer.bind_index();
	sborder.def_irregular_indexF("ofs",1,vsize,offsetof(HBarIndexUpload,offset_x));
	sborder.def_irregular_indexF("wdt",1,vsize,offsetof(HBarIndexUpload,bar_value));
	sborder.def_irregular_indexF("dmg",1,vsize,offsetof(HBarIndexUpload,damage));
	sborder.def_irregular_indexF("edg_trans[0]",1,vsize,offsetof(HBarIndexUpload,edgemod_left_lower));
	sborder.def_irregular_indexF("edg_trans[1]",1,vsize,offsetof(HBarIndexUpload,edgemod_left_upper));
	sborder.def_irregular_indexF("edg_trans[2]",1,vsize,offsetof(HBarIndexUpload,edgemod_right_lower));
	sborder.def_irregular_indexF("edg_trans[3]",1,vsize,offsetof(HBarIndexUpload,edgemod_right_upper));
	sborder.def_irregular_indexF("flt",2,vsize,offsetof(HBarIndexUpload,floating));
	sborder.def_irregular_indexF("target",1,vsize,offsetof(HBarIndexUpload,target_width));

	// 2D projection border
	sborder.upload_camera();

	// vertices indexed splice
	float splcverts[] = { pos.x,pos.y,0, pos.x,pos.y+height,1, };
	splcbuffer.bind();
	splcbuffer.upload_vertices(splcverts,sizeof(splcverts));
	splcbuffer.add_buffer();

	// compile splice shader
	ssplice.compile("./shader/healthbar/splice.vs","./shader/healthbar/splice.fs");
	ssplice.def_attributeF("position",2,0,3);
	ssplice.def_attributeF("edge_id",1,2,3);

	// splice indexing
	vsize = sizeof(HBarSpliceIndexUpload);
	splcbuffer.bind_index();
	ssplice.def_irregular_indexF("ofs[0]",2,vsize,offsetof(HBarSpliceIndexUpload,start));
	ssplice.def_irregular_indexF("ofs[1]",2,vsize,offsetof(HBarSpliceIndexUpload,end));
	ssplice.def_irregular_indexF("spread",1,vsize,offsetof(HBarSpliceIndexUpload,spread));

	// 2D projection splice
	ssplice.upload_camera();

	// calculate render specifications
	uint8_t t_index = 0;
	hpswap.dest = std::vector<std::vector<HBarDestination>>(phases.size());
	for (int i=0;i<phases.size();i++) {

		// calculate current phases combined hp
		float target_hp = 0;
		for (int j=0;j<phases[i];j++)
			target_hp += hp[t_index+j];

		// memory allocation
		hpswap.dest[i] = std::vector<HBarDestination>(phases[i]);

		// calculate positions and widths
		std::vector<float> t_pos,t_wdt;
		float draw_cursor = 0;
		for (int j=0;j<phases[i];j++) {
			hpswap.dest[i][j].position = draw_cursor;
			float t_width = (hp[t_index+j]/target_hp)*width;
			hpswap.dest[i][j].width = t_width;
			draw_cursor += t_width;
		}

		// increase index above phase counter
		t_index += phases[i];
	}

	// set name and phase counter
	Font hbfont = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",
			HB_TEXT_SIZE,HB_TEXT_SIZE);
	hpswap.phname = Text(hbfont);
	hpswap.phname.add(boss_name,glm::vec2(pos.x+TEXT_MV,pos.y+TEXT_DV));
	hpswap.phname.load();
	hpswap.phcnt = Text(hbfont);
	hpswap.phcnt.add("0/X",glm::vec2(0));
	hpswap.phcnt.load();
}

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
	glDrawArraysInstanced(GL_TRIANGLES,0,6,hpswap.upload.size());

	// setup & draw border
	sborder.enable();
	brdbuffer.bind();
	brdbuffer.bind_index();
	brdbuffer.upload_indices(hpswap.upload);
	sborder.upload_int("cnt_height",hpswap.max_height);
	sborder.upload_int("brd_space",BORDER_CLEARING);
	glDrawArraysInstanced(GL_LINES,0,8,hpswap.upload.size());

	// setup & draw splice
	ssplice.enable();
	splcbuffer.bind();
	splcbuffer.bind_index();
	splcbuffer.upload_indices(hpswap.upload_splice);
	ssplice.upload_int("cnt_height",hpswap.max_height);
	glDrawArraysInstanced(GL_LINES,0,2,hpswap.upload_splice.size());

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
{ hpswap.dmg_threshold += dmg*(frdy==HBState::READY); }

/*
	floating_nanobars() -> void (private)
	purpose: let nanobars float slightly based on the slicing directions
*/
void Healthbar::floating_nanobars()
{
	for (int i=0+1000*((uint8_t)frdy<2);i<hpswap.mntm.size();i++) {

		// apply momentum to position
		hpswap.upload[i].floating += hpswap.mntm[i];

		// mellow momentum through appearance of resistance
		hpswap.mntm[i] *= glm::vec2(NBMOMENTUM_RESISTANCE);
	}
}
