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
  fps = -1;
  ready = false;
  cap = cv::VideoCapture();
  drawcolor = cv::Scalar(255,0,0);
}

AviViewer::AviViewer(const std::string& filename,int fps)
{
  AviViewer();

  load(filename,fps);
}
  
AviViewer::AviViewer(const char* const filename,int fps)
{
  AviViewer(std::string(filename),fps);
}

AviViewer::~AviViewer()
{
  // destroy window
  cv::destroyWindow(winname);
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
  cv::namedWindow(winname, CV_WINDOW_NORMAL);
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

  return;
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

void AviViewer::command(VCMD cmd, const void* arg)
{
  if( cmd == CMD_STOP ){
    // stop video
    mode = MODE_STOP;
  }else if( cmd == CMD_TOGGLE_PLAYMODE ){
    // toggle play mode
    if( mode == MODE_PLAY ){
      mode = MODE_STOP;
    }else{
      mode = MODE_PLAY;
    }
  }else if( cmd == CMD_FORWARD_FRAME ){
    // step one frame forward
    if( mode == MODE_PLAY ){
      mode = MODE_STOP;
    }
    setframepos(frame_no+1);
    showimage();
  }else if( cmd == CMD_BACKWARD_FRAME ){
    // step one frame backward
    if( mode == MODE_PLAY ){
      mode = MODE_STOP;
    }
    setframepos(frame_no-1);
    showimage();
  }else if( cmd == CMD_SAVEFRAME ){
    char* filename = (char*) arg;
    // save image
    saveframe(filename);
  }else if( cmd == CMD_CLEARPAINT ){
    // clear paint mask
    paintmask = cv::Mat::zeros( image.size(), CV_8UC3 );
    showimage();
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
    command(CMD_TOGGLE_PLAYMODE);
  }else if( key == 'f' ){
    command(CMD_FORWARD_FRAME);
  }else if( key == 'b' ){
    command(CMD_BACKWARD_FRAME);
  }else if( key == 's' ){
    command(CMD_SAVEFRAME,"frame.png");
  }else if( key == 'c' ){
    command(CMD_CLEARPAINT);
  }

  return true;
}

void AviViewer::showimage()
{
  double time;
  char timetext[256];

  if( !cap.isOpened() || !ready ){
    return;
  }

  cv::Mat showimage = image.clone();
  // draw paint
  paintmask.copyTo( showimage, paintmask );

  // time text
  time = gettime(); 
  sprintf( timetext, "%.3lf sec", time );
  cv::putText( showimage, std::string(timetext), cv::Point(10,30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0,0,255) );

  cv::imshow(winname, showimage);
  return;
}

bool AviViewer::load(int fps)
{
  // open video file
  if( !cap.open(filename) ){
    error(ERROR_CANNOTOPEN);
    return false;
  }

  // get video information
  fCount = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);
  if( fps <= 0 && this->fps <= 0 ){
    this->fps = (int) cap.get(CV_CAP_PROP_FPS);
  }else if( this->fps <= 0 ){
    this->fps = fps;
  }
  
  return true;
}

bool AviViewer::load(const std::string& filename,int fps)
{
  this->filename = filename;

  return load(fps);
}

void AviViewer::setframepos(int no)
{
  // check if no is out of range
  if( no < 0 ){
    no = 0;
  }else if( no >= fCount ){
    no = fCount - 1;
  }

  if( no != frame_no ){
    // get image
    getimg(no);
  
    // set trackbar position
    cv::setTrackbarPos( trackname, winname, frame_no);
  }

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

  // ready to show image
  if( !ready ){
    ready = true;
  }

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
  if( no !=  (int) cap.get(CV_CAP_PROP_POS_FRAMES) ){
    std::cerr << no << std::endl;
    cap.set(CV_CAP_PROP_POS_FRAMES,no);
  }
  std::cerr << "frame_no=" << frame_no << " no=" << no << std::endl;

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

void AviViewer::saveframe(const char* const filename)
{
  saveframe(std::string(filename));
  return;
}

double AviViewer::gettime()
{
  return (double) frame_no / fps;
}

void AviViewer::setfps(int fps)
{
  if( fps > 0 ){
    this->fps = fps;
    showimage();
  }
  return;
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

int AviViewer::frameno()
{
  return frame_no;
}

void onChangeTrackbarAviViewer(int pos, void* userdata)
{
  AviViewer *avv = (AviViewer *) userdata;

  if( pos != avv->frameno() ){
    avv->setframepos(pos);
    avv->showimage();
  }

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
