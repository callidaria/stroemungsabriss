#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;

void main()
{
	vec4 proc = texture(tex,TexCoords);
	float alpha = (proc.r*0.2126)+(proc.g*0.7152)+(proc.b*0.0722);
	outColour = proc*alpha;
	//outColour=vec4(vec3(alpha),1.0);
}
