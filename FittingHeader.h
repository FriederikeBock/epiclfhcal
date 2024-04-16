/***********************************************************************************************
*** provided by,                                                                          ******
***     Friederike Bock, fbock@cern.ch                                                    ******
************************************************************************************************/

#ifndef FITTINGGENERAL
#define FITTINGGENERAL

    
  //__________________________________________________________________________________________________________
  // Fit noise nicely
  //__________________________________________________________________________________________________________
  Bool_t FitNoise (TH1D* histo, TF1* &fit, Double_t &mean, Double_t &meanErr, Double_t &sigma, Double_t &sigmaErr, Int_t cb, Int_t cc, TString baseName, TString nameGain, Int_t verbosity = 0){

    if (!histo) return kFALSE;
    if (!(histo->GetEntries() > 0)) return kFALSE;

    fit = new TF1(Form("%s_B%d_C%02d",baseName.Data(),cb,cc), "gaus", 0, 100);
    // preset parames
    fit->SetParameter(0, histo->GetMaximum()); // Amplitude
    fit->SetParameter(1, histo->GetMean());    // Mean
    fit->SetParameter(2, histo->GetStdDev());  // Sigma
    histo->Fit(fit,"QRMNE0");
    if (verbosity > 0)  std::cout << "Channel b: " << cb << "\t c: " << cc << "\t" << nameGain.Data() << "  mean: " << fit->GetParameter(1) << "\t width: " << fit->GetParameter(2) << std::endl;
    mean = fit->GetParameter(1);
    meanErr = fit->GetParError(1);
    sigma = fit->GetParameter(2);
    sigmaErr = fit->GetParError(2);
    return kTRUE;
  }

  
  //__________________________________________________________________________________________________________
  // Plot & Fit gain Corr
  //__________________________________________________________________________________________________________
  void FitAndPlotGainCorr (TH2D* hist, TF1* &fit, TString baseNameFit, Double_t minFX, Double_t maxFX, Double_t maxPX, Double_t maxPY,
                          Double_t &slope, Double_t &slopeErr, Double_t &offset, Double_t &offsetErr, 
                          Int_t cb, Int_t cc, Int_t sl, Int_t sc, 
                          Bool_t bDetailed, TCanvas* canvas, TString baseNameOut, Double_t textSizeRel = 0.04 ){
      fit = new TF1(Form("%s_B%d_C%02d",baseNameFit.Data(), cb,cc), "[0]+[1]*x", minFX, maxFX);
      hist->Fit(fit,"QRMNE0");
      
      slope       = fit->GetParameter(1);
      slopeErr    = fit->GetParError(1);
      offset      = fit->GetParameter(0);
      offsetErr   = fit->GetParError(0);
      
      if (!bDetailed) return;
      canvas->cd();
        SetStyleHistoTH2ForGraphs( hist, hist->GetXaxis()->GetTitle(), hist->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 1.25);  
        SetStyleFit(fit , 0, maxPX, 7, 7, kBlack);
        hist->GetXaxis()->SetRangeUser(0,maxPX);
        hist->GetYaxis()->SetRangeUser(0,maxPY);
        hist->Draw("colz");
        fit->Draw("same");
        
        TLegend* legend = GetAndSetLegend2( 0.42, 0.15, 0.95, 0.3,textSizeRel, 1, Form("CAEN B %d, C %d, Stack L %d, C%d",cb, cc, sl, sc), 42,0.2);
        legend->AddEntry(fit, "f(x) = a#upoint x + b", "l");
        legend->AddEntry((TObject*)0, Form("a = %2.2f #pm %2.2f",slope, slopeErr ) , " ");
        legend->AddEntry((TObject*)0, Form("b = %2.2f #pm %2.2f",offset, offsetErr ) , " ");
        legend->Draw();
      canvas->SaveAs(Form("%s_B%d_C%02d.pdf", baseNameOut.Data(), cb,cc));
      return;
  }


  double langaufun(double *x, double *par) {

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
    double invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
    double mpshift  = -0.22278298;       // Landau maximum location

    // Control constants
    double np = 100.0;      // number of convolution steps
    double sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

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



  TF1 *langaufit(TH1D *his, double *fitrange, double *startvalues, double *parlimitslo, double *parlimitshi, double *fitparams, double *fiterrors, double *ChiSqr, int *NDF, Int_t verbosity = 0)
  {
    // Once again, here are the Landau * Gaussian parameters:
    //   par[0]=Width (scale) parameter of Landau density
    //   par[1]=Most Probable (MP, location) parameter of Landau density
    //   par[2]=Total area (integral -inf to inf, normalization constant)
    //   par[3]=Width (sigma) of convoluted Gaussian function
    //
    // Variables for langaufit call:
    //   his             histogram to fit
    //   fitrange[2]     lo and hi boundaries of fit range
    //   startvalues[4]  reasonable start values for the fit
    //   parlimitslo[4]  lower parameter limits
    //   parlimitshi[4]  upper parameter limits
    //   fitparams[4]    returns the final fit parameters
    //   fiterrors[4]    returns the final fit errors
    //   ChiSqr          returns the chi square
    //   NDF             returns ndf

    int i;
    char FunName[100];

    sprintf(FunName,"Fitfcn_%s",his->GetName());

    TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
    if (ffitold) delete ffitold;

    TF1 *ffit = new TF1(FunName,langaufun,fitrange[0],fitrange[1],4);
    ffit->SetParameters(startvalues);
    ffit->SetParNames("Width","MP","Area","GSigma");

    for (i=0; i<4; i++) {
      ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
    }

    TString fitOption = "QRLMNE0";
    if (verbosity > 1) 
      fitOption = "RLMNE0";
    
    his->Fit(FunName,fitOption);   // fit within specified range, use ParLimits, do not plot

    ffit->GetParameters(fitparams);    // obtain fit parameters
    for (i=0; i<4; i++) {
      fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
    }
    ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
    NDF[0] = ffit->GetNDF();           // obtain ndf

    return (ffit);              // return fit function

  }


  int langaupro(double *params, double &maxx, double &FWHM) {

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
  
  
#endif
