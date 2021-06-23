﻿#include <opencv2/opencv.hpp>
#include "ascii.h"
#include "string.h"
#include "tools.h"
#include <nlohmann/json.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace dnn;
using namespace std;

enum choicecase {
	CREATE_JSON,
	COMPARE_IMG,
	ASCIIART,
	EXPORT_ASCIIART,
	ADV_EXPORT_ASCIIART,
	ADV_EXPORT_FILL_ASCIIART,
	DETECTION_FACE,
	DETECTION_CAR1,
	DETECTION_CAR2,
	ASCIIART_ADV,
	BEAUTIFUL_ART,
	BEAUTIFUL_ADV,
	BEAUTIFUL_ADV_ADV
};

int main(int argc, char* argv[]) try {
	string user_input;
	if (argc <= 2) {
		user_input = input("歡迎使用查圖程式\n(0).指紋化 (1).計算相似度 \n(2).Ascii藝術-1(及時處理) (3).Ascii藝術-2(匯出影片) (4).Ascii藝術-3(匯出彩色影片) (5).Ascii藝術-4(單一文字)\n(6)使用AI來變換臉部, (7)普通道路辨識車輛 (8)高速公路辨識車輛 (9)盲人藝術 (10)漂亮asciiart (11)輸出盲人\n輸入:  ");
	}
	else {
		user_input = argv[1];
	}

	auto switchVal = static_cast<choicecase>(convert<int>(user_input));
	setUseOptimized(1);
	ocl::setUseOpenCL(1);

	string file, imgPath, compareImg, fill_word;
	vector<int>getData;

	if (argc <= 2) {
		file = input("請輸入檔案名稱或是網址: ");
	}
	else {
		file = argv[2];
	}

	if (file.find("https") != std::string::npos) {
		remove("HTC.mp4");
		system(("youtube-dl -o HTC.%(ext)s -f mp4 " + file).c_str());
		file = "HTC.mp4";
	}

	AsciiArt ascii_art(file, "temp_video");
	switch (switchVal) {
	case CREATE_JSON:
		createJson(file, "json\\0.json");
		break;
	case COMPARE_IMG:
		compareImg = aHash(imread(file));
		getData = cmpHash(compareImg, "json\\0.json");
		printf("相似點位: %d, %02d:%02d", getData[1],
			getData[0] / 30 / 60, getData[0] / 30 % 60);
		break;
	case ASCIIART:
		system(("start " + file).c_str());
		ascii_art.asciiArt();
		break;
	case EXPORT_ASCIIART:
		ascii_art.advascii();
		break;
	case ADV_EXPORT_ASCIIART:
		ascii_art.advart(0, "");
		break;
	case ADV_EXPORT_FILL_ASCIIART:
		fill_word = input("選擇填充字體: ");
		ascii_art.advart(1, fill_word);
		break;
	case DETECTION_FACE:
		ascii_art.network();
		break;
	case DETECTION_CAR1:
		ascii_art.detectionCar1();
		break;
	case DETECTION_CAR2:
		ascii_art.detectionCar2();
		break;
	case ASCIIART_ADV:
		system(("start " + file).c_str());
		ascii_art.asciiAdvArt();
		break;
	case BEAUTIFUL_ART:
		ascii_art.beautifulart();
		break;
	case BEAUTIFUL_ADV:
		ascii_art.beautiadv(0);
		break;
	case BEAUTIFUL_ADV_ADV:
		ascii_art.beautiadv(1);
		break;
	}
	system("pause");
	return 0;
}
catch (exception& e) {
	cerr << e.what();
	return -1;
}