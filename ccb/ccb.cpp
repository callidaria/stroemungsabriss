#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <dirent.h>

#define RESET "\033[0m"
#define SELECT "\033[1;32m"

void write_selection(uint8_t itr,uint8_t index);
std::string read_components(uint8_t &itr,uint8_t &index,std::string &dir_path);
std::string build_engine_component(uint8_t &itr,uint8_t &index);
std::string build_project_component(uint8_t &itr,uint8_t &index);
std::string build_full_order(uint8_t &itr,uint8_t &index);
std::string count_lines(uint8_t &itr,uint8_t &index);

char inp;

int main(int argc,char* argv[])
{
	// setup and welcome message
	printf("welcome to cascabel console\n");
	std::string out,edir = "./ccb",pdir = "./script";
	uint8_t index = 0;

	// main loop
	while (true) {

		// setup component iterator
		uint8_t itr = 0;

		// write engine contents
		printf("\n\n\n\t\t\tENGINE\n");
		out += read_components(itr,index,edir);
		printf("\n");
		out += build_engine_component(itr,index);

		// write project contents
		printf("\n\n\n\t\t\t%sPROJECT\n",RESET);
		out += read_components(itr,index,pdir);
		printf("\n");
		out += build_project_component(itr,index);

		// write additional features
		printf("\n\n");
		out += build_full_order(itr,index);
		out += count_lines(itr,index);

		// write output message
		printf("\033[0m\n\n\n\n> %s\n",out.c_str());
		printf("\033[30;47m[c] jump to engine    [p] jump to project    [e] exit\033[0m\n");

		// read user input
		system("stty raw");
		inp = getchar();
		system("stty cooked");

		// kill when exit request
		if (inp=='e') break;

		// move selector
		index += (inp=='B'&&index<itr-1)-(inp=='A'&&index>0);

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

void write_selection(uint8_t itr,uint8_t index)
{
	if (itr==index) printf("%s  ",SELECT);
	else printf("%s",RESET);
}

std::string read_components(uint8_t &itr,uint8_t &index,std::string &dir_path)
{
	//open directory
	std::string out = "";
	DIR* dir = opendir(dir_path.c_str());
	struct dirent* found = readdir(dir);

	// grind through contents
	while (found!=NULL) {

		// print current component if not parent or this
		if (found->d_name[0]!='.') {

			// print selection
			write_selection(itr,index);

			// show component
			printf("%s %s\n",(found->d_type==DT_DIR)?"+":"  ->",found->d_name);
			itr++;
		}

		// read next component
		found = readdir(dir);
	}

	// close directory
	closedir(dir);
	return out;
}

std::string build_engine_component(uint8_t &itr,uint8_t &index)
{
	std::string out = "";
	write_selection(itr,index);
	printf("   BUILD ENGINE");
	itr++;
	return out;
}

std::string build_project_component(uint8_t &itr,uint8_t &index)
{
	std::string out = "";
	write_selection(itr,index);
	printf("   BUILD PROJECT\n");
	itr++;
	return out;
}

std::string build_full_order(uint8_t &itr,uint8_t &index)
{
	std::string out = "";
	write_selection(itr,index);
	printf(" BUILD ALL COMPONENTS\n");
	itr++;
	return out;
}

std::string count_lines(uint8_t &itr,uint8_t &index)
{
	// draw option
	std::string out = "";
	write_selection(itr,index);
	printf(" COUNT LINES\n");

	// run in chosen
	if (itr==index&&inp==' ') system("find ccb/src/ ccb/aud/ ccb/fcn/ ccb/gfx/ ccb/net/ ccb/frm/ ccb/mat/ ccb/ppe/ script/ shader/ main.cpp -type f | xargs wc -l | tail -n 1");

	// prepare next
	itr++;
	return out;
}