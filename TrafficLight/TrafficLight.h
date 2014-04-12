#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

// External includes
#include <stdio.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

// Internal includes
#include "TrafficLightDetector.h"
#include "LucasKanadeTracker.h"

#define RED_CENTER Point(466,100)
#define YELLOW_CENTER Point(465,113)
#define GREEN_CENTER Point(464,126)
#define LIGHT_RADIUS 6
#define RECT_TOP_LEFT Point(455,90)
#define RECT_BOTTOM_RIGHT Point(480,135)
#define MAIN_WINDOW_NAME "Main"
#define SETTINGS_WINDOW_NAME "Settings"
#define VIDEO_DIR "C:\\Users\\Igor\\Dropbox\\IASA\\SAIT\\TrafficLight\\"
#define VIDEO_FILENAME "merged_night.avi"
#define VIDEO_PATH VIDEO_DIR VIDEO_FILENAME

// Functions declarations
Mat assignTrafficLightStructure();
void mouseCallback(int event, int x, int y, int flags, void* userdata);

#endif