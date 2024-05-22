#ifndef SCR_BASE_IMPLEMENTATION
#define SCR_BASE_IMPLEMENTATION

#include "systems/input_map.h"
#include "systems/bullet_system.h"


struct Base
{
	static inline InputMap gIMap = InputMap();
	static inline BulletSystem gBSys = BulletSystem();
};
// TODO: make part of core somehow... or no core at all?

#endif
