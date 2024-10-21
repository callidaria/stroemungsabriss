#include "../../ccb/mat/math.h"
#include "../../ccb/gfx/renderer.h"


// information
constexpr uint8_t INFO_VERSION_RELEASE = 0;
constexpr uint8_t INFO_VERSION_SUBRELEASE = 0;
constexpr uint8_t INFO_VERSION_DEVSTEP = 6;
constexpr char INFO_VERSION_MODE_SUFFIX = 'c';
// MODI: c = "development", t = "QA build", p = "polishing", R = "release"


class MainMenu
{
public:

	MainMenu();
	~MainMenu() {  }

	void update();

private:

	void update_peripheral_annotations();

private:

	// text
	uint16_t m_DareMessage;
};
