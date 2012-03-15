#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <unistd.h>

#include "aviviewer.hpp"

void print_usage()
{
  std::cerr << "Usage: fbf [-f FPS]  VIDEOFILENAME" << std::endl;
}

int main(int argc, char*argv[])
{
  AviViewer avv;
  int fps = -1;
  int opt;
  std::istringstream fpstext;

  while( (opt = getopt(argc,argv,"hf:") ) != -1 ){
    switch( opt ){
    case 'f':
      fpstext.str(std::string(optarg));
      fpstext >> fps;
      break;
    case 'h':
      print_usage();
      exit(EXIT_SUCCESS);
    default:
      print_usage();
      exit(EXIT_FAILURE);
      break;
    }
  }

  if( optind != (argc-1) ){
    print_usage();
    exit(EXIT_FAILURE);
  }

  avv.load(std::string(argv[optind]),fps);

  avv.invoke();
  
  return 0;
}
