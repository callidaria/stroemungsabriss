#include "../../ccb/mat/math.h"

class UI
{
public:

	// construction
	UI() {  }
	~UI() {  }

	void update();

private:

	MainMenu m_MainMenu;
	ActionMenu m_ActionMenu;
};
