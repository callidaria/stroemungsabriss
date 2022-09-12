#include <iostream>
#include <vector>
#include <dirent.h>
#include <stdio.h>

int main(int argc,char* argv[])
{
	// setup and welcome message
	printf("welcome to cascabel console\n");
	std::string command;
	uint8_t index = 0;

	// main loop
	while (true) {

		// write engine contents
		printf("\n\n\n\t\t\tENGINE\n");
		DIR* edir = opendir("./ccb");
		struct dirent* found = readdir(edir);
		while (found!=NULL) {
			if (found->d_name[0]!='.') printf("%s %s\n",(found->d_type==DT_DIR)?"+":"  ->",found->d_name);
			found = readdir(edir);
		} closedir(edir);
		printf("\n   BUILD ENGINE");

		// write project contents
		printf("\n\n\n\t\t\tPROJECT\n");
		DIR* pdir = opendir("./script");
		found = readdir(pdir);
		while (found!=NULL) {
			if (found->d_name[0]!='.') printf("%s %s\n",(found->d_type==DT_DIR)?"+":"  ->",found->d_name);
			found = readdir(pdir);
		} closedir(pdir);
		printf("\n   BUILD PROJECT\n");
		printf("\n\n BUILD ALL COMPONENTS\n");
		printf(" COUNT LINES\n");

		// read user command
		std::cin>>command;

		if (command=="exit") break;
		else if (command=="build") system("ccb/ccb_build.sh");
		else if (command=="count_lines")
			system("find ccb/src/ ccb/aud/ ccb/fcn/ ccb/gfx/ ccb/net/ ccb/frm/ ccb/mat/ ccb/ppe/ script/ shader/ main.cpp -type f | xargs wc -l | tail -n 1");
		else printf("command not valid\n");
	}
	return 0;
}