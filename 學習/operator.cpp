#include "string.h"

string&& Advstring::str() { //���
	return std::move(this->str_);
}

Advstring Advstring::operator=(const Advstring& self) { //�ಾ
	this->str_ = self.str_;
	return Advstring(this->str_);
}

Advstring Advstring::operator+(const Advstring& self) { //�s�W
	this->str_ += self.str_;
	return Advstring(this->str_);
}