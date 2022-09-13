#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <dirent.h>

#define RESET "\033[0m"
#define SELECT "\033[1;32m"

#define ENGINE_ROOT "./ccb"
#define PROJECT_ROOT "./script"

// toolbox
void write_selection();
bool get_selected();
bool get_ftype(const char* file);
std::string get_outfile(const char* file);

// engine features
void offer_root(std::string &dir_path,std::string rt_dir);
std::string read_components(std::string &dir_path,uint8_t proj_idx);
std::string build_engine_component();
std::string build_project_component();
std::string build_full_order();
std::string count_lines();

// components
char inp;
bool update = false;
uint8_t itr,idx = 0;

int main(int argc,char* argv[])
{
	// setup
	std::string out,edir = ENGINE_ROOT,pdir = PROJECT_ROOT;
	uint8_t proj_idx;

	// main loop
	while (true) {

		// setup component iterator
		itr = 0;

		// prepare write
		printf("\033[0m\033[2J\033[1;1HCASCABEL CONSOLE\n");

		// write engine contents
		printf("\n\n\n\t\t\tENGINE\n");
		offer_root(edir,ENGINE_ROOT);
		out += read_components(edir,0);
		printf("\n");
		out += build_engine_component();
		proj_idx = itr;

		// write project contents
		printf("\n\n\n\t\t\t%sPROJECT\n",RESET);
		offer_root(pdir,PROJECT_ROOT);
		out += read_components(pdir,proj_idx);
		printf("\n");
		out += build_project_component();

		// write additional features
		printf("\n\n");
		out += build_full_order();
		out += count_lines();

		// write output message
		printf("\033[0m\n\n\n\n> %s\033[30;47m\n",out.c_str());
		printf("[r] run game  [b] build main  [SPACE] run selected  [RIGHT] open  [c] jump to engine  [p] jump to project  [e] exit\033[0m\n");

		// read user input
		if (!update) {
			system("stty raw");
			inp = getchar();
			system("stty cooked");
		} else update = false;

		// kill when exit request
		if (inp=='e') { system("clear");break; }
		else if (inp=='r') system("./yomisensei");
		else if (inp=='b') system("g++ main.cpp lib/* -o yomisensei -lGL -lGLEW -lSDL2 -lSDL2_net -lSOIL -lopenal");
		else if (inp=='c') idx = 0;
		else if (inp=='p') idx = proj_idx;

		// move selector
		idx += (inp=='B'&&idx<itr-1)-(inp=='A'&&idx>0);
		out = "";
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

bool get_ftype(const char* file)
{
	// convert to string
	std::string tfile = std::string(file);

	// find file extension
	bool reached_ext = false;
	std::string fext = "";
	for (char cfl : tfile) {
		if (reached_ext) fext += cfl;
		else if (cfl=='.') reached_ext = true;
	}

	// check for source file
	return fext=="cpp"||!reached_ext;
}

std::string get_outfile(const char* file)
{
	std::string out = std::string(file);
	out = out.substr(0,out.length()-3)+'o';
	return out;
}

void offer_root(std::string &dir_path,std::string rt_dir)
{
	if (dir_path!=rt_dir) {

		// render offer
		write_selection();
		printf(" ROOT\n");

		// check root request
		if (get_selected()) {
			dir_path = rt_dir;
			update = true;
		}

		// prepare next
		itr++;
	}
}

std::string read_components(std::string &dir_path,uint8_t proj_idx)
{
	//open directory
	std::string out = "";
	DIR* dir = opendir(dir_path.c_str());
	struct dirent* found = readdir(dir);

	// grind through contents
	while (found!=NULL) {

		// print current component if not parent or this
		if (found->d_name[0]!='.'&&get_ftype(found->d_name)) {

			// open directory on demand
			if (itr==idx&&inp=='C'&&found->d_type==DT_DIR) {
				idx = proj_idx;
				dir_path += "/"+std::string(found->d_name);
				update = true;
			}

			// compile source on demand
			else if (get_selected()&&found->d_type!=DT_DIR&&!update) {
				std::string out_file = get_outfile(found->d_name);
				system(("g++ "+dir_path+"/"+found->d_name+" -o lib/"+out_file+" -c -lGL -lGLEW -lSDL2 -lSDL2_net -lSOIL -lopenal").c_str());
				out = "compiled "+out_file;
			}

			// compile all sources in directory on demand
			else if (get_selected()&&found->d_type==DT_DIR&&!update) {

				// open target directory
				std::string compile_dir = dir_path+"/"+found->d_name;
				DIR* cdir = opendir(compile_dir.c_str());
				struct dirent* cfound = readdir(cdir);

				// grind through compile targets
				while (cfound!=NULL) {

					// filter source components
					if (cfound->d_name[0]!='.'&&get_ftype(cfound->d_name)) {

						// show current compile status
						printf("compiling %s\n",cfound->d_name);

						// compile file
						system(("g++ "+compile_dir+"/"+cfound->d_name+" -o lib/"+get_outfile(cfound->d_name)+" -c -lGL -lGLEW -lSDL2 -lSDL2_net -lSOIL -lopenal").c_str());
					} cfound = readdir(cdir);
				}
			}

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
	if (get_selected())
		system("find ccb/src/ ccb/aud/ ccb/fcn/ ccb/gfx/ ccb/net/ ccb/frm/ ccb/mat/ ccb/ppe/ script/ script/ui/ shader/ main.cpp -type f | xargs wc -l | tail -n 1");

	// prepare next
	itr++;
	return out;
}