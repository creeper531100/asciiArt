#include <iostream>


using namespace std;



string input(string&& print_str) {
	string scan;
	cout << print_str;
	cin >> scan;
	return scan;
}