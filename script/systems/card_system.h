#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/gfx/shader.h"

#include "../../ccb/mat/camera3d.h"
#include "../../ccb/mat/toolbox.h"

// upload capacity
constexpr uint8_t CARDSYSTEM_INDEX_REPEAT = 5;

// positioning
constexpr float CARDSYSTEM_CARD_WIDTH = 2.25f;
constexpr float CARDSYSTEM_CARD_HEIGHT = 3.5f;

// to handle which cards are in a pile, as well as pile 'physics'
struct DeckPile
{
	std::vector<uint8_t> cards;
};

class CardSystem
{
public:

	// construction
	CardSystem();
	~CardSystem() {  };

	// interaction
	void shuffle_all(glm::vec2 pos);
	void place_card(uint8_t id,glm::vec3 pos);
	void deal_card();

	// draw
	void render();

	// setters
	void set_position(uint8_t id,glm::vec3 pos);

private:

	// cascabel
	Buffer bfr = Buffer();
	Shader sdr = Shader();
	Camera3D cam3D = Camera3D(glm::vec3(0,4,10),1280.0f,720.0f,60.0f);

	// texture
	GLuint tex;

	// card information
	std::vector<float> icpos;
	std::vector<DeckPile> dpiles;
};