#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tools.h"
#include "string.h"
#include <nlohmann/json.hpp>
#include <utility>
#include <fstream>
#include <iomanip>


using namespace cv;
using namespace std;
using json = nlohmann::json;

string aHash(const Mat& img) {
	Mat gray;
	int s = 0;
	int avg = 0;
	string hash_str = "";
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

auto cmpHash(string& hash1, const string&& hash2) {
	json jsonfile;
	string jsonStr;
	ifstream in(hash2);
	in >> jsonfile;
	int n = 0, j = 0;
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
		pairs.push_back(row);
	sort(pairs.begin(), pairs.end(), [=](pair<int, int>& a, pair<int, int>& b)
		{
			return a.second < b.second;
		}
	);

	return pairs[0].first;
}

void createJson(string inputDir, const string& outputDir) {
	json jsonfile;
	int i = 0;
	vector<string> indexPath;
	string fileName;
	stringstream inter;
	VideoCapture cap(inputDir);
	
	if (!cap.isOpened()) {
		std::cerr << "ERROR: Could not open video " << inputDir << endl;
	}
	
	while (cap.isOpened()) {
		printf("%d\n", i++);
		Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		indexPath.emplace_back(aHash(frame));
	}
	jsonfile["HashMap"] = indexPath;
	ofstream file(outputDir);
	file << jsonfile;
}

int main() {
	string getInput = input("歡迎使用查圖程式\n(0).拆解圖片 (1).計算相似度");
	string file, imgPath;
	switch (convert<int>(getInput)) {
	case 0:
		file = input("請設定名稱: ");
		createJson(file, "json\\" + file + ".json");
		break;
	case 1:
		file = input("開啟檔案名稱: ");
		string compareImg = aHash(imread(file));
		int getTime = cmpHash(compareImg, "json\\000.json");
		printf("%02d:%02d", getTime / 30 / 60, getTime / 30 % 60);
		break;
	}
}