#include "TrafficLight.h"
#include "TLVideoWriter.h"

using namespace cv;

// Variables
Point coordsTopLeft(0,0);
Point coordsBottomRight(10,10);
Rect coords(coordsTopLeft, coordsBottomRight);

static VideoCapture capture;
static TrafficLightDetector detector;
static LucasKanadeTracker tracker;
Context context1, context2, context3;

int main() {
	
	printf("***** Traffic lights app *****\n");
	
	if (capture.open(VIDEO_PATH) == false) {
		printf("Unable to open file...\n");
		exit(1);
	}

	double rate = capture.get(CV_CAP_PROP_FPS); // frames per second
	int delay = 1000 / (int)rate;
	
	namedWindow(MAIN_WINDOW_NAME);
	setMouseCallback(MAIN_WINDOW_NAME, mouseCallback);

	Size frameSize = Size(2*((int) capture.get(CV_CAP_PROP_FRAME_WIDTH)), (int) capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	TLVideoWriter writer(SAVE_VIDEO_TO_FILE, OUTPUT_VIDEO_FILENAME, -1, 40.0, frameSize);
	
	setContexts(detector);
	
	Mat previous, frame;
	while (capture.read(frame)) {

		Mat result, concat;
		detector.brightnessDetect(frame, result);
		hconcat(frame, result, concat);
		imshow(MAIN_WINDOW_NAME, concat);
		writer.write(concat);

		char c = waitKey(delay/3);
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
		//printf("(%d,%d)\n", x, y);
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
}

void setContexts(TrafficLightDetector &detector) {
	// TODO refactor this!
	context1.redCenter = RED_CENTER_1;
	context1.yellowCenter = YELLOW_CENTER_1;
	context1.greenCenter = GREEN_CENTER_1;
	context1.lampRadius = LIGHT_RADIUS_1;
	context1.topLeft = RECT_TOP_LEFT_1;
	context1.botRight = RECT_BOTTOM_RIGHT_1;

	context2.redCenter = RED_CENTER_2;
	context2.yellowCenter = YELLOW_CENTER_2;
	context2.greenCenter = GREEN_CENTER_2;
	context2.lampRadius = LIGHT_RADIUS_2;
	context2.topLeft = RECT_TOP_LEFT_2;
	context2.botRight = RECT_BOTTOM_RIGHT_2;

	context3.redCenter = RED_CENTER_3;
	context3.yellowCenter = YELLOW_CENTER_3;
	context3.greenCenter = GREEN_CENTER_3;
	context3.lampRadius = LIGHT_RADIUS_3;
	context3.topLeft = RECT_TOP_LEFT_3;
	context3.botRight = RECT_BOTTOM_RIGHT_3;

	detector.contexts.push_back(context1);
	detector.contexts.push_back(context2);
	detector.contexts.push_back(context3);
}