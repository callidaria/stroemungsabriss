#ifndef SCR_PCHARS_JAEGER_JET
#define SCR_PCHARS_JAEGER_JET

#include "../../ccb/core.h"

#include "../world.h"

// movement collision bounds for the jaeger jet
constexpr uint8_t JET_BORDER_WIDTH = 42;
constexpr uint8_t JET_BORDER_HEIGHT = 18;

class JaegerJet : public Player
{
public:

	// contruction
	JaegerJet();
	~JaegerJet() {  }

	// draw
	virtual void update();

private:

	// render information
	uint16_t index_r2D,index_r3D;

	// system
	int32_t treg[3] = { 0,0,0 };
	int8_t ddur = 0;

	// animation
	float tilt = .0f;
};

#endif
