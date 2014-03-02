#include "TrafficLightDetector.h"

/*
 * Detection in grayscale by brightness.
 */
void TrafficLightDetector::brightnessDetect(const Mat &input, const Context context, Mat &output) {
	cvtColor(input, output, CV_RGB2GRAY);
	blur(output, output, Size(4,4));
	Mat cannyRoi = output(context.coords);
	Canny(cannyRoi, cannyRoi, 30, 100);
	
	/*int dilation_size = 1;
	Mat element = getStructuringElement( MORPH_ELLIPSE,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
	dilate(cannyRoi, cannyRoi, element);*/

	Mat cannyForContours;
	cannyRoi.copyTo(cannyForContours);
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
	findContours(cannyForContours, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));

	cvtColor(output, output, CV_GRAY2RGB);
	rectangle(output, context.coords, MY_COLOR_RED);

	/// Draw contours
	Mat roi = output(context.coords);
	for (int i = 0; i < contours.size(); i++) {
		if (hierarchy[i][3] >= 0) {
			// TODO check shape & color inside && contourArea(contours.at(i))
			drawContours(roi, contours, i, MY_COLOR_GREEN, -1, 8, hierarchy, 0, Point());
		}
	}
}

/*
 *  Attempt to recognize by color tracking in HSV. Detects good only green, but need to
 *  play also with S and V parameters range.
 */
void TrafficLightDetector::colorDetect(const Mat &input, Mat &output, const Rect coords, int Hmin, int Hmax) {
	
	if (input.channels() != 3) {
		return;
	}
	
	Mat hsv, thresholded;
	cvtColor(input, hsv, CV_RGB2HSV, 0);
	inRange(hsv, Scalar(Hmin,0,0), Scalar(Hmax,255,255), thresholded);

	cvtColor(thresholded, thresholded, CV_GRAY2RGB);
	thresholded.copyTo(output);

	rectangle(output, coords, MY_COLOR_RED);
}