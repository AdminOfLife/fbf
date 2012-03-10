#include <string>
#include <cv.h>
#include <highgui.h>

typedef enum _VERROR{
  ERROR_NOERROR,
  ERROR_CANNOTOPEN,
  ERROR_NOTOPENED,
  ERROR_SAVEFAILED,
} VERROR;

typedef enum _VMODE{
  MODE_PLAY,
  MODE_STOP,
} VMODE;

// trackbar callback
void onChangeTrackbarAviViewer(int pos, void* userdata);
// mouse callback
void onMouseAviViewer(int event, int x, int y, int flag, void* param);

class AviViewer
{
private:
  // infomation of the video
  std::string filename;  // filename of video file
  int fps;          // actual fps ( not always equal to video fps )

  // variables for controller
  cv::VideoCapture cap;
  int frame_no;          // frame number
  int fCount;      // number of frames

  // for gui
  std::string winname;  // window name
  std::string trackname;  // trackbar name

  // image to show
  cv::Mat image;
  cv::Mat paintmask;
  
  /// draw color
  cv::Scalar drawcolor;

  // play mode
  VMODE mode;

  // set next frame no
  void setnextframe(int no);
  // get image from file
  void getimg(int no);
  // get time of the current frame
  double gettime();
  // go into loop
  void loop();
  // get key interaction
  bool keyboard();
  // error
  void error(VERROR e);

public:
  // Constructor
  AviViewer();
  AviViewer(const std::string& filename);
  AviViewer(const char* const filename);
  // Destructor
  ~AviViewer();

  // load video file
  bool load();
  bool load(const std::string& filename);

  // run viewer
  void invoke();
  // set frame pos
  void setframepos(int no);
  // show image to window
  void showimage();
  void saveframe(const std::string& filename);

  // draw circle on mask image
  void drawCircleToMask(int x, int y);
};
