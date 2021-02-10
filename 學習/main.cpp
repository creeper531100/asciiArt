#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "tools.h"
#include "string.h"
#include "ascii.h"

using namespace cv;
using namespace std;

int main() {	
	string getInput = input("歡迎使用查圖程式\n(0).指紋化 (1).計算相似度 \n(2).Ascii藝術-1(及時處理) (3).Ascii藝術(匯出影片) (4).Ascii藝術-2(匯出影片(自訂文字); 不建議使用，除非你對字體有要求)\n輸入:  ");
	int choice = convert<int>(getInput);
	string file, imgPath, compareImg, save;
	vector<int>getData;
	file = input("開啟檔案名稱: ");
	if (choice != 0 && choice != 2)
		save = input("請指定儲存名稱: ");
	AsciiArt ascii_art(file, save);
	switch (choice) {
	case 0:
		createJson(file, "json\\" + file + ".json");
		break;
	case 1:
		compareImg = aHash(imread(file));
		getData = cmpHash(compareImg, "json\\" + save + ".json");
		printf("相似點位: %d, %02d:%02d", getData[1],
			getData[0] / 30 / 60, getData[0] / 30 % 60);
		break;
	case 2:
		ascii_art.asciiArt();
		break;
	case 3:
		ascii_art.advascii();
		break;
	case 4:
		ascii_art.advart();
		break;
	}
}