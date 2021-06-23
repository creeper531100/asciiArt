#include <windows.h>
#include "ascii.h"
using namespace cv;
using namespace std;

void AsciiArt::advascii() {
	time_t t_start = time(nullptr);
	initVideo();
	auto count = 0.0;
	Point origin;
	Size video_setting_size = Size(158, 71);
	VideoWriter writerMp4(this->saveDir, this->encoding, this->frameFPS, this->setVideoSize);

	Mat image;
	string showStr = "";

	Mat* pFrame = new Mat;
	run_ascii_art([&]() {
		image = Mat(this->setVideoSize, CV_8U, USAGE_ALLOCATE_DEVICE_MEMORY);
		image.setTo(Scalar(0, 0, 0));
		origin.y = 5;
		for (int i = 0; i < video_setting_size.area(); i++) {
			if (i % video_setting_size.width == 0) {
				origin.y += 15;
				origin.x = 0;
			}
			else
				origin.x += 12;
			showStr += this->lv[pFrame->at<uchar>(i) / 4];
			putText(image, showStr, origin, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 8, 0);
			showStr = "";
		}
		writerMp4.write(image);
		printf("進度: %f%%\r", (count++ / this->frameCount) * 100);
		}, pFrame);
	writerMp4.release();
	print_output_info(t_start);
}

void AsciiArt::advart(bool fill_color, string fill_word) {
	initVideo();
	Size video_setting_size = Size(158, 71);
	VideoWriter writerMp4(this->saveDir, this->encoding, this->frameFPS, this->setVideoSize);
	auto count = 0.0; //設置進度
	Point origin;
	time_t t_start = time(nullptr);

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
	print_output_info(t_start);
}
