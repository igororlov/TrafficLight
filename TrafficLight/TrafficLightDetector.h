#ifndef TRAFFIC_LIGHT_DETECTOR_H
#define TRAFFIC_LIGHT_DETECTOR_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>

#define MY_COLOR_RED Scalar(0,0,255)
#define MY_COLOR_GREEN Scalar(0,255,0)

using namespace cv;

class TrafficLightDetector {
public:
	TrafficLightDetector(Rect aCoords);
	void setCoords(Rect aCoords);
	void detect(Mat &image);
private:
	Rect coords;
};

#endif