#version 330 core

in vec2 position;
in vec2 texCoords;

in vec2 offset;
in float rotation_sin;
in float rotation_cos;
in vec2 i_tex;

out vec2 TexCoords;

// camera transformation
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// spritesheet rasterization
uniform int row = 1;				// amount of rows on spritesheet
uniform int col = 1;				// amount of columns on spritesheet

void main()
{
	// calculate subtexture position on atlas
	TexCoords = vec2(texCoords.x/col+i_tex.x/col,texCoords.y/row+i_tex.y/row);

	// instance rotation
	mat2 rmat = mat2(rotation_cos,-rotation_sin,rotation_sin,rotation_cos);
	vec2 rpos = rmat*position;

	// upload final position
	gl_Position = proj*view*model*vec4(rpos+offset,0,1);
}