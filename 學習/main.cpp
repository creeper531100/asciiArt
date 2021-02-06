#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tools.h"
#include "string.h"

using namespace cv;

using namespace std;

string aHash(Mat &&img) {
	Mat gray;
	int s = 0;
	int avg = 0;
	string hash_str = "";
	resize(img, img, Size(16, 9), 0, 0, INTER_AREA);
	cvtColor(img, gray, COLOR_RGB2GRAY);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			s += (int)gray.at<uchar>(i, j);
		}
	}
	avg = s / 144;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			if ((int)gray.at<uchar>(i, j) > avg)
				hash_str += "1";
			else
				hash_str += '0';
		}
	}
	return hash_str;
}

int cmpHash(string &hash1, string &&hash2) {
	int n = 0;
	if (hash1.length() != hash2.length())
		return -1;
	for (int i = 0; i < hash1.length(); i++) {
		n += hash1[i] ^ hash2[i];
	}
	return n;
}

auto compareImg(string img) {
	string folder_path = "D:\\source\\厩策\\厩策\\images\\*.png";
	vector<string> file_names;
	float index = 0.0;
	map<string, int> arraryMap;
	glob(folder_path, file_names);   //get file names
	for (auto row : file_names) {
		arraryMap[row] = cmpHash(img, aHash(imread(row)));
		printf("%f %%\r", ++index / (float)file_names.size() * 100);
	}

	vector<pair<string, int>> pairs;
	for (const auto& row : arraryMap)
		pairs.emplace_back(row);

	sort(pairs.begin(), pairs.end(), [=](pair<string, int>& a, pair<string, int>& b)
		{
			return a.second < b.second;
		}
	);
	
	return pairs;
}


int main() {
	string img0 = aHash(imread("D:\\source\\厩策\\厩策\\video.png"));
	auto imagePair = compareImg(img0);
	string filePath = imagePair[0].first;
	
	advstring getPath(filePath);
	system("cls");
	string addressRemove = getPath.split("\\")[5];
	advstring getIndex(addressRemove);
	string extensionRemove = getIndex.split(".")[0];
	int calcTime = convert<int>(extensionRemove);
	
	system(("start " + filePath).c_str());
	printf("%02d:%02d",calcTime / 30 / 60, calcTime / 30 % 60);
}
