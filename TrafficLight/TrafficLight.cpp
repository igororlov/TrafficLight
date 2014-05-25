#include "TrafficLight.h"
#include "TLVideoWriter.h"
#include "VideoSettingsLocal.h"

using namespace cv;

// Variables
static VideoCapture capture;
static TrafficLightDetector detector;
static LucasKanadeTracker tracker;
static VehicleDetector vehicleDetector;

static Mat frame;

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
	vehicleDetector.loadBackgroundImage(PATH_TO_BACKGROUND);
	vehicleDetector.loadMaskImage(PATH_TO_DETECTION_MASK);

	initMasks(PATH_TO_SHOW_MASK);

	Mat previous, resultImage;
	
	while (capture.read(frame)) {
		frame.copyTo(resultImage);

		Mat vehicleResult;

		cvtColor(frame, frame, CV_RGB2GRAY);
		LightState lightState = detector.brightnessDetect(frame);
		vector<Rect> boundedRects;
		bool isEnforced = vehicleDetector.backgroundDetect(frame, boundedRects);
		
		drawTrafficLights(resultImage, lightState);
		drawEnforcement(resultImage, isEnforced, lightState);
		drawBoundedRects(resultImage, boundedRects);

		imshow(MAIN_WINDOW_NAME, resultImage);
		
		writer.write(resultImage);

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

//static Point topleft;
//static Point botright;
//static Mat imgToSave;
//void mouseCallback(int event, int x, int y, int flags, void* userdata) {
//	static int frame_counter = 0;
//	static bool clickedOnce = false;
//	switch (event) {
//	case EVENT_LBUTTONDOWN:
//		if (!clickedOnce) {
//			printf("(%d,%d)\n", x, y);
//			topleft.x = x;
//			topleft.y = y;
//			clickedOnce = true;
//		} else {
//			botright.x = x;
//			botright.y = y;
//
//			frame_counter++;
//			char filename[300];
//			sprintf(filename, IMG_PATH_FORMAT, frame_counter);
//		
//			cvtColor(frame, imgToSave, CV_RGB2GRAY);
//			Rect roiRect(topleft, botright);
//			Mat roiImg = imgToSave(roiRect);
//			imwrite(filename, roiImg);
//			//fprintf(outfile, "%d.bmp 1 0 0 %d %d\n", frame_counter, roiRect.width, roiRect.height);
//			fprintf(outfile, "%d.bmp\n", frame_counter);
//			clickedOnce = false;
//		}
//		break;
//	}
//}

void setContexts(TrafficLightDetector &detector) {
	for (int i = 0; i < TL_COUNT; i++) {	
		contexts[i].lightState = UNDEFINED;
		detector.contexts.push_back(contexts[i]);
	}
}

void drawTrafficLights(Mat &targetImg, LightState lightState) {
	switch (lightState) {
	case GREEN:
		circle(targetImg, GREEN_DRAW_CENTER, LIGHT_DRAW_RADIUS, MY_COLOR_GREEN, -1);
		break;
	case YELLOW:
		circle(targetImg, YELLOW_DRAW_CENTER, LIGHT_DRAW_RADIUS, MY_COLOR_YELLOW, -1);
		break;
	case RED:
		circle(targetImg, RED_DRAW_CENTER, LIGHT_DRAW_RADIUS, MY_COLOR_RED, -1);
		break;
	case REDYELLOW:
		circle(targetImg, YELLOW_DRAW_CENTER, LIGHT_DRAW_RADIUS, MY_COLOR_YELLOW, -1);
		circle(targetImg, RED_DRAW_CENTER, LIGHT_DRAW_RADIUS, MY_COLOR_RED, -1);
		break;
	}
}

static Mat showMask, redMask, blueMask, greenMask;
void initMasks(char *pathToShowMask) {
	/* Initialize show mask */
	showMask = loadMask(pathToShowMask);
	Mat grayMask(showMask);
	cvtColor(showMask, showMask, CV_GRAY2RGB);
	showMask.copyTo(redMask);
	showMask.copyTo(blueMask);
	showMask.copyTo(greenMask);
	redMask.setTo(MY_COLOR_RED, grayMask);
	blueMask.setTo(MY_COLOR_BLUE, grayMask);
	greenMask.setTo(MY_COLOR_GREEN, grayMask);
}

void drawEnforcement(Mat &targetImg, bool isEnforced, LightState lightState) {
	
	addWeighted(targetImg, 1.0, showMask, -0.5, 0, targetImg);
	if (isEnforced) {
		if (lightState == GREEN)
			addWeighted(targetImg, 1.0, greenMask, 2.0, 0, targetImg);
		else
			addWeighted(targetImg, 1.0, redMask, 2.0, 0, targetImg);
	} else {
		addWeighted(targetImg, 1.0, blueMask, 2.0, 0, targetImg);
	}
	
}

void drawBoundedRects(Mat &targetImg, vector<Rect> boundedRects) {
	for (int i = 0; i< boundedRects.size(); i++) {
		if (boundedRects[i].width >= MIN_WIDTH && boundedRects[i].width <= MAX_WIDTH && boundedRects[i].height >= MIN_HEIGHT && boundedRects[i].height <= MAX_HEIGHT) {
			rectangle(targetImg, boundedRects[i].tl(), boundedRects[i].br(), MY_COLOR_PURPLE, 2, 8, 0);
		}
	}
}