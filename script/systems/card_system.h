#ifndef SCR_SYSTEMS_CARD_SYSTEM
#define SCR_SYSTEMS_CARD_SYSTEM

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/frm/frame.h"

#include "../../ccb/mat/camera3d.h"
#include "../../ccb/mat/lighting.h"
#include "../../ccb/mat/toolbox.h"

#include "../../ccb/gfx/shader.h"
#include "../../ccb/gfx/renderer2d.h"
#include "../../ccb/gfx/renderer3d.h"

#include "../struct/feature_base.h"
#include "../struct/playing_cards.h"
//#include "../ui/cursor.h"

// count
constexpr uint16_t CSYS_CURRENCY_CAP = 1024;

// timing
constexpr float CARDSYSTEM_DEAL_WAIT = .15f;

// structure to hold currency object & texture path
struct Currency
{
	const char* object;
	const char* texture,*specular,*normals,*emission;
	uint16_t value;
};

// currency stats
struct CurrencyStats
{
	uint16_t value;
	uint16_t spawn = 0;
};

// currency stacking
struct CurrencyStack
{
	glm::vec2 position;
	glm::vec3 direction;
	std::vector<std::vector<uint16_t>> stacks;
};

// to handle which cards are in a pile, as well as pile 'physics'
struct DeckPile
{
	std::vector<uint8_t> cards;
	glm::vec2 pos;
};

// open animation processes acting on card
struct CardAnimation
{
	uint8_t card_id;
	glm::vec3 s_pos,s_rot;
	glm::vec3 t_pos,t_rot;
	uint16_t ctime,etime;
};

// opposing player informations
struct OpposingPlayer
{
	glm::vec2 position;
	float rotation;
	std::vector<uint8_t> deal;
	std::vector<uint8_t> cards;
	CurrencyStack capital;
};

// pipeline entites to automatically deal cards
struct DealProcess
{
	uint8_t source;
	uint8_t target;
	uint8_t amount;
};

class CardSystem
{
public:

	// construction
	CardSystem() {  }
	CardSystem(CascabelBaseFeature* ccbf,std::vector<Currency> curr_path);
	~CardSystem() {  }

	// interaction
	void shuffle_all();
	void deal_card(uint8_t pid);
	void deal_card(uint8_t pid,uint8_t oid);
	void register_auto_deal(uint8_t pid,uint8_t target,uint8_t amount);
	void hand_to_pile(uint8_t pid);
	void opponent_to_pile(uint8_t oid,uint8_t pid,uint8_t idx);
	void add_currency(uint8_t cid,uint16_t count);
	void add_currency(uint8_t cid,uint8_t oid,uint16_t count);
	void set_currency(uint8_t cid,uint8_t sid,uint16_t count);
	void set_currency(uint8_t cid,uint8_t oid,uint8_t sid,uint16_t count);

	// creation
	void create_player(glm::vec2 pos,float rot,uint16_t capital);
	inline void create_pile(glm::vec2 pos) { dpiles.push_back({ {},pos }); }
	void create_currency_stack(glm::vec2 pos,float rot);

	// update
	void process_input();
	void update();
	void render();

	// setters
	void set_position(uint8_t id,glm::vec3 pos);
	void set_rotation(uint8_t id,glm::vec3 rot);
	void reset_rotation(uint8_t id);

	// getters
	glm::vec3 get_position(uint8_t id);
	glm::vec3 get_rotation(uint8_t id);

	// TODO: create a readable way of indexing playing cards
	// TODO: variable text annotations & descriptions for playing cards

private:

	// helpers
	void create_card(size_t& mem_id,glm::vec2 tex_id,bool deck_id);
	//void card_to_queue(uint8_t id);
	glm::vec3 get_card_screen_space(uint8_t id);

	// update
	void update_hand_position();
	void update_opponent(uint8_t oid);

	// animation
	void create_animation(uint8_t id,glm::vec3 pos,uint16_t etime);
	void create_animation(uint8_t id,glm::vec3 pos,glm::vec3 rot,uint16_t etime);
	void force_create_animation(uint8_t id,glm::vec3 pos,uint16_t etime);
	void force_create_animation(uint8_t id,glm::vec3 pos,glm::vec3 rot,uint16_t etime);
	void remove_animation(uint8_t id);
	int16_t get_animation_id(uint8_t id);

private:

	// cascabel
	CascabelBaseFeature* m_ccbf;

	// render
	uint16_t r3d_index,ir3d_index;

	// card information
	PlayingCards* pcards;
	std::vector<DeckPile> dpiles;
	std::vector<uint8_t> deal,hand;
	std::vector<float> hand_mod;
	uint8_t choice = 0;

	// currency information
	std::vector<CurrencyStats> currency_stats;
	CurrencyStack cstack = { glm::vec2(7,7),{} };
	std::vector<CurrencyStack> field_stacks;

	// npc
	std::vector<OpposingPlayer> ops;
	glm::mat4 phead_mat;

	// system
	std::vector<CardAnimation> c_anims;
	std::vector<DealProcess> auto_deals;
	uint8_t crr_deal = 0;
	float crr_dtime = 0;

	// controls
	//Cursor cursor = Cursor(m_frame,m_r2d);
	bool lfI = false,kinput = true;
	int32_t tmx = 0;
	float cstart = 0;
};

#endif
