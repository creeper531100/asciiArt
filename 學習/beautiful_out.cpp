#include <windows.h>
#include "ascii.h"
#include "string.h"
using namespace cv;
using namespace std;

void mask_img(Mat* pFrame) {
	Mat contours, hierarchy;
	Scalar avg = mean(*pFrame);
	int avg_color = avg.val[0];
	Canny(*pFrame, *pFrame, avg_color, avg_color);
	inRange(*pFrame, Scalar(0, 0, 0), Scalar(254, 254, 254), *pFrame);
}
//A
void AsciiArt::beautifulart() {
	time_t t_start = time(NULL);
	int count = 0;
	Size video_setting_size = Size(240, 67);
	initVideo(Size(video_setting_size.width * 8, video_setting_size.height * 16 - 16), video_setting_size);
	VideoWriter writer(this->saveDir, this->encoding, this->frameFPS, this->setVideoSize);
	vector<Mat> mat_arr;
	for (int i = 0; i <= 64; i++)
		mat_arr.emplace_back(imread("font\\font\\" + to_string(i) + ".png"));

	Mat* pFrame = new Mat;
	run_ascii_art([&]() {
		Mat vertical, horizontal;

		for (int i = 0; i < video_setting_size.area(); i++) {
			if (i % video_setting_size.width == 0) {
				vertical.push_back(horizontal);
				horizontal = mat_arr[0];
			}
			else
				hconcat(horizontal, mat_arr[pFrame->at<uchar>(i) / 4], horizontal);
		}
		writer.write(vertical);
		printf("進度: %f%%/%f %d %d\r", (count++ / this->frameCount) * 100, difftime(time(NULL), t_start), vertical.cols, vertical.rows);
		}, pFrame);

	writer.release();
	print_output_info(t_start);
}

void AsciiArt::beautiadv(int sw) {
	time_t t_start = time(NULL);
	int count = 0;
	Size video_setting_size = Size(480, 240);
	initVideo(Size(video_setting_size.width * 4 + 8, video_setting_size.height * 4), video_setting_size);
	VideoWriter writer(this->saveDir, this->encoding, this->frameFPS, this->setVideoSize);
	string folder_path = "font\\braille";
	vector<string> file_names;
	map<string, Mat> img;
	glob(folder_path, file_names);
	for (auto& row : file_names) {
		Advstring spilit(row);
		img[spilit.split("\\")[2]] = imread(row);
	}

	Mat* pFrame = new Mat;
	run_ascii_art([&]() {
		Mat vertical, horizontal;
		if (sw == 1)
			mask_img(pFrame);
		vector<vector<string>> deep_arr = braille_create(video_setting_size, pFrame);
		for (int i = 3; i < deep_arr.size(); i += 4) {
			horizontal = img["kkkk.png"];
			for (int j = 0; j < video_setting_size.height; j++)
				hconcat(horizontal, img[deep_arr[i - 3][j] + deep_arr[i - 2][j] + deep_arr[i - 1][j] + deep_arr[i][j] + ".png"], horizontal);
			vertical.push_back(horizontal);
		}
		writer.write(vertical);
		printf("進度: %f%%/%f %d %d\r", (count++ / this->frameCount) * 100, difftime(time(NULL), t_start), vertical.cols, vertical.rows);
		}, pFrame);
	writer.release();
	print_output_info(t_start);
}

