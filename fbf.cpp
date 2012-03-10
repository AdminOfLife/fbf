#include <string>
#include <cv.h>
#include <highgui.h>

#include "aviviewer.hpp"

int main()
{
  AviViewer avv;

  avv.load("test.avi");

  avv.invoke();
}
