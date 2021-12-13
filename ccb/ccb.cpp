#include <iostream>

int main(int argc,char* argv[])
{
	printf("welcome to cascabel console");
	std::string command;
	while (true) {
		printf("> ");std::cin>>command;
		if (command=="exit") break;
		else if (command=="build") system("ccb/ccb_build.sh");
		else if (command=="count_lines")
			system("find ccb/src/ ccb/aud/ ccb/fcn/ ccb/gfx/ ccb/net/ ccb/frm/ ccb/mat/ ccb/ppe/ script/ shader/ main.cpp -type f | xargs wc -l | tail -n 1");
		else printf("command not valid\n");
	}
	return 0;
}
