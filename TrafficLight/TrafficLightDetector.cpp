#include "TrafficLight.h"
#include "TrafficLightDetector.h"

TrafficLightDetector::TrafficLightDetector() {
	for (int i = 0; i < contexts.size(); i++) {
		contexts[i].lightState = UNDEFINED;
	}
}

void TrafficLightDetector::brightnessDetect(const Mat &input, Mat &output) {
	
	cvtColor(input, output, CV_RGB2GRAY);

	for (int i = 0; i < contexts.size(); i++) {
		Context context = contexts[i];
		Mat roi = output(Rect(context.topLeft, context.botRight));
		threshold(roi, roi, 0, 255, THRESH_BINARY | THRESH_OTSU);

		bool display_red = false;
		bool display_yellow = false;
		bool display_green = false;

		if (getBrightnessRatioInCircle(output, context.redCenter, context.lampRadius) > 0.5) {
			display_red = true;
		}

		if (getBrightnessRatioInCircle(output, context.yellowCenter, context.lampRadius) > 0.5) {
			display_yellow = true;
		}

		if (getBrightnessRatioInCircle(output, context.greenCenter, context.lampRadius) > 0.5) {
			display_green = true;
		}

		int currentLightsCode = getCurrentLightsCode(display_red, display_yellow, display_green);
		contexts[i].lightState = determineState(contexts[i].lightState, currentLightsCode);
		
		// Make ROI black
		roi.setTo(Scalar(0));
	}

	cvtColor(output, output, CV_GRAY2RGB);

	drawTrafficLights(output);
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

	double ratio = ((double)whitePoints) / (whitePoints + blackPoints);

	//printf("ratio: %f\nwhitePoints: %d\nlackPoints: %d\n\n", ratio, whitePoints, blackPoints);

	return ratio;
}

int getCurrentLightsCode(bool display_red, bool display_yellow, bool display_green) {
	return (int)display_red + 2 * ((int) display_yellow) + 4 * ((int) display_green); 
}

LightState determineState(LightState previousState, int currentLightsCode) {
	//printf("Previous state: %d, currentCode: %d, Switched state to %d\n", previousState, currentLightsCode, STATE_TRANSITION_MATRIX[previousState][currentLightsCode]);
	return STATE_TRANSITION_MATRIX[previousState][currentLightsCode];	
}

void TrafficLightDetector::drawTrafficLights(Mat &output) {

	for (int i = 0; i < contexts.size(); i++) {
		
		rectangle(output, contexts[i].topLeft, contexts[i].botRight, MY_COLOR_WHITE, 2);

		switch (contexts[i].lightState) {
		case GREEN:
			circle(output, contexts[i].greenCenter, contexts[i].lampRadius, MY_COLOR_GREEN, -1);
			break;
		case YELLOW:
			circle(output, contexts[i].yellowCenter, contexts[i].lampRadius, MY_COLOR_YELLOW, -1);
			break;
		case RED:
			circle(output, contexts[i].redCenter, contexts[i].lampRadius, MY_COLOR_RED, -1);
			break;
		case REDYELLOW:
			circle(output, contexts[i].yellowCenter, contexts[i].lampRadius, MY_COLOR_YELLOW, -1);
			circle(output, contexts[i].redCenter, contexts[i].lampRadius, MY_COLOR_RED, -1);
			break;
		default:
			//printf("State not defined.\n");
			break;
		}
	}
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