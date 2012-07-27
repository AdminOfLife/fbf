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

typedef enum _VCMD{
  CMD_STOP,
  CMD_TOGGLE_PLAYMODE,
  CMD_FORWARD_FRAME,
  CMD_BACKWARD_FRAME,
  CMD_SAVEFRAME,
  CMD_CLEARPAINT,
} VCMD;

// trackbar callback
void onChangeTrackbarAviViewer(int pos, void* userdata);
void onChangeTrackbarBrightness(int pos, void* userdata);
void onChangeTrackbarContrast(int pos, void* userdata);
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
  bool ready; // whether image is ready.
  
  /// draw color
  cv::Scalar drawcolor;

  // play mode
  VMODE mode;
  // brightness and contrast settings
  int brightness;
  int contrast;

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
  // save
  void saveframe(const std::string& filename);
  void saveframe(const char* const filename);
  // change brightness and contrast
  void changeBrightness( cv::Mat& image );

public:
  // Constructor
  AviViewer();
  AviViewer(const std::string& filename, int fps=-1);
  AviViewer(const char* const filename, int fps=-1);
  // Destructor
  ~AviViewer();

  // load video file
  bool load(int fps=-1);
  bool load(const std::string& filename,int fps=-1);

  // run viewer
  void invoke();
  // set frame pos
  void setframepos(int no);
  // show image to window
  void showimage();
  // use cmd
  void command(VCMD cmd, const void* arg=NULL);
  // change fps
  void setfps(int fps);
  // returns current frame
  int frameno();
  // set brightness
  void setbrightness(int brightness);
  // set contrast
  void setcontrast(int contrast);

  // draw circle on mask image
  void drawCircleToMask(int x, int y);
};
