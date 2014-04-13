#include "TrafficLight.h"

using namespace cv;

// Variables
Point coordsTopLeft(0,0);
Point coordsBottomRight(10,10);
Rect coords(coordsTopLeft, coordsBottomRight);

static VideoCapture capture;
static TrafficLightDetector detector;
static LucasKanadeTracker tracker;

int main() {
	
	printf("***** Traffic lights app *****\n");
	Mat trafficLightStructure = assignTrafficLightStructure();

	if (capture.open(VIDEO_PATH) == false) {
		printf("Unable to open file...\n");
		exit(1);
	}

	double rate = capture.get(CV_CAP_PROP_FPS); // frames per second
	int delay = 1000 / (int)rate;
	
	namedWindow(MAIN_WINDOW_NAME);

	Mat previous, frame;
	while (capture.read(frame)) {		
		Mat result, concat;
		detector.brightnessDetect(frame, result);
		hconcat(frame, result, concat);
		imshow(MAIN_WINDOW_NAME, concat);

		char c = waitKey(delay);
		if (c == 27) {
			break;
		} else if (c == 32) {
			waitKey(0);
		}
		previous.release();
		previous.data = frame.data;
	}

	capture.release();

	return 0;
}

Mat assignTrafficLightStructure() {
	Mat trafficLightStructure(3, 2, CV_8U);
	trafficLightStructure.at<uchar>(0,0) = 1;
	trafficLightStructure.at<uchar>(0,1) = 0;
	trafficLightStructure.at<uchar>(1,0) = 1;
	trafficLightStructure.at<uchar>(1,1) = 0;
	trafficLightStructure.at<uchar>(2,0) = 1;
	trafficLightStructure.at<uchar>(2,1) = 1;
	return trafficLightStructure;
}
