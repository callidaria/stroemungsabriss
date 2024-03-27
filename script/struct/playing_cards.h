#ifndef SCR_STRUCTURES_PLAYING_CARDS
#define SCR_STRUCTURES_PLAYING_CARDS

#include "../../ccb/core.h"
#include "../../ccb/gfx/shader.h"
#include "../../ccb/gfx/mesh.h"
#include "../../ccb/gfx/renderer3d.h"

#include "../../ccb/fcn/buffer.h"

#include "feature_base.h"

// count
constexpr uint8_t CSYS_CARD_COUNT = 112;

// positioning
constexpr float CARDSYSTEM_CARD_WIDTH = 2.25f;
constexpr float CARDSYSTEM_CARD_HEIGHT = 3.5f;
constexpr float CARD_HWIDTH = CARDSYSTEM_CARD_WIDTH/2;
constexpr float CARD_HHEIGHT = CARDSYSTEM_CARD_HEIGHT/2;

// upload capacity
constexpr uint8_t CARDSYSTEM_INDEX_REPEAT = 12;

enum CardVID {
	CARD_POSITION_X,CARD_POSITION_Y,CARD_POSITION_Z,
	CARD_TCOORD_X,CARD_TCOORD_Y,
	CARD_NORMALS_X,CARD_NORMALS_Y,CARD_NORMALS_Z,
	CARD_SIDES,
	CARD_VERTEX_REPEAT
};

enum CardIID {
	ICARD_POSITION_X,ICARD_POSITION_Y,ICARD_POSITION_Z,
	ICARD_TEXTURE_X,ICARD_TEXTURE_Y,
	ICARD_SINROT_X,ICARD_SINROT_Y,ICARD_SINROT_Z,
	ICARD_COSROT_X,ICARD_COSROT_Y,ICARD_COSROT_Z,
	ICARD_DECK,
	CARD_INSTANCE_REPEAT
};

class PlayingCards : public ShadowGeometry
{
public:

	// construction
	PlayingCards() {  }
	PlayingCards(CascabelBaseFeature* ccbf,StageSetup* set_rigs,glm::vec3 sdir);
	~PlayingCards() {  }

	// draw
	virtual void render_shadow();
	void render();

public:

	// render
	std::vector<float> rqueue = std::vector<float>(CSYS_CARD_COUNT*CARD_INSTANCE_REPEAT);

private:

	// cascabel
	Buffer bfr = Buffer();
	Shader sdr = Shader();
	CascabelBaseFeature* m_ccbf;
	StageSetup* m_setRigs;

	// render
	GLuint tex;

	// math
	glm::vec3 shadow_dir;
};

#endif
