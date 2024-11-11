#include "TileSpectra.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

ClassImp(TileSpectra);

int TileSpectra::GetCellID(){
  return cellID;
}

bool TileSpectra::Fill(double l, double h){
  hspectraLG.Fill(l);
  hspectraHG.Fill(h);
  hspectraLGHG.Fill(l,h);
  return true;
}

bool TileSpectra::FitNoise(double* out){
  TFitResultPtr result;
  // estimate LG pedestal per channel
  BackgroundLG=TF1(Form("fped%sLGCellID%d",TileName.Data(),cellID),"gaus",0,400);
  BackgroundLG.SetNpx(400);
  BackgroundLG.SetParLimits(1,0,hspectraLG.GetMean()+100);     // might need to make these values settable
  BackgroundLG.SetParLimits(2,0,100);     // might need to make these values settable    
  BackgroundLG.SetParLimits(0,0,hspectraLG.GetEntries());
  BackgroundLG.SetParameter(0,hspectraLG.GetEntries()/5);
  BackgroundLG.SetParameter(1,hspectraLG.GetMean());
  BackgroundLG.SetParameter(2,10);
  result=hspectraLG.Fit(&BackgroundLG,"QRMEN0S"); // initial fit
  double minLGFit = result->Parameter(1)-2*result->Parameter(2);
  double maxLGFit = result->Parameter(1)+2*result->Parameter(2);
  if (debug > 1) std::cout << minLGFit << "\t" << maxLGFit << "\t" << hspectraLG.GetEntries() << "\t" << hspectraLG.GetMean()<< std::endl;
  result=hspectraLG.Fit(&BackgroundLG,"QRMEN0S","", minLGFit, maxLGFit);  // limit to 2sigma
  bpedHG=true;
  calib->PedestalMeanL=result->Parameter(1);//Or maybe we do not want to do it automatically, only if =0?
  calib->PedestalSigL =result->Parameter(2);//Or maybe we do not want to do it automatically, only if =0?
  out[0]=result->Parameter(1);
  out[1]=result->Error(1);
  out[2]=result->Parameter(2);
  out[3]=result->Error(2);
  
  // estimate HG pedestal per channel
  BackgroundHG=TF1(Form("fped%sHGCellID%d",TileName.Data(),cellID),"gaus",0,400);
  BackgroundHG.SetNpx(400);
  BackgroundHG.SetParLimits(1,0,hspectraHG.GetMean()+100);     // might need to make these values settable
  BackgroundHG.SetParLimits(2,0,100);     // might need to make these values settable    
  BackgroundHG.SetParLimits(0,0,hspectraHG.GetEntries());
  BackgroundHG.SetParameter(0,hspectraHG.GetEntries()/5);
  BackgroundHG.SetParameter(1,hspectraHG.GetMean());
  BackgroundHG.SetParameter(2,10);
  result=hspectraHG.Fit(&BackgroundHG,"QRMEN0S");      // initial fit
  double minHGFit = result->Parameter(1)-2*result->Parameter(2);
  double maxHGFit = result->Parameter(1)+2*result->Parameter(2);
  if (debug > 1) std::cout << minHGFit << "\t" << maxHGFit << std::endl;
  result=hspectraHG.Fit(&BackgroundHG,"QRMEN0S","",minHGFit, maxHGFit);  // limit to 2sigma range of previous fit
  bpedLG=true;
  
  calib->PedestalMeanH=result->Parameter(1);//Or maybe we do not want to do it automatically, only if =0?
  calib->PedestalSigH =result->Parameter(2);//Or maybe we do not want to do it automatically, only if =0?
  out[4]=result->Parameter(1);
  out[5]=result->Error(1);
  out[6]=result->Parameter(2);
  out[7]=result->Error(2);
  
  return true;
}

bool TileSpectra::FitNoiseWithBG(double* out){
  return true;
}

TH1D* TileSpectra::GetHG(){
  return &hspectraHG;
}

TH1D* TileSpectra::GetLG(){
  return &hspectraLG;
}

TH1D* TileSpectra::GetHGLGcomb(){
  return &hcombined;
}

TProfile* TileSpectra::GetLGHGcorr(){
  return &hspectraLGHG;
}

TF1* TileSpectra::GetBackModel(int lh){
  if(lh==0){
    return &BackgroundLG;
  }
  else if (lh==1){
    return &BackgroundHG;
  }
  else return nullptr;
}

TF1* TileSpectra::GetSignalModel(int lh){
    if(lh==0){
    return &SignalLG;
  }
  else if (lh==1){
    return &SignalHG;
  }
  else return nullptr;
}

TF1* TileSpectra::GetCorrModel(){
  return &HGLGcorr;
}

void TileSpectra::Write(){
  hspectraHG.Write();
  if(bpedHG)BackgroundHG.Write();
  hspectraLG.Write();
  if(bpedLG)BackgroundLG.Write();
  hspectraLGHG.Write();
}

double TileSpectra::langaufun(double *x, double *par) {

  //Fit parameters:
  //par[0]=Width (scale) parameter of Landau density
  //par[1]=Most Probable (MP, location) parameter of Landau density
  //par[2]=Total area (integral -inf to inf, normalization constant)
  //par[3]=Width (sigma) of convoluted Gaussian function
  //
  //In the Landau distribution (represented by the CERNLIB approximation),
  //the maximum is located at x=-0.22278298 with the location parameter=0.
  //This shift is corrected within this function, so that the actual
  //maximum is identical to the MP parameter.

  // Numeric constants
  static double invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
  static double mpshift  = -0.22278298;       // Landau maximum location

  // Control constants
  static double np = 100.0;      // number of convolution steps
  static double sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

  // Variables
  double xx;
  double mpc;
  double fland;
  double sum = 0.0;
  double xlow,xupp;
  double step;
  double i;


  // MP shift correction
  mpc = par[1] - mpshift * par[0];

  // Range of convolution integral
  xlow = x[0] - sc * par[3];
  xupp = x[0] + sc * par[3];

  step = (xupp-xlow) / np;

  // Convolution integral of Landau and Gaussian by sum
  for(i=1.0; i<=np/2; i++) {
    xx = xlow + (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);

    xx = xupp - (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);
  }

  return (par[2] * step * sum * invsq2pi / par[3]);
}


int TileSpectra::langaupro(double *params, double &maxx, double &FWHM) {

  // Searches for the location (x value) at the maximum of the
  // Landau-Gaussian convolute and its full width at half-maximum.
  //
  // The search is probably not very efficient, but it's a first try.
  double p,x,fy,fxr,fxl;
  double step;
  double l,lold;
  int i = 0;
  int MAXCALLS = 10000;

  // Search for maximum
  p = params[1] - 0.1 * params[0];
  step = 0.05 * params[0];
  lold = -2.0;
  l    = -1.0;

  while ( (l != lold) && (i < MAXCALLS) ) {
    i++;
    lold = l;
    x = p + step;
    l = langaufun(&x,params);
    if (l < lold)
      step = -step/10;
    p += step;
  }

  if (i == MAXCALLS)
    return (-1);
  maxx = x;
  fy = l/2;

  // Search for right x location of fy
  p = maxx + params[0];
  step = params[0];
  lold = -2.0;
  l    = -1e300;
  i    = 0;
    
  while ( (l != lold) && (i < MAXCALLS) ) {
    i++;
    lold = l;
    x = p + step;
    l = TMath::Abs(langaufun(&x,params) - fy);
    if (l > lold)
      step = -step/10;
    p += step;
  }

  if (i == MAXCALLS)
    return (-2);
  fxr = x;

  // Search for left x location of fy
  p = maxx - 0.5 * params[0];
  step = -params[0];
  lold = -2.0;
  l    = -1e300;
  i    = 0;

  while ( (l != lold) && (i < MAXCALLS) ) {
    i++;
    lold = l;
    x = p + step;
    l = TMath::Abs(langaufun(&x,params) - fy);
    if (l > lold)
      step = -step/10;
    p += step;
  }

  if (i == MAXCALLS)
    return (-3);


  fxl = x;
  FWHM = fxr - fxl;
  return (0);
}
  
