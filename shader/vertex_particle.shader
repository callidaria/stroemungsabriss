#version 330 core

in vec3 position;
in vec2 texCoords;

in vec3 particle_position;
in vec2 anim_index;

out vec2 TexCoords;
out float dproj;

// texture information
uniform int anim_rows = 1;
uniform int anim_cols = 1;

// geometry
uniform float scale = 1.0;

// camera perspective
uniform mat4 view;
uniform mat4 proj;

void main()
{
	// animation spritesheet picker
	TexCoords = vec2(texCoords.x/anim_cols+anim_index.x/anim_cols,
			texCoords.y/anim_rows+anim_index.y/anim_rows);

	// orthogonal particle faces in relation to camera
	vec3 cpos = vec3(view[0][0],view[1][0],view[2][0])*scale*position.x
			+ vec3(view[0][1],view[1][1],view[2][1])*scale*position.y;
	gl_Position = proj*view*vec4(cpos+particle_position,1.0);
	dproj = gl_Position.w;
}