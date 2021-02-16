#pragma once

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <SOIL/SOIL.h>

class Mesh
{
public:
	Mesh(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 ip,float is,glm::vec3 ir,unsigned int* mofs)
		: texpath(t),smpath(sm),nmpath(nm),empath(em),pos(ip),scl(is),rot(ir),ofs(*mofs)
	{
		glGenTextures(1,&tex);
		glGenTextures(1,&specmap);glGenTextures(1,&normap);glGenTextures(1,&emitmap);
		std::vector<unsigned int> ovi,oui,oni;
		std::vector<glm::vec3> verts,norm;std::vector<glm::vec2> uv;
		FILE* file = fopen(m,"r");
		if(file==NULL) { std::cout<<"couldn't find obj "<<m<<"\n"; }
		while(true) {
			char lh[128];int res = fscanf(file,"%s",lh);
			if(res==EOF) break;
			else {
				if(strcmp(lh,"v")==0) {
					glm::vec3 p;
					fscanf(file,"%f %f %f\n",&p.x,&p.y,&p.z);
					verts.push_back(p);
				} else if(strcmp(lh,"vt")==0) {
					glm::vec2 p;
					fscanf(file,"%f %f\n",&p.x,&p.y);
					uv.push_back(p);
				} else if(strcmp(lh,"vn")==0) {
					glm::vec3 p;
					fscanf(file,"%f %f %f\n",&p.x,&p.y,&p.z);
					norm.push_back(p);
				} else if(strcmp(lh,"f")==0) {
					unsigned int vi[3],ui[3],ni[3];
					fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vi[0],&ui[0],&ni[0],&vi[1],&ui[1],&ni[1],&vi[2],&ui[2],&ni[2]);
					glm::vec3 pproc=glm::vec3(vi[0],vi[1],vi[2]);
					glm::vec3 nproc=glm::vec3(ni[0],ni[1],ni[2]);
					pproc=transform(pproc,pos,scl,rot);
					nproc=rotate(nproc,rot);
					ovi.push_back(vi[0]);ovi.push_back(vi[1]);ovi.push_back(vi[2]);
					oui.push_back(ui[0]);oui.push_back(ui[1]);oui.push_back(ui[2]);
					oni.push_back(ni[0]);oni.push_back(ni[1]);oni.push_back(ni[2]);
				}
			}
		} glm::vec3 tg(1.0f);glm::vec3 btg(1.0f);
		for(int i=0;i<ovi.size();i++) {
			if (i%3==0&&i<ovi.size()) {
				glm::vec3 e1=verts[ovi[i+1]-1]-verts[ovi[i]-1];
				glm::vec3 e2=verts[ovi[i+2]-1]-verts[ovi[i]-1];
				glm::vec2 duv1=uv[oui[i+1]-1]-uv[oui[i]-1];
				glm::vec2 duv2=uv[oui[i+2]-1]-uv[oui[i]-1];
				float ff=1.0f/(duv1.x*duv2.y-duv2.x*duv1.y);
				tg.x=ff*(duv2.y*e1.x-duv1.y*e2.x);
				tg.y=ff*(duv2.y*e1.y-duv1.y*e2.y);
				tg.z=ff*(duv2.y*e1.z-duv1.y*e2.z);
				tg=glm::normalize(tg);
				btg.x=ff*(-duv2.x*e1.x+duv1.x*e2.x);
				btg.y=ff*(-duv2.x*e1.y+duv1.x*e2.y);
				btg.z=ff*(-duv2.x*e1.z+duv1.x*e2.z);
				btg=glm::normalize(btg);
			}
			unsigned int tvi=ovi[i];unsigned int tui=oui[i];unsigned int tni=oni[i];
			glm::vec3 tv=verts[tvi-1];glm::vec2 tu=uv[tui-1];glm::vec3 tn=norm[tni-1];
			tv=transform(tv,pos,scl,rot);tn=rotate(tn,rot);
			v.push_back(tv.x);v.push_back(tv.y);v.push_back(tv.z);
			v.push_back(tu.x);v.push_back(tu.y);
			v.push_back(tn.x);v.push_back(tn.y);v.push_back(tn.z);
			v.push_back(tg.x);v.push_back(tg.y);v.push_back(tg.z);
			v.push_back(btg.x);v.push_back(btg.y);v.push_back(btg.z);
		}
		size=v.size()/14;*mofs+=size;
	}
	void texture()
	{
		glBindTexture(GL_TEXTURE_2D,tex);
		int width,height;
		unsigned char* image=SOIL_load_image(texpath,&width,&height,0,
				SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
				GL_UNSIGNED_BYTE,image);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D,specmap);
		image=SOIL_load_image(smpath,&width,&height,0,SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
				GL_UNSIGNED_BYTE,image);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D,normap);
		image=SOIL_load_image(nmpath,&width,&height,0,SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
				GL_UNSIGNED_BYTE,image);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D,emitmap);
		image=SOIL_load_image(empath,&width,&height,0,SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
				GL_UNSIGNED_BYTE,image);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
private:
	glm::vec3 transform(glm::vec3 o,glm::vec3 p,float s,glm::vec3 r)
	{ o=rotate(o,r);o*=s;o+=p;return o; }
	glm::vec3 rotate(glm::vec3 o,glm::vec3 r)
	{
		o=glm::rotate(o,glm::radians(r.x),glm::vec3(1,0,0));
		o=glm::rotate(o,glm::radians(r.y),glm::vec3(0,1,0));
		o=glm::rotate(o,glm::radians(r.z),glm::vec3(0,0,1));
		return o;
	}
private:
	const char* texpath;
	const char* smpath;
	const char* nmpath;
	const char* empath;
public:
	unsigned int ofs,size;
	std::vector<float> v;
	unsigned int tex,specmap,normap,emitmap;
	glm::vec3 pos,rot;
	float scl;
};
