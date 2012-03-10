#include <string>
#include <cv.h>
#include <highgui.h>

typedef enum _VERROR{
  ERROR_NOERROR,
  ERROR_CANNOTOPEN,
  ERROR_NOTOPENED,
} VERROR;

// trackbar callback
void onChangeTrackbarAviViewer(int pos, void* userdata);

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

  // image to show
  cv::Mat image;

  // for gui
  std::string winname;  // window name
  std::string trackname;  // trackbar name

  // set next frame no
  void setnextframe(int no);
  // get image from file
  void getimg(int no);
  // show image to window
  void showimage();
  // get time of the current frame
  double gettime();
  // go into loop
  void loop();
  // get key interaction
  bool keyboard();

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

  void error(VERROR e);
};
