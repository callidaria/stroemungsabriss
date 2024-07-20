#include "input.h"

/*
	TODO
*/
Input::Input()
{
	text_input_stop();
}

/*
	!O(n)b .input events /update -> (public)
	purpose: process keyboard, controller and mouse input
	\param running: value to negate if the window gets closed
*/
void Input::update(bool& running)
{
	// reset
	mouse.mw = 0;

	// process events and set input activity
	while (SDL_PollEvent(&m_event))
	{
		//bool relevant_motion = false;

		// switch input handling (not my favourite necessarily)
		int32_t motion;
		switch (m_event.type)
		{

		// keyboard input
		case SDL_KEYDOWN: kb.ka[m_event.key.keysym.scancode] = true;
			break;
		case SDL_KEYUP: kb.ka[m_event.key.keysym.scancode] = false;
			break;

		// mouse input
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse.mx,&mouse.my);
			mouse.mxfr = (float)mouse.mx/Config::vFrameResolutionWidth;
			mouse.myfr = (float)(Config::vFrameResolutionHeight-mouse.my)/Config::vFrameResolutionHeight;
			break;
		case SDL_MOUSEBUTTONDOWN: mouse.mb[m_event.button.button-1] = true;
			break;
		case SDL_MOUSEBUTTONUP: mouse.mb[m_event.button.button-1] = false;
			break;
		case SDL_MOUSEWHEEL: mouse.mw = m_event.wheel.y;
			break;

		// controller input
		case SDL_CONTROLLERAXISMOTION:
			motion = SDL_GameControllerGetAxis(m_gc[0],(SDL_GameControllerAxis)m_event.caxis.axis);
			xb[0].xba[m_event.caxis.axis] = motion;
			//relevant_motion = (abs(motion)>Config::iGeneralPeripheralAxisDeadzone)||relevant_motion;
			break;
		case SDL_CONTROLLERBUTTONDOWN: xb[0].xbb[m_event.cbutton.button] = true;
			break;
		case SDL_CONTROLLERBUTTONUP: xb[0].xbb[m_event.cbutton.button] = false;
			break;
		// face buttons have the default xbox layout so for sony it is X=A,O=B,sq=X and delta=Y
		// results in SDL_CONTROLLER_BUTTON_* const for nintendo controllers while a&b is exchanged

		// controller hotswapping
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
			kill_controllers();
			load_controllers();
			break;

		// window close request
		case SDL_QUIT: running = false;
			break;
		}
		// FIXME: switch input refuses to be read. conn ok but no prints

		// update preferred peripheral
		/*
		cpref_peripheral = (cpref_peripheral||m_event.type==SDL_CONTROLLERBUTTONDOWN||relevant_motion)
				&& !(m_event.type==SDL_MOUSEBUTTONDOWN||m_event.type==SDL_KEYDOWN);
		mpref_peripheral =
				(mpref_peripheral&&!cpref_peripheral&&m_event.type!=SDL_KEYDOWN)
				|| m_event.type==SDL_MOUSEMOTION||m_event.type==SDL_MOUSEBUTTONDOWN
				|| m_event.type==SDL_MOUSEWHEEL;
		*/

		/*if (m_event.type==SDL_KEYDOWN&&m_event.key.keysym.sym==SDLK_BACKSPACE&&tline.length()>0)
			tline.pop_back();
		if (m_event.type==SDL_TEXTINPUT) tline += m_event.text.text;*/
		// TODO: make this code happen sensibly within this new input processing implementation
	}
}

/*
	!O(n)m .plugged controllers /function -> (private)
	purpose: load all currently plugged-in controllers
*/
void Input::load_controllers()
{
	// allocate memory
	m_gc.resize(SDL_NumJoysticks()), xb.resize(SDL_NumJoysticks());

	// open peripheral devices
	uint8_t gcc = 0;
	while (SDL_IsGameController(gcc)) m_gc[gcc] = SDL_GameControllerOpen(gcc), gcc++;
}

/*
	!O(n)m .previously opened controllers /function -> (private)
	purpose: close all controllers & remove their input reference structures
*/
void Input::kill_controllers()
{
	for (int i=0;i<m_gc.size();i++) SDL_GameControllerClose(m_gc[i]);
	m_gc.clear(),xb.clear();
}
