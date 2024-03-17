#include <iostream>
#include <fstream>
#include <sstream>
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
constexpr std::string path_mingw = "C:/MinGW/lib";
constexpr std::string cmp_winlinker = "-lSDL2 -lSDL2main -lglew32 -lopengl32 -lOpenAL32 -lpthread -pg -O3";
constexpr std::string cmp_windef = "-DGLEW_STATIC -DSDL_MAIN_HANDLED -DSTB_IMAGE_STATIC";
#endif
std::string cmp_linking = "-lGL -lGLEW -lSDL2 -lopenal -lpthread -lassimp";
std::string cmp_extras = "-std=c++17 -pg -g -O3 -fno-gcse";

// toolbox
void write_selection();
bool get_selected();
bool get_ftype(const char* file);
std::string get_outfile(const char* file,uint8_t offs=3);
char get_input_char();
void grind_annotations(const char* path);
void grind_packages(std::string path,std::vector<std::string> &packages);
void grind_includes(std::string file,std::vector<std::string> &out);
void assembly_analysis_mode(std::string path);

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
		printf("[r] run game  [b] build main  [SPACE] run selected  [h] include recursion  [a] assembly analysis  [RIGHT] open  [c] jump to engine  [p] jump to project  [e] exit\n");
		SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
		printf("\033[0m\n\n\n\n> %s\033[30;47m\n",out.c_str());
		printf("[r] run game  [b] build main  [SPACE] run selected  [h] include recursion  [a] assembly analysis  [RIGHT] open  [c] jump to engine  [p] jump to project  [e] exit\033[0m\n");
#endif

		// read user input
		if (!update) inp = get_input_char();
		else update = false;

		// kill when exit request
		if (inp=='e') { system("clear");break; }
		else if (inp=='r') { system("./yomisensei");waiting=true; }
		else if (inp=='b') { system(("g++ main.cpp lib/* -o yomisensei "+cmp_linking+" "+cmp_extras).c_str());waiting=true; }
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
{ return (itr==idx&&inp==' ')||(itr==idx&&inp=='h'); }

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

void assembly_analysis_mode(std::string path)
{
	// extract function implementations
	std::ifstream file(path,std::ios::in);
	std::string cline;
	std::vector<std::string> func_names,func_codes;
	std::string pfunc_name = "",pfunc_code = "",ptrunc_code = "";
	uint8_t indent = 0;
	while (getline(file,cline)) {

		// exclude compiler macros
		if (cline[0]=='#') continue;

		// content analysis of line
		for (auto cc : cline) {
			indent -= cc=='}';

			// read method name & classifications
			if (cc=='{'&&indent==0) {

				// navigate trunc code until method definition
				uint16_t i = ptrunc_code.length()-1,pivot = 0;
				bool bracket = false;
				uint8_t scount = 2;
				while (scount||bracket) {
					if (ptrunc_code[i]==')') { bracket = true;pivot = i+1; }
					else if (ptrunc_code[i]=='(') { bracket = false;scount = 2; }
					scount -= ptrunc_code[i]==':';
					i--;
				} i += 3;

				// write function description
				while (i<pivot) {
					if (ptrunc_code[i]!='\t'&&ptrunc_code[i]!='\n') pfunc_name += ptrunc_code[i];
					i++;
				}

			// save method implementation & reset function code
			} else if (cc=='}'&&!indent) {
				func_names.push_back(pfunc_name),func_codes.push_back("#include \""+path.substr(0,path.size()-3)+"h\"\n\n"+ptrunc_code+pfunc_code+'}');
				pfunc_name = "",pfunc_code = "",ptrunc_code = "";

			// execute copy mode
			} if (indent) pfunc_code += cc;
			else if (!indent&&cc!='}') ptrunc_code += cc;
			indent += cc=='{';
		}

		// write newlines for aesthetics
		if (indent) pfunc_code += '\n';
		else ptrunc_code += '\n';
	} file.close();

	// show functions & selection
	bool asm_cnf = false;
	uint16_t asm_idx = 0;
	char asm_inp = 'x';
	while (true) {

		// process print
		printf("\033[0m\033[2J\033[1;1H ASSEMBLY MODE\n\n");
		for (uint16_t i=0;i<func_names.size();i++) {
			if (asm_idx==i) printf("%s",SELECT);
			printf("-> %s%s\n",func_names[i].c_str(),RESET);
		}
		if (asm_idx==func_names.size()) printf("%s",SELECT);
		printf("\nCOMPLETE ASSEMBLY%s\n",RESET);
		if (asm_idx==func_names.size()+1) printf("%s",SELECT);
		printf("[Q]UIT%s",RESET);

		// process input
		asm_inp = get_input_char();
		asm_cnf = asm_inp==' ';
		if (asm_inp=='q'||asm_inp=='Q'||asm_cnf) break;
		asm_idx += (asm_inp=='B'&&asm_idx<func_names.size()+1)-(asm_inp=='A'&&asm_idx>0);
	}

	// show code snippet before assembly when confirmation has been given
	if (asm_cnf) {
		printf("\033[0m\033[2J\033[1;1HC++:\n\n%s\nEND\n\n\n",func_codes[asm_idx].c_str());
		std::ofstream tmp_cmp("tmp_cmp.cpp");
		tmp_cmp << func_codes[asm_idx];
		tmp_cmp.close();
		printf("proceed to compiled assembly by pressing any key...\n");
		get_input_char();

		// translate to assembly & prepare analysis output
		system(("g++ tmp_cmp.cpp "+cmp_extras+" -S -fverbose-asm").c_str());
		std::ifstream tmp_asm("tmp_cmp.s");
		std::string asmline;
		std::vector<std::string> clasms;
		bool dumpstate = true;
		std::string rfname,csnip,linebuf,llinebuf;
		printf("ASSEMBLY:\n\n");
		while (getline(tmp_asm,asmline)) {

			// check for annotation lines & external instructions
			if (asmline[0]=='#') {

				// write relevant lines & dump includes
				if (dumpstate) clasms.clear();
				else if (llinebuf!=linebuf) printf("%s%s\n%s",COLOUR_DEPRECATED,csnip.c_str(),RESET);
				if (!dumpstate) {
					for (auto al : clasms) printf("%s\n",al.c_str());
					printf("\n");
					llinebuf = linebuf;
					clasms.clear();
				}

				// split up annotation string
				std::istringstream asmant(asmline);
				getline(asmant,rfname,':'),getline(asmant,linebuf,':'),getline(asmant,csnip);
				// FIXME: ??use sscanf instead??

				// dumpstate
				dumpstate = rfname!="# tmp_cmp.cpp";
			} else clasms.push_back(asmline);
		} for (auto al : clasms) printf("%s\n",al.c_str());
		tmp_asm.close();
		printf("press any key to continue...\n");
		get_input_char();
		system("rm tmp_cmp.cpp tmp_cmp.s");
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
				system(("g++ "+dir_path+"/"+found->d_name+" -o lib/"+out_file+" "+cmp_extras+" -c").c_str());

				// get subsequent classes related to recompiled object
				uint16_t ifile = 0;
				std::vector<std::string> ifiles;
				std::string coname(found->d_name);
				grind_includes(coname.substr(0,coname.length()-3)+'h',ifiles);

				// process related files in tree & jitterly turn off directory search for complete compile
				// FIXME: check proceedings earlier to save time
				while (!comp_all&&inp=='h') {
					if (ifile>=ifiles.size()) break;

					// process tree information
					std::string sout = get_outfile(ifiles[ifile].substr(ifiles[ifile].find_last_of('/')+1,ifiles[ifile].length()).c_str(),1);
					printf("subsequently compiling %s towards %s\n",ifiles[ifile].c_str(),sout.c_str());

					// get related files to subsequently recompiled module (god help me)
					std::cout << sout.substr(0,sout.length()-1)+'h' << '\n';
					grind_includes(sout.substr(0,sout.length()-1)+'h',ifiles);

					// compile current file related to main compile target
					system(("g++ "+ifiles[ifile].substr(0,ifiles[ifile].length()-1)+"cpp -o lib/"+sout+" "+cmp_extras+" -c").c_str());

					ifile++;
				} out = "compiled "+out_file;

			// run compile to assembly request
			} else if (inp=='a'&&found->d_type!=DT_DIR&&itr==idx) assembly_analysis_mode((dir_path+"/"+std::string(found->d_name)).c_str());

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
						system(("g++ "+compile_dir+"/"+cfound->d_name+" -o lib/"+get_outfile(cfound->d_name)+" "+cmp_extras+" -c").c_str());
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
		system("find ccb/ script/ shader/ main.cpp ccb.cpp -type f | xargs wc -l | tail -n 1");
#endif
	}

	// prepare next
	itr++;
	return out;
}
