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
	Camera3D(glm::vec3 pos,glm::vec3 tgt,float ratio,float fov);

	// utility
	void rotate_around_target(float deg);
	void update();

public:

	// conversion matrices
	glm::mat4 view3D,proj3D;
	glm::vec3 position,target;

private:
};
inline Camera3D g_Camera3D = Camera3D(
		glm::vec3(0,-4,2),glm::vec3(0),
		(float)g_Config.vFrameResolutionWidth/(float)g_Config.vFrameResolutionHeight,90.f
	);
// TODO: remake these as structs with basic utility


// transformation math
// basic functionality of 2D geometry
struct Transform2D
{
	// utility
	void transform(glm::vec2 p,float w,float h,float r);
	void transform(glm::vec2 p,float w,float h,float r,glm::vec2 a);
	inline void translate(glm::vec2 p) { model = glm::translate(model,glm::vec3(p.x,p.y,0)); }
	inline void scale(float w,float h) { model[0][0] = w, model[1][1] = h; }
	void scale(float w,float h,glm::vec2 a);
	void rotate(float r);
	void rotate(float r,glm::vec2 a);
	inline void to_origin() { transform(position,width,height,rotation); }

	// data
	const glm::vec2 position;
	const float width,height;
	float rotation = .0f;
	glm::mat4 model = glm::mat4(1.f);
};

// basic functionality of 3D geometry
struct Transform3D
{
	// utility
	void transform(glm::vec3 pos,float scl,glm::vec3 rot);
	void transform(glm::vec3 pos,float scl,glm::vec3 rot,glm::vec3 arbit);
	inline void translate(glm::vec3 p) { model = glm::translate(model,glm::vec3(p.x,p.y,p.z)); }
	inline void scale(float scl) { model[0][0] = scl,model[1][1] = scl,model[2][2] = scl; }
	void scale(float scl,glm::vec3 arbit);
	void rotate(glm::vec3 rot);
	void rotate(glm::vec3 rot,glm::vec3 arbit);
	inline void to_origin() { transform(position,scaling,rotation); }

	// data
	const glm::vec3 position;
	const float scaling;
	glm::vec3 rotation;
	glm::mat4 model = glm::mat4(1.f);
};

#endif
