#include "player.h"

Player::Player(Renderer2D* r2d)
	: m_r2d(r2d)
{
	ri = m_r2d->add(glm::vec2(200,200),200,200,"res/flyfighter.png");
}
Player::~Player() {  }
void Player::update()
{
	m_r2d->prepare();m_r2d->render_sprite(ri,ri+1);
}
