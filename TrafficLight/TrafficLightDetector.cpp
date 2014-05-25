#include "TrafficLight.h"
#include "TrafficLightDetector.h"

TrafficLightDetector::TrafficLightDetector() {}

LightState TrafficLightDetector::brightnessDetect(const Mat &input) {
	
	Mat tmpImage;
	input.copyTo(tmpImage);

	for (int i = 0; i < contexts.size(); i++) {
		Context context = contexts[i];
		Mat roi = tmpImage(Rect(context.topLeft, context.botRight));
		threshold(roi, roi, 0, 255, THRESH_BINARY | THRESH_OTSU);

		bool display_red = (getBrightnessRatioInCircle(tmpImage, context.redCenter, context.lampRadius) > 0.5);
		bool display_yellow = (getBrightnessRatioInCircle(tmpImage, context.yellowCenter, context.lampRadius) > 0.5);
		bool display_green = (getBrightnessRatioInCircle(tmpImage, context.greenCenter, context.lampRadius) > 0.5);

		int currentLightsCode = getCurrentLightsCode(display_red, display_yellow, display_green);
		contexts[i].lightState = determineState(contexts[i].lightState, currentLightsCode);
		
		// Make ROI black
		roi.setTo(Scalar(0));
	}
	return contexts[0].lightState;
}

double getBrightnessRatioInCircle(const Mat &input, const Point center, const int radius) {

	int whitePoints = 0;
	int blackPoints = 0;

	for (int i = center.x - radius; i < center.x + radius; i++) {
		for (int j = center.y - radius; j < center.y + radius; j++) {
			if ((i - center.x)*(i - center.x) + (j - center.y)*(j - center.y) <= radius*radius) {
				(input.at<uchar>(j,i) == 0) ? blackPoints++ : whitePoints++;
			}
		}
	}
	//printf("Ratio: %f\n", ((double)whitePoints) / (whitePoints + blackPoints));

	return ((double)whitePoints) / (whitePoints + blackPoints);
}

int getCurrentLightsCode(bool display_red, bool display_yellow, bool display_green) {
	return (int)display_red + 2 * ((int) display_yellow) + 4 * ((int) display_green); 
}

LightState determineState(LightState previousState, int currentLightsCode) {
	//printf("Previous state: %d, currentCode: %d, Switched state to %d\n", previousState, currentLightsCode, STATE_TRANSITION_MATRIX[previousState][currentLightsCode]);
	return STATE_TRANSITION_MATRIX[previousState][currentLightsCode];	
}



/*
 *  Attempt to recognize by color tracking in HSV. Detects good only green, but need to
 *  play also with S and V parameters range.
 */
void TrafficLightDetector::colorDetect(const Mat &input, Mat &output, const Rect coords, int Hmin, int Hmax) {
	
	if (input.channels() != 3) {
		return;
	}
	
	Mat hsv, thresholded;
	cvtColor(input, hsv, CV_RGB2HSV, 0);
	inRange(hsv, Scalar(Hmin,0,0), Scalar(Hmax,255,255), thresholded);

	cvtColor(thresholded, thresholded, CV_GRAY2RGB);
	thresholded.copyTo(output);

	rectangle(output, coords, MY_COLOR_RED);
}