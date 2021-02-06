#pragma once
#ifndef STRING_H 
#define STRING_H 
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string&& lvalue();

class advstring {
public:
	advstring(const string&);
	advstring operator=(const advstring&);
	advstring operator+(const advstring&);
	string upper();
	string&& __str__();
	string replace(char&&, char&&);
	vector<string> split(char&&);
	void join(const vector<string>&);
	void join(const string&);
	bool possession(char&&);
private:
	string str;
};

#endif