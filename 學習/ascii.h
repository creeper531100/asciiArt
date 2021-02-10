#pragma once
#ifndef ASCIIART_H 
#define ASCIIART_H 
#include <iostream>

using namespace std;

string aHash(cv::Mat);
vector<int> cmpHash(string, string);
void createJson(string, string);

class AsciiArt {
public:
	AsciiArt(string , string);
	void asciiArt();
	void advascii();
	void advart();
private:
	string inputDir_;
	string saveDir_;
	cv::VideoCapture cap;
	void initVideo(cv::Size, cv::Size);
	string lv;
	double frameCount;
	double frameFPS;
	int encoding;
	cv::Size videoSize;
	cv::Size dsize;
};

#endif
