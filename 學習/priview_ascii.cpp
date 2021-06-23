#include <windows.h>
#include "ascii.h"
#include "tools.h"
using namespace cv;
using namespace std;

void video_interval(chrono::time_point<chrono::system_clock>* c_start, int& frame_interval, int& error) {
	std::this_thread::sleep_until(*c_start + chrono::microseconds(frame_interval));
	wstring info = to_wstring(frame_interval) + L" / " + to_wstring(error);
	SetConsoleTitle(info.c_str());
	*c_start = chrono::system_clock::now();
}

HANDLE&& handle_console(wchar_t** screen, cv::Size& dsize) {
	*screen = new wchar_t[dsize.area()];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	return move(hConsole);
}

void AsciiArt::asciiArt() {
	Size video_setting_size = Size(237, 64);
	initVideo(Size(CAP_PROP_FRAME_WIDTH, CAP_PROP_FRAME_HEIGHT), video_setting_size);
	int error = 0;
	Mat* pFrame = new Mat;
	DWORD dwBytesWritten = 0;
	HANDLE hConsole = handle_console(&this->screen, this->dsize);
	auto start = chrono::system_clock::now();
	run_ascii_art([&]() {
		for (int j = 0; j < video_setting_size.area(); j++) {
			this->screen[j] = this->lv[pFrame->at<uchar>(j) / 4];
		}
		WriteConsoleOutputCharacter(hConsole, this->screen, video_setting_size.area(), { 0, 0 }, &dwBytesWritten);
		video_interval(&start, this->frame_interval, error);
		}, pFrame);
	delete[] this->screen;
}

void AsciiArt::asciiAdvArt() {
	Size video_setting_size = Size(240, 120);
	initVideo(Size(CAP_PROP_FRAME_WIDTH, CAP_PROP_FRAME_HEIGHT), video_setting_size);
	init_word();
	int error = 0;
	Mat* pFrame = new Mat;
	DWORD dwBytesWritten = 0;
	HANDLE hConsole = handle_console(&this->screen, this->dsize);
	auto start = chrono::system_clock::now();

	run_ascii_art([&]() {
		vector<vector<string>> deep_arr = braille_create(video_setting_size, pFrame);
		wstring basic_string;
		for (int i = 3; i < deep_arr.size(); i += 4) {
			for (int j = 0; j < video_setting_size.height; j++) {
				basic_string.push_back(this->map_pairs[deep_arr[i - 3][j] + deep_arr[i - 2][j] + deep_arr[i - 1][j] + deep_arr[i][j]]);
			}
		}
		WriteConsoleOutputCharacterW(hConsole, basic_string.c_str(), video_setting_size.area(), { 0, 0 }, &dwBytesWritten);
		video_interval(&start, this->frame_interval, error);
		}, pFrame);
	delete[] this->screen;
}