#include <utility>
#include "string.h"

using namespace std;

advstring::advstring(string str) : str_(std::move(str)) {//init
	this->str_ = str_;
}

string advstring::upper() { //轉大寫
	string upper_str;
	for (auto row : this->str_) {
		if (row >= 97 && row <= 122)
			upper_str += row - 32;
		else
			upper_str += row;
	}
	return upper_str;
}

vector<string> advstring::split(string&& delimiter) { //分割特定字元
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



void advstring::join(const vector<string>& new_str) { //加入文字
	for (const auto& row : new_str)
		this->str_ += row;
}

void advstring::join(const string& new_str) {
	this->str_ += new_str;
}

string advstring::replace(char&& original_str, char&& replace_str) { //取代文字
	for (auto& row : this->str_) {
		if (row == original_str)
			row = replace_str;
	}
	return this->str_;
}

bool advstring::possession(char&& possession_str) { //檢查
	for (auto row : this->str_) {
		if (row == possession_str)
			return true;
	}
	return false;
}

