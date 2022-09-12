#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <dirent.h>

#define RESET "\033[0m"
#define SELECT "\033[1;32m"

// toolbox
void write_selection();
bool get_selected();

// engine features
std::string read_components(std::string &dir_path);
std::string build_engine_component();
std::string build_project_component();
std::string build_full_order();
std::string count_lines();

// components
char inp;
uint8_t itr,idx = 0;

int main(int argc,char* argv[])
{
	// setup and welcome message
	printf("welcome to cascabel console\n");
	std::string out,edir = "./ccb",pdir = "./script";

	// main loop
	while (true) {

		// setup component iterator
		itr = 0;

		// write engine contents
		printf("\n\n\n\t\t\tENGINE\n");
		out += read_components(edir);
		printf("\n");
		out += build_engine_component();

		// write project contents
		printf("\n\n\n\t\t\t%sPROJECT\n",RESET);
		out += read_components(pdir);
		printf("\n");
		out += build_project_component();

		// write additional features
		printf("\n\n");
		out += build_full_order();
		out += count_lines();

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
		idx += (inp=='B'&&idx<itr-1)-(inp=='A'&&idx>0);

		/*else if (command=="build") system("ccb/ccb_build.sh");
		else printf("command not valid\n");*/
	}
	return 0;
}

void write_selection()
{
	if (itr==idx) printf("%s  ",SELECT);
	else printf("%s",RESET);
}

bool get_selected()
{
	return itr==idx&&inp==' ';
}

std::string read_components(std::string &dir_path)
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
			write_selection();

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

std::string build_engine_component()
{
	// draw option
	std::string out = "";
	write_selection();
	printf("   BUILD ENGINE");

	// run in chosen
	if (get_selected()) {
		system("ccb/ccb_build.sh");
		out = "all engine components built";
	}

	// prepare next
	itr++;
	return out;
}

std::string build_project_component()
{
	std::string out = "";
	write_selection();
	printf("   BUILD PROJECT\n");
	itr++;
	return out;
}

std::string build_full_order()
{
	std::string out = "";
	write_selection();
	printf(" BUILD ALL COMPONENTS\n");
	itr++;
	return out;
}

std::string count_lines()
{
	// draw option
	std::string out = "";
	write_selection();
	printf(" COUNT LINES\n");

	// run in chosen
	if (get_selected()) system("find ccb/src/ ccb/aud/ ccb/fcn/ ccb/gfx/ ccb/net/ ccb/frm/ ccb/mat/ ccb/ppe/ script/ shader/ main.cpp -type f | xargs wc -l | tail -n 1");

	// prepare next
	itr++;
	return out;
}