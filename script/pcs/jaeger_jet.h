#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"

// movement collision bounds for the jaeger jet
constexpr uint8_t JET_BORDER_WIDTH = 42;
constexpr uint8_t JET_BORDER_HEIGHT = 18;

class JaegerJet : public Player
{
public:

	// contruction
	JaegerJet(CascabelBaseFeature* ccbf);
	~JaegerJet() {  }

	// draw
	virtual void update();

private:

	// ranged functions
	static void jet_wait(BulletSystem* bsys,int32_t* treg);
	static void jet_wide(BulletSystem* bsys,int32_t* treg);
	static void jet_focus(BulletSystem* bsys,int32_t* treg);
	static void jet_scientific(BulletSystem* bsys,int32_t* treg);

private:

	// render information
	CascabelBaseFeature* m_ccbf;
	uint16_t index_r2D,index_r3D;

	// system
	int32_t treg[3] = { 0,0,0 };
	int8_t ddur = 0;

	// animation
	float tilt = 0;

	// ranged method list
	/*
		func(BulletSystem*,int32_t*) -> void
			=> func(bsys,treg) -> void
		bsys: bullet system used to spawn player projectiles
		treg: register holding information to save beyond static method
	*/
	std::vector<void(*)(BulletSystem*,int32_t*)> rng_flib;
};