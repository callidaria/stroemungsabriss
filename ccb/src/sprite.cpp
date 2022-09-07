#include "../gfx/sprite.h"

/*
	constructor(vec2,float,float,const char*)
	p: the sprites origin position
	w: width of the sprite
	h: height of the sprite
	t: path to sprite texture
	purpose: calculate vertex upload values and generate texture
*/
Sprite::Sprite(glm::vec2 p,float w,float h,const char* t)
	: pos(p),sclx(w),scly(h),texpath(t)
{
	// definition of the corner vertices
	v[0] = p.x;v[1] = p.y+h;v[2] = 0.0f;v[3] = 0.0f;
	v[4] = p.x+w;v[5] = p.y+h;v[6] = 1.0f;v[7] = 0.0f;
	v[8] = p.x+w;v[9] = p.y;v[10] = 1.0f;v[11] = 1.0f;
	v[12] = p.x;v[13] = p.y;v[14] = 0.0;v[15] = 1.0f;

	// texture generation
	glGenTextures(1,&tex);
} Sprite::~Sprite() {  }

/*
	texture() -> void
	purpose: upload file data to texture to use in shader program as sampler2D
*/
void Sprite::texture()
{
	// setup
	glBindTexture(GL_TEXTURE_2D,tex);
	int width,height;

	// load texture data
#ifdef __WIN32__

	unsigned char* image = stbi_load(texpath,&width,&height,0,STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
	stbi_image_free(image);

#else

	unsigned char* image = SOIL_load_image(texpath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
	SOIL_free_image_data(image);

#endif

	// texture specifications and mipmap
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
	setup() -> void
	purpose: ready the components of sprite to be ready for drawing
*/
void Sprite::setup()
{
	glBindTexture(GL_TEXTURE_2D,tex);
}

/*
	transform(vec2,vec2,float) -> void
	tp: position to translate the sprite to
	ts: new scaling of position
	tr: rotation of the sprite around its center
	purpose: change all attributes of the sprites position at once without reverse translations
*/
void Sprite::transform(glm::vec2 tp,glm::vec2 ts,float tr)
{
	model = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x+sclx/2+tp.x,pos.y+scly/2+tp.y,0));
	model = glm::rotate(model,glm::radians(tr),glm::vec3(0,0,1));
	model = glm::scale(model,glm::vec3(ts.x,ts.y,1));
	model = glm::translate(model,glm::vec3(-(pos.x+sclx/2),-(pos.y+scly/2),0));
}

/*
	transform(vec2,vec2,float,vec2) -> void
	tp: position to translate the sprite to
	ts: new scaling of position
	tr: rotation of the sprite around its center
	a: arbitrary axis to use as pseudo center to rotate, scale and translate around
	purpose: transform sprite position but around an arbitrary, externally defined axis
*/
void Sprite::transform(glm::vec2 tp,glm::vec2 ts,float tr,glm::vec2 a)
{
	float ax = a.x-(pos.x+sclx/2),ay = a.y-(pos.y+scly/2);
	model = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x+sclx/2+tp.x+ax,pos.y+scly/2+tp.y+ay,0));
	model = glm::rotate(model,glm::radians(tr),glm::vec3(0,0,1));
	model = glm::scale(model,glm::vec3(ts.x,ts.y,1));
	model = glm::translate(model,glm::vec3(-(pos.x+sclx/2+ax),-(pos.y+scly/2+ay),0));
	// FIXME: last 3 lines are double-up code
}

/*
	translate(vec2) -> void
	tp: vector holding direction and length to move the sprite towards
	purpose: transform sprite position from the existing position by the given amount and direction
*/
void Sprite::translate(glm::vec2 tp)
{
	model = glm::translate(model,glm::vec3(tp.x,tp.y,0));
	// FIXME: change values directly like in scale for optimization
}

/*
	scale(float,float) -> void
	wscale: x-axis scale describing the sprites width
	hscale: y-axis scale describing the sprites height
	purpose: set the sprites scaling
*/
void Sprite::scale(float wscale,float hscale)
{
	model[0][0] = wscale;
	model[1][1] = hscale;
}

/*
	scale_arbit(float,float) -> void
	DEPRECATED: this scaling seems to be outdated.
		-> if we need an inverse translation before scale, not like this please.
*/
void Sprite::scale_arbit(float wscale,float hscale)
{
	model = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x,pos.y,0));
	model = glm::scale(model,glm::vec3(wscale,hscale,1));
	model = glm::translate(model,glm::vec3(-pos.x,-pos.y,0));
}

/*
	rotate(float) -> void
	rot: the sprites delta rotation
	purpose: rotates the sprite: this is not a setter!
		the previous rotation will be changed, not reset before rotation
*/
void Sprite::rotate(float rot)
{
	model = glm::rotate(model,glm::radians(rot),glm::vec3(0,0,1));
}