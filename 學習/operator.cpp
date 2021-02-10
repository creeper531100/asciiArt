#include "string.h"

string&& Advstring::str() { //顯示
	return std::move(this->str_);
}

Advstring Advstring::operator=(const Advstring& self) { //轉移
	this->str_ = self.str_;
	return Advstring(this->str_);
}

Advstring Advstring::operator+(const Advstring& self) { //新增
	this->str_ += self.str_;
	return Advstring(this->str_);
}