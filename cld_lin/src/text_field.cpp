#include "../fcn/text_field.h"

TextField::TextField(Font* fnt,Renderer2D* r2d,glm::vec2 pos,const char* bp,glm::vec2 bpos,float width,float height)
	: m_r2d(r2d),m_pos(pos),m_bpos(bpos),m_width(width),m_height(height)
{
	rinst = m_r2d->add(bpos,width,height,bp,3,1,30,0);
	txt = Text(fnt);
}
void TextField::render(Frame* frame,Camera2D* cam2d,glm::vec4 col)
{
	bool hover = false;
	if (frame->mouse.mxfr>=m_bpos.x&&frame->mouse.mxfr<=m_bpos.x+m_width
		&&frame->mouse.myfr>=m_bpos.y&&frame->mouse.myfr<=m_bpos.y+m_height) {
		if (frame->mouse.mcl) input_active = true;
		hover = true;
	} else { if (frame->mouse.mcl) input_active = false; }
	if (frame->event_active&&input_active) { // !!evil branching ...also avoid frame pointer here
		for (int i=0;i<285;i++) { // ??maybe not iterate through all ascii instances for the alphabet
			if (frame->m_fe.type==SDL_KEYDOWN&&frame->m_fe.key.keysym.scancode==i) {
				char c = (char)93+i; // breakdown to fatales and make instance for special input
				m_pos.x += txt.add(c,m_pos);
				cnt += c;
				char_amount++;
			}
		} txt.load_wcam(cam2d);
	} // !!write input prefix for activating

	m_r2d->prepare();
	m_r2d->render_state(rinst,glm::vec2(0,0+input_active+hover));
	txt.prepare();
	txt.render(char_amount,col);
}
