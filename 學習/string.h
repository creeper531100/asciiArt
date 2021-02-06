#pragma once
#ifndef STRING_H 
#define STRING_H 
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class advstring {
public:
	advstring(string);
	advstring operator=(const advstring&);
	advstring operator+(const advstring&);
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