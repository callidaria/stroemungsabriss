#version 330 core

in vec2 position;
in vec2 texCoords;

in vec2 offset;
in vec2 scale;
in float rotation;
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
	// rotation
	float rd_rotation = radians(rotation);
	float rotation_sin = sin(rd_rotation), rotation_cos = cos(rd_rotation);
	Position = mat2(rotation_cos,-rotation_sin,rotation_sin,rotation_cos)*Position;

	// position & scaling
	Position *= scale;
	Position += offset;

	// upload final position
	gl_Position = proj*view*vec4(Position,0,1);
}
