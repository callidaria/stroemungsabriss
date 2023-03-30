#include "casino_spike.h"

/*
	TODO
*/
CasinoSpike::CasinoSpike(CascabelBaseFeature* ccbf)
	: m_ccbf(ccbf)
{
	index_r3D = m_ccbf->r3d->add("./res/test_floor.obj","./res/floor_colour.jpg",
			"./res/floor_spec.jpg","./res/floor_normal.jpg","./res/none.png",glm::vec3(0),
			1,glm::vec3());
}

/*
	TODO
*/
void CasinoSpike::render()
{
	m_ccbf->r3d->prepare();
	m_ccbf->r3d->render_mesh(index_r3D,index_r3D+1);
}