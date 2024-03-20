#version 330 core

in vec3 Position;

out vec4 outColour;

// light map
uniform samplerCube irradiance_map;

// sampling settings
uniform float roughness = .0;
uniform int sample_count = 2048;

// constants
const float MATH_PI = 3.141592653;

void main()
{
	// handling hemisphere normals & tangents
	vec3 normals = normalize(Position);
	bool flipped = abs(normals.z)<.9;
	vec3 up = vec3(int(!flipped),0,int(flipped));
	vec3 ntangent = normalize(cross(up,normals));
	vec3 nbitangent = cross(normals,ntangent);

	// precalculations
	float asq = pow(roughness,4.0);
	float lodmod = .5*float(roughness!=0.0);

	// iterate through samples
	vec3 colours = vec3(0.0);
	float cmb_wangle = 0.0;
	for (int i=0;i<sample_count;i++) {

		// hammersley set (in-character work smarter not harder version)
		uint vdcorput = uint(i);
		vdcorput = (vdcorput<<16u)|(vdcorput>>16u);
		vdcorput = ((vdcorput&0x55555555u)<<1u)|((vdcorput&0xAAAAAAAAu)>>1u);
		vdcorput = ((vdcorput&0x33333333u)<<2u)|((vdcorput&0xCCCCCCCCu)>>2u);
		vdcorput = ((vdcorput&0x0F0F0F0Fu)<<4u)|((vdcorput&0xF0F0F0F0u)>>4u);
		vdcorput = ((vdcorput&0x00FF00FFu)<<8u)|((vdcorput&0xFF00FF00u)>>8u);
		vec2 hammersley = vec2(float(i)/float(sample_count),float(vdcorput)*2.3283064365386963e-10);
		// TODO: i really need to work on my bitwise skills

		// lobe geometry cast
		float phi = 2.0*MATH_PI*hammersley.x;
		vec2 theta = vec2(0,sqrt((1.0-hammersley.y)/(1.0+(asq-1.0)*hammersley.y)));
		theta.x = sqrt(1.0-pow(theta.y,2.0));
		vec3 halfway = vec3(cos(phi)*theta.x,sin(phi)*theta.x,theta.y);
		vec3 lobecast = normalize(ntangent*halfway.x+nbitangent*halfway.y+normals*halfway.z);
		vec3 light_dir = normalize(2.0*dot(normals,lobecast)*lobecast-normals);
		float dlgt_in = max(dot(normals,light_dir),0.0);

		// complete sample and add weight
		colours += texture(irradiance_map,light_dir).rgb*dlgt_in;
		cmb_wangle += dlgt_in;
	} outColour = vec4(colours/cmb_wangle,1.0);
}
