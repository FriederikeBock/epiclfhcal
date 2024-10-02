#ifndef CALIB_H
#define CALIB_H

#include <cstddef>
#include <map>
#include "Setup.h"

//class RootCalibWrapper;

class Calib{

 public:
  Calib() {}
  virtual ~Calib() {}
  
  //Deleting copy constructor
  //Calib(const Calib&)=delete;
  //Calib& operator=(const Calib&)=delete;

  //static Calib* GetInstance(){
  //  if(instancePtr==NULL) instancePtr = new Calib();
  //  return instancePtr;
  //}
  
  //friend class RootCalibWrapper;
  
  double GetPedestal (int /**/) const;
  double GetPedestal (int /**/, int /**/, int /**/, int /**/) const;
  double GetScaleHigh(int /**/) const;
  double GetScaleHigh(int /**/, int /**/, int /**/, int /**/) const;
  double GetScaleLow (int /**/) const;
  double GetScaleLow (int /**/, int /**/, int /**/, int /**/) const;
  //void   Restore(RootCalibWrapper&);
  void   SetPedestal (double, int);
  void   SetPedestal (double, int, int, int, int);
  void   SetScaleHigh(double, int);
  void   SetScaleHigh(double, int, int, int, int);
  void   SetScaleLow (double, int);
  void   SetScaleLow (double, int, int, int, int);
 private:
  
  //static Calib* instancePtr;
  
  
  std::map<int, double> Pedestal;
  std::map<int, double> ScaleH;
  std::map<int, double> ScaleL;
  //Setup* setup=Setup::GetInstance();
  ClassDef(Calib,1)
};


#endif
