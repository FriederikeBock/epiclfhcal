#ifndef TILESPECTRA_H
#define TILESPECTRA_H

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TF1.h"
#include "TPad.h" 
#include "TCanvas.h"
#include "TLegend.h"

class TileSpectra{



 public:
  Spectra(){}
  ~Spectra(){}

  bool Fill(double, double);
  bool FitNoise(double*);
  bool FitNoiseWithBG(double*);
  //bool FitAndPlotGainCorr(double*);//Maybe should be two separate functions

  TH1D* GetHG();
  TH1D* GetLG();
  TH1D* GetHGLGcomb();
  TProfile* GetLGHGcorr();

  TF1* GetBackModel();
  TF1* GetSignalModel();
  TF1* GetCorrModel();
  
 protected:
  TF1* Background;
  TF1* Signal;
  TF1* HGLGcorr;
  TH1D* hspectraHG;
  TH1D* hspectraLG;
  TH1D* hcombined;
  TProfile* hspectraLGHG;

  ClassDef(TileSpectra,1);
};

class FitTools {
 public:
  static double langaufun(double */*x*/, double */*par*/);
  static int langaupro(double */*params*/, double &/*maxx*/, double &/*FWHM*/);
};

#endif
