#include <iostream>

int main(int argc,char* argv[])
{
	printf("welcome to cascabel console");
	std::string command;
	while (true) {
		printf("> ");std::cin>>command;
		if (command=="exit") break;
		else if (command=="build") system("ccb/ccb_build.sh");
		else printf("command not valid\n");
	}
	return 0;
}
