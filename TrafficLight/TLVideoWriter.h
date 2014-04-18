#ifndef TL_VIDEO_WRITER_H
#define TL_VIDEO_WRITER_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

class TLVideoWriter {
public:
	TLVideoWriter(bool writeToFile, const char* filename, int codec, double fps, Size frameSize);
	void write(Mat& frame);
private:
	VideoWriter writer;
	bool writeToFile;
};

#endif