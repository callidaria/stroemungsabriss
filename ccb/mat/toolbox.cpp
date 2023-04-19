#include "toolbox.h"

/*
	TODO
*/
void Toolbox::load_object(const char* path,std::vector<float> &ov,glm::vec3 pos,
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
	glm::vec3 tg(1.0f),btg(1.0f);
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

			// calculate bitangent
			btg.x = ff*(-duv2.x*e1.x+duv1.x*e2.x);
			btg.y = ff*(-duv2.x*e1.y+duv1.x*e2.y);
			btg.z = ff*(-duv2.x*e1.z+duv1.x*e2.z);
			btg = glm::normalize(btg);
			// FIXME: using a matrix calculation might be significantly faster
		}  // FIXME: remove branch from multiupload

		// get read vertices to process and save
		unsigned int tvi = ovi[i],tui = oui[i],tni = oni[i];
		glm::vec3 tv = verts[tvi-1],tn = norm[tni-1];
		glm::vec2 tu = uv[tui-1];

		// translate vertices & normals
		transform_vector(tv,pos,scl,rot);
		rotate_vector(tn,rot);

		// save data to buffer vector
		ov.push_back(tv.x);ov.push_back(tv.y);ov.push_back(tv.z);		// vertex positions
		ov.push_back(tu.x);ov.push_back(tu.y);							// texture coordinates
		ov.push_back(tn.x);ov.push_back(tn.y);ov.push_back(tn.z);		// normals
		ov.push_back(tg.x);ov.push_back(tg.y);ov.push_back(tg.z);		// tangents
		ov.push_back(btg.x);ov.push_back(btg.y);ov.push_back(btg.z);	// bitangents
	}
}

/*
	calculate_vecangle(vec2,vec2) -> float (static)
	a: first vector, 0 degrees towards the origin
	b: second vector, returned degrees from first vector
	returns: angle between vectors in radians
*/
float Toolbox::calculate_vecangle(glm::vec2 a,glm::vec2 b)
{
	return glm::acos(glm::dot(a,b)/(glm::length(a)*glm::length(b)));
}

/*
	TODO
*/
void Toolbox::transform_vector(glm::vec3 &ov,glm::vec3 pos,float scl,glm::vec3 rot)
{
	rotate_vector(ov,rot);
	ov *= scl;
	ov += pos;
}

/*
	TODO
*/
void Toolbox::rotate_vector(glm::vec3 &ov,glm::vec3 rot)
{
	ov = glm::rotate(ov,glm::radians(rot.x),glm::vec3(1,0,0));
	ov = glm::rotate(ov,glm::radians(rot.y),glm::vec3(0,1,0));
	ov = glm::rotate(ov,glm::radians(rot.z),glm::vec3(0,0,1));
}

/*
	create_sprite_canvas() -> std::vector<float>
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
		pos.x+width,pos.y+height,1.0f,0.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x,pos.y,0.0f,1.0f,
		pos.x,pos.y+height,0.0f,0.0f,
	};
	return out;
}

/*
	PARAMETER DEFINITIONS:
	tex: reference to be associated with texture value when bound
	path: path to file holding texture value
*/

/*
	load_texture(GLuint,const char*) -> void (static)
	purpose: load texture value, generate mipmap and associate it with given texture reference
*/
void Toolbox::load_texture(GLuint tex,const char* path)
{
	// bind and load texture data
	load_texture_function_head(tex,path);

	// texture parameters & mipmap
	set_texture_parameter_clamp_to_edge();
	set_texture_parameter_linear_mipmap();
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
	load_texture(GLuint,const char*,float) -> void (static)
	overloads previous load_texture()
	bias: mipmapping level-of-detail bias
	purpose: load mipmapped texture with modified lod bias
*/
void Toolbox::load_texture(GLuint tex,const char* path,float bias)
{
	// bind and load texture data
	load_texture_function_head(tex,path);

	// texture parameters & mipmap
	set_texture_parameter_clamp_to_edge();
	set_texture_parameter_linear_mipmap();
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_LOD_BIAS,bias);
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
	load_texture_unfiltered(GLuint,const char*) -> void (static)
	purpose: load texture without mipmapping
*/
void Toolbox::load_texture_unfiltered(GLuint tex,const char* path)
{
	// bind and load texture data
	load_texture_function_head(tex,path);

	// texture paramteres without mipmap
	set_texture_parameter_clamp_to_edge();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	load_texture_repeat(GLuint,const char*) -> void (static)
	purpose: load repeating mipmapped texture
*/
void Toolbox::load_texture_repeat(GLuint tex,const char* path)
{
	// bind and load texture data
	load_texture_function_head(tex,path);

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
	ls.push_back(i*4);ls.push_back(i*4+1);ls.push_back(i*4+2);		// map first triangle
	ls.push_back(i*4+2);ls.push_back(i*4+3);ls.push_back(i*4+0);	// map second triangle
}

/*
	set_texture_parameter_linear_mipmap() -> void (private,static)
	purpose: define texture as mipmappable
*/
void Toolbox::set_texture_parameter_linear_mipmap()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	TODO
*/
void Toolbox::set_texture_parameter_nearest_unfiltered()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
}

/*
	set_texture_parameter_clamp_to_edge() -> void (private,static)
	purpose: define texture as to be stretched out towards the borders
*/
void Toolbox::set_texture_parameter_clamp_to_edge()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

/*
	TODO
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
	load_texture_function_head(GLuint tex,const char* path) -> void (private,static)
	purpose: load texture value from given file
*/
void Toolbox::load_texture_function_head(GLuint tex,const char* path)
{
	// setup
	int width,height;
	glBindTexture(GL_TEXTURE_2D,tex);

	// load texture data from source
#ifdef __WIN32__

	unsigned char* image = stbi_load(path,&width,&height,0,STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
	stbi_image_free(image);

#else

	unsigned char* image = SOIL_load_image(path,&width,&height,0,SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
	SOIL_free_image_data(image);

#endif
}