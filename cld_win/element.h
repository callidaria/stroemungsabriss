#pragma once

#include <iostream>
#include <vector>

class Element
{
public:
	Element() { }
	Element(std::string ival) : val(ival) { }
	Element(std::string ival, Element* iprev) : val(ival), prev(iprev) { }
	void add_next(std::string nval)
	{ Element n = Element(nval,this); next.push_back(n); }
	std::string get_val() { return val; }
	void set_val(std::string nval) { val = nval; }
	Element* get_next(int i)
	{ if (i < next.size()) return &next.at(i); return NULL; }
	int get_next_size() { return next.size(); }
	Element* get_prev() { return prev; }
	bool has_next() { return next.size()>0; }
private:
	std::string val;
	std::vector<Element> next;
	Element* prev;
};
