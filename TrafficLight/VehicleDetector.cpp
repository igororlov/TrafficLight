#include "VehicleDetector.h"

VehicleDetector::VehicleDetector() {}

void VehicleDetector::loadBackgroundImage(char *pathToBackgroundImg) {
	backgroundImage = imread(pathToBackgroundImg);
	if (backgroundImage.channels() > 1) {
		cvtColor(backgroundImage, backgroundImage, CV_RGB2GRAY);
	}
}

void VehicleDetector::loadMaskImage(char *pathToMaskFile) {
	mask = loadMask(pathToMaskFile);
	Scalar sumPixels = sum(mask);
	maskArea = sumPixels.val[0] / 255;
}

bool VehicleDetector::backgroundDetect(const Mat& grayFrame, vector<Rect> &boundedRects) {

	Mat differenceImage;
	absdiff(grayFrame, backgroundImage, differenceImage);

	Mat thresholdedImage;
	threshold(differenceImage, thresholdedImage, 70, 255, THRESH_BINARY);

	int an = 2;
	Mat element = getStructuringElement(MORPH_RECT, Size(2*an+1, 2*an+1), Point(an, an) );
	morphologyEx(thresholdedImage, thresholdedImage, MORPH_OPEN, element);
	//erode(thresholdedImage, thresholdedImage, element);
	
	Mat maskedThresholded;
	thresholdedImage.copyTo(maskedThresholded, mask);

	Scalar sumPixels = sum(maskedThresholded);
	double whitePixelsCount = sumPixels.val[0] / 255;
	bool isEnforced = ( whitePixelsCount / maskArea ) > ENFORCEMENT_RATIO_THRESHOLD;
	
	// Find bounded rects
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(thresholdedImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point>> contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	for (int i = 0; i < contours.size(); i++) { 
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	
	boundedRects = boundRect;

	return isEnforced;
}

void VehicleDetector::haarCascadeDetect(const Mat& grayFrame, Mat& output) {
	// No implementation yet.
}

Mat loadMask(char *pathToMaskFile) {
	vector<Point> vertices, polygonRoi;
	int x,y;
	FILE* maskFile = fopen(pathToMaskFile, "r");
	while (fscanf(maskFile, "%d %d", &x, &y) != EOF) {
		Point p(x,y);
		vertices.push_back(p);
	}
	Mat mask = Mat::zeros(562, 512, CV_8UC1);
	vector<Point> ROI_Poly;
	approxPolyDP(vertices, polygonRoi, 1.0, true);
 
	// Fill polygon white
	fillConvexPoly(mask, &polygonRoi[0], polygonRoi.size(), 255, 8, 0);

	return mask;
}