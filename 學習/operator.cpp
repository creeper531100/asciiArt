#include "string.h"

string&& advstring::__str__() { //顯示
	return std::move(this->str);
}

advstring advstring::operator=(const advstring& self) { //轉移
	this->str = self.str;
	return advstring(this->str);
}

advstring advstring::operator+(const advstring& self) { //新增
	this->str += self.str;
	return advstring(this->str);
}