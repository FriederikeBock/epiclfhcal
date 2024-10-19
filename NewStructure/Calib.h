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
  
  double GetPedestalMeanH (int /**/) const;
  double GetPedestalMeanL (int /**/) const;
  double GetPedestalMeanH (int /**/, int /**/, int /**/, int /**/) const;
  double GetPedestalMeanL (int /**/, int /**/, int /**/, int /**/) const;
  double GetPedestalSigH (int /**/) const;
  double GetPedestalSigL (int /**/) const;
  double GetPedestalSigH (int /**/, int /**/, int /**/, int /**/) const;
  double GetPedestalSigL (int /**/, int /**/, int /**/, int /**/) const;
  double GetScaleHigh(int /**/) const;
  double GetScaleHigh(int /**/, int /**/, int /**/, int /**/) const;
  double GetScaleLow (int /**/) const;
  double GetScaleLow (int /**/, int /**/, int /**/, int /**/) const;
  //void   Restore(RootCalibWrapper&);
  void   SetPedestalMeanH (double, int);
  void   SetPedestalMeanL (double, int);
  void   SetPedestalMeanH (double, int, int, int, int);
  void   SetPedestalMeanL (double, int, int, int, int);
  void   SetPedestalSigH (double, int);
  void   SetPedestalSigL (double, int);
  void   SetPedestalSigH (double, int, int, int, int);
  void   SetPedestalSigL (double, int, int, int, int);
  void   SetScaleHigh(double, int);
  void   SetScaleHigh(double, int, int, int, int);
  void   SetScaleLow (double, int);
  void   SetScaleLow (double, int, int, int, int);
 private:
  
  //static Calib* instancePtr;
  
  
  std::map<int, double> PedestalMeanH;
  std::map<int, double> PedestalMeanL;
  std::map<int, double> PedestalSigH;
  std::map<int, double> PedestalSigL;
  std::map<int, double> ScaleH;
  std::map<int, double> ScaleL;
  //Setup* setup=Setup::GetInstance();
  ClassDef(Calib,1)
};


#endif
