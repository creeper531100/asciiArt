#pragma once
#ifndef INITDATA_H 
#define INITDATA_H 
class formatJsonData {
	/*enum settingData {
		GET_DETECT_RANGE_WIDTH,
		GET_TRIGGER_LINE_HEIGHT,
		GET_START_TRIGGER_LINE_WIDTH,
		GET_END_TRIGGER_LINE_WIDTH,
		GET_HISTORY,
		GET_VAR_THRESHOULD,
		GET_DETECT_SHADOWS,
		GET_LEARNING_RATE,
		GET_ELEMENT_KSIZE_WIDTH,
		GET_ELEMENT_KSIZE_HEIGHT,
		GET_MEDIANBLUR_KSIZE1,
		GET_MEDIANBLUR_KSIZE2,
		GET_PUT_TEXTX,
		GET_PUT_TEXTY,
		GET_WAIT_KEY
	};*/
public:
	void initData();
	int getDetectRangeWidth;
	int getDetectRangeHeight;
	int getTriggerLineHeight;
	int getTriggerLineHeightEnd;
	int getStartTriggerLineWidth;
	int getEndTriggerLineWidth;
	int getHistory;
	int getVarThreshould;
	int getDetectShadows;
	float getLearningRate;
	int getElementKsizeWidth;
	int getElementKsizeHeight;
	int getMedianBlurKsize1;
	int getMedianBlurKsize2;
	int getPutTextX;
	int getPutTextY;
	int getWaitKey;
	int getColorR;
	int getColorG;
	int getColorB;
};
#endif
