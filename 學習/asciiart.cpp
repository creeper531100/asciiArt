#include <codecvt>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "ascii.h"
#include "tools.h"
#include <windows.h>
#include <wchar.h>
#include <nlohmann/json.hpp>

#include "string.h"

using namespace cv;
using namespace std;
using json = nlohmann::json;

AsciiArt::AsciiArt(string inputDir, string saveDir = nullptr) {
	this->inputDir = inputDir;
	this->saveDir = saveDir + ".mp4";
	this->run = "ffmpeg -i temp_video.mp4 -i "+ this->inputDir + " -c copy -map 0:v:0 -map 1:a:0 output.mp4";
	this->lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
}

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
	this->getVideoSize = Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));//設置影片解析度
	this->dsize = setDsize;//設置壓縮解析度
}

void AsciiArt::asciiArt() {
	Size video_setting_size = Size(237, 64);
	initVideo(Size(CAP_PROP_FRAME_WIDTH, CAP_PROP_FRAME_HEIGHT), video_setting_size);
	int frame_interval = (1 / this->frameFPS) * 1000;
	int error = 0;
	wchar_t* screen = new wchar_t[video_setting_size.area()];

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	time_t c_start = time(NULL);
	DWORD dwBytesWritten = 0;
	
	while (this->cap.isOpened()) {
		c_start = clock();
		string str;
		Mat frame;
		this->cap >> frame;
		if (frame.empty()) break;
		resize(frame, frame, this->dsize, 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		for (int j = 0; j < video_setting_size.area(); j++) {
			screen[j] = this->lv[frame.at<uchar>(j) / 4];
		}
		WriteConsoleOutputCharacter(hConsole, screen, video_setting_size.area(), { 0, 0 }, &dwBytesWritten);
		printf("%s", str.c_str());
		//int delay = difftime(clock(), c_start);

		int delay = difftime(clock(), c_start);
		bool need_sleep = (delay < frame_interval) && (delay > 0);
		need_sleep ? Sleep(frame_interval - delay) : void(error++);
		wstring info = to_wstring(frame_interval) + L" - " + to_wstring(delay) + L" = " + to_wstring(frame_interval - 9) + L"ERROR: " + to_wstring(error);
		SetConsoleTitle(info.c_str());
	}
	delete[] screen;
}

void AsciiArt::asciiAdvArt() {
	//SetConsoleOutputCP(CP_UTF8);
	//setlocale(LC_ALL, "");
	Size video_setting_size = Size(240, 120);
	
	init_word();
	initVideo(Size(CAP_PROP_FRAME_WIDTH, CAP_PROP_FRAME_HEIGHT), video_setting_size);
	int frame_interval = (1 / this->frameFPS) * 1000;
	time_t c_start = time(NULL);
	int error = 0;
	
	wchar_t* screen = new wchar_t[video_setting_size.area()];
	int resolution = video_setting_size.area();
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	while (this->cap.isOpened()) {
		c_start = clock();
		vector<vector<string>> deep_arr;
		Mat frame;
		this->cap >> frame;
		if (frame.empty()) break;
		resize(frame, frame, this->dsize, 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		wstring* basic_string = new wstring;
		for (int i = 0; i < video_setting_size.height; i++) {
			vector<string> deep;
			Scalar avg = mean(frame);
			int avg_color = avg.val[0];
			
			for (int j = 1; j < video_setting_size.width; j+=2) {
				if (frame.at<uchar>(i, j - 1) > avg_color) {
					if (frame.at<uchar>(i, j) > avg_color)
						deep.emplace_back("m");
					else
						deep.emplace_back("y");
				}
				else {
					if (frame.at<uchar>(i, j) > avg_color)
						deep.emplace_back("z");
					else
						deep.emplace_back("k");
				}
			}
			deep_arr.emplace_back(deep);
		}
		for (int i = 3; i < deep_arr.size(); i+=4) {
			for (int j = 0; j < video_setting_size.height; j ++) {
				basic_string->push_back(this->map_pairs[deep_arr[i - 3][j] + deep_arr[i - 2][j] + deep_arr[i - 1][j] + deep_arr[i][j]]);
			}
		}
		//wchar_t* ptr = _wcsdup(basic_string->c_str());
		//swprintf_s(screen, 40, L"delay: %d", delay);
		WriteConsoleOutputCharacterW(hConsole, basic_string->c_str(), resolution, { 0, 0 }, &dwBytesWritten);

		delete basic_string;
		int delay = difftime(clock(), c_start);
		bool need_sleep = (delay < frame_interval) && (delay > 0);
		need_sleep ? Sleep(frame_interval - delay) : void(error++);
		wstring info = to_wstring(frame_interval) + L" - " + to_wstring(delay) + L" = " + to_wstring(frame_interval - delay) + L"ERROR: " + to_wstring(error);

		SetConsoleTitle(info.c_str());
		//free(ptr);
	}
	delete[] screen;
}

void AsciiArt::advascii() {
	//初始化各項數據
	initVideo(); //導入影片
	auto count = 0.0; //設置進度
	Point origin;
	Size video_setting_size = Size(158, 71);
	VideoWriter writerMp4(this->saveDir, this->encoding, this->frameFPS, this->setVideoSize);
	//主程式
	const time_t t_start = time(nullptr);
	
	//UMat Uframe;
	Mat frame;
	Mat image;
	string showStr = "";
	int avg = 0;
	
	while (this->cap.isOpened()) {
		auto start = std::chrono::high_resolution_clock::now();
		this->cap >> frame;
		image = Mat(this->setVideoSize, CV_8U, USAGE_ALLOCATE_DEVICE_MEMORY);
		image.setTo(Scalar(0, 0, 0));
		if (frame.empty()) break;
		resize(frame, frame, this->dsize);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		//Uframe.copyTo(frame);
		origin.y = 5;
		for (int i = 0; i < video_setting_size.area(); i++) {
			if (i % video_setting_size.width == 0) {
				origin.y += 15;
				origin.x = 0;
			}
			else
				origin.x += 12;
			showStr.push_back(this->lv[frame.at<uchar>(i) / 4]);
			putText(image, showStr, origin, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 8, 0);
			showStr = "";
		}
		writerMp4.write(image);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		avg += duration.count();
		printf("進度: %f%%/%f/%d\r", (count++ / this->frameCount) * 100, difftime(time(NULL), t_start), duration.count());
	}
	writerMp4.release();
	system(this->run.c_str());
	remove(this->saveDir.c_str());
	int totalTime = difftime(time(NULL), t_start);
	printf("\nused %02d:%02d/%f", totalTime / 60, totalTime % 60, avg / count);
}

void AsciiArt::advart(bool fill_color, string fill_word) {
	initVideo();
	Size video_setting_size = Size(158, 71);
	VideoWriter writerMp4(this->saveDir, this->encoding, this->frameFPS, this->setVideoSize);
	auto count = 0.0; //設置進度
	Point origin;
	const time_t t_start = time(nullptr);

	Mat frame, gray;
	Mat image;
	string showStr = "";
	int avg = 0;

	while (this->cap.isOpened()) {
		auto start = std::chrono::high_resolution_clock::now();
		this->cap >> frame;
		image = Mat(this->setVideoSize, CV_8UC4, USAGE_ALLOCATE_DEVICE_MEMORY);
		image.setTo(Scalar(0, 0, 0));
		if (frame.empty()) break;
		resize(frame, frame, this->dsize);
		cvtColor(frame, gray, COLOR_RGB2GRAY);
		origin.y = 5;
		for (int i = 0; i < video_setting_size.area(); i++) {
			string showStr = "";
			if (i % video_setting_size.width == 0) {
				origin.y += 15;
				origin.x = 0;
			}
			else
				origin.x += 12;
			fill_color ? showStr = fill_word : showStr += this->lv[gray.at<uchar>(i) / 4];
			putText(image, showStr, origin, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(frame.at<Vec3b>(i)[0], frame.at<Vec3b>(i)[1], frame.at<Vec3b>(i)[2]), 1, 8, 0);
		}
		writerMp4.write(image);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		avg += duration.count();
		printf("進度: %f%%/%f/%d\r", (count++ / this->frameCount) * 100, difftime(time(NULL), t_start), duration.count());
	}
	writerMp4.release();
	system(this->run.c_str());
	remove(this->saveDir.c_str());
	int totalTime = difftime(time(NULL), t_start);
	printf("\nused %02d:%02d/%f", totalTime / 60, totalTime % 60, avg / count);
}

void AsciiArt::beautifulart() {
	time_t t_start, t_end;
	int count = 0;

	Size video_setting_size = Size(240, 67);
	
	initVideo(Size(video_setting_size.width * 8, video_setting_size.height * 16 - 16), video_setting_size);
	VideoWriter writer(this->saveDir, this->encoding, this->frameFPS, this->setVideoSize);
	
	t_start = time(NULL);
	vector<Mat> mat_arr;
	for (int i = 0; i <= 64; i++)
		mat_arr.emplace_back(imread("font\\font\\" + to_string(i) + ".png"));
	while (this->cap.isOpened()) {
		Mat frame;
		this->cap >> frame;
		if (frame.empty()) break;//125x35
		resize(frame, frame, this->dsize, 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		Mat vertical, horizontal;

		for (int i = 0; i < video_setting_size.area(); i++) {
			if (i % video_setting_size.width == 0) {
				vertical.push_back(horizontal);
				horizontal = mat_arr[0];
			}
			else
				hconcat(horizontal, mat_arr[frame.at<uchar>(i) / 4], horizontal);
		}
		writer.write(vertical);
		printf("進度: %f%%/%f %d %d\r", (count++ / this->frameCount) * 100, difftime(time(NULL), t_start), vertical.cols, vertical.rows);
	}
	writer.release();
	t_end = time(NULL);
	system(this->run.c_str());
	remove(this->saveDir.c_str());
	int totalTime = difftime(t_end, t_start);
	printf("used %02d:%02d", totalTime / 60, totalTime % 60);
}

void AsciiArt::beautiadv() {
	time_t t_start, t_end;
	int count = 0;

	Size video_setting_size = Size(480, 240);

	initVideo(Size(video_setting_size.width * 4 + 8, video_setting_size.height * 4), video_setting_size);
	VideoWriter writer(this->saveDir, this->encoding, this->frameFPS, this->setVideoSize);

	t_start = time(NULL);
	string folder_path = "font\\braille";
	vector<string> file_names;
	map<string, Mat> img;
	glob(folder_path, file_names);
	
	
	for (auto& row : file_names) {
		Advstring spilit(row);
		img[spilit.split("\\")[2]] = imread(row);
	}
	
	while (this->cap.isOpened()) {
		Mat frame;
		this->cap >> frame;
		if (frame.empty()) break;//125x35
		resize(frame, frame, this->dsize, 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		Mat vertical, horizontal;
		vector<vector<string>> deep_arr;
		for (int i = 0; i < video_setting_size.height; i++) {
			vector<string> deep;
			Scalar avg = mean(frame);
			int avg_color = avg.val[0];

			for (int j = 1; j < video_setting_size.width; j += 2) {
				if (frame.at<uchar>(i, j - 1) > avg_color) {
					if (frame.at<uchar>(i, j) > avg_color)
						deep.emplace_back("m");
					else
						deep.emplace_back("y");
				}
				else {
					if (frame.at<uchar>(i, j) > avg_color)
						deep.emplace_back("z");
					else
						deep.emplace_back("k");
				}
			}
			deep_arr.emplace_back(deep);
		}

		for (int i = 3; i < deep_arr.size(); i += 4) {
			horizontal = img["kkkk.png"];
			for (int j = 0; j < video_setting_size.height; j++) {
				hconcat(horizontal, img[deep_arr[i - 3][j] + deep_arr[i - 2][j] + deep_arr[i - 1][j] + deep_arr[i][j] + ".png"], horizontal);
			}
			vertical.push_back(horizontal);
		}
		writer.write(vertical);
		printf("進度: %f%%/%f %d %d\r", (count++ / this->frameCount) * 100, difftime(time(NULL), t_start), vertical.cols, vertical.rows);
	}
	writer.release();
	t_end = time(NULL);
	system(this->run.c_str());
	remove(this->saveDir.c_str());
	int totalTime = difftime(t_end, t_start);
	printf("used %02d:%02d", totalTime / 60, totalTime % 60);
}