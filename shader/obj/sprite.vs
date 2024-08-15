#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 TexCoords;

// camera transformation
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// spritesheet rasterization
uniform int row = 1;			// amount of rows on spritesheet
uniform int col = 1;			// amount of columns on spritesheet
uniform vec2 i_tex = vec2(0,0);		// 2D identification of subtexture on spritesheet raster
uniform float vFlip = 0.0;		// identifies if texture is flipped

void main()
{
	TexCoords = vec2(texCoords.x/col+i_tex.x/col,abs(vFlip-(texCoords.y/row+i_tex.y/row)));
	gl_Position = proj*view*model*vec4(position.xy,0.0,1.0);
}