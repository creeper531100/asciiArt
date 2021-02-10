#include <utility>
#include "string.h"

using namespace std;

Advstring::Advstring(string str) : str_(std::move(str)) {//init
	this->str_ = str_;
}

string Advstring::upper() { //��j�g
	string upper_str;
	for (auto row : this->str_) {
		if (row >= 97 && row <= 122)
			upper_str += row - 32;
		else
			upper_str += row;
	}
	return upper_str;
}

vector<string> Advstring::split(string&& delimiter) { //���ίS�w�r��
	int pos_start = 0, pos_end, delim_len = delimiter.length();
	vector<string> res;

	while ((pos_end = this->str_.find(delimiter, pos_start)) != string::npos) {
		string token = this->str_.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(this->str_.substr(pos_start));
	return res;
}



void Advstring::join(const vector<string>& new_str) { //�[�J��r
	for (const auto& row : new_str)
		this->str_ += row;
}

void Advstring::join(const string& new_str) {
	this->str_ += new_str;
}

string Advstring::replace(char&& original_str, char&& replace_str) { //���N��r
	for (auto& row : this->str_) {
		if (row == original_str)
			row = replace_str;
	}
	return this->str_;
}

bool Advstring::possession(char&& possession_str) { //�ˬd
	for (auto row : this->str_) {
		if (row == possession_str)
			return true;
	}
	return false;
}

