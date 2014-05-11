#ifndef VEHICLE_DETECT_H
#define VEHICLE_DETECT_H

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define INPUT_RESIZE_PERCENT 40
#define DATA_DIR "C:\\Users\\Igor\\Dropbox\\IASA\\SAIT\\TrafficLight\\Data\\VehicleDetect\\"
#define CASCADE_FILE DATA_DIR "cars3.xml"
#define VIDEO_FILENAME "video6.mp4"
#define VIDEO_PATH DATA_DIR VIDEO_FILENAME

void detect(IplImage *img);

#endif