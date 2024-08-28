#include "toolbox.h"

/*
	!O(n) .filesize /function -> (static)
	purpose: copy file data to a new location
	\param origin: path to file to copy from
	\param dest: destination path of new file
*/
void Toolbox::copy_file(const char* origin,const char* dest)
{
	// open files
	std::ifstream read_file(origin,std::ios::in);
	std::ofstream write_file(dest,std::ios::app);

	// write file content
	std::string line;
	while (getline(read_file,line)) write_file << line << '\n';

	// close files
	read_file.close();
	write_file.close();
}

/*
	TODO
*/
std::vector<std::string> Toolbox::split_string(std::string str,char delim)
{
	// prepare buffer read
	std::vector<std::string> out;
	std::stringstream bfr(str);
	std::string substr;

	// execute buffer read until end, then return substring list
	while (std::getline(bfr,substr,delim))
		out.push_back(substr);
	return out;
}
// FIXME: contemplate improving performance

/*
	calculate_vecangle(vec2,vec2) -> float (static)
	a: first vector, 0 degrees towards the origin
	b: second vector, returned degrees from first vector
	returns: angle between vectors in radians
*/
/*
float Toolbox::calculate_vecangle(glm::vec2 a,glm::vec2 b)
{ return glm::acos(glm::dot(a,b)/(glm::length(a)*glm::length(b))); }
*/

/*
	!O(1)b /+function -> (public,static)
	purpose: convert a boolean to a float with controlled delay
	\param tval: reference of float, being transformed according to boolean
	\param tspeed: controlled delay interval
	\param cnd: boolean condition, defining target float value
*/
void Toolbox::transition_float_on_condition(float& tval,float tspeed,bool cnd)
{
	tval += tspeed*(cnd-!cnd);
	tval = (tval<.0f) ? .0f : (tval>1.f) ? 1.f : tval;
	/*uint8_t tmin = (mtransition<.0f),tmax = (mtransition>1.f);
	mtransition = mtransition-(mtransition-1.f)*tmax+abs(mtransition)*tmin;*/
	// TODO: compare linear transition with sinespeed transition implementation
	// 		also relate the results of this todo to the ftransition in main render method
}
// TODO: optimize further (!use assembly translation as guide!)
// TODO: maybe even set some bounds to enhance the transition range possibilities
// TODO: i feel like this implementation lacks wit
