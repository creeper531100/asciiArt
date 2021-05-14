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
	void advascii();
	void advart(bool, string);
	void network();
	void detectionCar1();
	void detectionCar2();
private:
	string inputDir;
	string saveDir;
	string run;
	cv::VideoCapture cap;
	void initVideo(cv::Size setVideoSize = cv::Size(1920, 1080),
		cv::Size setDsize = cv::Size(158, 71));
	string lv;
	double frameCount;
	double frameFPS;
	int encoding;
	cv::Size setVideoSize;
	cv::Size getVideoSize;
	cv::Size dsize;
};
#endif