#include <iostream>
#include <string>
#include <cv.h>
#include <highgui.h>

#include "aviviewer.hpp"

// Constructor
AviViewer::AviViewer()
{
  // initialization
  filename = "";
  frame_no = -1;
  fCount = -1;
  cap = cv::VideoCapture();
  drawcolor = cv::Scalar(255,0,0);
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
  winname = "FBF: "+filename;
  trackname = "FrameNo";

  // make window
  cv::namedWindow(winname, CV_WINDOW_AUTOSIZE);
  // make trackbar
  cv::createTrackbar( trackname, winname, NULL, fCount, onChangeTrackbarAviViewer, this);
  // Mouse callback
  cvSetMouseCallback( winname.c_str(), onMouseAviViewer, (void*)this);

  // load first image
  setframepos(0);
  // make mask image
  paintmask = cv::Mat::zeros( image.size(), CV_8UC3 );

  showimage();

  loop();
}

void AviViewer::loop()
{
  while( keyboard() ){
    if( mode == MODE_PLAY ){
      setframepos(frame_no+1);
      showimage();
    }
  }
  
  return;
}

bool AviViewer::keyboard()
{
  int key = cv::waitKey(30);

  if( key == 'q' ){
    // Quit
    return false;
  }else if( key == 'p' ){
    // Change Play mode
    if( mode == MODE_PLAY ){
      mode = MODE_STOP;
    }else{
      mode = MODE_PLAY;
    }
  }else if( key == 'f' ){
    // step one frame forward
    if( mode == MODE_PLAY ){
      mode = MODE_STOP;
    }
    setframepos(frame_no+1);
    showimage();
  }else if( key == 'b' ){
    // step one frame backward
    if( mode == MODE_PLAY ){
      mode = MODE_STOP;
    }
    setframepos(frame_no-1);
    showimage();
  }else if( key == 's' ){
    // save image
    saveframe("frame.png");
  }else if( key == 'c' ){
    // clear paint
    paintmask = cv::Mat::zeros( image.size(), CV_8UC3 );
    showimage();
  }

  return true;
}

void AviViewer::showimage()
{
  cv::Mat showimage = image.clone();
  paintmask.copyTo( showimage, paintmask );
  cv::imshow(winname, showimage);
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

  return true;
}

bool AviViewer::load(const std::string& filename)
{
  this->filename = filename;
  
  return load();
}

void AviViewer::setframepos(int no)
{
  // get image
  getimg(no);
  
  // set trackbar position
  cv::setTrackbarPos( trackname, winname, frame_no);

  return;
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

void AviViewer::saveframe(const std::string& filename)
{
  if( !cv::imwrite(filename, image) ){
    error(ERROR_SAVEFAILED);
    return;
  }
}

double AviViewer::gettime()
{
  return (double) frame_no / fps;
}

void AviViewer::error(VERROR e)
{
  switch(e){
  case ERROR_NOERROR:
    break;

  case ERROR_CANNOTOPEN:
    std::cerr << "Can't open :" << filename << std::endl;
    break;

  case ERROR_NOTOPENED:
    std::cerr << "Not opened video file" << std::endl;
    break;

  case ERROR_SAVEFAILED:
    std::cerr << "Failed to save frame" << std::endl;
    break;
  }

  return;
}

void AviViewer::drawCircleToMask(int x, int y)
{
  cv::circle( paintmask, cv::Point(x,y), 2, drawcolor, -1, CV_AA );

  return;
}

void onChangeTrackbarAviViewer(int pos, void* userdata)
{
  AviViewer *avv = (AviViewer *) userdata;

  avv->setframepos(pos);
  avv->showimage();

  return;
}

void onMouseAviViewer( int event, int x, int y, int flags, void* param)
{
  AviViewer *avv = (AviViewer *) param;
  if( flags == CV_EVENT_FLAG_LBUTTON ){
    avv->drawCircleToMask(x,y);
    avv->showimage();
  }
}
