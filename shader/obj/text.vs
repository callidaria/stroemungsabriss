#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 TexCoords;

// text attributes
uniform vec2 offset;
uniform float size;
uniform vec2 scale;
uniform vec2 bearing;

// camera
uniform mat4 view;
uniform mat4 proj;

void main()
{
    vec2 Position = position+.5;
    Position = scale*Position+offset;
    Position += bearing-vec2(0,scale.y);
	gl_Position = proj*view*vec4(Position,.0,1.);
	TexCoords = texCoords;
}
