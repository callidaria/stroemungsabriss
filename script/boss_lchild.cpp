#include "boss_lchild.h"

void BossLChild::run(Player* pchar,Frame* f,Renderer2D* r2d)
{
	// TODO: setup lost child

	bool run = true;
	while (run) {
		//f->print_fps();f->input(run,false);f->clear(.1f,.1f,.1f);
		if (f->kb.ka[SDL_SCANCODE_ESCAPE]) break;

		// TODO: run lost child

		f->update();
	}
}
