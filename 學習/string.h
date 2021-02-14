#pragma once
#ifndef STRING_H 
#define STRING_H 
#include <string>
#include <vector>

using namespace std;

class Advstring {
public:
	Advstring(string);
	Advstring operator=(const Advstring&);
	Advstring operator+(const Advstring&);
	string upper();
	string&& str();
	string replace(char&&, char&&);
	vector<string> split(string&&);
	void join(const vector<string>&);
	void join(const string&);
	bool possession(char&&);
private:
	string str_;
};

#endif