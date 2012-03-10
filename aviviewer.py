#! /usr/bin/python
# -*- coding: utf-8 -*-

from cv import *
import sys
from optparse import OptionParser

class AviViewer(object):
  _WINDOWNAME = "Aviviewer"
  _TRACKBARNAME = "frame"

  def __init__(self,filename,fps=-1):
    # parameters
    self.fps = fps
    self.no = -1          # frame number
    self.numframe = -1    # number of frames
    self.cap = None       # cvCapture
    self.filename = filename  # name of the video file
    self.playmode = False # play mode disable
    self.font = InitFont(CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0)  # text font
    self.color = CV_RGB(255,0,0) # text color
    self.paintcolor = CV_RGB(0,0,255) # paint color
    self.paintradius = 2
    # load video file
    self.load()
    # get number of frames
    self.numframe = GetCaptureProperty(self.cap, CV_CAP_PROP_FRAME_COUNT) # number of frames
    # use video fps
    if self.fps <= 0:
      self.fps = GetCaptureProperty(self.cap, CV_CAP_PROP_FPS)
    # set first image
    self.getimg(0)
    # make paint
    self.paintimg = CloneImage(self.img)
    SetZero(self.paintimg)

  def load(self):
    # load video file
    self.cap = CaptureFromFile(self.filename)
    if not self.cap:
      sys.stderr.write("error: %s cannot open.\n"%(self.filename))
      exit()
    return

  def run(self):
    # window
    NamedWindow(self._WINDOWNAME)
    # trackbar
    CreateTrackbar(self._TRACKBARNAME, self._WINDOWNAME, self.no, int(self.numframe), self.onChangeTrackbar)
    # mouse
    SetMouseCallback(self._WINDOWNAME, self.onMouse)
    self.show()
    WaitKey(10)
    # go into loop
    self.loop()
    
  def loop(self):
    prevno = self.no
    # gui loop back start
    while self.keyboard():
      if self.playmode:
        self.getimg(self.no+1)
      if self.bImgChanged:
        self.show()
        prevno = self.no
        self.bImgChanged = False
    return

  def getimg(self,no):
    if no < 0:
      no = 0
    if no >= self.numframe:
      no = self.numframe - 1
    if no != self.nextframeno():
      self.setnextframeno(no)
    if self.no != no:
      self.img = QueryFrame(self.cap)
      self.no = no
      self.update()
    return

  def gettime(self):
    return (float(self.no) / self.fps)

  def gettimestring(self):
    t = self.gettime()
    return "%0.3f"%t

  def show(self):
    # time text
    timedimg = CloneImage(self.img)
    PutText(timedimg, self.gettimestring()+"s", (30,30), self.font, self.color)
    # paint
    Add(timedimg,self.paintimg,timedimg)
    ShowImage(self._WINDOWNAME,timedimg)
    self.setTrackbarPos(self.no)
    return
        
  def nextframeno(self):
    return GetCaptureProperty(self.cap, CV_CAP_PROP_POS_FRAMES)
  
  def setnextframeno(self,no):
    return SetCaptureProperty(self.cap, CV_CAP_PROP_POS_FRAMES,no)

  def keyboard(self):
    key = WaitKey(30)
    if key == ord("q"):  # quit
      return False 
    elif key == ord("p"): # play & pose
      self.playmode = not self.playmode
    elif key == ord("f"): # next frame
      if not self.playmode:
        self.getimg(self.no+1)
    elif key == ord("b"): # prev frame
      if not self.playmode:
        self.getimg(self.no-1)
    elif key == ord("s"): # save frame
      if not self.playmode:
        SaveImage("frame%d.png"%(self.no),self.img)
    elif key == ord("c"): # command
      pass
    elif key == ord("d"): # delete paint
      SetZero(self.paintimg)
      self.update()
    else:
      pass
    return True

  def onChangeTrackbar(self,x):
    # trackbar call back function
    self.getimg(x)
    return

  def setTrackbarPos(self,x):
    return SetTrackbarPos(self._TRACKBARNAME, self._WINDOWNAME,x)

  def onMouse(self,event,x,y,flags,param):
    # mouse call back function
    if flags == CV_EVENT_FLAG_LBUTTON:
      Circle(self.paintimg,(x,y),self.paintradius,self.paintcolor,-1)
      self.update()
    return

  def update(self):
    self.bImgChanged = True
    return


if __name__ == "__main__":
  # parse option
  usage = "%prog VIDEOFILENAME"
  parser = OptionParser(usage=usage)
  parser.add_option("-f","--fps",type='float',dest="fps",default=200,help="fps(do not need if using video file fps)")
  
  (options,args) = parser.parse_args()
  try:
    filename  = args[0]
    fps = options.fps
  except:
    parser.error("Input file must be given.")
    exit(0)

  avivw = AviViewer(filename,fps)
  avivw.run()
