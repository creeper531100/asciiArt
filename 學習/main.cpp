#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "tools.h"
#include "string.h"
#include <nlohmann/json.hpp>
#include <utility>
#include <windows.h>
#include <fstream>

using namespace cv;
using namespace std;
using json = nlohmann::json;

auto aHash(const Mat& img) {
	Mat gray, imgresize;
	int s = 0;
	int avg = 0;
	string hash_str = "";
	resize(img, imgresize, Size(16, 9), 0, 0, INTER_CUBIC);
	cvtColor(imgresize, gray, COLOR_RGB2GRAY);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			s += (int)gray.at<uchar>(i, j);
		}
	}
	avg = s / 144;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			((int)gray.at<uchar>(i, j) > avg) ? hash_str += "1" : hash_str += '0';
		}
	}
	return hash_str;
}

auto cmpHash(string& hash1, const string&& hash2) {
	json jsonfile;
	string jsonStr;
	ifstream in(hash2);
	in >> jsonfile;
	int n = 0, j = 0;
	vector<int> returnData = {0, 0};
	map<int, int> maps;
	vector<pair<int, int>> pairs;

	for (string row : jsonfile["HashMap"]) {
		n = 0;
		for (int i = 0; i < hash1.length(); i++) {
			n += hash1[i] ^ row[i];
			maps[j] = n;
		}
		j++;
	}

	for (auto& row : maps)
		pairs.emplace_back(row);
	
	sort(pairs.begin(), pairs.end(), [=](pair<int, int>& a, pair<int, int>& b)
		{
			return a.second < b.second;
		}
	);
	returnData[0] = pairs[0].first;
	returnData[1] = pairs[0].second;
	return returnData;
}

void createJson(string inputDir, const string& outputDir) {
	json jsonfile;
	vector<string> indexPath;
	string fileName;
	VideoCapture cap(inputDir);
	if (!cap.isOpened()) {
		std::cerr << "ERROR: Could not open video " << inputDir << endl;
	}
	int i = 0;
	while (cap.isOpened()) {
		printf("%d\r", i++);
		Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		indexPath.emplace_back(aHash(frame));
	}
	jsonfile["HashMap"] = indexPath;
	ofstream file(outputDir);
	file << jsonfile;
	file.close();
}

void asciiArt(string inputDir) {
	string fileName;
	VideoCapture cap(inputDir);
	char lv[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,^`'. ";
	if (!cap.isOpened()) {
		std::cerr << "ERROR: Could not open video " << inputDir << endl;
	}
	system("cls");
	while (cap.isOpened()) {
		Mat frame;
		string str;
		cap >> frame;
		resize(frame, frame, Size(237, 62), 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		for (int i = 0; i < 62; i++) {
			for (int j = 0; j < 237; j++) {
				str += lv[(int)(frame.at<uchar>(i, j) / 3.9)];
			}
			str += "\n";
		}
		cout << str;
		Sleep(13);
		if (frame.empty())
			break;
	}
}


int main() {
	string getInput = input("歡迎使用查圖程式\n(0).指紋化 (1).計算相似度 (2).Ascii藝術");
	string file, imgPath, compareImg;
	vector<int>getData;
	switch (convert<int>(getInput)) {
	case 0:
		file = input("請設定名稱: ");
		createJson(file, "json\\" + file + ".json");
		break;
	case 1:
		file = input("開啟檔案名稱: ");
		compareImg = aHash(imread(file));
		getData = cmpHash(compareImg, "json\\0.json");
		printf("相似點位: %d, %02d:%02d", getData[1],
			getData[0] / 30 / 60, getData[0] / 30 % 60);
		break;
	case 2:
		file = input("開啟檔案名稱: ");
		asciiArt(file);
		break;
	case 3:
		file = input("開啟檔案名稱: ");
		asciiArt(file);
		break;
	} 
}