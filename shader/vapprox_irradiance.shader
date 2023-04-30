#version 330 core

in vec3 position;

out vec3 Position;
//ut vec3 Normals;
//out vec3 Right;
//out vec3 Up;

uniform mat4 proj = mat4(1.0);
uniform mat4 view = mat4(1.0);

void main()
{
	// fragment precalculations or pass through
	Position = position;
	//Normals = normalize(position);
	//Right = normalize(cross(vec3(0,1,0),Normals));
	//Up = normalize(cross(Normals,Right));

	// position output
	gl_Position = (proj*view*vec4(position,1.0)).xyww;
}