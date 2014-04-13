#include "TrafficLight.h"
#include "TrafficLightDetector.h"

TrafficLightDetector::TrafficLightDetector() {
	lightState = UNDEFINED;
}

/*
 * Detection in grayscale by brightness.
 */
void TrafficLightDetector::brightnessDetect(const Mat &input, const Context context, Mat &output) {
	cvtColor(input, output, CV_RGB2GRAY);
	blur(output, output, Size(4,4));
	Mat cannyRoi = output(context.coords);
	Canny(cannyRoi, cannyRoi, 30, 100);
	
	/*int dilation_size = 1;
	Mat element = getStructuringElement( MORPH_ELLIPSE,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
	dilate(cannyRoi, cannyRoi, element);*/

	Mat cannyForContours;
	cannyRoi.copyTo(cannyForContours);
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
	findContours(cannyForContours, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));

	cvtColor(output, output, CV_GRAY2RGB);
	rectangle(output, context.coords, MY_COLOR_RED);

	/// Draw contours
	Mat roi = output(context.coords);
	for (int i = 0; i < contours.size(); i++) {
		if (hierarchy[i][3] >= 0) {
			// TODO check shape & color inside && contourArea(contours.at(i))
			drawContours(roi, contours, i, MY_COLOR_GREEN, -1, 8, hierarchy, 0, Point());
		}
	}
}

void TrafficLightDetector::brightnessDetect(const Mat &input, Mat &output) {
	cvtColor(input, output, CV_RGB2GRAY);

	int dilation_size = 1;
	Mat roi = output(Rect(RECT_TOP_LEFT, RECT_BOTTOM_RIGHT));
	threshold(roi, roi, 0, 255, THRESH_BINARY | THRESH_OTSU);

	bool display_red = (getBrightnessRatioInCircle(output, RED_CENTER, LIGHT_RADIUS) > 0.5);
	bool display_yellow = (getBrightnessRatioInCircle(output, YELLOW_CENTER, LIGHT_RADIUS) > 0.5);
	bool display_green = (getBrightnessRatioInCircle(output, GREEN_CENTER, LIGHT_RADIUS) > 0.5);

	cvtColor(output, output, CV_GRAY2RGB);

	int currentLightsCode = getCurrentLightsCode(display_red, display_yellow, display_green);

	lightState = determineState(lightState, currentLightsCode);

	drawTrafficLights(output, lightState);
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

	return ((double)whitePoints) / (whitePoints + blackPoints);
}

int getCurrentLightsCode(bool display_red, bool display_yellow, bool display_green) {
	return (int)display_red + 2 * ((int) display_yellow) + 4 * ((int) display_green); 
}

LightState determineState(LightState previousState, int currentLightsCode) {
	//printf("Previous state: %d, currentCode: %d, Switched state to %d\n", previousState, currentLightsCode, STATE_TRANSITION_MATRIX[previousState][currentLightsCode]);
	return STATE_TRANSITION_MATRIX[previousState][currentLightsCode];	
}

void drawTrafficLights(Mat &output, LightState state) {
	switch (state) {
	case GREEN:
		circle(output, GREEN_CENTER, LIGHT_RADIUS, MY_COLOR_GREEN, -1);
		break;
	case YELLOW:
		circle(output, YELLOW_CENTER, LIGHT_RADIUS, MY_COLOR_YELLOW, -1);
		break;
	case RED:
		circle(output, RED_CENTER, LIGHT_RADIUS, MY_COLOR_RED, -1);
		break;
	case REDYELLOW:
		circle(output, YELLOW_CENTER, LIGHT_RADIUS, MY_COLOR_YELLOW, -1);
		circle(output, RED_CENTER, LIGHT_RADIUS, MY_COLOR_RED, -1);
		break;
	default:
		printf("State not defined.\n");
		break;
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