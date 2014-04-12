#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

// External includes
#include <stdio.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

// Internal includes
#include "TrafficLightDetector.h"
#include "LucasKanadeTracker.h"

#define MAIN_WINDOW_NAME "Main"
#define SETTINGS_WINDOW_NAME "Settings"
#define VIDEO_DIR "C:\\Users\\Igor\\Dropbox\\IASA\\SAIT\\TrafficLight\\"
#define VIDEO_FILENAME "merged_day.avi"
#define VIDEO_PATH VIDEO_DIR VIDEO_FILENAME

// Functions declarations
Mat assignTrafficLightStructure();
void mouseCallback(int event, int x, int y, int flags, void* userdata);

// Variables
Point coordsTopLeft(0,0);
Point coordsBottomRight(10,10);
Rect coords(coordsTopLeft, coordsBottomRight);;

#endif