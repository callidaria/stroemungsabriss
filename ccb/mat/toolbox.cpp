#include "toolbox.h"

/*
	load_object(const char*,vector<float>&,vec3,float scl,vec3) -> void (static)
	path: path to .obj file
	ov: vertices, extracted from given .obj file
	pos: direct modification of vertex positions
	scl: direct modification of object scaling
	rot: direct modification of vertex rotation around the object's origin point
	purpose: load object from file
	// TODO: update documentation
*/
uint32_t Toolbox::load_object(const char* path,std::vector<float> &ov,glm::vec3 pos,
		float scl,glm::vec3 rot)
{
	// setup vertex information lists
	std::vector<uint32_t> ovi,oui,oni;	// raw reads from source: vertex,uv,normals
	std::vector<glm::vec3> verts,norm;	// position vertices & normals
	std::vector<glm::vec2> uv;			// texture coordinates

	// read source file
	FILE* file = fopen(path,"r");
	while(true) {

		// read next line if exists
		char lh[128];
		int res = fscanf(file,"%s",lh);
		if (res==EOF) break;
		else {

			// check value prefix
			if (strcmp(lh,"v")==0) {			// vertex prefix
				glm::vec3 p;
				fscanf(file,"%f %f %f\n",&p.x,&p.y,&p.z);
				verts.push_back(p);
			} else if (strcmp(lh,"vt")==0) {	// texture coordinate prefix
				glm::vec2 p;
				fscanf(file,"%f %f\n",&p.x,&p.y);
				uv.push_back(p);
			} else if (strcmp(lh,"vn")==0) {	// normals prefix
				glm::vec3 p;
				fscanf(file,"%f %f %f\n",&p.x,&p.y,&p.z);
				norm.push_back(p);
			} else if(strcmp(lh,"f")==0) {		// element index prefix

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
				ovi.push_back(vi[0]);ovi.push_back(vi[1]);ovi.push_back(vi[2]);		// position
				oui.push_back(ui[0]);oui.push_back(ui[1]);oui.push_back(ui[2]);		// tex coords
				oni.push_back(ni[0]);oni.push_back(ni[1]);oni.push_back(ni[2]);		// normals
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
		ov.push_back(tv.x),ov.push_back(tv.y),ov.push_back(tv.z),	// vertex positions
		ov.push_back(tu.x),ov.push_back(tu.y),						// texture coordinates
		ov.push_back(tn.x),ov.push_back(tn.y),ov.push_back(tn.z),	// normals
		ov.push_back(tg.x),ov.push_back(tg.y),ov.push_back(tg.z);	// tangents
		out++;
	} return out;
}

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
	ov *= scl;
	ov += pos;
}

/*
	rotate_vector(vec3&,vec3) -> void (static)
	purpose: 3D vector rotation
*/
void Toolbox::rotate_vector(glm::vec3 &ov,glm::vec3 rot)
{
	ov = glm::rotate(ov,glm::radians(rot.x),glm::vec3(1,0,0));
	ov = glm::rotate(ov,glm::radians(rot.y),glm::vec3(0,1,0));
	ov = glm::rotate(ov,glm::radians(rot.z),glm::vec3(0,0,1));
}

/*
	start_debug_logging(DebugLogData&,const char*) -> void
	purpose: start time difference debug recording
	\param dld: variable holding runtime debug data, such as recording keys & their string id
	\param tname: title of debug log, to describe output information to recognize their allegiance
*/
void Toolbox::start_debug_logging(DebugLogData &dld,const char* tname)
{
	dld.task_name = tname;
	dld.last_ticks = SDL_GetTicks();
	dld.key_list = {  };
	printf("\033[1;35mprocessing -> %s\033[0m",tname);
}

/*
	add_timekey(DebugLogData&,const char*) -> void
	purpose: create a timekey, recording the time difference since the last key or the data creation
	\param dld: variable holding runtime debug data, such as recording keys & their string id
	\param kname: string id of created key, to describe what was achieved within the recorded time
*/
void Toolbox::add_timekey(DebugLogData &dld,const char* kname)
{
	DebugLogKey nkey;
	nkey.key_name = kname;
	nkey.delta_ticks = SDL_GetTicks()-dld.last_ticks;
	dld.key_list.push_back(nkey);
	dld.last_ticks = SDL_GetTicks();
	printf("...");
}

/*
	flush_debug_logging(DebugLogData) -> void
	purpose: writes structured runtime debug log data to the console
	\param dld: variable holding runtime debug data, such as recording keys & their string id
*/
void Toolbox::flush_debug_logging(DebugLogData dld)
{
	printf("\n----------------------------------------\n");
	printf("| \033[1;34m%s\033[0m\n",dld.task_name);
	printf("----------------------------------------\n");
	uint32_t total_time = 0;
	for(auto key : dld.key_list) {
		printf("| \033[1;32m%s\033[0m: \033[1;33m%i\033[0m\n",key.key_name,key.delta_ticks);
		total_time += key.delta_ticks;
	} printf("----------------------------------------\n");
	printf("| total time: \033[1;35m%i\033[0m\n",total_time);
	printf("----------------------------------------\n");
}

/*
	PARAMETER DEFINITIONS:
	ov: vector to be transformed
	rot: rotation of vector, without directional reset
*/

/*
	create_sprite_canvas() -> std::vector<float> (static)
	returns: primitive canvas vertices without camera ready coordinate system and element draw
*/
std::vector<float> Toolbox::create_sprite_canvas()
{
	std::vector<float> out = {
		-1.0f,1.0f,0.0f,1.0f,-1.0f,-1.0f,0.0f,0.0f,1.0f,-1.0f,1.0f,0.0f,	// first triangle
		-1.0f,1.0f,0.0f,1.0f,1.0f,-1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,1.0f		// second triangle
	};
	return out;
}

/*
	PARAMETER DEFINITIONS:
	pos: origin position of canvas
	width: vertex distanced width of canvas
	height: vertex distanced height of canvas
*/

/*
	create_sprite_canvas(vec2,float,float) -> std::vector<float> (static)
	returns: created canvas vertices to later base 2D object generation on
*/
std::vector<float> Toolbox::create_sprite_canvas(glm::vec2 pos,float width,float height)
{
	std::vector<float> out = {
		pos.x,pos.y+height,0.0f,0.0f,
		pos.x+width,pos.y+height,1.0f,0.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x,pos.y,0.0f,1.0f
	};
	return out;
}
// FIXME: not intuitive if vertices returned are expecting element buffer

/*
	create_sprite_canvas_triangled(vec2,float,float) -> std::vector<float> (static)
	returns: the same canvas as create_sprite_canvas, but without relying on element buffer
*/
std::vector<float> Toolbox::create_sprite_canvas_triangled(glm::vec2 pos,float width,float height)
{
	std::vector<float> out = {
		pos.x,pos.y+height,0.0f,0.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x+width,pos.y+height,1.0f,0.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x,pos.y+height,0.0f,0.0f,
		pos.x,pos.y,0.0f,1.0f,
	};
	return out;
}

/*
	PARAMETER DEFINITIONS:
	tex: reference to be associated with texture value when bound
	path: path to file holding texture value
	corrected: defines colour format as corrected sRGB (true) or uncorrected RGBA (false)
*/

/*
	load_texture(uint32_t,const char*,bool) -> void (static)
	purpose: load texture value, generate mipmap and associate it with given texture reference
*/
void Toolbox::load_texture(uint32_t tex,const char* path,bool corrected)
{
	// bind and load texture data
	load_texture_function_head(tex,path,corrected);

	// texture parameters & mipmap
	set_texture_parameter_clamp_to_edge();
	set_texture_parameter_linear_mipmap();
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
	load_texture(uint32_t,const char*,float,bool) -> void (static)
	overloads previous load_texture()
	bias: mipmapping level-of-detail bias
	purpose: load mipmapped texture with modified lod bias
*/
void Toolbox::load_texture(uint32_t tex,const char* path,float bias,bool corrected)
{
	// bind and load texture data
	load_texture_function_head(tex,path,corrected);

	// texture parameters & mipmap
	set_texture_parameter_clamp_to_edge();
	set_texture_parameter_linear_mipmap();
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_LOD_BIAS,bias);
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
	load_texture_unfiltered(uint32_t,const char*,bool) -> void (static)
	purpose: load texture without mipmapping
*/
void Toolbox::load_texture_unfiltered(uint32_t tex,const char* path,bool corrected)
{
	// bind and load texture data
	load_texture_function_head(tex,path,corrected);

	// texture paramteres without mipmap
	set_texture_parameter_clamp_to_edge();
	set_texture_parameter_linear_unfiltered();
}

/*
	load_texture_repeat(uint32_t,const char*,bool) -> void (static)
	purpose: load repeating mipmapped texture
*/
void Toolbox::load_texture_repeat(uint32_t tex,const char* path,bool corrected)
{
	// bind and load texture data
	load_texture_function_head(tex,path,corrected);

	// texture parameters repeating
	set_texture_parameter_texture_repeat();
	set_texture_parameter_linear_mipmap();
	glGenerateMipmap(GL_TEXTURE_2D);
}
// TODO: break edge cases apart and let the statii be determined separately with additional methods

/*
	generate_elements(uint16_t,std::vector<uint16_t>&) -> void (static)
	i: objects index to use to rasterize element value generation
	ls: element list input to add generated elements to
	purpose: generate buffer elements based on object list index
*/
void Toolbox::generate_elements(uint16_t i,std::vector<unsigned int> &ls)
{
	ls.push_back(i*4);ls.push_back(i*4+2);ls.push_back(i*4+1);	// map first triangle
	ls.push_back(i*4+2);ls.push_back(i*4);ls.push_back(i*4+3);	// map second triangle
}

/*
	set_texture_parameter_linear_mipmap() -> void (static)
	purpose: define texture as mipmappable
*/
void Toolbox::set_texture_parameter_linear_mipmap()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	set_texture_parameter_linear_unfiltered() -> void
	purpose: define texture as simply linearly filtered
*/
void Toolbox::set_texture_parameter_linear_unfiltered()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	set_texture_parameter_nearest_unfiltered() -> void (static)
	purpose: define texture as unfiltered
*/
void Toolbox::set_texture_parameter_nearest_unfiltered()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
}

/*
	set_cubemap_texture_parameters() -> void
	purpose: define filtered cubemap texture
*/
void Toolbox::set_cubemap_texture_parameters()
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	set_cubemap_texture_parameters_mipmap() -> void
	purpose: define filtered cubemap with multiple levels of detail
*/
void Toolbox::set_cubemap_texture_parameters_mipmap()
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

/*
	set_texture_parameter_clamp_to_edge() -> void (static)
	purpose: define texture as to be stretched out towards the edge
*/
void Toolbox::set_texture_parameter_clamp_to_edge()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

/*
	set_texture_parameter_clamp_to_border() -> void (static)
	purpose: define texture as to be scaled up towards custom borders, to avoid ratio manipulation
*/
void Toolbox::set_texture_parameter_clamp_to_border()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
}

/*
	set_texture_parameter_texture_repeat() -> void (private,static)
	purpose: define texture as repeatable
*/
void Toolbox::set_texture_parameter_texture_repeat()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

/*
	load_texture_function_head(GLuint tex,const char* path,bool) -> void (private,static)
	purpose: load texture value from given file
*/
void Toolbox::load_texture_function_head(uint32_t tex,const char* path,bool corrected)
{
	// setup
	int width,height;
	glBindTexture(GL_TEXTURE_2D,tex);
	int32_t format = corrected ? GL_SRGB : GL_RGBA;

	// load texture data from source
	unsigned char* image = stbi_load(path,&width,&height,0,STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
	stbi_image_free(image);
}