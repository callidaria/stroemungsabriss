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

	// origin translation
	vec2 Position = (model*vec4(position,0,1)).xy;

	// instance translation
	Position = mat2(rotation_cos,-rotation_sin,rotation_sin,rotation_cos)*Position;
	Position += offset;

	// upload final position
	gl_Position = proj*view*vec4(Position,0,1);
}
