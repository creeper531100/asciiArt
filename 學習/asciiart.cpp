#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "ascii.h"
#include "tools.h"
#include <windows.h>

using namespace cv;
using namespace std;

AsciiArt::AsciiArt(string inputDir, string saveDir = nullptr) : inputDir_(std::move(inputDir)),
													   saveDir_(std::move(saveDir)){
	this->inputDir_ = inputDir_;
	this->saveDir_ = saveDir_ + ".mp4";
	this->lv = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
}

void AsciiArt::initVideo(Size setVideoSize = Size(1920, 1080), 
						 Size setDsize = Size(158, 71))
{
	VideoCapture cap(this->inputDir_);
	int error = 0;
	char lv[] = " .'`^,:;l!i><~+_--?][}{)(|/rxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
	if (!cap.isOpened()) {
		std::cerr << "ERROR: Could not open video " << this->inputDir_ << endl;
	}
	system("cls");
	this->cap = cap;
	this->frameCount = cap.get(CAP_PROP_FRAME_COUNT);		   //�]�m�`�i��
	this->frameFPS = cap.get(CAP_PROP_FPS);					  //�]�mFPS
	this->encoding = VideoWriter::fourcc('D', 'I', 'V', 'X');//�]�m�s�X
	this->videoSize = setVideoSize;			//�]�m�v���ѪR��
	this->dsize = setDsize;			   //�]�m���Y�ѪR��
}



void AsciiArt::asciiArt() {
	initVideo();
	time_t c_start, t_start = time(NULL);
	int error = 0;
	auto dsize = Size(237, 62);//�]�m���Y�ѪR��

	while (this->cap.isOpened()) {
		c_start = clock();
		string str;
		Mat frame;
		this->cap >> frame;
		if (frame.empty()) break;
		resize(frame, frame, dsize, 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		for (int i = 0; i < 62; i++) {
			for (int j = 0; j < 237; j++) {
				str.push_back(this->lv[frame.at<uchar>(i, j) / 4]);
			}
			str.push_back('\n');
		}
		printf("%s", str.c_str());
		int delay = difftime(clock(), c_start);
		delay < 30 ? Sleep(30 - delay) : void(error++);
	}
	int totalTime = difftime(time(NULL), t_start);
	printf("used %02d:%02d\nerror:%d", totalTime / 60, totalTime % 60, error);
}

void AsciiArt::advascii() {
	//��l�ƦU���ƾ�
	initVideo(); //�ɤJ�v��
	auto count = 0.0; //�]�m�i��
	Point origin;
	VideoWriter writerMp4(this->saveDir_, this->encoding , this->frameFPS, this->videoSize);
	//�D�{��
	time_t t_start = time(NULL);
	while (this->cap.isOpened()) {
		Mat frame;
		this->cap >> frame;
		Mat image = Mat::zeros(this->videoSize, CV_8UC3);
		image.setTo(Scalar(0, 0, 0));
		if (frame.empty()) break;
		resize(frame, frame, this->dsize, 0, 0, INTER_CUBIC);
		cvtColor(frame, frame, COLOR_RGB2GRAY);
		origin.y = 5;
		for (int i = 0; i < 71; i++) {
			origin.y += 15;
			for (int j = 0; j < 158; j++) {
				origin.x += 12;
				string showStr = convert<string>(this->lv[frame.at<uchar>(i, j) / 4]);
				putText(image, showStr, origin, FONT_HERSHEY_SIMPLEX,
					0.5, Scalar(255, 255, 255),
					1, 8, 0);
			}
			origin.x = 0;
		}
		writerMp4.write(image);
		printf("�i��: %f%%/%f\r", (count++ / this->frameCount) * 100, difftime(time(NULL), t_start));
	}
	writerMp4.release();
	int totalTime = difftime(time(NULL), t_start);
	printf("used %02d:%02d", totalTime / 60, totalTime % 60);
}

void AsciiArt::advart() {
	string path;
	time_t t_start, t_end;
	int count = 0;
	initVideo(Size(1008, 560), Size(125, 35));
	VideoWriter writer(this->saveDir_, this->encoding, this->frameFPS, this->videoSize);
	t_start = time(NULL);
	while (this->cap.isOpened()) {
		Mat frame;
		this->cap >> frame;
		if (frame.empty()) break;//125x35
		resize(frame, frame, this->dsize, 0, 0, INTER_CUBIC);
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
		printf("�i��: %f%%/%f\r", (count++ / frameCount) * 100, difftime(time(NULL), t_start));
	}
	writer.release();
	t_end = time(NULL);
	int totalTime = difftime(t_end, t_start);
	printf("used %02d:%02d", totalTime / 60, totalTime % 60);
}