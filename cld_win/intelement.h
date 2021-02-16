#pragma once

#include <iostream>
#include <vector>

class INTElement
{
public:
	INTElement() { }
	INTElement(int ival) : val(ival) { }
	INTElement(int ival, INTElement* iprev) : val(ival), prev(iprev) { }
	void add_next(int nval)
	{ INTElement n = INTElement(nval,this); next.push_back(n); }
	int get_val() { return val; }
	void set_val(int nval) { val = nval; }
	INTElement* get_next(int i)
	{ if (i < next.size()) return &next.at(i); return NULL; }
	int get_next_size() { return next.size(); }
	INTElement* get_prev() { return prev; }
	bool has_next() { return next.size()>0; }
private:
	int val;
	std::vector<INTElement> next;
	INTElement* prev;
};
