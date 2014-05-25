#include "TLVideoWriter.h"

TLVideoWriter::TLVideoWriter(bool writeToFile, const char* filename, int codec, double fps, Size frameSize) {
	this->writeToFile = writeToFile;
	if (writeToFile) {
		writer.open(filename, codec, fps, frameSize);
	}
}

void TLVideoWriter::write(Mat& frame) {
	if (writeToFile) {
		writer.write(frame);
	}
}