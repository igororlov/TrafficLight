#ifndef VEHICLE_DETECTOR_H
#define VEHICLE_DETECTOR_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

#define ENFORCEMENT_RATIO_THRESHOLD 0.01

class VehicleDetector {
public:
	VehicleDetector();
	void loadBackgroundImage(char *pathToBackgroundImg);
	void loadMaskImage(char *pathToMaskFile);
	bool backgroundDetect(const Mat& grayFrame, Mat& output);
	void haarCascadeDetect(const Mat& grayFrame, Mat& output);
private:
	Mat backgroundImage;
	Mat mask;
	double maskArea;
};

Mat loadMask(char *pathToMaskFile);

#endif