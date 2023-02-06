#include "game.h"

Game::Game(Frame* f,Renderer2D* r2d,Renderer3D* r3d,RendererI* rI,Camera2D* cam2d)
	: m_frame(f),m_r2d(r2d),m_r3d(r3d),m_rI(rI),m_cam2d(cam2d)
{ ccbf = { m_frame,m_r2d,m_r3d,m_rI,&m_bSys,&m_player }; }

void Game::run(uint32_t &rstate,CCBManager* ccbm)
{
	m_bgenv.load(rstate,ccbm);

	// TODO: reduce
	uint32_t stg_idx2d;
	int32_t fwd_treg[16] = { 0 };
	glm::vec2 ePos = glm::vec2(615,600);
	stg_ld.at(rstate)(&ccbf,stg_idx2d,fwd_treg);

	// card game test
	std::vector<Currency> curr_path;
	curr_path.push_back({
		"./res/coin.obj","./res/coin_tex.png","./res/none.png",
		"./res/dnormal.png","./res/none.png",5
	});curr_path.push_back({
		"./res/coin.obj","./res/coin10_tex.png","./res/none.png",
		"./res/dnormal.png","./res/none.png",10
	});
	CardSystem crdSystem = CardSystem(m_frame,m_r2d,m_r3d,curr_path);
	crdSystem.create_player(glm::vec2(0,-10),180,1000);
	crdSystem.create_player(glm::vec2(-10,0),90,1000);
	crdSystem.create_player(glm::vec2(10,0),-90,1000);
	crdSystem.create_pile(glm::vec2(0,0));
	crdSystem.register_auto_deal(0,0,7);
	crdSystem.register_auto_deal(0,1,7);
	crdSystem.register_auto_deal(0,2,7);
	crdSystem.register_auto_deal(0,3,7);
	crdSystem.create_currency_stack(glm::vec2(4,3),0);
	crdSystem.create_currency_stack(glm::vec2(1,-3),180);
	crdSystem.create_currency_stack(glm::vec2(-3,0),90);

	// vertex & texture load
	Camera3D cam3d = Camera3D(1280.0f,720.0f);
	m_r2d->load(m_cam2d);m_rI->load(m_cam2d);m_r3d->load(&cam3d);

	// ortho 3D element lighting
	Light3D l3d_ortho = Light3D(m_r3d,0,glm::vec3(640,360,10000),glm::vec3(1,1,1),1);
	l3d_ortho.set_amnt(1);
	l3d_ortho.upload();

	// ui
	Healthbar hbar = Healthbar(glm::vec2(440,690),790,15,{ 3,4 },
			{ 10000,5000,10000,10000,5000,5000,10000 },"The Dancing Pilot");

	// lightweight action menu
	ActionMenu lgt_menu = ActionMenu(m_frame);

	// update until exit condition
	uint32_t running = rstate+1;
	bool trg_deal = false;
	while (running) {  // ??maybe kill check if flush with static func ref

		// frame
		m_frame->print_fps();
		m_frame->calc_time_delta();
		m_frame->input(running,false);
		m_frame->clear(.1f,.1f,.1f);

		// action menu update
		lgt_menu.update(&m_player,running);
		lgt_menu.bind();

		// stage
		/*m_bgenv.update(rstate);
		stg_upd.at(rstate)(&ccbf,stg_idx2d,ePos,fwd_treg);

		// player
		m_player.update(rstate,fwd_treg[11]);

		// bullet system
		m_bSys.render();*/

		// draw card game test
		if (m_frame->kb.ka[SDL_SCANCODE_J]&&!trg_deal) crdSystem.deal_card(0);
		else if (m_frame->kb.ka[SDL_SCANCODE_H]&&!trg_deal) crdSystem.hand_to_pile(1);
		else if (m_frame->kb.ka[SDL_SCANCODE_G]&&!trg_deal) crdSystem.deal_card(0,0);
		else if (m_frame->kb.ka[SDL_SCANCODE_U]&&!trg_deal) crdSystem.deal_card(0,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_I]&&!trg_deal) crdSystem.deal_card(0,2);
		else if (m_frame->kb.ka[SDL_SCANCODE_E]&&!trg_deal) crdSystem.opponent_to_pile(0,1,0);
		else if (m_frame->kb.ka[SDL_SCANCODE_R]&&!trg_deal) crdSystem.opponent_to_pile(1,1,0);
		else if (m_frame->kb.ka[SDL_SCANCODE_T]&&!trg_deal) crdSystem.opponent_to_pile(2,1,0);
		else if (m_frame->kb.ka[SDL_SCANCODE_Q]&&!trg_deal) crdSystem.register_auto_deal(1,0,4);
		else if (m_frame->kb.ka[SDL_SCANCODE_1]&&!trg_deal) crdSystem.add_currency(0,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_2]&&!trg_deal) crdSystem.add_currency(1,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_3]&&!trg_deal) crdSystem.move_currency(0,0,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_4]&&!trg_deal) crdSystem.move_currency(1,0,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_5]&&!trg_deal) crdSystem.add_currency(0,0,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_6]&&!trg_deal) crdSystem.add_currency(1,0,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_7]&&!trg_deal) crdSystem.move_currency(0,0,1,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_8]&&!trg_deal) crdSystem.move_currency(1,0,1,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_9]&&!trg_deal) crdSystem.add_currency(0,1,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_0]&&!trg_deal) crdSystem.add_currency(1,1,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_N]&&!trg_deal) crdSystem.move_currency(0,1,2,1);
		else if (m_frame->kb.ka[SDL_SCANCODE_M]&&!trg_deal) crdSystem.move_currency(1,1,2,1);
		trg_deal = m_frame->kb.ka[SDL_SCANCODE_J] || m_frame->kb.ka[SDL_SCANCODE_H]
				|| m_frame->kb.ka[SDL_SCANCODE_G] || m_frame->kb.ka[SDL_SCANCODE_U]
				|| m_frame->kb.ka[SDL_SCANCODE_I] || m_frame->kb.ka[SDL_SCANCODE_E]
				|| m_frame->kb.ka[SDL_SCANCODE_R] || m_frame->kb.ka[SDL_SCANCODE_T]
				|| m_frame->kb.ka[SDL_SCANCODE_Q] || m_frame->kb.ka[SDL_SCANCODE_1]
				|| m_frame->kb.ka[SDL_SCANCODE_2] || m_frame->kb.ka[SDL_SCANCODE_3]
				|| m_frame->kb.ka[SDL_SCANCODE_4] || m_frame->kb.ka[SDL_SCANCODE_5]
				|| m_frame->kb.ka[SDL_SCANCODE_6] || m_frame->kb.ka[SDL_SCANCODE_9]
				|| m_frame->kb.ka[SDL_SCANCODE_0] || m_frame->kb.ka[SDL_SCANCODE_7]
				|| m_frame->kb.ka[SDL_SCANCODE_8] || m_frame->kb.ka[SDL_SCANCODE_N]
				|| m_frame->kb.ka[SDL_SCANCODE_M];
		crdSystem.process_input();
		crdSystem.render();

		// healthbar
		hbar.register_damage(fwd_treg[10]);
		hbar.render();

		// action menu render
		lgt_menu.render();

		// swap
		m_frame->update();
	} rstate = 0;  // TODO: choose between menu &|| desktop skip
}