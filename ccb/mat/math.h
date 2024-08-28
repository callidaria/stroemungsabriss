#ifndef CCB_MATH_SELFTITLED
#define CCB_MATH_SELFTITLED

#include "toolbox.h"


// coordinate systems
// 2D camera translates sprite and other 2D canvas position and scaling
class Camera2D
{
public:

	// construction
	Camera2D(float x_bounds,float y_bounds);

public:

	// conversion matrices
	glm::mat4 view2D,proj2D;
};
inline Camera2D g_Camera2D = Camera2D(1280.f,720.f);

// TODO 3D camera


// transformation math
// basic functionality of 2D geometry
struct Transform2D
{
	// transformation combined
	void transform(glm::vec2 p,float w,float h,float r);
	void transform(glm::vec2 p,float w,float h,float r,glm::vec2 a);
	void to_origin();

	// transformation utility
	inline void translate(glm::vec2 p) { model = glm::translate(model,glm::vec3(p.x,p.y,0)); }
	inline void scale(float w,float h) { model[0][0] = w, model[1][1] = h; }
	inline void rotate(float r) { model = glm::rotate(model,glm::radians(r),glm::vec3(0,0,1)); }

	// arbitrary transformation
	void scale(float w,float h,glm::vec2 a);
	void rotate(float r,glm::vec2 a);

	// attributes
	const glm::vec2 position;
	const float width,height;
	float rot = .0f;

	// transform
	glm::mat4 model = glm::mat4(1.f);
};

// basic functionality of 3D geometry
struct Transform3D
{
	// TODO

	// transform
	glm::mat4 model = glm::mat4(1.f);
};

#endif
