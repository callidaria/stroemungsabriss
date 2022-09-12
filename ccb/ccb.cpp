#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <dirent.h>

void read_components(uint8_t &itr,uint8_t &index,std::string &dir_path);

int main(int argc,char* argv[])
{
	// setup and welcome message
	printf("welcome to cascabel console\n");
	std::string command,edir = "./ccb",pdir = "./script";
	uint8_t index = 0;

	// main loop
	while (true) {

		// setup component iterator
		uint8_t itr = 0;

		// read user input
		system("stty raw");
		char inp = getchar();
		system("stty cooked");

		// kill when exit request
		if (inp=='e') break;

		// move selector
		index += (inp=='B')-(inp=='A');

		// write engine contents
		printf("\n\n\n\t\t\tENGINE\n");
		read_components(itr,index,edir);
		printf("\n   BUILD ENGINE");

		// write project contents
		printf("\n\n\n\t\t\tPROJECT\n");
		read_components(itr,index,pdir);
		printf("\n   BUILD PROJECT\n");
		printf("\n\n BUILD ALL COMPONENTS\n");
		printf(" COUNT LINES\n");
		printf("%i\n",index);

		// read user command
		/*std::cin>>command;

		if (command=="exit") break;
		else if (command=="build") system("ccb/ccb_build.sh");
		else if (command=="count_lines")
			system("find ccb/src/ ccb/aud/ ccb/fcn/ ccb/gfx/ ccb/net/ ccb/frm/ ccb/mat/ ccb/ppe/ script/ shader/ main.cpp -type f | xargs wc -l | tail -n 1");
		else printf("command not valid\n");*/
	}
	return 0;
}

void read_components(uint8_t &itr,uint8_t &index,std::string &dir_path)
{
	//open directory
	DIR* dir = opendir(dir_path.c_str());
	struct dirent* found = readdir(dir);

	// grind through contents
	while (found!=NULL) {

		// print current component if not parent or this
		if (found->d_name[0]!='.')
			printf("%s %s\n",(found->d_type==DT_DIR)?"+":"  ->",found->d_name);

		// read next component
		found = readdir(dir);
	}

	// close directory
	closedir(dir);
}