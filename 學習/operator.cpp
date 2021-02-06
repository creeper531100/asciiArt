#include "string.h"

string&& advstring::str() { //顯示
	return std::move(this->str_);
}

advstring advstring::operator=(const advstring& self) { //轉移
	this->str_ = self.str_;
	return advstring(this->str_);
}

advstring advstring::operator+(const advstring& self) { //新增
	this->str_ += self.str_;
	return advstring(this->str_);
}