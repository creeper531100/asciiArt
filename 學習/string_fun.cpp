#include "string.h"

using namespace std;

string&& lvalue() {
	string val = "string";
	return std::move(val);
}

advstring::advstring(const string& str) : str(str) {//init
	this->str = str;
}

string advstring::upper() { //轉大寫
	string upperStr;
	for (int i = 0; i < this->str.length(); i++) {
		if (this->str[i] >= 97 && this->str[i] <= 122)
			upperStr += this->str[i] - 32;
		else
			upperStr += this->str[i];
	}
	return upperStr;
}

vector<string> advstring::split(char&& symbol) { //分割特定字元
	vector<int> findSymbol;
	vector<string> splitStr;
	for (int i = 0; i < this->str.length(); i++) {
		if (this->str[i] == symbol)
			findSymbol.push_back(i);
	}
	for (int i = 0; i < findSymbol.size(); i += 2) {
		splitStr.push_back(this->str.substr(findSymbol[i] + 1, (findSymbol[i + 1] - findSymbol[i]) - 1));
	}
	return splitStr;
}

void advstring::join(const vector<string>& newStr) { //加入文字
	for (auto row : newStr)
		this->str += row;
}

void advstring::join(const string& newStr) {
	this->str += newStr;
}

string advstring::replace(char&& originalStr, char&& replaceStr) { //取代文字
	for (int i = 0; i < this->str.length(); i++) {
		if (this->str[i] == originalStr)
			this->str[i] = replaceStr;
	}
	return this->str;
}

bool advstring::possession(char&& possessionStr) { //檢查
	for (auto row : this->str) {
		if (row == possessionStr)
			return 1;
	}
	return 0;
}
