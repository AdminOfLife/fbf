#include <string>
#include <cv.h>
#include <highgui.h>

#include "aviviewer.hpp"

// Constructor
AviViewer::AviViewer()
{
  // initialization
  this->filename = "";
  this->frame_no = -1;
  this->fCount = -1;
  this->cap = cv::VideoCapture();
}

AviViewer::AviViewer(const std::string& filename)
{
  AviViewer();

  this->filename = filename;
  load();
}
  
AviViewer::AviViewer(const char* const filename)
{
  AviViewer(std::string(filename));
}

AviViewer::~AviViewer()
{
  // dummy
}

void AviViewer::invoke()
{
  if( !cap.isOpened() ){
    error(ERROR_NOTOPENED);
    return;
  }

  // set gui name
  winname = filename;
  trackname = "track";

  // make window
  cv::namedWindow(winname, CV_WINDOW_AUTOSIZE);
  // make trackbar
  cv::createTrackbar( trackname, winname, NULL, fCount, onChangeTrackbarAviViewer, this);
  
  showimage();

  loop();
}

void AviViewer::loop()
{
  while( keyboard() ){
    getimg(frame_no+1);
    showimage();
  }
  
  return;
}

bool AviViewer::keyboard()
{
  int key = cv::waitKey(30);
  if( key == 'q' ){
    return false;
  }

  return true;
}

void AviViewer::showimage()
{
  cv::imshow(winname, image);
  return;
}

bool AviViewer::load()
{
  // open video file
  if( !cap.open(filename) ){
    error(ERROR_CANNOTOPEN);
    return false;
  }

  // get video information
  fCount = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);
  fps = (int) cap.get(CV_CAP_PROP_FPS);

  // load first image
  getimg(0);

  return true;
}

bool AviViewer::load(const std::string& filename)
{
  this->filename = filename;
  
  return load();
}

void AviViewer::getimg(int no)
{
  if( !cap.isOpened() ){
    error(ERROR_NOTOPENED);
    return;
  }

  // set next frame to no
  setnextframe(no);
  
  // get image
  cap >> image;

  return;
}

void AviViewer::setnextframe(int no)
{
  // check if no is out of range
  if( no < 0 ){
    no = 0;
  }else if( no >= fCount ){
    no = fCount - 1;
  }

  // set frame position
  if( no != (int) cap.get(CV_CAP_PROP_POS_FRAMES) ){
    cap.set(CV_CAP_PROP_POS_FRAMES,no);
  }

  frame_no = no;

  return;
}

double AviViewer::gettime()
{
  return (double) frame_no / fps;
}

void AviViewer::error(VERROR e)
{
  return;
}

void onChangeTrackbarAviViewer(int pos, void* userdata)
{
  AviViewer *avv = (AviViewer *) userdata;

  return;
}

