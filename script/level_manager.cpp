#include "level_manager.h"

void LevelManager::run(uint32_t lnr)
{
	switch (lnr) {
	case 0:lchild.run(m_frame,m_r2d);break;
	case 1:dpilot.run(m_frame,m_r2d);break;
	default:printf("unknown lnr reference\n");
	}
}
