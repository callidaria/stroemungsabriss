#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform vec2 tPixelOffset = vec2(1./1280.,1./720.);

// settings
uniform float darkness_cutoff = .0625;
uniform float contrast_threshold = .125;
uniform float subpixel_detail = .75;
uniform float subpixel_removal = .25;
uniform float edge_iterations = 8;

// math
float calc_luminance(vec4 pixel)
{ return dot(pixel.rgb,vec3(.299,.587,.144)); }
// optimization by disregarding blue as a relevant colour (publication). dabadeedabaday
// { return pixel.g*(.587/.299)+pixel.r; }

// This will be finished at a later point in development (probably 0.1.0)
// right now i don't have the time to babysit this abomination
// i started this implementation early because of the dropshadows with gbuffer in menu
// but now it dawns on me, that there is a MUCH MUCH easier way to achieve those shadows with msaa
// also the dropshadows will be delayed for now anyways because today is deadline day :/
// !!!also also there isn't even an issue open for dropshadow implementation in menu!!! wth?

void main()
{
	// local pixels
	vec4 pxCenter = texture(tex,TexCoords);
	vec4 pxUp = texture(tex,vec2(TexCoords.x,TexCoords.y+tPixelOffset.y));
	vec4 pxLeft = texture(tex,vec2(TexCoords.x-tPixelOffset.x,TexCoords.y));
	vec4 pxRight = texture(tex,vec2(TexCoords.x+tPixelOffset.x,TexCoords.y));
	vec4 pxDown = texture(tex,vec2(TexCoords.x,TexCoords.y-tPixelOffset.y));

	// local luminance
	float lmCenter = calc_luminance(pxCenter);
	float lmUp = calc_luminance(pxUp);
	float lmLeft = calc_luminance(pxLeft);
	float lmRight = calc_luminance(pxRight);
	float lmDown = calc_luminance(pxDown);

	// high contrast edge detection & check for early shader termination
	float maxContrast = max(lmCenter,max(max(lmUp,lmLeft),max(lmRight,lmDown)));
	float rContrast = maxContrast-min(lmCenter,min(min(lmUp,lmLeft),min(lmRight,lmDown)));
	if (rContrast<max(darkness_cutoff,maxContrast*contrast_threshold)) {
		outColour = pxCenter;
		return;
	}

	// diagonal pixels & their luminance
	vec4 pxDgUL = texture(tex,vec2(TexCoords.x-tPixelOffset.x,TexCoords.y+tPixelOffset.y));
	vec4 pxDgUR = texture(tex,vec2(TexCoords.x+tPixelOffset.x,TexCoords.y+tPixelOffset.y));
	vec4 pxDgDL = texture(tex,vec2(TexCoords.x-tPixelOffset.x,TexCoords.y-tPixelOffset.y));
	vec4 pxDgDR = texture(tex,vec2(TexCoords.x+tPixelOffset.x,TexCoords.y-tPixelOffset.y));
	float lmDgUL = calc_luminance(pxDgUL);
	float lmDgUR = calc_luminance(pxDgUR);
	float lmDgDL = calc_luminance(pxDgDL);
	float lmDgDR = calc_luminance(pxDgDR);

	// dice pattern luminance combination
	// not yet established as superior implememtation, this will go through some iterations
	float lmcVert = lmUp+lmDown;
	float lmcHorz = lmLeft+lmRight;
	float lmcUp = lmDgUL+lmDgUR;
	float lmcLeft = lmDgUL+lmDgDL;
	float lmcRight = lmDgUR+lmDgDR;
	float lmcDown = lmDgDL+lmDgDR;

	// edge detection
	vec2 edgeDir = vec2(abs(-2.*lmLeft+lmcLeft)+abs(-2.*lmCenter+lmcVert)*2.
			+ abs(-2.*lmRight+lmcRight),abs(-2.*lmUp+lmcUp)+abs(-2.*lmCenter+lmcHorz)*2.
			+ abs(-2.*lmDown+lmcDown));
	// possible faith-based implementation derivative (untested as of right now)
	// would make dice pattern combination obsolete
	//vec2 edgeDir = vec2(abs((.25*lmDgUL)+(-.5*lmUp)+(.25*lmDgUR))
	//		+ abs((.5*lmLeft)+(-1.*lmCenter)+(.50*lmRight))
	//		+ abs((.25*lmDgDL)+(-.5*lmDown)+(.25*lmDgDR)),
	//		abs((.25*lmDgUL)+(-.5*lmLeft)+(.25*lmDgDL))
	//		+ abs((.5*lmUp)+(-1.*lmCenter)+(.5*lmDown))
	//		+ abs((.25*lmDgUR)+(-.5*lmRight)+(.25*lmDgDR)));
	int edgeHorz = int(edgeDir.x>=edgeDir.y);
	int edgeVert = 1-edgeHorz;

	// gradient calculation
	float luminance[2],grad[2];
	luminance[0] = lmDown*edgeHorz+lmLeft*edgeVert;
	luminance[1] = lmUp*edgeHorz+lmRight*edgeVert;
	grad[0] = luminance[0]-lmCenter;
	grad[1] = luminance[1]-lmCenter;
	int idx_steep = int(abs(grad[1])>abs(grad[0]));
	float fsGrad = grad[idx_steep]*.25;
	// FIXME: i refuse to use ternary in shader, is this a valid implementation do we keep mult?
	// in my "hellfire: the defense" the next step is a lot faster due to this idea

	// calculate local luminance average
	float pxSize = (tPixelOffset.x*edgeVert+tPixelOffset.y*edgeHorz)*(idx_steep*2.-1);
	float average = (luminance[idx_steep]+lmCenter)*.5;

	// subpixel shift along edge
	vec2 pxPos[2];
	pxPos[0] = TexCoords+vec2(pxSize*edgeVert*.5,pxSize*edgeHorz*.5);
	vec2 pxStep = vec2(tPixelOffset.x*edgeHorz,tPixelOffset.y*edgeVert);
	pxPos[1] = pxPos[0]+pxStep;
	pxPos[0] -= pxStep;

	// iterate edges
	int edgeRel[2];
	edgeRel[0] = 1,edgeRel[1] = 1;
	int i = 0;
	while (i<edge_iterations&&!bool(edgeRel[0])&&!bool(edgeRel[1])) {

		// check edge relevancy
		edgeRel[0] = int(abs(calc_luminance(texture(tex,pxPos[0]))-average)>fsGrad);
		edgeRel[1] = int(abs(calc_luminance(texture(tex,pxPos[1]))-average)>fsGrad);

		// proceed along edge if relevant & increase iterator
		pxPos[0] -= pxStep*edgeRel[0];
		pxPos[1] += pxStep*edgeRel[1];
		i++;
	}

	// edge distance
	float negDistance = (TexCoords.x-pxPos[0].x)+(TexCoords.y-pxPos[0].y);
	float posDistance = (pxPos[1].x-TexCoords.x)+(pxPos[1].y-TexCoords.y);
	float ofsDistance = -min(negDistance,posDistance)/(negDistance+posDistance)+.5;

	// calculate final coordinate
	vec2 fUV = TexCoords+vec2(ofsDistance)*vec2(tPixelOffset.x*edgeVert,tPixelOffset.y*edgeHorz);
	vec3 colour_result = texture(tex,fUV).rgb;
	outColour = vec4(colour_result,pxCenter.a);
}
