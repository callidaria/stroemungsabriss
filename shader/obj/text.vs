#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 TexCoords;

// text attributes
uniform vec2 offset;
uniform float scale;
uniform vec2 size;
uniform vec2 bearing;

// camera
uniform mat4 view;
uniform mat4 proj;

void main()
{
    // setup information for scaling
    vec2 Position = position+.5;
    vec2 Size = size*scale;
    vec2 Bearing = scale*bearing;

    // align character
    Position = Size*Position;
    Position += offset+Bearing;
    Position.y -= Size.y;

    // output
	gl_Position = proj*view*vec4(Position,.0,1.);
	TexCoords = texCoords;
}
