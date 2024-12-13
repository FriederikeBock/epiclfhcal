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
    hspectraHG    = TH1D(Form("hspectra%sHGCellID%d",name.Data(),id),Form("ADC spectrum High Gain CellID %d; HG ADC (arb. units); counts ",id),4200,-200,4000);
    hspectraHG.SetDirectory(0);
    hspectraLG    = TH1D(Form("hspectra%sLGCellID%d",name.Data(),id),Form("ADC spectrum Low  Gain CellID %d; LG ADC (arb. units); counts",id),4200,-200,4000);
    hspectraLG.SetDirectory(0);
    hTriggPrim    = TH1D(Form("hTriggerPrimitive%sCellID%d",name.Data(),id),Form("Trigger primitive CellID %d; HG ADC (arb. units); counts ",id),500,0,4000);
    hTriggPrim.SetDirectory(0);
    hspectraLGHG  = TProfile(Form("hCoorspectra%sLGHGCellID%d",name.Data(),id),Form("ADC Low  Gain/High Gain correlation CellID %d; LG ADC (arb. units); HG ADC (arb. units)",id),800,0,800);
    hspectraLGHG.SetDirectory(0);
    hspectraHGLG  = TProfile(Form("hCoorspectra%sHGLGCellID%d",name.Data(),id),Form("ADC High  Gain/Low Gain correlation CellID %d; HG ADC (arb. units); LG ADC (arb. units)",id),4100,-100,4000);
    hspectraHGLG.SetDirectory(0);
  }
  ~TileSpectra(){}

  bool Fill(double, double);
  bool FillSpectra(double, double);
  bool FillCorr(double, double);
  bool FillTrigger(double);
  
  bool FitNoise(double*, int);
  bool FitMipHG(double*, double*, int, int, bool );
  bool FitMipLG(double*, double*, int, int, bool );
  bool FitCorr(int);
  bool FitNoiseWithBG(double*);

  int GetCellID();
  TH1D* GetHG();
  TH1D* GetLG();
  TH1D* GetTriggPrim();
  TH1D* GetHGLGcomb();
  TProfile* GetLGHGcorr();
  TProfile* GetHGLGcorr();

  TF1* GetBackModel(int);
  TF1* GetSignalModel(int);
  TF1* GetCorrModel(int);
  TileCalib* GetCalib();
  
  
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
  bool bcorrLGHG;
  bool bTriggPrim;
  TF1 BackgroundLG;
  TF1 BackgroundHG;
  TF1 SignalLG;
  TF1 SignalHG;
  TF1 HGLGcorr;
  TF1 LGHGcorr;
  TH1D hspectraHG;
  TH1D hspectraLG;
  TH1D hTriggPrim;
  TH1D hcombined;
  TProfile hspectraLGHG;
  TProfile hspectraHGLG;
  static double langaufun(double */*x*/, double */*par*/);
  static int langaupro(double */*params*/, double &/*maxx*/, double &/*FWHM*/);

  ClassDef(TileSpectra,2);
};


#endif
