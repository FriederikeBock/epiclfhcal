#ifndef CALIB_H
#define CALIB_H

#include <cstddef>
#include <map>
#include "TTimeStamp.h"
#include "Setup.h"

struct TileCalib{
  double PedestalMeanH;
  double PedestalMeanL;
  double PedestalSigH;
  double PedestalSigL;
  double ScaleH;
  double ScaleWidthH;
  double ScaleL;
  double ScaleWidthL;
  double LGHGCorr;
  double HGLGCorr;
} ;

class Calib{

 public:
  Calib() {}
  virtual ~Calib() {}
    
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
  double GetScaleWidthHigh(int /**/) const;
  double GetScaleWidthHigh(int /**/, int /**/, int /**/, int /**/) const;
  double GetScaleLow (int /**/) const;
  double GetScaleLow (int /**/, int /**/, int /**/, int /**/) const;
  double GetScaleWidthLow (int /**/) const;
  double GetScaleWidthLow (int /**/, int /**/, int /**/, int /**/) const;
  double GetScaleLGHGCorr (int /**/) const;
  double GetScaleLGHGCorr (int /**/, int /**/, int /**/, int /**/) const;
  double GetScaleHGLGCorr (int /**/) const;
  double GetScaleHGLGCorr (int /**/, int /**/, int /**/, int /**/) const;
  double GetAverageScaleHigh() const;
  double GetAverageScaleLow() const;
  double GetAverageHGLGCorr() const;
  double GetAverageLGHGCorr() const;
  TileCalib* GetTileCalib(int /**/);
  TileCalib* GetTileCalib(int /**/, int /**/, int /**/, int /**/);
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
  void   SetScaleWidthHigh(double, int);
  void   SetScaleWidthHigh(double, int, int, int, int);
  void   SetScaleLow (double, int);
  void   SetScaleLow (double, int, int, int, int);
  void   SetScaleWidthLow (double, int);
  void   SetScaleWidthLow (double, int, int, int, int);
  void   SetScaleLGHGCorr (double, int);
  void   SetScaleLGHGCorr (double, int, int, int, int);
  void   SetScaleHGLGCorr (double, int);
  void   SetScaleHGLGCorr (double, int, int, int, int);

  int GetRunNumber(void);
  const TTimeStamp* GetBeginRunTime(void) const;
  double GetVov(void);
  double GetVop(void);

  void SetRunNumber(int);//How to handle pedestal and mip which may come from different runs?
  void SetBeginRunTime(TTimeStamp);//How to handle pedestal and mip which may come from different runs?
  void SetVop(double);//This should anyway be the same for ped/mip/data
  void SetVov(double);//This should anyway be the same for ped/mip/data
  
 private:
   
  std::map<int, TileCalib> CaloCalib;
  int RunNumber;
  TTimeStamp BeginRunTime;
  double Vop;
  double Vov;
  ClassDef(Calib,3)
};


#endif
