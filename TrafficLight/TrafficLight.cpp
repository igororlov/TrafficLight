#include "TrafficLight.h"

using namespace cv;

static VideoCapture capture;
static TrafficLightDetector detector(Rect(coordsTopLeft, coordsBottomRight));
static LucasKanadeTracker tracker;

int main() {
	
	printf("***** Traffic lights app *****\n");

	if (capture.open(VIDEO_PATH) == false) {
		printf("Unable to open file...\n");
		exit(1);
	}

	double rate = capture.get(CV_CAP_PROP_FPS); // frames per second
	int delay = 1000 / (int)rate;
	namedWindow(WINDOW_NAME);
	setMouseCallback(WINDOW_NAME, mouseCallback);

	Mat previous, frame;
	
	while (capture.read(frame)) {

		tracker.getNewCoords(previous, frame, coords);
		detector.detect(frame);

		imshow(WINDOW_NAME, frame);

		char c = waitKey(delay);
		if (c == 27) {
			break;
		} else if (c == 32) {
			waitKey(0);
		}
		previous.release();
		previous.data = frame.data;
	}

	return 0;
}

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
	switch (event) {
	case EVENT_LBUTTONDOWN:
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
	coords = Rect(coordsTopLeft, coordsBottomRight);
	detector.setCoords(coords);
}
