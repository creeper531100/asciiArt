#pragma once
#ifndef TOOLS_H 
#define TOOLS_H
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

string input(string&&);

// println�榡��X(����)----------------------------------
template <typename T>
void println(T& str) {
    cout << str << "\n"; //�J�줣�P���A������X
}

template <typename T, typename ...Ts>
void println(T first, Ts... rest) {
    cout << first;
    println(rest...); //�ѥ]���j
}

//print �榡��X -----------------------------------------
void print(const char* format) { //�J�줣�P���A������X
    cout << format;
}

template<typename T, typename... Targs>
void print(const char* format, T value, Targs... Fargs) {
    for (; *format != '\0'; format++) { //�M�X�r��
        if (*format == '%') { 
            cout << value; //�J��%�A����ܼ�
            print(format + 1, Fargs...);//�ѥ]���j
            return;
        }
        cout << *format;//�p�G�S���ܼơA��X�r��
    }
}

// Int/Str���A�ഫ -----------------------------------------
template<typename T, typename V>
T convert(const V& val) {
    stringstream convertVar;
    T var;
    convertVar << val;
    convertVar >> var;
    return var;
}
#endif