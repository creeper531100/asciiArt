#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "ascii.h"
#include "string.h"
#include "tools.h"
#include <nlohmann/json.hpp>

using namespace cv;
using namespace dnn;
using namespace std;

enum choicecase {
	CREATE_JSON,
	COMPARE_IMG,
	ASCIIART,
	EXPORT_ASCIIART,
	ADV_EXPORT_ASCIIART,
	DETECTION_FACE,
	DETECTION_CAR1,
	DETECTION_CAR2
};

int main() try {
	string getInput = input("歡迎使用查圖程式\n(0).指紋化 (1).計算相似度 \n(2).Ascii藝術-1(及時處理) (3).Ascii藝術(匯出影片) (4).Ascii藝術-2(匯出影片(自訂文字); 不建議使用，除非你對字體有要求)\n\
							\n(5)使用AI來變換臉部, (6)普通道路辨識車輛 (7)高速公路辨識車輛 \n輸入:  ");
	auto switchVal = static_cast<choicecase>(convert<int>(getInput));

	string file, imgPath, compareImg, save;
	vector<int>getData;
	file = input("開啟檔案名稱: ");
	
	if (switchVal != CREATE_JSON &&
		switchVal != ASCIIART &&
		switchVal != COMPARE_IMG &&
		switchVal != DETECTION_CAR1 &&
		switchVal != DETECTION_CAR2) {
		save = input("請指定儲存名稱: ");
	}
	
	AsciiArt ascii_art(file, save);
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
		ascii_art.asciiArt();
		break;
	case EXPORT_ASCIIART:
		ascii_art.advascii();
		break;
	case ADV_EXPORT_ASCIIART:
		ascii_art.advart();
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
	}
	system("pause");
	return 0;
}
catch (exception& e) {
	cerr << e.what();
	return -1;
}