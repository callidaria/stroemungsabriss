#pragma once

#include <glm/glm.hpp>
#include "../gfx/shader.h"
#include "../mat/camera3d.h"



/*
 *		!!!ATTENTION!!!
 *	THIS IS WORK IN PROGRESS AND GUARANTEED TO BE WRONG AND LITTERED WITH BUGS
 *	=> PLEASE WAIT WHILE THE EXTERMINATION TEAM ARRIVES
 * */
class Terrain
{
public:
	Terrain(Camera3D* c,glm::vec3 pos,float w,float h,const char* txp,const char* hmp,unsigned int tr)
	{
		ts = Shader();
		glGenTextures(1,&tex);

		SDL_Surface* img=SDL_LoadBMP(hmp);
		if (!img) printf("couldn't find heightmap\n");
		float heights[img->w*img->h];
		std::vector<float> verts;
		float vdw=w/img->w;
		float vdh=h/img->h;
		for (int y=0;y<img->h;y++) {
			for (int x=0;x<img->w;x++) {
				Uint32 px=((unsigned int*)img->pixels)[y*img->pitch/4+x];
				Uint8 r,g,b;
				SDL_GetRGB(px,img->format,&r,&g,&b);
				heights[y*img->w+x]=((float)r/255.0f);
			}
		}
		for (int y=0;y<img->h;y++) {
			for (int x=0;x<img->w;x++) {
				verts.push_back(pos.x+x*vdw);
				verts.push_back(heights[y*img->w+x]*100);
				verts.push_back(pos.z+y*vdh);
				verts.push_back(x/(float)img->w);
				verts.push_back(y/(float)img->h);
				glm::vec3 tn=glm::vec3(heights[y*img->w+x-1]-heights[y*img->w+x+1],2.0f,
						heights[(y-1)*img->w+x]-heights[(y+1)*img->w+x]);
				tn=glm::normalize(tn);
				verts.push_back(0.0f);
				verts.push_back(1.0f);
				verts.push_back(0.0f);
			}
		}
		for (int y=0;y<img->h-1;y++) {
			for (int x=0;x<img->w-1;x++) {
				e.push_back(y*img->w+x);
				e.push_back((y+1)*img->w+x);
				e.push_back((y+1)*img->w+x+1);
				e.push_back(y*img->w+x);
				e.push_back(y*img->w+x+1);
				e.push_back((y+1)*img->w+x+1);
			}
		}

		glGenVertexArrays(1,&vao);
		glGenBuffers(1,&vbo);
		glGenBuffers(1,&ebo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER,verts.size()*sizeof(float),verts.data(),GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,e.size()*sizeof(int),e.data(),GL_STATIC_DRAW);

		ts.compile3d("shader/vertex_terrain.shader","shader/fragment_terrain.shader");
		glBindTexture(GL_TEXTURE_2D,tex);
		int width,height;
		unsigned char* image=SOIL_load_image(txp,&width,&height,0,SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		ts.upload_int("tex",0);
		ts.upload_int("shadow_map",1);
		ts.upload_int("tex_repeat",tr);
	}
	void render(Camera3D* c,glm::mat4 shadow)
	{
		glDisable(GL_CULL_FACE);
		glActiveTexture(GL_TEXTURE0);
		ts.enable();
		glBindVertexArray(vao);
		c->update();
		ts.upload_matrix("view",c->view3D);
		ts.upload_matrix("proj",c->proj3D);
		ts.upload_matrix("light_trans",shadow);
		glBindTexture(GL_TEXTURE_2D,tex);
		glDrawElements(GL_TRIANGLES,e.size(),GL_UNSIGNED_INT,0);
	}
public:
	Shader ts;
private:
	unsigned int vao,vbo,ebo;
	unsigned int tex,hmap;
	std::vector<int> e;
};
