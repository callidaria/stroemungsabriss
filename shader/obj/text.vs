#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 TexCoords;

// text attributes
uniform vec2 offset;
uniform vec2 scale;

// camera
uniform mat4 view;
uniform mat4 proj;

void main()
{
    vec2 Position = scale*position+offset;
	gl_Position = proj*view*vec4(Position,.0,1.);
	TexCoords = texCoords;
}
