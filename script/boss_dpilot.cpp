#include "boss_dpilot.h"

void BossDPilot::run(Frame* f,Renderer2D* r2d)
{
	// TODO: setup dancing pilot

	bool run = true;
	while (run) {
		f->print_fps();f->input(run,false);f->clear(.1f,.1f,.1f);
		if (f->kb.ka[SDL_SCANCODE_ESCAPE]) break;

		// TODO: run dancing pilot

		f->update();
	}
}
