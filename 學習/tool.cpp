#include <iostream>
#include <windows.h>
#include <opencv2/core/types.hpp>

using namespace std;

string input(string&& print_str) {
	string scan;
	cout << print_str;
	cin >> scan;
	return scan;
}