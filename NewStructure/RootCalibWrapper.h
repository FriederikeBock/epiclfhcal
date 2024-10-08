#ifndef ROOTCALIBWRAPPER_H
#define ROOTCALIBWRAPPER_H

#include <cstddef>
#include <map>
#include "Setup.h"

class Calib;

class RootCalibWrapper{
 public:
  RootCalibWrapper(){}
  RootCalibWrapper(Calib*);
  virtual ~RootCalibWrapper(){}

  friend class Calib;
 private:

  std::map<int, double> Pedestal;
  std::map<int, double> ScaleH;
  std::map<int, double> ScaleL;
  ClassDef(RootCalibWrapper,1)
};


#endif
