#version 330 core

in vec2 TexCoords;

out vec4 outColour;

// sunlight structure definition
struct light_sun {
	vec3 position;
	vec3 colour;
	float intensity;
};

// pointlight structure definition
struct light_point {
	vec3 position;
	vec3 colour;
	float constant;
	float linear;
	float quadratic;
	float intensity;
};

// spotlight structure definition
struct light_spot {
	vec3 position;
	vec3 colour;
	vec3 direction;
	float cut_in;
	float cut_out;
};

// g-buffer components
uniform sampler2D gbuffer_colour;
uniform sampler2D gbuffer_position;
uniform sampler2D gbuffer_normals;
uniform sampler2D gbuffer_materials;

// camera information
uniform vec3 view_pos;
uniform int spec_exponent = 16;
uniform float spec_intensity = 1.0;

// post-processing
uniform float lens_exposure = 1.0;
uniform float gamma = 2.2;

// lights
uniform int sunlight_count = 0;
uniform int pointlight_count = 0;
uniform int spotlight_count = 0;
uniform light_sun sunlight[5];
uniform light_point pointlight[128];
uniform light_spot spotlight[32];

// light maps
uniform samplerCube irradiance_map;
uniform samplerCube specular_map;
uniform sampler2D specular_brdf;

// shadows
uniform sampler2D shadow_map;
uniform vec3 light_position;
uniform mat4 shadow_matrix;

// light processing definitions
vec3 lumen_sun(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_sun sl);
vec3 lumen_sun_pbs(vec3 colour,vec3 position,vec3 normals,float metallic,float roughness,
		light_sun sl);
vec3 lumen_point(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_point pl);
vec3 lumen_point_pbs(vec3 colour,vec3 position,vec3 normals,float metallic,float roughness,
		light_point pl);
vec3 lumen_spot(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_spot sl);
vec3 lumen_process_pbs(vec3 colour,vec3 light_dir,vec3 influence,vec3 normals,vec3 halfway,
		float metallic,float roughness);

// math
float schlick_beckmann_approx(float dlgt_rel,float roughness);

// constant
const float PI = 3.141592653;

// global
vec3 camera_dir;
vec3 bfresnel;
float asq;
float dlgt_out;
float schlick_out;

void main()
{
	// read g-buffer information
	vec4 colourxspec = texture(gbuffer_colour,TexCoords);
	vec4 positionxnull = texture(gbuffer_position,TexCoords);
	vec4 normalsxemission = texture(gbuffer_normals,TexCoords);
	vec4 materials = texture(gbuffer_materials,TexCoords);

	// translate g-buffer information
	vec3 colour = colourxspec.rgb;
	float speculars = colourxspec.a;
	vec3 position = positionxnull.rgb;
	vec3 normals = normalsxemission.rgb;
	float lemission = normalsxemission.a;
	float metallic = materials.r;
	float roughness = materials.g;
	float ambient_occlusion = materials.b;

	// precalculations
	camera_dir = normalize(view_pos-position);
	bfresnel = mix(vec3(.04),colour,metallic);
	asq = pow(roughness,4.0);
	dlgt_out = max(dot(normals,camera_dir),0.0);
	schlick_out = schlick_beckmann_approx(dlgt_out,roughness);

	// light emission from irradiance map
	vec3 fresnel = mix(vec3(.04),colour,metallic);
	fresnel = fresnel+(max(vec3(1.0-roughness),fresnel)-fresnel)
			* pow(clamp(1.0-max(dot(normals,camera_dir),0.0),0.0,1.0),5.0);
	vec3 ibdiff = texture(irradiance_map,normals).rgb*colour*(vec3(1.0)-fresnel)*(1.0-metallic);
	vec2 pcbrdf = texture(specular_brdf,vec2(max(dot(normals,camera_dir),0.0),roughness)).rg;
	vec3 ibspec = textureLod(specular_map,reflect(-camera_dir,normals),roughness*4.0).rgb
			* (fresnel*pcbrdf.x+pcbrdf.y);
	vec3 glb_colours = (ibdiff+ibspec)*ambient_occlusion;

	// process light sources
	vec3 sdw_colours = vec3(0);
	vec3 lgt_colours = vec3(0);
	for (int i=0;i<sunlight_count;i++)
		sdw_colours += lumen_sun_pbs(colour,position,normals,metallic,roughness,sunlight[i]);
	for (int j=0;j<pointlight_count;j++)
		lgt_colours += lumen_point_pbs(colour,position,normals,metallic,roughness,pointlight[j]);
		//lgt_colours += lumen_point(colour,position,normals,speculars,pointlight[j]);
	//for (int k=0;k<spotlight_count;k++)
	//	lgt_colours += lumen_spot(colour,position,normals,speculars,spotlight[k]);

	// process shadows with dynamic bias for sloped surfaces
	vec4 rltp = shadow_matrix*vec4(position,1.0);
	vec3 ltp = (rltp.xyz/rltp.w)*.5+.5;
	vec3 slight_position = normalize(light_position-position);
	float obj_depth = ltp.z;
	float bias = tan(acos(dot(normals,slight_position)))*.00001;
	float gshadow = min(1.0-dot(normals,slight_position),1.0);
	//float shadow = mix(float(texture(shadow_map,ltp.xy).r<(obj_depth-bias)),.0,gshadow);
	float shadow = max(float(texture(shadow_map,ltp.xy).r<(obj_depth-bias)),gshadow);

	// combine lighting stages
	vec3 cmb_colours = lgt_colours+sdw_colours+glb_colours*(1.0-lemission)*(1.0-shadow*.66);

	// process emission
	vec3 emit_colours = colour*lemission;
	cmb_colours = max(cmb_colours,emit_colours);
	// TODO: figure out if space over time is the best call in this case

	// colour corrections: high dynamic exposure & gamma correction
	cmb_colours = vec3(1.0)-exp(-cmb_colours*lens_exposure);
	cmb_colours = pow(cmb_colours,vec3(1.0/gamma));

	// return colour composition
	outColour = vec4(cmb_colours,1.0);
	//outColour = vec4(vec3(gshadow,gshadow-1.0,0),1.0);
}

// specular processing
vec3 process_specular(vec3 colour,vec3 lgt_colour,float in_speculars,vec3 spec_dir,float fresnel)
{
	float spec = pow(max(dot(camera_dir,spec_dir),0),spec_exponent)*spec_intensity;//*pow(fresnel,.25);
	return spec*lgt_colour*in_speculars;
}

// sunlight processing
vec3 lumen_sun(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_sun sl)
{
	// precalculations
	vec3 light_dir = normalize(sl.position-position);
	vec3 spec_dir = reflect(-light_dir,normals);
	float fresnel = max(1-dot(camera_dir,normals),.1);

	// specular & diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = colour*diff*sl.colour;
	vec3 specular = process_specular(colour,sl.colour,in_speculars,spec_dir,fresnel);

	// combine sunlight shading components
	//return (mix(diffusion,vec3(diff*.4),in_speculars*pow(fresnel,4))+specular)*sl.intensity;
	return diffusion+specular*sl.intensity;
}

// physical based sunlight processing
vec3 lumen_sun_pbs(vec3 colour,vec3 position,vec3 normals,float metallic,float roughness,
		light_sun sl)
{
	// precalculations
	vec3 light_dir = normalize(sl.position-position);
	vec3 halfway = normalize(camera_dir+light_dir);
	vec3 influence = sl.colour*sl.intensity;

	// return sunlight colours
	return lumen_process_pbs(colour,light_dir,influence,normals,halfway,metallic,roughness);
}

// pointlight processing
vec3 lumen_point(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_point pl)
{
	// precalculations
	vec3 light_dir = normalize(pl.position-position);
	vec3 spec_dir = reflect(-light_dir,normals);
	float fresnel = max(1-dot(camera_dir,normals),.1);

	// specular & diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = colour*diff*pl.colour;
	vec3 specular = process_specular(colour,pl.colour,in_speculars,spec_dir,fresnel);

	// calculate pointlight influence
	float distance = length(pl.position-position);
	float attenuation = 1/(pl.constant+pl.linear*distance+pl.quadratic*(distance*distance));
	return (diffusion+specular)*attenuation*pl.intensity;
}

// physical based pointlight processing
vec3 lumen_point_pbs(vec3 colour,vec3 position,vec3 normals,float metallic,float roughness,
		light_point pl)
{
	// precalculations
	vec3 light_dir = normalize(pl.position-position);
	vec3 halfway = normalize(camera_dir+light_dir);

	// calculate pointlight influence
	float distance = length(pl.position-position);
	float attenuation = 1/(pl.constant+pl.linear*distance+pl.quadratic*(distance*distance));
	vec3 influence = pl.colour*attenuation;

	// return pointlight colours
	return lumen_process_pbs(colour,light_dir,influence,normals,halfway,metallic,roughness);
}

// spotlight processing
vec3 lumen_spot(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_spot sl)
{
	// precalculations
	vec3 light_dir = normalize(sl.position-position);
	vec3 spec_dir = reflect(-light_dir,normals);
	float fresnel = max(1-dot(camera_dir,normals),.1);

	// specular & diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = colour*diff*sl.colour;
	vec3 specular = process_specular(colour,sl.colour,in_speculars,spec_dir,fresnel);

	// calculate spotlight influence
	float theta = dot(light_dir,normalize(-sl.direction));
	float epsilon = sl.cut_in-sl.cut_out;
	float intensity = clamp((theta-sl.cut_out)/epsilon,0,1);
	return (diffusion+specular)*intensity;
}

// physical based light processing
vec3 lumen_process_pbs(vec3 colour,vec3 light_dir,vec3 influence,vec3 normals,vec3 halfway,
		float metallic,float roughness)
{
	// distribution component
	float throwbridge_reitz = asq/(PI*pow(pow(max(dot(normals,halfway),0.0),2.0)*(asq-1.0)+1.0,2.0));

	// fresnel component through approximation
	vec3 fresnel = bfresnel+(1.0-bfresnel)
			* pow(clamp(1.0-max(dot(normals,camera_dir),0.0),0.0,1.0),5.0);

	// geometry component
	float dlgt_in = max(dot(normals,light_dir),0.0);
	float smith = schlick_beckmann_approx(dlgt_in,roughness)*schlick_out;

	// calculate specular brdf
	vec3 cook_torrance = (throwbridge_reitz*fresnel*smith)/(4.0*dlgt_in*dlgt_out+.0001);
	return ((vec3(1.0)-fresnel)*(1.0-metallic)*colour/PI+cook_torrance)*influence*dlgt_in;
}

// function to approximate physical based lighting geometry
float schlick_beckmann_approx(float dlgt_rel,float roughness)
{
	float direct_lgt = pow(roughness+1.0,2)/8.0;
	return dlgt_rel/(dlgt_rel*(1-direct_lgt)+direct_lgt);
}