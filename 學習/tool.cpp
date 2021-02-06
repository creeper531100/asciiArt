#include <iostream>

using namespace std;


string input(string&& printstr) {
	string scan;
	cout << printstr;
	cin >> scan;
	return scan;
}
