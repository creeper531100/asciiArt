#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/bgsegm.hpp>
#include "ascii.h"
#include "tools.h"
#include <windows.h>
#include <nlohmann/json.hpp>

using namespace cv;
using namespace dnn;
using namespace std;
using namespace bgsegm;
using json = nlohmann::json;

void AsciiArt::network() {
	Net net = readNetFromCaffe("MobileNetSSD_deploy.prototxt", "MobileNetSSD_deploy.caffemodel");
	setUseOptimized(1);
	int count = 0;
	vector<vector<int>> point;
	vector<int> xy, numArr;
	Mat imgs = imread("face.png");
	initVideo(Size(CAP_PROP_FRAME_WIDTH, CAP_PROP_FRAME_HEIGHT));
	VideoWriter writerMp4(this->saveDir_, this->encoding, this->frameFPS, this->getVideoSize);
	while (this->cap.isOpened()) {
		Mat mat;
		this->cap >> mat;
		int num = 0;
		if (mat.empty()) break;
		if (count % 3 == 0) {
			numArr.clear();
			point.clear();
			Mat inputBlob = blobFromImage(mat, 1.0, mat.size(), Scalar(104.0, 177.0, 123.0), false);
			net.setInput(inputBlob, "data");
			Mat detection = net.forward();
			Mat detectionMat(detection.size[2], detection.size[3], CV_32F, (float*)detection.data);
			float confidenceThreshould = 0.75;
			for (int i = 0; i < detectionMat.rows; i++) {
				float confidence = detectionMat.at<float>(i, 2);
				if (confidence > confidenceThreshould) {
					xy.emplace_back(static_cast<int>(detectionMat.at<float>(i, 3) * mat.cols));
					xy.emplace_back(static_cast<int>(detectionMat.at<float>(i, 4) * mat.rows));
					xy.emplace_back(static_cast<int>(detectionMat.at<float>(i, 5) * mat.cols));
					xy.emplace_back(static_cast<int>(detectionMat.at<float>(i, 6) * mat.rows));
					point.emplace_back(xy);
					xy.clear();
					numArr.emplace_back(num++);
				}
			}
		}
		//x0 = 0, y0 = 1, x1 = 2, y1 = 3;
		for (auto row : point) try {
			Mat smallImage;
			resize(imgs, smallImage, Size(row[2] - row[0], row[3] - row[1]));
			Rect roi(Point(row[0], row[1]), Size(smallImage.cols, smallImage.rows));
			Mat destinationROI = mat(roi);
			smallImage.copyTo(destinationROI);
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
		imshow("mat", mat);
		waitKey(1);
		printf("進度: %f%%\r", (count++ / this->frameCount) * 100);
		writerMp4.write(mat);
	}
	writerMp4.release();
}

void AsciiArt::detectionCar1() {
	VideoCapture capture("C:\\Users\\creep\\OneDrive\\文件\\oCam\\A.mp4");
	Mat frame;
	Mat gray;
	Mat gray_dilate1;
	Mat background, foreground, foreground_BW;
	Mat mid_filer;
	int num = 0;
	Mat frame_0;
	while (1) {
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		capture >> frame;

		imshow("frame_resize", frame);
		cvtColor(frame, gray, COLOR_RGB2GRAY);
		for (int i = 0; i < 10; ++i) {
			medianBlur(gray, gray, 5);//N次除噪
		}

		if (num == 0) {
			background = gray.clone();
		}
		absdiff(gray, background, foreground);

		Mat element = getStructuringElement(MORPH_RECT, Size(3, 2));

		threshold(foreground, foreground_BW, 20, 255, 0);
		medianBlur(foreground_BW, mid_filer, 5);//第二次除噪，二值化需要去除噪點

		dilate(mid_filer, gray_dilate1, element);

		findContours(gray_dilate1, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			boundRect[i] = boundingRect(contours[i]);
			if (boundRect[i].width > 27 && boundRect[i].height > 27 && boundRect[i].width < 175 && boundRect[i].height < 175)
				rectangle(frame, boundRect[i], Scalar(0, 255, 0));
		}
		imshow("gray_dilate1", frame);
		waitKey(22);
		num++;
	}
}

void AsciiArt::detectionCar2() {

	VideoCapture capture("D:\\下載\\ALIB.mp4");
	Mat bsmk, frame;
	int count = 0;
	int locks = 0;
	int b = 0;
	bool meichongfu = true;

	Ptr<BackgroundSubtractor> removeBg = createBackgroundSubtractorMOG2(5, 30, true);
	while (1) {
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		capture >> frame;
		removeBg->apply(frame, bsmk, 0.01);

		threshold(bsmk, bsmk, 254, 255, 0);
		medianBlur(bsmk, bsmk, 5);//第二次除噪降低雜訊

		Mat element = getStructuringElement(MORPH_RECT, Size(8, 5));
		dilate(bsmk, bsmk, element);
		medianBlur(bsmk, bsmk, 5);//第二次除噪降低雜訊

		line(frame, Point(0, 520), Point(600, 520), Scalar(255, 0, 255), 1, LINE_AA);

		findContours(bsmk, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
		vector<Rect> boundRect(contours.size());
		int a = 0;
		for (int i = 0; i < contours.size(); i++) {
			boundRect[i] = boundingRect(contours[i]);
			if (boundRect[i].width > 55 && boundRect[i].height > 55) {
				rectangle(frame, boundRect[i], Scalar(0, 255, 0));
				line(frame, Point(boundRect[i].x, boundRect[i].y + boundRect[i].width / 2), Point(boundRect[i].x + boundRect[i].height, boundRect[i].y + boundRect[i].width / 2), Scalar(255, 0, 255), 1, LINE_AA);
				putText(frame, "num:" + to_string(i + 1), Point(boundRect[i].x, boundRect[i].y), FONT_HERSHEY_COMPLEX,
					1, Scalar(0, 255, 0));
				if (boundRect[i].y + boundRect[i].width / 2 > 520)
					a++;
			}
		}
		if (locks != a) {
			if (a > 1) {
				b = b + (a - locks);
				locks = a;
				meichongfu = false;
			}
			else if (meichongfu) {
				locks = a;
				b += locks;
			}
			else {
				locks -= a;
				meichongfu = true;
			}
		}
		putText(frame, to_string(b), Point(20, 20), FONT_HERSHEY_COMPLEX,
			1, Scalar(0, 255, 0));

		imshow("gray_dilate1", frame);
		waitKey(1);
		count++;
	}
}