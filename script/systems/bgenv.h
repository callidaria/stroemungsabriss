#include <iostream>
#include <string>
#include <vector>

#include "../../ccb/fcn/ccb_manager.h"
#include "../../ccb/gfx/renderer2d.h"

class BGEnv
{
public:
	BGEnv(Renderer2D* r2d) :m_r2d(r2d) {  }
	~BGEnv() {  }
	void load(uint32_t rstate,CCBManager* ccbm);
	void update(uint32_t &rstate);
private:
	static void env_lchild(Renderer2D* r2d,uint32_t &m_idx2d);
	static void env_dpilot(Renderer2D* r2d,uint32_t &m_idx2d);
private:
	Renderer2D* m_r2d;
	uint32_t idx2d;

	std::vector<void(*)(Renderer2D*,uint32_t&)> env_slib = {
		env_lchild,env_dpilot
	};std::vector<std::string> lvpathlib = { "lchild","dpilot" };
};
