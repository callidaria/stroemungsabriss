#version 330 core

in vec3 Position;

out vec4 outColour;

// colour source
uniform samplerCube irradiance_map;

// approximation geometry
uniform float riemann_interval = .01;

// constant
const float MATH_PI = 3.141592653;

void main()
{
	// high resolution
	vec3 Normals = normalize(Position);
	vec3 Right = normalize(cross(vec3(0,1,0),Normals));
	vec3 Up = normalize(cross(Normals,Right));

	// approximate convoluted reflectance integral
	int iterations = 0;
	vec3 colours = vec3(0);
	for (float phi=0.0;phi<2.0*MATH_PI;phi+=riemann_interval) {
		for (float theta=0.0;theta<.5*MATH_PI;theta+=riemann_interval) {
			vec3 tsph_coord = vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
			vec3 psph_coord = tsph_coord.x*Right+tsph_coord.y*Up+tsph_coord.z*Normals;
			colours += texture(irradiance_map,psph_coord).rgb*cos(theta)*sin(theta);
			iterations++;
		}
	}

	// colour output
	outColour = vec4((MATH_PI*colours*(1.0/float(iterations))).rgb,1.0);
}