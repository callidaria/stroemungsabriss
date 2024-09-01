#ifndef CCB_MATH_SELFTITLED
#define CCB_MATH_SELFTITLED

#include "toolbox.h"
#include "../fcn/config.h"


// coordinate systems
// 2D camera translates sprite and other arbitrary 2D canvae
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


// 3D camera translates meshes and other arbitrary 3D geometry
class Camera3D
{
public:

	// construction
	Camera3D(glm::vec3 position,glm::vec3 target,float ratio,float fov);

	// utility
	void rotate_around_target(float deg);
	void update();

public:

	// conversion matrices
	glm::mat4 view3D,proj3D;

private:

	glm::vec3 m_position,m_target;
};
inline Camera3D g_Camera3D = Camera3D(
		glm::vec3(0,-4,2),glm::vec3(0),
		(float)g_Config.vFrameResolutionWidth/(float)g_Config.vFrameResolutionHeight,90.f
	);


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
