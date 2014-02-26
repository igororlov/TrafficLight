#ifndef LK_TRACKER_H
#define LK_TRACKER_H

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

const int MAX_COUNT = 500;

using namespace std;
using namespace cv;

class LucasKanadeTracker {
public:
	Rect getNewCoords(Mat& previous, Mat& current, Rect oldCoords);
private:
    vector<Point2f> points[2];
};

#endif