#version 330 core

in vec4 Position;
in vec2 TexCoords;
in vec3 Normals;
in vec4 light_transpos;
in vec3 TanLightPos;
in vec3 TanViewPos;
in vec3 TanPosition;

out vec4 outColour;

uniform sampler2D tex;
uniform sampler2D sm;
uniform sampler2D emit;
uniform sampler2D shadow_map;
uniform sampler2D nmap;

uniform samplerCube cm;

uniform float ambient = 0.0;
uniform vec3 view_pos;

uniform int specular = 2;
uniform float spec_int = 1;
uniform mat4 model;

uniform int amnt_light_sun = 0;
uniform int amnt_light_point = 0;
uniform int amnt_light_spot = 0;
struct light_sun {
	vec3 pos;
	vec3 col;
	float ins;
};
struct light_point {
	vec3 pos;
	vec3 col;
	float lconst;
	float lin;
	float quad;
	float ins;
};
struct light_spot {
	vec3 pos;
	vec3 col;
	vec3 dir;
	float cut_in;
	float cut_out;
};
uniform light_sun al[5];
uniform light_point pl[5];
uniform light_spot sl[5];

vec4 lumen_sun(vec4 o,light_sun l);
vec4 lumen_point(vec4 o,light_point l);
vec4 lumen_spot(vec4 o,light_spot l);
float calc_shadow(vec4 ltp);

void main()
{
	vec4 mix=texture(tex,TexCoords);
	vec4 amb=vec4(mix.rgb*ambient,mix.a);

	vec4 alo=vec4(0.0,0.0,0.0,0.0);
	for (int i=0;i<amnt_light_sun;i++) alo+=lumen_sun(mix,al[i]);
	vec4 plo=vec4(0.0,0.0,0.0,0.0);
	for (int j=0;j<amnt_light_point;j++) plo+=lumen_point(mix,pl[j]);
	vec4 slo=vec4(0.0,0.0,0.0,0.0);
	for (int k=0;k<amnt_light_spot;k++) slo+=lumen_spot(mix,sl[k]);

	vec3 emitmap=vec3(texture(emit,TexCoords));
	float shadow=calc_shadow(light_transpos);
	vec4 all=amb+(alo+plo+slo)*(1.0-shadow);
	outColour=vec4(max(all.rgb,emitmap),all.a);

	//vec3 I=normalize(Position.xyz-view_pos);
	//vec3 R=reflect(I,Normals);
	//outColour=vec4(texture(cm,R).rgb,1.0);
}
vec4 lumen_sun(vec4 o,light_sun l)
{
	vec3 norm=texture(nmap,TexCoords).rgb;
	norm=normalize(norm*2.0-1.0);
	vec3 light_dir=normalize(TanLightPos-TanPosition);

	vec3 view_dir=normalize(TanViewPos-TanPosition);
	float fresnel=max(1-dot(view_dir,norm),0.1);

	float diff=max(dot(light_dir,norm),0.0);
	vec3 cdiff=diff*l.col;

	vec3 spec_dir=reflect(-light_dir,norm);
	float spec=pow(max(dot(view_dir,spec_dir),0.0),32)*1.0*pow(fresnel,0.25);
	vec3 cspec=spec*l.col;

	vec3 specmap=vec3(texture(sm,TexCoords));
	vec3 emitmap=vec3(texture(emit,TexCoords));
	vec3 out_diff=o.rgb*cdiff;
	vec3 out_spec=(cspec+diff*0.4);
	return vec4(mix(out_diff,vec3(diff*0.4),specmap*pow(fresnel,4))+cspec*specmap,o.a);
}
vec4 lumen_point(vec4 o,light_point l)
{
	vec3 norm=normalize(Normals.xyz);
	vec3 diff_dir=normalize(l.pos-Position.xyz);
	float diff=max(dot(norm,diff_dir),0.0);
	vec3 cdiff=diff*l.col;
	vec3 view_dir=normalize(view_pos-Position.xyz);
	vec3 spec_dir=reflect(-diff_dir,norm);
	float spec=pow(max(dot(view_dir,spec_dir),0.0),specular)*spec_int;
	vec3 cspec=spec*l.col;

	float dist=length(l.pos-Position.xyz);
	float atten=1.0/(l.lconst+l.lin*dist+l.quad*(dist*dist));
	vec3 specmap=vec3(texture(sm,TexCoords));
	vec3 out_spec=o.rgb*cspec*specmap*atten;
	vec3 out_diff=o.rgb*cdiff*atten;
	return vec4(out_spec+out_diff,o.a);
}
vec4 lumen_spot(vec4 o,light_spot l)
{
	vec3 norm=normalize(Normals.xyz);
	vec3 diff_dir=normalize(l.pos-Position.xyz);
	float diff=max(dot(norm,diff_dir),0.0);
	vec3 cdiff=diff*l.col;
	vec3 view_dir=normalize(view_pos-Position.xyz);
	vec3 spec_dir=reflect(-diff_dir,norm);
	float spec=pow(max(dot(view_dir,spec_dir),0.0),specular)*spec_int;
	vec3 cspec=spec*l.col;

	float theta=dot(diff_dir,normalize(-l.dir));
	float epsilon=l.cut_in-l.cut_out;
	float ins=clamp((theta-l.cut_out)/epsilon,0.0,1.0);
	vec3 specmap=vec3(texture(sm,TexCoords));
	vec3 out_spec=o.rgb*cspec*specmap*ins;
	vec3 out_diff=o.rgb*cdiff*ins;
	return vec4(out_spec+out_diff,o.a);
}
float calc_shadow(vec4 ltp)
{
	vec3 pc=ltp.xyz/ltp.w;
	pc=pc*0.5+0.5;
	float curr_depth=pc.z;
	float out_shadow=0.0;
	vec2 texel=1.0/textureSize(shadow_map,0);
	for (int x=-1;x<=1;++x) {
		for (int y=-1;y<=1;++y) {
			float pcf_depth=texture(shadow_map,pc.xy+vec2(x,y)*texel)
				.r;
			out_shadow+=curr_depth-0.00001>pcf_depth?1.0:0.0;
		}
	} out_shadow/=9.0;
	return out_shadow;
}
