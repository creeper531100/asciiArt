#include <codecvt>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "ascii.h"
#include <windows.h>
#include <wchar.h>
#include <nlohmann/json.hpp>

using namespace cv;
using namespace std;
using json = nlohmann::json;

AsciiArt::AsciiArt(string inputDir, string saveDir = nullptr) {
	this->inputDir = inputDir;
	this->saveDir = saveDir + ".mp4";
	this->run = "ffmpeg -i temp_video.mp4 -i " + this->inputDir + " -c copy -map 0:v:0 -map 1:a:0 output.mp4";
	this->lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
}
//a
void AsciiArt::initVideo(Size setVideoSize, Size setDsize) {
	VideoCapture cap(this->inputDir);
	if (!cap.isOpened())
		std::cerr << "ERROR: Could not open video " << this->inputDir << endl;
	system("cls");
	this->cap = cap;
	this->frameCount = cap.get(CAP_PROP_FRAME_COUNT);		   //設置總張數
	this->frameFPS = cap.get(CAP_PROP_FPS);					  //設置FPS
	this->encoding = VideoWriter::fourcc('D', 'I', 'V', 'X');//設置編碼
	this->setVideoSize = setVideoSize;
	this->frame_interval = (1 / this->frameFPS) * 1000000;
	this->getVideoSize = Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));//設置影片解析度
	this->dsize = setDsize;//設置壓縮解析度
}

void AsciiArt::run_ascii_art(function<void()> run_function, Mat* mFrame) {
	while (this->cap.isOpened()) {
		this->cap >> *mFrame;
		if (mFrame->empty()) break;
		resize(*mFrame, *mFrame, this->dsize, 0, 0, INTER_CUBIC);
		cvtColor(*mFrame, *mFrame, COLOR_RGB2GRAY);
		run_function();
	}
}

vector<vector<string>>&& AsciiArt::braille_create(Size& video_setting_size, Mat* pFrame) {
	vector<vector<string>>* deep_arr = new vector<vector<string>>;
	for (int i = 0; i < video_setting_size.height; i++) {
		vector<string> deep;
		Scalar avg = mean(*pFrame);
		int avg_color = avg.val[0];
		for (int j = 1; j < video_setting_size.width; j += 2) {
			if (pFrame->at<uchar>(i, j - 1) > avg_color) {
				if (pFrame->at<uchar>(i, j) > avg_color)
					deep.emplace_back("m");
				else
					deep.emplace_back("y");
			}
			else {
				if (pFrame->at<uchar>(i, j) > avg_color)
					deep.emplace_back("z");
				else
					deep.emplace_back("k");
			}
		}
		deep_arr->emplace_back(deep);
	}
	return move(*deep_arr);
}

void AsciiArt::print_output_info(time_t t_start) {
	system(this->run.c_str());
	remove(this->saveDir.c_str());
	int totalTime = difftime(time(NULL), t_start);
	printf("\nused %02d:%02d", totalTime / 60, totalTime % 60);
}

HANDLE&& handle_console(wchar_t** screen, cv::Size& dsize) {
	*screen = new wchar_t[dsize.area()];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	return move(hConsole);
}

HANDLE&& handle_console() {
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	return move(hConsole);
}