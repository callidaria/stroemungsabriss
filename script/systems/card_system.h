#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/gfx/shader.h"

#include "../../ccb/mat/camera3d.h"
#include "../../ccb/mat/toolbox.h"

// upload capacity
constexpr uint8_t CARDSYSTEM_UPLOAD_REPEAT = 6;
constexpr uint8_t CARDSYSTEM_INDEX_REPEAT = 12;

// positioning
constexpr float CARDSYSTEM_CARD_WIDTH = 2.25f;
constexpr float CARDSYSTEM_CARD_HEIGHT = 3.5f;

// to handle which cards are in a pile, as well as pile 'physics'
struct DeckPile
{
	std::vector<uint8_t> cards;
	glm::vec2 pos;
};

class CardSystem
{
public:

	// construction
	CardSystem();
	~CardSystem() {  };

	// interaction
	void shuffle_all();
	void deal_card(uint8_t pid);
	void hand_to_pile(uint8_t pid,uint8_t idx);
	void move_to_pile(uint8_t pid,uint8_t cid);

	// creation
	void create_pile(glm::vec2 pos);

	// draw
	void render();

	// setters
	void set_position(uint8_t id,glm::vec3 pos);
	void set_rotation(uint8_t id,glm::vec3 rot);
	void reset_rotation(uint8_t id);

private:

	// helpers
	void create_card(glm::vec2 tex_id,bool deck_id);
	void update_hand_position();

private:

	// cascabel
	Buffer bfr = Buffer();
	Shader sdr = Shader();
	Camera3D cam3D = Camera3D(glm::vec3(0,4,15),1280.0f,720.0f,60.0f);

	// texture
	GLuint tex;

	// card information
	std::vector<float> icpos;
	std::vector<DeckPile> dpiles;
	std::vector<uint8_t> hand;
};