#include "string.h"

string&& advstring::__str__() { //���
	return std::move(this->str);
}

advstring advstring::operator=(const advstring& self) { //�ಾ
	this->str = self.str;
	return advstring(this->str);
}

advstring advstring::operator+(const advstring& self) { //�s�W
	this->str += self.str;
	return advstring(this->str);
}