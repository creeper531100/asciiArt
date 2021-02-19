#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "tools.h"
#include "string.h"
#include <nlohmann/json.hpp>
#include <utility>
#include <fstream>
#include "ascii.h"

using namespace cv;
using namespace std;
using json = nlohmann::json;

string aHash(Mat img) {
	Mat gray, imgresize;
	int s = 0;
	int avg = 0;
	string hash_str = "";
	resize(img, imgresize, Size(16, 9));
	cvtColor(imgresize, gray, COLOR_RGB2GRAY);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			s += static_cast<int>(gray.at<uchar>(i, j));
		}
	}
	avg = s / 144;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			(static_cast<int>(gray.at<uchar>(i, j) > avg) ? hash_str += "1" : hash_str += '0');
		}
	}
	return hash_str;
}

vector<int> cmpHash(string hash1, string hash2) {
	json jsonfile;
	string jsonStr;
	ifstream in(hash2);
	in >> jsonfile;
	int n = 0, j = 0;
	vector<int> returnData = { 0, 0 };
	map<int, int> maps;
	vector<pair<int, int>> pairs;

	for (string row : jsonfile["HashMap"]) {
		n = 0;
		for (int i = 0; i < hash1.length(); i++) {
			n += hash1[i] ^ row[i];
		}
		maps[j++] = n;
	}

	for (auto& row : maps) {
		pairs.emplace_back(row);
	}

	sort(pairs.begin(), pairs.end(), [=](pair<int, int>& a, pair<int, int>& b)
		{
			return a.second < b.second;
		}
	);
	for (auto& row : pairs) {
		pairs.emplace_back(row);
	}

	returnData[0] = pairs[0].first;
	returnData[1] = pairs[0].second;
	return returnData;
}
void createJson(string inputDir, string outputDir) {
	string outDir = outputDir;
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
	ofstream file(outDir, fstream::out);
	file << jsonfile;
	file.close();
}