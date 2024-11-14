#ifndef TILESPECTRA_H
#define TILESPECTRA_H

#include "TObject.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TF1.h"
#include "TPad.h" 
#include "TCanvas.h"
#include "TLegend.h"
#include "Calib.h"

class TileSpectra: public TObject{

 public:
  TileSpectra():TObject(){}
  TileSpectra(TString name, int id, TileCalib* cal, int deb=0):TObject()
  {
    TileName      = name;
    cellID        = id;
    calib         = cal;
    debug         = deb;
    bpedHG        = false;
    bpedLG        = false;
    bmipHG        = false;
    bmipLG        = false;
    bcorrHGLG     = false;
    hspectraHG    = TH1D(Form("hspectra%sHGCellID%d",name.Data(),id),Form("ADC spectrum High Gain CellID %d",id),4200,-200,4000);
    hspectraHG.SetDirectory(0);
    hspectraLG    = TH1D(Form("hspectra%sLGCellID%d",name.Data(),id),Form("ADC spectrum Low  Gain CellID %d",id),4200,-200,4000);
    hspectraLG.SetDirectory(0);
    hspectraLGHG  = TProfile(Form("hCoorspectra%sLGHGCellID%d",name.Data(),id),Form("ADC Low  Gain/High Gain correlation CellID %d",id),900,-100,800);
    hspectraLGHG.SetDirectory(0);
  }
  ~TileSpectra(){}

  bool Fill(double, double);
  bool FillSpectra(double, double);
  bool FillCorr(double, double);
  bool FitNoise(double*);
  bool FitNoiseWithBG(double*);
  //bool FitAndPlotGainCorr(double*);//Maybe should be two separate functions

  int GetCellID();
  TH1D* GetHG();
  TH1D* GetLG();
  TH1D* GetHGLGcomb();
  TProfile* GetLGHGcorr();

  TF1* GetBackModel(int);
  TF1* GetSignalModel(int);
  TF1* GetCorrModel();

  void Write(bool);
 protected:
  TString TileName;
  int cellID;
  TileCalib* calib;
  int debug;
  bool bpedHG;
  bool bpedLG;
  bool bmipHG;
  bool bmipLG;
  bool bcorrHGLG;
  TF1 BackgroundLG;
  TF1 BackgroundHG;
  TF1 SignalLG;
  TF1 SignalHG;
  TF1 HGLGcorr;
  TH1D hspectraHG;
  TH1D hspectraLG;
  TH1D hcombined;
  TProfile hspectraLGHG;
  static double langaufun(double */*x*/, double */*par*/);
  static int langaupro(double */*params*/, double &/*maxx*/, double &/*FWHM*/);

  ClassDef(TileSpectra,1);
};


#endif
