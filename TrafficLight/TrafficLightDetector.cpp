#include "TrafficLightDetector.h"

TrafficLightDetector::TrafficLightDetector(Rect aCoords) {
	coords = aCoords;
}

void TrafficLightDetector::setCoords(Rect aCoords) {
	coords = aCoords;
}

void TrafficLightDetector::detect(Mat &image) {
	rectangle(image, coords, MY_COLOR_RED);
}