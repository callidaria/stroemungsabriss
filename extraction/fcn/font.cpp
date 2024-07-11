#include "font.h"

/*
	will be restructured and thus documenting now is ill advised
*/
Font::Font(const char* ff,const char* tf,float w,float h)
	: tp(tf),mw(w)
{
	FILE* file = fopen(ff,"r");
	if (file==NULL) printf("\033[1;31mcouldn't find font %s\033[0m\n",ff);
	int offset = 0;
	while (true) { // !!bad habit
		char lh[128]; // !!every new definition as char*
		int res = fscanf(file,"%s",lh);
		if (res==EOF) break;
		else {
			if (strcmp(lh,"kernings")==0) break;
			else if (strcmp(lh,"char")==0) {
				int mid,mx,my,mw,mh,mxo,myo,mxa,mp,mc;
				fscanf(file,
" id=%d\t\tx=%d\ty=%d\twidth=%d\theight=%d\txoffset=%d\tyoffset=%d\txadvance=%d\tpage=%i\tchnl=%d\n", // bad way
					&mid,&mx,&my,&mw,&mh,&mxo,&myo,&mxa,&mp,&mc);
				id[offset] = mid;
				x[offset] = mx;
				y[offset] = my;
				wdt[offset] = mw;
				hgt[offset] = mh;
				xo[offset] = mxo;
				yo[offset] = myo;
				xa[offset] = mxa;
				offset++;
			}
		}
	}
	v[0] = w; v[1] = 0; v[2] = 1.0f; v[3] = 0.0f;
	v[4] = 0; v[5] = -h; v[6] = 0.0f; v[7] = 1.0f;
	v[8] = w; v[9] = -h; v[10] = 1.0f; v[11] = 1.0f;
	v[12] = w; v[13] = 0; v[14] = 1.0f; v[15] = 0.0f;
	v[16] = 0; v[17] = 0; v[18] = 0.0f; v[19] = 0.0f;
	v[20] = 0; v[21] = -h; v[22] = 0.0; v[23] = 1.0f;
}

/*
	get_spritesheet_location(char) -> uint8_t
	c: char to get spritesheet location id for
	purpose: get spritesheet id for desired char, to reference char gfx in font
	returns: spritesheet id of given char
*/
uint8_t Font::get_spritesheet_location(char c)
{
	int i = 0;
	while (i<96) { // ??maybe alternate iteration until correct index that is more performant
		if (id[i]==(int)c) break;
		i++;
	}
	return i;
}
// FIXME: optimize please

/*
	!O(n) .characters in word /+function -> (public)
	purpose: predict x-axis space taken up by given word written in this font
	\param word: text up for space estimation
	\returns screenwidth taken up by given word
*/
uint32_t Font::estimate_textwidth(const char* word)
{
	uint32_t out_width = 0;
	for (uint32_t i=0;i<strlen(word);i++) {
		if (word[i]!=' ') out_width += xa[get_spritesheet_location(word[i])]*(mw/83.f);
		else out_width += 57.0f*(mw/83.0f);
	} return out_width;
}
