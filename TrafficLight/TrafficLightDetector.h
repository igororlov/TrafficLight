#ifndef TRAFFIC_LIGHT_DETECTOR_H
#define TRAFFIC_LIGHT_DETECTOR_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>

#define MY_COLOR_RED Scalar(0,0,255)
#define MY_COLOR_GREEN Scalar(0,255,0)
#define MY_COLOR_YELLOW Scalar(60,255,255)

using namespace cv;

enum LightState { GREEN, YELLOW, RED, REDYELLOW, UNDEFINED };

const LightState STATE_TRANSITION_MATRIX[5][8] = { 
	{ GREEN, RED, YELLOW, REDYELLOW, GREEN, GREEN, GREEN, UNDEFINED },
	{ YELLOW, RED, YELLOW, REDYELLOW, GREEN, GREEN, GREEN, UNDEFINED },
	{ RED, RED, REDYELLOW, REDYELLOW, GREEN, GREEN, GREEN, UNDEFINED },
	{ REDYELLOW, REDYELLOW, REDYELLOW, REDYELLOW, GREEN, GREEN, GREEN, UNDEFINED },
	{ UNDEFINED, RED, YELLOW, REDYELLOW, GREEN, GREEN, GREEN, UNDEFINED }
};

double getBrightnessRatioInCircle(const Mat &input, const Point center, const int radius);
int getCurrentLightsCode(bool display_red, bool display_yellow, bool display_green);
LightState determineState(LightState previousState, int currentLightsCode);
void drawTrafficLights(Mat &output, LightState state);

class Context {
public:
	Context(Rect aCoords, Mat aStructure) : coords(aCoords), trafficLightStructure(aStructure) {}
	Rect coords;
	Mat trafficLightStructure;
};

class TrafficLightDetector {
public:
	TrafficLightDetector();
	void brightnessDetect(const Mat &input, const Context context, Mat &output);
	void brightnessDetect(const Mat &input, const int thresh, Mat &output);
	void colorDetect(const Mat &input, Mat &output, const Rect coords, int Hmin, int Hmax);

private:
	LightState lightState;
};

#endif