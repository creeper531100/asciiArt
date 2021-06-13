#pragma once
#include <opencv2/opencv.hpp>
#ifndef ASCIIART_H 
#define ASCIIART_H
using namespace std;

string aHash(cv::Mat);
vector<int> cmpHash(string, string);
void createJson(string, string);


class AsciiArt {
public:
	AsciiArt(string, string);
	void asciiArt();
	void asciiAdvArt();
	void advascii();
	void advart(bool, string);
	void beautifulart();
	void beautiadv();
	void network();
	void detectionCar1();
	void detectionCar2();
	wchar_t* screen;
	cv::Size dsize;
private:
	void run_ascii_art(function<void()>, cv::Mat*);
	void print_output_info(time_t);
	vector<vector<string>>&& braille_create(cv::Size&, cv::Mat*);
	void init_word();
	string inputDir;
	string saveDir;
	string run;
	int frame_interval;
	cv::VideoCapture cap;
	void initVideo(cv::Size setVideoSize = cv::Size(1920, 1080),
		cv::Size setDsize = cv::Size(158, 71));
	string lv;
	map<string, wchar_t> map_pairs;
	double frameCount;
	double frameFPS;
	int encoding;
	cv::Size setVideoSize;
	cv::Size getVideoSize;
};
#endif