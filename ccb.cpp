#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include <stdio.h>
#include <dirent.h>

#ifdef __WIN32__

#include <windows.h>
#include <conio.h>

#else

// colour definitions
#define RESET "\033[0m"
#define SELECT "\033[1;32m"
#define COLOUR_TODO "\033[1;35m"
#define COLOUR_FIXME "\033[1;33m"
#define COLOUR_DEPRECATED "\033[1;34m"

#endif

// root directory definitions
#define ENGINE_ROOT "./ccb"
#define PROJECT_ROOT "./script"

// windows compile predefinition
#ifdef __WIN32__
const std::string path_mingw = "C:/MinGW/lib";
const std::string cmp_winlinker = "-lSDL2 -lSDL2main -lglew32 -lopengl32 -lOpenAL32";
const std::string cmp_windef = "-DGLEW_STATIC -DSDL_MAIN_HANDLED -DSTB_IMAGE_STATIC";
#endif

// toolbox
void write_selection();
bool get_selected();
bool get_ftype(const char* file);
std::string get_outfile(const char* file,uint8_t offs=3);
char get_input_char();
void grind_annotations(const char* path);
void grind_packages(std::string path,std::vector<std::string> &packages);
void grind_includes(std::string file,std::vector<std::string> &out);

// engine features
void offer_root(std::string &dir_path,std::string rt_dir);
std::string read_components(std::string &dir_path,uint8_t proj_idx,bool &comp_all,bool grind_tasks);
std::string build_engine_component(bool &comp_all);
std::string build_project_component(bool &comp_all);
std::string build_full_order(bool &comp_engine,bool &comp_project);
std::string show_annotated_tasks(bool &grind_tasks);
std::string count_lines();

// windows console colour setup
#if __WIN32__
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

// components
char inp;
bool update = false,waiting = false;
uint8_t itr,idx = 0;

int main(int argc,char* argv[])
{
	// setup
	bool comp_engine = false,comp_project = false,grind_tasks = false;
	std::string out,edir = ENGINE_ROOT,pdir = PROJECT_ROOT;
	uint8_t proj_idx;

	// main loop
	while (true) {

		// setup component iterator
		itr = 0;

		// wait until read confirmation
		if (waiting) {
			std::cout << "end of input...\n";
			get_input_char();
		} waiting = false;

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
		out += read_components(edir,0,comp_engine,grind_tasks);
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
		out += read_components(pdir,proj_idx,comp_project,grind_tasks);
		printf("\n");
		out += build_project_component(comp_project);
		grind_tasks = false;

		// write additional features
		printf("\n\n");
		out += build_full_order(comp_engine,comp_project);
		out += show_annotated_tasks(grind_tasks);
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
		if (!update) inp = get_input_char();
		else update = false;

		// kill when exit request
		if (inp=='e') { system("clear");break; }
		else if (inp=='r') { system("./yomisensei");waiting=true; }
		else if (inp=='b') { system("g++ main.cpp lib/* -o yomisensei -lGL -lGLEW -lSDL2 -lSDL2_net -lopenal");waiting=true; }
		else if (inp=='c') idx = 0;
		else if (inp=='p') idx = proj_idx;

		// move selector
#ifdef __WIN32__
		idx += (inp=='s'&&idx<itr-1)-(inp=='w'&&idx>0);
#else
		idx += (inp=='B'&&idx<itr-1)-(inp=='A'&&idx>0);
#endif

		// reset output
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
{ return itr==idx&&inp==' '; }

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

std::string get_outfile(const char* file,uint8_t offs)
{
	std::string out = std::string(file);
	out = out.substr(0,out.length()-offs)+'o';
	return out;
}

char get_input_char()
{
	system("stty raw");
	char out = getchar();
	system("stty cooked");
	return out;
}

void grind_annotations(const char* path)
{
	// print currently grinding path
	printf("annotation results for %s:\n",path);

	// navigate through code files
	std::ifstream gfile(path,std::ios::in);
	std::string gline;
	uint16_t line_number = 0;
	while (getline(gfile,gline)) {

		// recognize tasking comments
		size_t f_todo = gline.find("TODO"),f_fixme = gline.find("FIXME"),f_deprecated = gline.find("DEPRECATED");

		// process todo annotation
		if (f_todo!=std::string::npos) {
			gline.erase(gline.begin(),gline.begin()+f_todo);
			printf("%sline %i => %s%s",COLOUR_TODO,line_number,gline.substr(0,4).c_str(),RESET);
			printf("%s\n",gline.substr(4,gline.length()).c_str());
		}

		// recognize fixme annotation
		else if (f_fixme!=std::string::npos) {
			gline.erase(gline.begin(),gline.begin()+f_fixme);
			printf("%sline %i => %s%s",COLOUR_FIXME,line_number,gline.substr(0,5).c_str(),RESET);
			printf("%s\n",gline.substr(5,gline.length()).c_str());
		}

		// recognize deprecated annotation
		else if (f_deprecated!=std::string::npos) {
			gline.erase(gline.begin(),gline.begin()+f_deprecated);
			printf("%sline %i => %s%s",COLOUR_DEPRECATED,line_number,gline.substr(0,10).c_str(),RESET);
			printf("%s\n",gline.substr(10,gline.length()).c_str());
		}

		// increment line counter
		line_number++;
	} printf("END\n\n");
}
// FIXME: yeah i know, just try to stop me

void grind_packages(std::string path,std::vector<std::string> &packages)
{
	// setup directory grind
	packages.push_back(path);
	DIR* dir = opendir(path.c_str());
	struct dirent* found = readdir(dir);

	// read all packages and files from given root
	while (found!=NULL) {
		if (found->d_type==DT_DIR&&found->d_name[0]!='.') packages.push_back(path+found->d_name+'/');
		found = readdir(dir);
	} closedir(dir);
}

void grind_includes(std::string file,std::vector<std::string> &out)
{
	// setup package grind
	file = file.substr(0,file.length()-3)+'h';
	std::vector<std::string> packages;
	grind_packages("ccb/",packages);
	grind_packages("script/",packages);
	for (auto package : packages) {

		// grind direcories for code files
		DIR* dir = opendir(package.c_str());
		struct dirent* found = readdir(dir);
		while (found!=NULL) {
			std::string cname(found->d_name);
			if (found->d_type!=DT_DIR&&cname.substr(cname.length()-3,cname.length())!="cpp"&&std::find(out.begin(),out.end(),package+found->d_name)==out.end()) {

				// navigate through code files
				std::ifstream gfile(package+found->d_name,std::ios::in);
				std::string gline;
				while (getline(gfile,gline)) {

					// looking for includes and compare
					if (gline.find(file)!=std::string::npos) {
						out.push_back(package+found->d_name);
						break;
					}
				}
			} found = readdir(dir);
		} closedir(dir);
	}
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

std::string read_components(std::string &dir_path,uint8_t proj_idx,bool &comp_all,bool grind_tasks)
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
#ifdef __WIN32__
			bool open_request = inp=='d';
#else
			bool open_request = inp=='C';
#endif
			if (itr==idx&&open_request&&found->d_type==DT_DIR) {
				idx = proj_idx;
				dir_path += "/"+std::string(found->d_name);
				update = true;
			}

			// compile source on demand
			else if ((get_selected()&&found->d_type!=DT_DIR&&!update)||(!update&&comp_all&&found->d_type!=DT_DIR)) {
				std::string out_file = get_outfile(found->d_name);
				system(("g++ "+dir_path+"/"+found->d_name+" -o lib/"+out_file+" -c").c_str());

				// get subsequent classes related to recompiled object
				uint16_t ifile = 0;
				std::vector<std::string> ifiles;
				grind_includes(found->d_name,ifiles);

				// process related files in tree
				while (true) {

					if (ifile>=ifiles.size()) break;

					// process tree information
					std::string sout = get_outfile(ifiles[ifile].substr(ifiles[ifile].find_last_of('/')+1,ifiles[ifile].length()).c_str(),1);
					printf("subsequently compiling %s towards %s\n",ifiles[ifile].c_str(),sout.c_str());

					// get related files to subsequently recompiled module (god help me)
					grind_includes(sout.substr(0,sout.length())+".h",ifiles);

					// compile current file related to main compile target
					system(("g++ "+ifiles[ifile]+" -o lib/"+sout+" -c").c_str());

					ifile++;
				} out = "compiled "+out_file;
			} // FIXME: world.h falls back towards this case. stop world.h from grinding subsequent sources when collectively compiled

			// grind sources at respective root
			else if (!update&&grind_tasks&&found->d_type!=DT_DIR) grind_annotations((dir_path+"/"+found->d_name).c_str());

			// compile all sources in directory on demand
			else if ((get_selected()&&found->d_type==DT_DIR&&!update)||(!update&&comp_all&&found->d_type==DT_DIR)||!update&&grind_tasks&&found->d_type==DT_DIR) {

				// open target directory
				std::string compile_dir = dir_path+"/"+found->d_name;
				DIR* cdir = opendir(compile_dir.c_str());
				struct dirent* cfound = readdir(cdir);

				// grind through compile targets
				while (cfound!=NULL&&comp_all) {

					// filter source components
					if (cfound->d_name[0]!='.'&&get_ftype(cfound->d_name)) {

						// show current compile status
						printf("compiling %s\n",cfound->d_name);

						// compile file
						system(("g++ "+compile_dir+"/"+cfound->d_name+" -o lib/"+get_outfile(cfound->d_name)+" -c").c_str());
					} cfound = readdir(cdir);
				}

				// grind to search annotated tasks
				while (cfound!=NULL&&grind_tasks) {
					if (cfound->d_name[0]!='.'&&get_ftype(cfound->d_name)) grind_annotations((compile_dir+"/"+cfound->d_name).c_str());
					cfound = readdir(cdir);
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
// FIXME: lots of duplicate code & unsorted checks

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
	if (get_selected()) {
		comp_engine = true;comp_project = true;update = true;inp = 'x';
		system("rm ./lib/*");
	}

	// draw option
	std::string out = "";
	write_selection();
	printf(" BUILD ALL COMPONENTS\n");
	itr++;
	return out;
}

std::string show_annotated_tasks(bool &grind_tasks)
{
	// get input
	if (get_selected()) { grind_tasks = true;update = true;inp = 'x'; }

	// draw option
	std::string out = "";
	write_selection();
	printf(" SHOW TASK ANNOTATIONS\n");
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
	if (get_selected()) {
#ifdef __WIN32__
		system("echo feature not available on windows");
#else
		/*std::string command = "find ";
		std::vector<std::string> package_paths;
		grind_packages("ccb/",package_paths);
		grind_packages("script/",package_paths);
		grind_packages("shader/",package_paths);
		for (auto package : package_paths) {
			std::cout << package << "\n->";
			system(("find "+package+" -type f | xargs wc -l | tail -n 1").c_str());
			command += package+' ';
		}*/
		system("find ccb/ script/ shader/ main.cpp ccb.cpp -type f | xargs wc -l | tail -n 1");
#endif
	}

	// prepare next
	itr++;
	return out;
}