#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform vec2 fres = vec2(1920,1080); // ??obsolete if calculated pos is uniform
uniform float vgnt = 0.45;
uniform sampler2D tex;
uniform sampler2D splash;
uniform sampler2D title;
uniform sampler2D select;

void main()
{
	// texturing
	vec4 proc = texture(tex,TexCoords);
	vec4 sproc = texture(splash,TexCoords);
	vec4 tproc = texture(title,TexCoords);
	vec4 selproc = texture(select,TexCoords); // !!NAMING

	// sepia effect
	vec2 pos = (gl_FragCoord.xy/fres)-vec2(0.5); // !!PERF: precalc in vertex
	float vignette = smoothstep(0.75,0.75-vgnt,length(pos));
	proc.rgb = mix(proc.rgb,proc.rgb*vignette,0.75);
	float alpha = (proc.r*0.299)+(proc.g*0.587)+(proc.b*0.114);
	vec3 sepia = vec3(alpha)*vec3(1.35,1.0,0.65);

	// colour intermission
	vec4 cproc = sproc+tproc+selproc;
	outColour = vec4(mix(proc.rgb,sepia,0.75),1.0)+cproc;
	/* if (cproc.r>0||cproc.g>0||cproc.b>0)
		//outColour = vec4(0.5-(proc.b/2)sproc.r,0,proc.b,1.0); // !!no branches
		outColour = vec4(cproc.rgb,1.0);
	else outColour = vec4(mix(proc.rgb,sepia,0.75),1.0); */
}
