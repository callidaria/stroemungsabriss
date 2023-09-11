#version 330 core

in vec3 position;
in float edge_id;

out vec3 Colour;

// geometry
uniform float luext[2] = { 0,0 };
uniform int is_hrz = 0;

// camera
uniform mat4 proj = mat4(1);
uniform mat4 view = mat4(1);

void main()
{
	// inversion & extension indexing based on edge id
	int is_upper = int(edge_id>1);
	int efac = (int(edge_id)-2*is_upper)*2-1;

	// calculate width modification
	vec2 vmod = vec2(0);
	vmod[is_hrz] = luext[is_upper]*efac;

	// final vertex position
	gl_Position = proj*view*vec4(position.xy+vmod,.0,1.);
}
