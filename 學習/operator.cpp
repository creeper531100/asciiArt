#include "string.h"

string&& advstring::str() { //���
	return std::move(this->str_);
}

advstring advstring::operator=(const advstring& self) { //�ಾ
	this->str_ = self.str_;
	return advstring(this->str_);
}

advstring advstring::operator+(const advstring& self) { //�s�W
	this->str_ += self.str_;
	return advstring(this->str_);
}