#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform vec2 fres = vec2(1920,1080); // ??obsolete if calculated pos is uniform
uniform float vgnt = 0.45;
uniform float ptrans = 0.0;
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
	vec2 pos = (gl_FragCoord.xy/fres)-vec2(0.5);
	float vignette = smoothstep(0.75,0.75-vgnt,length(pos));
	vec3 sepia_proc = mix(proc.rgb,proc.rgb*vignette,0.75);
	//float alpha = (sepia_proc.r*0.299)+(sepia_proc.g*0.587)+(sepia_proc.b*0.114);
	//if (ptrans>0)
	float alpha = (proc.r*0.2126)+(proc.g*0.7152)+(proc.b*0.0722);
		//alpha = 0.0;
	//vec3 sepia = vec3(alpha)*vec3(1.35,1.0,0.65);
	vec4 sepia = proc*alpha;

	// colour mixing
	vec4 cproc = vec4(sproc.rgb+tproc.rgb+selproc.rgb,1.0);
	int opaque = 1-int(cproc.r+cproc.g+cproc.b+0.9);
	//outColour = vec4(mix(sepia_proc.rgb,sepia,0.75)*opaque,1.0)+cproc-proc.b+vec4(0,0,proc.b,1.0);
	outColour = sepia+cproc-proc.b+vec4(0,0,proc.b,1.0);
}
