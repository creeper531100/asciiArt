#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/bgsegm.hpp>
#include "ascii.h"
#include "initData.h"

using namespace cv;
using namespace dnn;
using namespace std;
using namespace bgsegm;
using json = nlohmann::json;

void AsciiArt::network() {
	enum positive_xy {
		x0, y0, x1, y1
	};
	enum net_forward {
		NET_SIMILAR = 2, NET_ROWS = 2, NET_COLS, NET_X0, NET_Y0, NET_X1, NET_Y1
	};
	
	Net net = readNetFromCaffe("MobileNetSSD_deploy.prototxt", "MobileNetSSD_deploy.caffemodel");
	
	setUseOptimized(true);
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
			Mat detectionMat(detection.size[NET_ROWS], detection.size[NET_COLS], CV_32F, (float*)detection.data);
			for (int i = 0; i < detectionMat.rows; i++) {
				float confidence = detectionMat.at<float>(i, NET_SIMILAR);
				if (confidence > 0.75) {
					xy.emplace_back(static_cast<int>(detectionMat.at<float>(i, NET_X0) * mat.cols));
					xy.emplace_back(static_cast<int>(detectionMat.at<float>(i, NET_Y0) * mat.rows));
					xy.emplace_back(static_cast<int>(detectionMat.at<float>(i, NET_X1) * mat.cols));
					xy.emplace_back(static_cast<int>(detectionMat.at<float>(i, NET_Y1) * mat.rows));
					point.emplace_back(xy);
					xy.clear();
					numArr.emplace_back(num++);
				}
			}
		}
		for (auto row : point) try {
			Mat smallImage;
			resize(imgs, smallImage, Size(row[x1] - row[x0], row[y1] - row[y0]));
			Rect roi(Point(row[x0], row[y0]), Size(smallImage.cols, smallImage.rows));
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
	initVideo(Size(CAP_PROP_FRAME_WIDTH, CAP_PROP_FRAME_HEIGHT));

	
	Mat gray;
	Mat gray_dilate1;
	Mat background, foreground, foreground_BW;
	Mat mid_filer;
	int num = 0;
	Mat frame_0;
	while (this->cap.isOpened()) {
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		
		Mat frame;
		this->cap >> frame;

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
	initVideo(Size(CAP_PROP_FRAME_WIDTH, CAP_PROP_FRAME_HEIGHT));
	formatJsonData getData;
	getData.initData();
	
	int count = 0;
	int lastCount = 0;
	int carCount = 0;
	bool meichongfu = true;

	Ptr<BackgroundSubtractor> removeBg = createBackgroundSubtractorMOG2(getData.getHistory, getData.getVarThreshould, getData.getDetectShadows);
	while (1) {
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		Mat bsmk, frame;
		this->cap >> frame;
		
		removeBg->apply(frame, bsmk, getData.getLearningRate);

		threshold(bsmk, bsmk, 254, 255, 0);
		medianBlur(bsmk, bsmk, getData.getMedianBlurKsize1);//第二次除噪降低雜訊
		
		Mat element = getStructuringElement(MORPH_RECT, Size(getData.getElementKsizeWidth, getData.getElementKsizeHeight));
		dilate(bsmk, bsmk, element);
		
		medianBlur(bsmk, bsmk, getData.getMedianBlurKsize2);//第二次除噪降低雜訊
		
		line(frame, Point(getData.getStartTriggerLineWidth, getData.getTriggerLineHeight), Point(getData.getEndTriggerLineWidth, getData.getTriggerLineHeight), Scalar(getData.getColorG, getData.getColorB, getData.getColorR), 1, LINE_AA);

		findContours(bsmk, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
		vector<Rect> boundRect(contours.size());
		int lineCount = 0;
		for (int i = 0; i < contours.size(); i++) {
			boundRect[i] = boundingRect(contours[i]);
			if (boundRect[i].width > getData.getDetectRangeWidth && boundRect[i].height > getData.getDetectRangeHeight) {
				rectangle(frame, boundRect[i], Scalar(getData.getColorR, getData.getColorG, getData.getColorB));
				line(frame, Point(boundRect[i].x, boundRect[i].y + boundRect[i].width / 2), Point(boundRect[i].x + boundRect[i].height, boundRect[i].y + boundRect[i].width / 2), Scalar(getData.getColorG, getData.getColorB, getData.getColorR), 1, LINE_AA);
				putText(frame, "num:" + to_string(i + 1), Point(boundRect[i].x, boundRect[i].y), FONT_HERSHEY_COMPLEX,
					1, Scalar(getData.getColorR, getData.getColorG, getData.getColorB));
				if (boundRect[i].y + boundRect[i].width / 2 > getData.getTriggerLineHeight)
					lineCount++;
			}
		}
		if (lastCount != lineCount) {//假如車輛有變動
			if (lineCount > 1) {//假如汽車大於2輛
				carCount = carCount + (lineCount - lastCount); //將上次的數量減掉這次的數量
				lastCount = lineCount; // lineCount 復值 lastCount
				meichongfu = false;//確認沒重複
			}
			else if (meichongfu) {//假如沒重複
				lastCount = lineCount;// lineCount 復值 lastCount
				carCount += lastCount;//數量增加
			}
			else {
				lastCount -= lineCount;//如果上次超過兩輛，需要將lastCount賦歸
				meichongfu = true;//確認有重複
			}
		}
		putText(frame, to_string(carCount), Point(getData.getPutTextX, getData.getPutTextY), FONT_HERSHEY_COMPLEX,
			1, Scalar(0, 255, 0));

		imshow("gray_dilate1", frame);
		waitKey(getData.getWaitKey);
		count++;
	}
}