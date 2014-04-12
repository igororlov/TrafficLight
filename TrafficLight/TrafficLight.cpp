#include "TrafficLight.h"

using namespace cv;

// Variables
Point coordsTopLeft(0,0);
Point coordsBottomRight(10,10);
Rect coords(coordsTopLeft, coordsBottomRight);

static VideoCapture capture;
static TrafficLightDetector detector;
static LucasKanadeTracker tracker;
static int thresh = 120;

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
	setMouseCallback(MAIN_WINDOW_NAME, mouseCallback);

    namedWindow(SETTINGS_WINDOW_NAME);
	createTrackbar("BrtThresh", SETTINGS_WINDOW_NAME, &thresh, 256);
	//createTrackbar("Hmax", SETTINGS_WINDOW_NAME, &Hmax, HSVmax);

	Mat previous, frame;
	while (capture.read(frame)) {

		//tracker.getNewCoords(previous, frame, coords);
		
		//circle(frame, RED_CENTER, LIGHT_RADIUS, MY_COLOR_RED, 1);
		//circle(frame, YELLOW_CENTER, LIGHT_RADIUS, MY_COLOR_YELLOW, 1);
		//circle(frame, GREEN_CENTER, LIGHT_RADIUS, MY_COLOR_GREEN, 1);
		
		Mat result, concat;
		//Context context(coords, trafficLightStructure);
		//detector.brightnessDetect(frame, context, result);
		detector.brightnessDetect(frame, thresh-1, result);
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

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
	
	switch (event) {
	case EVENT_LBUTTONDOWN:
		//printf("(%d,%d)", x, y);
		coordsTopLeft.x = x;
		coordsTopLeft.y = y;
		break;
	case EVENT_RBUTTONDOWN:
		coordsBottomRight.x = x;
		coordsBottomRight.y = y;
		break;
	default:
		break;
	}
	//coords = Rect(coordsTopLeft, coordsBottomRight);
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
