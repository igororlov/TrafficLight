#ifndef VIDEO_PATH_LOCAL_H
#define VIDEO_PATH_LOCAL_H

#include "Context.h"

#define VIDEO_DIR "C:\\Users\\Igor\\Dropbox\\IASA\\SAIT\\TrafficLight\\Data\\TrafficLight\\"

// VID_20140313_180927.mp4
/*#define VIDEO_FILENAME "VID_20140313_180927.mp4"
#define TL_COUNT 3
Context contexts[TL_COUNT] = {
	Context(Point(139,214), Point(139,219), Point(139,225), 3, Point(135,210), Point(143,228)),
	Context(Point(407,119), Point(407,132), Point(407,143), 4, Point(400,113), Point(414,150)),
	Context(Point(586,212), Point(586,224), Point(586,234), 5, Point(579,204), Point(593,241))
};*/

// merged_day & merged_night
#define VIDEO_FILENAME "demo_night.avi" // "merged_day_small.avi"
#define BACKGROUND_FILENAME "background_night.bmp"
#define DETECTION_MASK_FILENAME "detection_mask.txt"
#define SHOW_MASK_FILENAME "show_mask.txt"
#define TL_COUNT 1
Context contexts[TL_COUNT] = {
	Context(Point(466,100), Point(465,113), Point(464,126), 6, Point(455,90), Point(480,135))
};


// 2.mp4
/*#define VIDEO_FILENAME "2.mp4"
#define TL_COUNT 1
Context contexts[TL_COUNT] = {
	Context(Point(282,199), Point(282,207), Point(281,213), 1, Point(278,196), Point(285,215))
};*/

#define VIDEO_PATH VIDEO_DIR VIDEO_FILENAME
#define PATH_TO_BACKGROUND VIDEO_DIR BACKGROUND_FILENAME
#define PATH_TO_DETECTION_MASK VIDEO_DIR DETECTION_MASK_FILENAME
#define PATH_TO_SHOW_MASK VIDEO_DIR SHOW_MASK_FILENAME
#define VIEW_SPEEDUP 1 // x times faster view (smaller delay) 

#define SAVE_VIDEO_TO_FILE 0
#define OUTPUT_VIDEO_FILENAME "Baglai-Orlov_traffic_violations_demo_night.avi" // name must end with output.avi, see .gitignore file.

#define IMG_PATH_FORMAT "C:\\Main\\Temp\\Cars\\deleteMe\\%d.bmp"

#endif