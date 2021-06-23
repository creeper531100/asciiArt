#pragma once
#ifndef TOOLS_H
#define TOOLS_H
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
using namespace std;

string input(string&&);
HANDLE&& handle_console(wchar_t**, cv::Size&);

// println�榡��X(����)----------------------------------
template <typename T>
void println(T& str) {
	cout << str << "\n"; //�J�줣�P���A������X
}

template <typename T, typename ...Ts>
void println(T first, Ts ... rest) {
	cout << first;
	println(rest...); //�ѥ]���j
}

//print �榡��X -----------------------------------------
inline void print(const char* format) {
	//�J�줣�P���A������X
	cout << format;
}

template <typename T, typename... Targs>
void print(const char* format, T value, Targs ... fargs) {
	for (; *format != '\0'; format++) {
		//�M�X�r��
		if (*format == '%') {
			cout << value; //�J��%�A����ܼ�
			print(format + 1, fargs...); //�ѥ]���j
			return;
		}
		cout << *format; //�p�G�S���ܼơA��X�r��
	}
}

// Int/Str���A�ഫ -----------------------------------------
template <typename T, typename V>
T convert(const V& val) {
	stringstream convert_var;
	T var;
	convert_var << val;
	convert_var >> var;
	return var;
}


#endif
