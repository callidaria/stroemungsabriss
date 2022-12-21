#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <dirent.h>

#ifdef __WIN32__

#include <windows.h>
#include <conio.h>

#else

// colour definitions
#define RESET "\033[0m"
#define SELECT "\033[1;32m"

#endif

// root directory definitions
#define ENGINE_ROOT "./ccb"
#define PROJECT_ROOT "./script"

// windows compile predefinition
#ifdef __WIN32__
const std::string path_mingw = "C:/MinGW/lib";
const std::string cmp_winlinker = "-llibSDL2 -lglew32 -lglew32s -lopengl32 -lSOIL -lOpenAL32";
const std::string cmp_windef = "-DGLEW_STATIC -DSDL_MAIN_HANDLED";
#endif

// toolbox
void write_selection();
bool get_selected();
bool get_ftype(const char* file);
std::string get_outfile(const char* file);

// engine features
void offer_root(std::string &dir_path,std::string rt_dir);
std::string read_components(std::string &dir_path,uint8_t proj_idx,bool &comp_all);
std::string build_engine_component(bool &comp_all);
std::string build_project_component(bool &comp_all);
std::string build_full_order(bool &comp_engine,bool &comp_project);
std::string count_lines();

// windows console colour setup
#if __WIN32__
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

// components
char inp;
bool update = false;
uint8_t itr,idx = 0;

int main(int argc,char* argv[])
{
	// setup
	bool comp_engine = false,comp_project = false;
	std::string out,edir = ENGINE_ROOT,pdir = PROJECT_ROOT;
	uint8_t proj_idx;

	// main loop
	while (true) {

		// setup component iterator
		itr = 0;

		// prepare write
#ifdef __WIN32__
		system("cls");
		printf("CASCABEL CONSOLE\n");
#else
		printf("\033[0m\033[2J\033[1;1HCASCABEL CONSOLE\n");
#endif

		// write engine contents
		printf("\n\n\n\t\t\tENGINE\n");
		offer_root(edir,ENGINE_ROOT);
		out += read_components(edir,0,comp_engine);
		printf("\n");
		out += build_engine_component(comp_engine);
		proj_idx = itr;

		// write project contents
#ifdef __WIN32__
		SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("\n\n\n\t\t\tPROJECT\n");
#else
		printf("\n\n\n\t\t\t%sPROJECT\n",RESET);
#endif
		offer_root(pdir,PROJECT_ROOT);
		out += read_components(pdir,proj_idx,comp_project);
		printf("\n");
		out += build_project_component(comp_project);

		// write additional features
		printf("\n\n");
		out += build_full_order(comp_engine,comp_project);
		out += count_lines();

		// write output message
#ifdef __WIN32__
		SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("\n\n\n\n> %s\033[30;47m\n",out.c_str());
		SetConsoleTextAttribute(hConsole,BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		printf("[r] run game  [b] build main  [SPACE] run selected  [RIGHT] open  [c] jump to engine  [p] jump to project  [e] exit\n");
		SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
		printf("\033[0m\n\n\n\n> %s\033[30;47m\n",out.c_str());
		printf("[r] run game  [b] build main  [SPACE] run selected  [RIGHT] open  [c] jump to engine  [p] jump to project  [e] exit\033[0m\n");
#endif

		// read user input
#ifdef __WIN32__
		if (!update) inp = _getch();
#else
		if (!update) { system("stty raw");inp = getchar();system("stty cooked"); }
#endif
		else update = false;

		// kill when exit request
		if (inp=='e') {
#ifdef __WIN32__
			system("cls");
#else
			system("clear");
#endif
			break;
		} else if (inp=='r') system("./yomisensei");
		else if (inp=='b')
#ifdef __WIN32__
			system(("g++ main.cpp lib/* -o yomisensei.exe -L\""+path_mingw+"\" "+cmp_winlinker+' '+cmp_windef).c_str());
#else
			system("g++ main.cpp lib/* -o yomisensei -lGL -lGLEW -lSDL2 -lSDL2_net -lSOIL -lopenal");
#endif
		else if (inp=='c') idx = 0;
		else if (inp=='p') idx = proj_idx;

		// move selector
#ifdef __WIN32__
		idx += (inp=='s'&&idx<itr-1)-(inp=='w'&&idx>0);
#else
		idx += (inp=='B'&&idx<itr-1)-(inp=='A'&&idx>0);
#endif
		out = "";
	}
	return 0;
}

void write_selection()
{
#if __WIN32__
	if (itr==idx) SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
	else SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
	if (itr==idx) printf("%s  ",SELECT);
	else printf("%s",RESET);
#endif
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
			inp = 'x';
		}

		// prepare next
		itr++;
	}
}

std::string read_components(std::string &dir_path,uint8_t proj_idx,bool &comp_all)
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
			else if ((get_selected()&&found->d_type!=DT_DIR&&!update)||(!update&comp_all&&found->d_type!=DT_DIR)) {
				std::string out_file = get_outfile(found->d_name);

#ifdef __WIN32__
				system(("g++ "+dir_path+"/"+found->d_name+" -o lib/"+out_file+" -c -L\""+path_mingw+"\" "+cmp_winlinker+' '+cmp_windef).c_str());
#else
				system(("g++ "+dir_path+"/"+found->d_name+" -o lib/"+out_file+" -c -lGL -lGLEW -lSDL2 -lSDL2_net -lSOIL -lopenal").c_str());
#endif

				out = "compiled "+out_file;
			}

			// compile all sources in directory on demand
			else if ((get_selected()&&found->d_type==DT_DIR&&!update)||(!update&&comp_all&&found->d_type==DT_DIR)) {

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
	} comp_all = false;

	// close directory
	closedir(dir);
	return out;
}

std::string build_engine_component(bool &comp_all)
{
	// draw option
	std::string out = "";
	if (get_selected()) { comp_all = true;update = true;inp = 'x'; }

	// prepare next
	write_selection();
	printf("   BUILD ENGINE");
	itr++;
	return out;
}

std::string build_project_component(bool &comp_all)
{
	// get input
	if (get_selected()) { comp_all = true;update = true;inp = 'x'; }

	// draw option
	std::string out = "";
	write_selection();
	printf("   BUILD PROJECT\n");
	itr++;
	return out;
}

std::string build_full_order(bool &comp_engine,bool &comp_project)
{
	// get input
	if (get_selected()) { comp_engine = true;comp_project = true;update = true;inp = 'x'; }

	// draw option
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
#ifdef __WIN32__
		system("echo feature not available on windows");
#else
		system("find ccb/aud/ ccb/fcn/ ccb/frm/ ccb/gfx/ ccb/mat/ ccb/net/ ccb/ppe/ script/ script/boss/ script/menu/ script/struct/ script/systems script/ui/ shader/ main.cpp ccb.cpp -type f | xargs wc -l | tail -n 1");
#endif
	// TODO: auto find component directories

	// prepare next
	itr++;
	return out;
}