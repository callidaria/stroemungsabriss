#include "level_manager.h"

void LevelManager::run(uint32_t lnr)
{
	m_r2d->reset_shader(); // §§TEST
	switch (lnr) {
	case LNR_ID::LCHILD:lchild.run(&m_pchar,m_frame,m_r2d);break;
	case LNR_ID::DPILOT:dpilot.run(&m_pchar,m_frame,m_r2d);break;
	default:printf("unknown lnr reference\n");
	}
}
