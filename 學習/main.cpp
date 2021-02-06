#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
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

string aHash(Mat &&img) {
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

auto cmpHash(string &hash1,const string &&hash2) {
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
	
	for (auto &row : maps)
		pairs.push_back(row);

	sort(pairs.begin(), pairs.end(), [=](pair<int, int>& a, pair<int, int>& b)
		{
			return a.second < b.second;
		}
	);

	return pairs[0].first;
}

void writeHash(const string &folder_path, string setAnimeName) {
	json jsonfile;
	vector<string> file_names;
	vector<string> indexPath;

	string saveFileName = "json\\";
	saveFileName = saveFileName + setAnimeName + ".json";
	float prog = 0.0;
	glob(folder_path, file_names);
	cout << endl;
	for (auto row : file_names) {
		indexPath.emplace_back(aHash(imread(row)));
		printf("%s : %f\n", row.c_str(), prog);
		printf("進度: %f%%\r", ++prog / (float)file_names.size() * 100);
	}
	jsonfile["HashMap"] = indexPath;
	ofstream file(saveFileName);
	file << jsonfile;
}

void createImg(string inputDir,  string outputDir) {
	string commando = "start create.py " + inputDir + " " + outputDir;
	system(commando.c_str());
}

int main() {
	string getInput = input("歡迎使用查圖程式\n(0).拆解圖片 (1).計算相似度 (2).自動: ");
	string file, imgPath;
	switch (convert<int>(getInput)) {
		
	case 0:
		file = input("請設定檔案名稱: ");
		createImg(file, "img");
		break;
	case 1:
		file = input("請設定名稱: ");
		writeHash("img", file);
		system("del /F /S img\\*.png");
		break;
	case 2:
		file = input("開啟檔案名稱: ");
		string compareImg = aHash(imread(file));
		int getTime = cmpHash(compareImg, "json\\000.json");
		printf("%02d:%02d", getTime/30/60, getTime/30%60);
		break;
	}
}
/*void createImg(string inputDir, const string& outputDir) {
	string fileName;
	stringstream inter;
	VideoCapture cap(inputDir);
	if (!cap.isOpened()){
		std::cerr << "ERROR: Could not open video " << inputDir << std::endl;
	}
	int frame_count = 0;
	while (cap.isOpened()){
		Mat frame;
		cap >> frame; //get a new frame from the video
		if (frame.empty()){
			break;
		}
		inter << setw(5) << setfill('0') << frame_count << ".jpg";
		inter >> fileName;
		string filePath = outputDir + fileName + ".jpg";
		cv::imwrite(filePath, frame);
		frame_count++;
	}
}*/