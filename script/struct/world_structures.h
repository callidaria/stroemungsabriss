#ifndef SCR_STRUCTURES_WORLD_STRUCTURES
#define SCR_STRUCTURES_WORLD_STRUCTURES

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/frm/framebuffer.h"


// describes game board setup routines for loading and rendering
template <typename Derivative> class Board
{
public:
	void load() { (static_cast<Derivative*>(this))->load(); };
	void update() { (static_cast<Derivative*>(this))->update(); };
};

// describes any form of large scale user interface the player interacts with mainly, when enabled
class UI
{
public:
	virtual ~UI() {  }
	virtual void render(FrameBuffer*,bool&,bool&) {  }
};

// describes all environments the player visits during playtime
class Scene
{
public:
	virtual ~Scene() {  }
	virtual void render() {  }
};

// describes all objects/character, that can be controlled by the player
class Player
{
public:
	virtual ~Player() {  }
	virtual void update() {  }

public:
	glm::vec3 position = glm::vec3(0);
};

// describes any form of enemy, that can be considered significant and/or deserves a health bar
class Boss
{
public:
	virtual ~Boss() {  }
	virtual void update(glm::vec2) {  }
};

#endif

// TODO: split 2D and 3D render instructions to reduce amount of prepare calls
