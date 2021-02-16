#include "../gfx/cubemap.h"

Cubemap::Cubemap(std::vector<const char*> tp) // !!description && maybe stack ?
{
	// setup
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
	glGenTextures(1,&tex);
	glBindVertexArray(vao);

	// buffer data
	float verts[] = {
		-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,
		-1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(verts),&verts,GL_STATIC_DRAW);

	// shader compile
	s.compile("shader/vertex_skybox.shader","shader/fragment_skybox.shader");
	s.enable_location("position");
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

	// cubemap textures
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	int width,height;
	for (int i = 0; i < tp.size(); i++) {
		unsigned char* image = SOIL_load_image(tp.at(i),&width,&height,0,SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);
		SOIL_free_image_data(image);
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
	glBindVertexArray(vao);
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
