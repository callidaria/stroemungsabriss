#version 330 core

in vec4 Position;
in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform float ambient = 1.0;
uniform float lint = 1.0;
uniform float linr = 0.0014;
uniform float quad = 0.000007;
uniform int pla = 0;
uniform int sla = 0;

struct light_point {
	vec2 pos;
};
struct light_spot {
	vec2 pos;
	vec2 dir;
	float icut;
	float ocut;
};
uniform light_point pl[5];
uniform light_spot sl[5];

vec4 lumen_point(vec4 o,light_point l);
vec4 lumen_spot(vec4 o,light_spot l);

void main()
{
	vec4 mix = texture(tex, TexCoords);
	vec4 amb = vec4(mix.rgb*ambient,mix.a);

	vec4 plo = vec4(0.0,0.0,0.0,0.0);
	vec4 slo = vec4(0.0,0.0,0.0,0.0);
	for (int i=0;i<pla;i++) plo+=lumen_point(mix,pl[i]);
	for (int j=0;j<sla;j++) slo+=lumen_spot(mix,sl[j]);
	outColour = amb+plo+slo;
}
vec4 lumen_point(vec4 o,light_point l)
{
	float d = length(l.pos-Position.xy);
	float lumen = lint/(1.0+linr*d+quad*(d*d));
	return vec4(o.rgb*lumen,o.a);
}
vec4 lumen_spot(vec4 o,light_spot l)
{
	vec2 pos_dir = normalize(l.pos-Position.xy);
	float cos_icut = cos(radians(l.icut));
	float cos_ocut = cos(radians(l.ocut));
	float t = dot(pos_dir,-normalize(l.dir));
	float e = cos_icut-cos_ocut;
	float i = clamp((t-cos_ocut)/e,0.0,1.0);
	float d = length(l.pos-Position.xy);
	float lumen = (lint/(1.0+linr*d+quad*(d*d)))*i;
	return vec4(o.rgb*lumen,o.a);
}
