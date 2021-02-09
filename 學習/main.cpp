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
	time_t c_start, t_start, t_end;
	VideoCapture cap(inputDir);
	int error = 0;
	char lv[] = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
	if (!cap.isOpened()) {
		std::cerr << "ERROR: Could not open video " << inputDir << endl;
	}
	system("cls");
	t_start = time(NULL);
	while (cap.isOpened()) {
		c_start = clock();
		string str;
		Mat frame;
		cap >> frame;
		if (frame.empty()) break;
		resize(frame, frame, Size(237, 62), 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		for (int i = 0; i < 62; i++) {
			for (int j = 0; j < 237; j++) {
				str.push_back(lv[frame.at<uchar>(i, j) / 4]);
			}
			str.push_back('\n');
		}
		printf("%s", str.c_str());
		int delay = difftime(clock(), c_start);
		delay < 30 ? Sleep(30 - delay) : void(error++);
	}
	t_end = time(NULL);
	int totalTime = difftime(t_end, t_start);
	printf("used %02d:%02d\nerror:%d", totalTime / 60, totalTime % 60, error);
}

void advart(string inputDir, string saveDir) {
	string fileName;
	string path;
	time_t t_start, t_end;
	VideoCapture cap(inputDir);
	float fps = cap.get(CAP_PROP_FPS);
	auto count = 0.0, frameCount = cap.get(CAP_PROP_FRAME_COUNT);
	if (!cap.isOpened()) cerr << "ERROR: Could not open video " << inputDir << endl;
	VideoWriter writer(saveDir + ".mp4", VideoWriter::fourcc('D', 'I', 'V', 'X')
		, fps, Size(1008, 560));
	t_start = time(NULL);
	while (cap.isOpened()) {
		Mat frame;
		cap >> frame;
		if (frame.empty()) break;//125x35
		resize(frame, frame, Size(125, 35), 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		Mat R, z;
		for (int i = 0; i < 35; i++) {
			z = imread("font\\63.png");
			for (int j = 0; j < 125; j++) {
				path = "font\\" + to_string((int)frame.at<uchar>(i, j) / 4) + ".png";
				hconcat(z, imread(path), z);
			}
			R.push_back(z);
		}
		writer.write(R);
		printf("進度: %f%%/%f\r", (count++ / frameCount) * 100, difftime(time(NULL), t_start));
	}
	writer.release();
	t_end = time(NULL);
	int totalTime = difftime(t_end, t_start);
	printf("used %02d:%02d", totalTime / 60, totalTime % 60);
}

void advascii(string inputDir, string saveDir) {
	time_t t_start, t_end;
	VideoCapture cap(inputDir);
	auto count = 0.0,
		frameCount = cap.get(CAP_PROP_FRAME_COUNT),
		frameFPS = cap.get(CAP_PROP_FPS);
	char lv[] = " .'`^,:;l!i><~+_-?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
	Point origin;
	if (!cap.isOpened()) cerr << "ERROR: Could not open video " << inputDir << endl;
	VideoWriter writerMp4(saveDir + ".mp4", VideoWriter::fourcc('D', 'I', 'V', 'X')
		, frameFPS, Size(1920, 1080));
	t_start = time(NULL);
	while (cap.isOpened()) {
		Mat frame;
		cap >> frame;
		Mat image = Mat::zeros(Size(1920, 1080), CV_8UC3);
		image.setTo(Scalar(0, 0, 0));
		if (frame.empty()) break;//125x35
		resize(frame, frame, Size(158, 71), 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		origin.y = 5;
		for (int i = 0; i < 71; i++) {
			origin.y += 15;
			for (int j = 0; j < 158; j++) {
				origin.x += 12;
				string showStr = convert<string>(lv[frame.at<uchar>(i, j) / 4]);
				putText(image, showStr, origin, FONT_HERSHEY_SIMPLEX,
					0.5, Scalar(255, 255, 255),
					1, 8, 0);
			}
			origin.x = 0;
		}
		writerMp4.write(image);
		printf("進度: %f%%/%f\r", (count++ / frameCount) * 100, difftime(time(NULL), t_start));
	}
	writerMp4.release();
	t_end = time(NULL);
	int totalTime = difftime(t_end, t_start);
	printf("used %02d:%02d", totalTime / 60, totalTime % 60);
}


int main() {	
	string getInput = input("歡迎使用查圖程式\n(0).指紋化 (1).計算相似度 \n(2).Ascii藝術-1(及時處理) (3).Ascii藝術(匯出影片) (4).Ascii藝術-2(匯出影片(自訂文字); 不建議使用，除非你對字體有要求)\n輸入:  ");
	string file, imgPath, compareImg, save;
	vector<int>getData;
	switch (convert<int>(getInput)) {
	case 0:
		file = input("請設定名稱: ");
		createJson(file, "json\\" + file + ".json");
		break;
	case 1:
		file = input("開啟檔案名稱: ");
		save = input("請指定儲存名稱: ");
		compareImg = aHash(imread(file));
		getData = cmpHash(compareImg, "json\\" + save + ".json");
		printf("相似點位: %d, %02d:%02d", getData[1],
			getData[0] / 30 / 60, getData[0] / 30 % 60);
		break;
	case 2:
		file = input("開啟檔案名稱: ");
		asciiArt(file);
		break;
	case 3:
		file = input("開啟檔案名稱: ");
		save = input("請指定儲存路徑: ");
		advascii(file, save);
		break;
	case 4:
		file = input("開啟檔案名稱: ");
		save = input("請指定儲存路徑: ");
		advart(file, save);
		break;
	} 
}