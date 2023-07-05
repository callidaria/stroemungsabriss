#version 330 core

in vec2 TexCoords;
in float dproj;

out vec4 outColour;

uniform sampler2D tex;

uniform mat4 proj;

void main()
{
	vec4 inv_depth = inverse(proj)*vec4(0,0,gl_FragCoord.z*2.0-1.0,1.0);
	float gdepth = (inv_depth/inv_depth.w).z;
	outColour = vec4(gl_FragCoord.z,0,0,1); //texture(tex,TexCoords);
	outColour = vec4(gl_FragCoord.z,dproj/10.0,0,1);
}