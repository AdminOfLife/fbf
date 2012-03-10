#include <string>
#include <cv.h>
#include <highgui.h>

#include "aviviewer.hpp"

int main(int argc, char*argv[])
{
  AviViewer avv;

  avv.load(argv[1]);

  avv.invoke();
}
