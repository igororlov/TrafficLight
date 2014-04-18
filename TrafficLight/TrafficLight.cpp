#include "TrafficLight.h"
#include "TLVideoWriter.h"
#include "VideoSettingsLocal.h"

using namespace cv;

// Variables
static VideoCapture capture;
static TrafficLightDetector detector;
static LucasKanadeTracker tracker;

int main() {
	
	printf("***** Traffic lights app *****\n");
	
	if (capture.open(VIDEO_PATH) == false) {
		printf("Unable to open file...\n");
		exit(1);
	}

	double rate = capture.get(CV_CAP_PROP_FPS); // frames per second
	int delay = 1000 / (int)rate; // in milliseconds
	
	namedWindow(MAIN_WINDOW_NAME);
	setMouseCallback(MAIN_WINDOW_NAME, mouseCallback);

	// Set up video writer
	Size frameSize = Size(2*((int) capture.get(CV_CAP_PROP_FRAME_WIDTH)), (int) capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	TLVideoWriter writer(SAVE_VIDEO_TO_FILE, OUTPUT_VIDEO_FILENAME, -1, rate * VIEW_SPEEDUP, frameSize);
	
	setContexts(detector);
	
	Mat previous, frame;
	
	while (capture.read(frame)) {
		Mat result, concat;
		
		detector.brightnessDetect(frame, result);
		hconcat(frame, result, concat);
		imshow(MAIN_WINDOW_NAME, concat);
		writer.write(concat);

		char c = waitKey(delay / VIEW_SPEEDUP);
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
		printf("(%d,%d)\n", x, y);
	default:
		break;
	}
}

void setContexts(TrafficLightDetector &detector) {
	for (int i = 0; i < TL_COUNT; i++) {	
		contexts[i].lightState = UNDEFINED;
		detector.contexts.push_back(contexts[i]);
	}
}