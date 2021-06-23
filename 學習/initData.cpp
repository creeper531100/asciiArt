#include "initData.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

void formatJsonData::initData() try {
	json jsonfile;
	ifstream in("setting.json");
	in >> jsonfile;
	this->getDetectRangeHeight = jsonfile["detectRangeHeight"];
	this->getDetectRangeWidth = jsonfile["detectRangeWidth"];
	this->getTriggerLineHeight = jsonfile["triggerLineHeight"];
	this->getTriggerLineHeightEnd = jsonfile["triggerLineHeightEnd"];
	this->getStartTriggerLineWidth = jsonfile["startTriggerLineWidth"];
	this->getEndTriggerLineWidth = jsonfile["endTriggerLineWidth"];
	this->getHistory = jsonfile["history"];
	this->getVarThreshould = jsonfile["varThreshould"];
	this->getDetectShadows = jsonfile["detectShadows"];
	this->getLearningRate = jsonfile["learningRate"];
	this->getElementKsizeWidth = jsonfile["elementKsizeWidth"];
	this->getElementKsizeHeight = jsonfile["elementKsizeHeight"];
	this->getMedianBlurKsize1 = jsonfile["medianBlurKsize1"];
	this->getMedianBlurKsize2 = jsonfile["medianBlurKsize2"];
	this->getPutTextX = jsonfile["putTextX"];
	this->getPutTextY = jsonfile["putTextY"];
	this->getWaitKey = jsonfile["waitKey"];
	this->getColorR = jsonfile["colorR"];
	this->getColorG = jsonfile["colorG"];
	this->getColorB = jsonfile["colorB"];

}
catch (exception e) {
	cout << "¥¼Åª¨ìsetting.jsonÀÉ®×!" << endl << e.what() << endl;
	this->getDetectRangeHeight = 55;
	this->getDetectRangeWidth = 55;
	this->getTriggerLineHeight = 520;
	this->getTriggerLineHeightEnd = 560;
	this->getStartTriggerLineWidth = 0;
	this->getEndTriggerLineWidth = 600;
	this->getHistory = 5;
	this->getVarThreshould = 30;
	this->getDetectShadows = true;
	this->getLearningRate = 0.01;
	this->getElementKsizeWidth = 8;
	this->getElementKsizeHeight = 5;
	this->getMedianBlurKsize1 = 5;
	this->getMedianBlurKsize2 = 5;
	this->getPutTextX = 20;
	this->getPutTextY = 20;
	this->getWaitKey = 1;
	this->getColorR = 0;
	this->getColorG = 255;
	this->getColorB = 0;
}
