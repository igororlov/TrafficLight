#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

// External includes
#include <stdio.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

// Internal includes
#include "VideoSettingsLocal.h"
#include "TrafficLightDetector.h"
#include "LucasKanadeTracker.h"

#define MAIN_WINDOW_NAME "Main"
#define SETTINGS_WINDOW_NAME "Settings"

// Functions declarations
void mouseCallback(int event, int x, int y, int flags, void* userdata);
void setContexts(TrafficLightDetector &detector);

#endif