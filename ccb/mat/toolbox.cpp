#include "toolbox.h"

/*
	load_object(const char*,vector<float>&,vec3,float scl,vec3) -> void (static) !O(n)
	purpose: load object from file
	\param path: path to .obj file
	\param ov: vertex array to save extracted data from .obj file to
	\param pos: direct modification of vertex positions
	\param scl: direct modification of object scaling
	\param rot: direct modification of vertex rotation around the object's origin point#
	\returns amount of written vertices (vertices NOT values)
*/
uint32_t Toolbox::load_object(const char* path,std::vector<float>& ov,glm::vec3 pos,
		float scl,glm::vec3 rot)
{
	// setup vertex information lists
	std::vector<uint32_t> ovi,oui,oni;
	std::vector<glm::vec3> verts,norm;
	std::vector<glm::vec2> uv;

	// read source file
	FILE* file = fopen(path,"r");
	while(true) {

		// read next line if exists
		char lh[128];
		int res = fscanf(file,"%s",lh);
		if (res==EOF) break;
		else {

			// check value prefix
			// position prefix
			if (strcmp(lh,"v")==0) {
				glm::vec3 p;
				fscanf(file,"%f %f %f\n",&p.x,&p.y,&p.z);
				verts.push_back(p);
			}

			// texture coordinate prefix
			else if (strcmp(lh,"vt")==0) {
				glm::vec2 p;
				fscanf(file,"%f %f\n",&p.x,&p.y);
				uv.push_back(p);
			}

			// normals prefix
			else if (strcmp(lh,"vn")==0) {
				glm::vec3 p;
				fscanf(file,"%f %f %f\n",&p.x,&p.y,&p.z);
				norm.push_back(p);
			}

			// faces prefix
			else if(strcmp(lh,"f")==0) {

				// read element node for current triangle
				unsigned int vi[3],ui[3],ni[3];
				fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vi[0],&ui[0],&ni[0],&vi[1],&ui[1],&ni[1],&vi[2],&ui[2],&ni[2]);
				glm::vec3 pproc = glm::vec3(vi[0],vi[1],vi[2]);
				glm::vec3 nproc = glm::vec3(ni[0],ni[1],ni[2]);

				// translate triangle
				transform_vector(pproc,pos,scl,rot);
				rotate_vector(nproc,rot);

				// save values position vertices
				ovi.push_back(vi[0]),ovi.push_back(vi[1]),ovi.push_back(vi[2]);
				oui.push_back(ui[0]),oui.push_back(ui[1]),oui.push_back(ui[2]);
				oni.push_back(ni[0]),oni.push_back(ni[1]),oni.push_back(ni[2]);
			}
		}
	}

	// translate vertex data to object vertices
	uint32_t out = 0;
	glm::vec3 tg(1.0f);
	for(int i=0;i<ovi.size();i++) {

		// precalculations for normal mapping
		if (i%3==0&&ovi.size()) {
			glm::vec3 e1 = verts[ovi[i+1]-1]-verts[ovi[i]-1];
			glm::vec3 e2 = verts[ovi[i+2]-1]-verts[ovi[i]-1];
			glm::vec2 duv1 = uv[oui[i+1]-1]-uv[oui[i]-1];
			glm::vec2 duv2 = uv[oui[i+2]-1]-uv[oui[i]-1];

			// calculate tangent
			float ff = 1.0f/(duv1.x*duv2.y-duv2.x*duv1.y);
			tg.x = ff*(duv2.y*e1.x-duv1.y*e2.x);
			tg.y = ff*(duv2.y*e1.y-duv1.y*e2.y);
			tg.z = ff*(duv2.y*e1.z-duv1.y*e2.z);
			tg = glm::normalize(tg);
			// FIXME: using a matrix calculation might be significantly faster
		}

		// get read vertices to process and save
		unsigned int tvi = ovi[i],tui = oui[i],tni = oni[i];
		glm::vec3 tv = verts[tvi-1],tn = norm[tni-1];
		glm::vec2 tu = uv[tui-1];

		// translate vertices & normals
		transform_vector(tv,pos,scl,rot);
		rotate_vector(tn,rot);

		// save data to buffer vector
		ov.push_back(tv.x),ov.push_back(tv.y),ov.push_back(tv.z);
		ov.push_back(tu.x),ov.push_back(tu.y);
		ov.push_back(tn.x),ov.push_back(tn.y),ov.push_back(tn.z);
		ov.push_back(tg.x),ov.push_back(tg.y),ov.push_back(tg.z);
		out++;
	}
	return out;
}
// FIXME: it was written [-NAS] a long time ago, there are some things to optimize here

/*
	calculate_vecangle(vec2,vec2) -> float (static)
	a: first vector, 0 degrees towards the origin
	b: second vector, returned degrees from first vector
	returns: angle between vectors in radians
*/
float Toolbox::calculate_vecangle(glm::vec2 a,glm::vec2 b)
{ return glm::acos(glm::dot(a,b)/(glm::length(a)*glm::length(b))); }

/*
	transform_vector(vec3&,vec3,float,vec3) -> void (static)
	pos: additive modification of vector direction & endpoint
	scl: multiplicative modification of vector length
	purpose: full vector transformation
*/
void Toolbox::transform_vector(glm::vec3 &ov,glm::vec3 pos,float scl,glm::vec3 rot)
{
	rotate_vector(ov,rot);
	ov *= scl,ov += pos;
}

/*
	rotate_vector(vec3&,vec3) -> void (static)
	ov: vector to be transformed
	rot: rotation of vector, without directional reset
	purpose: 3D vector rotation
*/
void Toolbox::rotate_vector(glm::vec3 &ov,glm::vec3 rot)
{
	ov = glm::rotate(ov,glm::radians(rot.x),glm::vec3(1,0,0));
	ov = glm::rotate(ov,glm::radians(rot.y),glm::vec3(0,1,0));
	ov = glm::rotate(ov,glm::radians(rot.z),glm::vec3(0,0,1));
}

/*
	!O(1)b /+function -> (public,static)
	purpose: convert a boolean to a float with controlled delay
	\param tval: reference of float, being transformed according to boolean
	\param tspeed: controlled delay interval
	\param cnd: boolean condition, defining target float value
*/
void Toolbox::transition_float_on_condition(float &tval,float tspeed,bool cnd)
{
	tval += tspeed*(cnd-!cnd);
	tval = (tval<.0f) ? .0f : (tval>1.f) ? 1.f : tval;
	/*uint8_t tmin = (mtransition<.0f),tmax = (mtransition>1.f);
	mtransition = mtransition-(mtransition-1.f)*tmax+abs(mtransition)*tmin;*/
	// TODO: compare linear transition with sinespeed transition implementation
	// 		also relate the results of this todo to the ftransition in main render method
}
// TODO: optimize further (!use assembly translation as guide!)
// TODO: maybe even set some bounds to enhance the transition range possibilities
// TODO: i feel like this implementation lacks wit

/*
	start_debug_logging(DebugLogData&,const char*) -> void
	purpose: start time difference debug recording
	\param dld: variable holding runtime debug data, such as recording keys & their string id
	\param tname: title of debug log, to describe output information to recognize their allegiance
*/
void Toolbox::start_debug_logging(DebugLogData &dld,const char* tname)
{
	dld.task_name = tname;
	dld.last_ticks = std::chrono::steady_clock::now();
	printf("\033[1;35mprocessing -> %s\033[0m",tname);
}

/*
	add_timekey(DebugLogData&,const char*) -> void (static) !O(1)
	purpose: create a timekey, recording the time difference since the last key or the data creation
	\param dld: variable holding runtime debug data, such as recording keys & their string id
	\param kname: string id of created key, to describe what was achieved within the recorded time
*/
void Toolbox::add_timekey(DebugLogData &dld,const char* kname)
{
	// add time log key
	DebugLogKey nkey = {
		.key_name = kname,
		.delta_ticks = (std::chrono::steady_clock::now()-dld.last_ticks).count()*CONVERSION_MULT_MILLISECONDS,
	};
	dld.key_list.push_back(nkey);

	// update needed leftmost table cell width
	uint8_t key_width = strlen(kname);
	dld.max_name_width = (dld.max_name_width<key_width) ? key_width : dld.max_name_width;

	// signal progression
	printf("...");
	
	// reset counter
	dld.last_ticks = std::chrono::steady_clock::now();
}

/*
	flush_debug_logging(DebugLogData) -> void (static) !O(1)
	purpose: writes structured runtime debug log data to the console
	\param dld: variable holding runtime debug data, such as recording keys & their string id
*/
void Toolbox::flush_debug_logging(DebugLogData dld)
{
	// output task headline
	std::string head_line = '|'+std::string(dld.max_name_width+23,'-')+'|';
	printf("\n%s\n",head_line.c_str());
	printf("%s\n",produce_logging_cell(dld.task_name,"\033[1;34m",dld.max_name_width+21).c_str());

	// output table contents
	std::string pref_line = '|'+std::string(dld.max_name_width+2,'-')+'|'+std::string(20,'-')+'|';
	printf("%s\n",pref_line.c_str());
	double total_time = .0;
	for(auto key : dld.key_list) {
		printf("%s%17fms |\n",
				produce_logging_cell(key.key_name,"\033[1;32m",dld.max_name_width).c_str(),key.delta_ticks);
		total_time += key.delta_ticks;
	}

	// output result row and close table
	printf("%s\n",pref_line.c_str());
	printf("%s%17fms |\n",produce_logging_cell("total","\033[1;35m",dld.max_name_width).c_str(),total_time);
	printf("%s\n\n",head_line.c_str());
}

/*
	TODO
*/
void Toolbox::thread_detached_continue(ThreadState& state)
{
	std::unique_lock lock(state.mux);
	lock.unlock();
	state.cond.notify_one();
}

/*
	TODO
*/
void Toolbox::thread_detached_stop(ThreadState& state)
{
	std::unique_lock lock(state.mux);
	state.cond.wait(lock);
}
// FIXME: todo unique lock creation may abuse performace a bit,
//		also parameters are many but my patience is limited...

/*
	PARAMETER DEFINITIONS:
	vs: reference to vertex array
	ofs: memory offset of vertex array write. !already rasterized, not an index. actual float offset!
	pos: origin position of canvas
	width: vertex distanced width of canvas
	height: vertex distanced height of canvas
*/

/*
	O(1) /+load -> (static)
	purpose: create vertices for a sprite, supported by an additional element buffer
	NOTE: expects memory to be allocated
*/
void Toolbox::create_sprite_canvas(std::vector<float>& vs,size_t& ofs,glm::vec2 pos,float width,float height)
{
	vs[ofs++] = pos.x,			vs[ofs++] = pos.y+height,	vs[ofs++] = .0f,	vs[ofs++] = .0f,
	vs[ofs++] = pos.x+width,	vs[ofs++] = pos.y+height,	vs[ofs++] = 1.f,	vs[ofs++] = .0f,
	vs[ofs++] = pos.x+width,	vs[ofs++] = pos.y,			vs[ofs++] = 1.f,	vs[ofs++] = 1.f,
	vs[ofs++] = pos.x,			vs[ofs++] = pos.y,			vs[ofs++] = .0f,	vs[ofs++] = 1.f;
}

/*
	O(1) /+load -> (static)
	purpose: create vertices for a sprite without element buffer, drawn per triangle
	NOTE: expects memory to be allocated
*/
void Toolbox::create_sprite_canvas_triangled(std::vector<float>& vs,size_t& ofs,
		glm::vec2 pos,float width,float height)
{
	vs[ofs++] = pos.x,			vs[ofs++] = pos.y+height,	vs[ofs++] = .0f,	vs[ofs++] = .0f,
	vs[ofs++] = pos.x+width,	vs[ofs++] = pos.y,			vs[ofs++] = 1.f,	vs[ofs++] = 1.f,
	vs[ofs++] = pos.x+width,	vs[ofs++] = pos.y+height,	vs[ofs++] = 1.f,	vs[ofs++] = .0f,
	vs[ofs++] = pos.x+width,	vs[ofs++] = pos.y,			vs[ofs++] = 1.f,	vs[ofs++] = 1.f,
	vs[ofs++] = pos.x,			vs[ofs++] = pos.y+height,	vs[ofs++] = .0f,	vs[ofs++] = .0f,
	vs[ofs++] = pos.x,			vs[ofs++] = pos.y,			vs[ofs++] = .0f,	vs[ofs++] = 1.f;
}

/*
	O(1) /+load -> (static)
	purpose: generate buffer elements based on object list index
	\param k0: reference to current element memory offset. !actual memory offset, not to be rasterized!
	\param k1: current triangle starting vertex index
	\param e: element array
	NOTE: expects memory to be allocated
*/
void Toolbox::generate_elements(size_t& k0,size_t& k1,std::vector<uint32_t>& e)
{
	e[k0++] = k1,		e[k0++] = k1+2,		e[k0++] = k1+1,
	e[k0++] = k1+2,		e[k0++] = k1,		e[k0++] = k1+3;
	k1 += PATTERN_SPRITE_LOAD_REPEAT;;
}
