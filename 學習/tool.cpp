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


HANDLE&& handle_console(wchar_t** screen, cv::Size& dsize) {
	*screen = new wchar_t[dsize.area()];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	return move(hConsole);
}

HANDLE&& handle_console() {
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	return move(hConsole);
}