#pragma once
#ifndef TOOLS_H
#define TOOLS_H
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

string input(string&&);

// println格式輸出(換行)----------------------------------
template <typename T>
void println(T& str) {
	cout << str << "\n"; //遇到不同型態重載輸出
}

template <typename T, typename ...Ts>
void println(T first, Ts ... rest) {
	cout << first;
	println(rest...); //解包遞迴
}

//print 格式輸出 -----------------------------------------
inline void print(const char* format) {
	//遇到不同型態重載輸出
	cout << format;
}

template <typename T, typename... Targs>
void print(const char* format, T value, Targs ... fargs) {
	for (; *format != '\0'; format++) {
		//尋訪字串
		if (*format == '%') {
			cout << value; //遇到%，顯示變數
			print(format + 1, fargs...); //解包遞迴
			return;
		}
		cout << *format; //如果沒有變數，輸出字串
	}
}

// Int/Str型態轉換 -----------------------------------------
template <typename T, typename V>
T convert(const V& val) {
	stringstream convert_var;
	T var;
	convert_var << val;
	convert_var >> var;
	return var;
}
#endif
