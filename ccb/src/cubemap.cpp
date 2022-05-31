#include "../gfx/cubemap.h"

Cubemap::Cubemap(std::vector<const char*> tp) // !!description && maybe stack ?
{
	// setup
	buffer = Buffer();
	buffer.bind();  // FIXME: kill this call??
	glGenTextures(1,&tex);

	// buffer data
	float verts[] = {
		-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,
		-1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f
	}; buffer.upload_vertices(verts,sizeof(verts));

	// shader compile
	s.compile("shader/vertex_skybox.shader","shader/fragment_skybox.shader");
	s.def_attributeF("position",3,0,3);

	// cubemap textures
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	int width,height;
	for (int i = 0; i < tp.size(); i++) {
#ifdef __WIN32__
		unsigned char* image = stbi_load(tp.at(i), &width, &height, 0, STBI_rgb); // ??alpha needed
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
#else
		unsigned char* image = SOIL_load_image(tp.at(i), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
#endif
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}
void Cubemap::prepare()
{
	s.enable();
	buffer.bind();
}
void Cubemap::prepare_wcam(Camera3D* c)
{
	prepare();
	c->update();
	s.upload_matrix("view",glm::mat4(glm::mat3(c->view3D)));
	s.upload_matrix("proj",c->proj3D); // !!render with inverted face culling
}
void Cubemap::set_cubemap() // ??smart BUT placement
{
	// ??enabling the shader first maybe
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
}
void Cubemap::render()
{
	set_cubemap();
	glDrawArrays(GL_TRIANGLES,0,36);
}
