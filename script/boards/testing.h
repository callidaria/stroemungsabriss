#ifndef BRD_TESTAREA
#define BRD_TESTAREA

#include <iostream>
#include "../struct/world_structures.h"

class TestArea  // : public Board<TestArea>
{
public:

	// construction
	TestArea() {  }

	// implementation
	void load();
	void update();
};

#endif
