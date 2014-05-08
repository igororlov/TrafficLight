#include "VehicleDetect.h"

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

int main(int argc, char** argv)
{
  CvCapture *capture;
  IplImage  *frame;

  cascade = (CvHaarClassifierCascade*) cvLoad(CASCADE_FILE, 0, 0, 0);
  storage = cvCreateMemStorage(0);
  capture = cvCaptureFromAVI(VIDEO_PATH);

  assert(cascade && storage && capture);

  cvNamedWindow("video", 1);

  IplImage* frame1 = cvQueryFrame(capture);
  frame = cvCreateImage(cvSize((int)((frame1->width*INPUT_RESIZE_PERCENT)/100) , (int)((frame1->height*INPUT_RESIZE_PERCENT)/100)), frame1->depth, frame1->nChannels);

  do {
    frame1 = cvQueryFrame(capture);

    if(!frame1)
      break;

    cvResize(frame1, frame);

    detect(frame);

    int key = cvWaitKey(10);
    if (key == 27) {
		break;
	} else if (key == 32) {
		cvWaitKey(0);
	}

  } while(1);

  cvDestroyAllWindows();
  cvReleaseImage(&frame);
  cvReleaseCapture(&capture);
  cvReleaseHaarClassifierCascade(&cascade);
  cvReleaseMemStorage(&storage);

  return 0;
}

void detect(IplImage *img)
{
  CvSize img_size = cvGetSize(img);
  CvSeq *object = cvHaarDetectObjects(
    img,
    cascade,
    storage,
    1.1, //1.1,//1.5, //-------------------SCALE FACTOR
    1, //2        //------------------MIN NEIGHBOURS
    0, //CV_HAAR_DO_CANNY_PRUNING
    cvSize(0,0),//cvSize( 30,30), // ------MINSIZE
    img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
    );

  std::cout << "Total: " << object->total << " cars" << std::endl;
  for(int i = 0 ; i < ( object ? object->total : 0 ) ; i++)
  {
    CvRect *r = (CvRect*)cvGetSeqElem(object, i);
    cvRectangle(img,
      cvPoint(r->x, r->y),
      cvPoint(r->x + r->width, r->y + r->height),
      CV_RGB(255, 0, 0), 2, 8, 0);
  }

  cvShowImage("video", img);
}