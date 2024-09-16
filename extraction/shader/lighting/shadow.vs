#version 330 core

in vec3 position;

//uniform mat4 shadow_projection;
uniform mat4 proj = mat4(1.);
uniform mat4 view = mat4(1.);
uniform mat4 model = mat4(1.);

void main()
{
	//gl_Position=shadow_mat*model*vec4(position,1.0);
	gl_Position=proj*view*model*vec4(position,1.0);
}
