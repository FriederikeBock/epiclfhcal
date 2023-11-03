//This code fits the noise subtracted data to a Landau-Gauss fit
//This creates a per channel (1-64) TGraph of the MPV values and writes it to a root file
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TRandom3.h>
#include <TMath.h>

const int gMaxChannels = 64;
const int gMaxBoard    = 1;

//void CheckFitStatus(TF1* fitFunction, TH1* histogram, int numpara)
//{
//    TF1 *result = (TF1*)histogram->Fit(fitFunction, "S", ""); // "S" for silent fit
//    // Check the fit status
////    int fitStatus = result->Status();
//    if (fitStatus == 0)
//    {
//        for(int i=0; i<numpara; i++ )
//        {
//            // Fit was successful
//            fitval = fitFunction->GetParameter(i);
//        }
//    }
//    else {
//        // Fit failed
//        std::cout << "Fit failed. Fit status: " << fitStatus << std::endl;
//
//        // Handle the failure or take appropriate action
//    }
//}

void FitWithParameterLimitExpansion(int maxIterations, float increment, TF1* fitFunction, TH1* histogram) {
    
    histogram->Fit(fitFunction, "Q");
    
    int numFitParameters = fitFunction->GetNpar();
    
    for(int i=0; i < numFitParameters; i++)
    {
        // Access the fit parameter 'Mean' and its limits
        double fitval = fitFunction->GetParameter(i);
        double fitvalerr = fitFunction->GetParError(i);
        double fitvalLowerLimit, fitvalUpperLimit;
        fitFunction->GetParLimits(i, fitvalLowerLimit, fitvalUpperLimit);
        
        // Perform an iterative fit with parameter limit expansion
        for (int j = 0; j < maxIterations; j++) {
            // Check if the fit parameter is at its upper limit
            if (fitval == fitvalUpperLimit) {
                // Increase the upper limit by a small increment (you can adjust this)
                fitvalUpperLimit += 0.1;
                
                // Set the new parameter limits
                fitFunction->SetParLimits(i, fitvalLowerLimit, fitvalUpperLimit);
                
                // Perform the fit again
                histogram->Fit(fitFunction, "Q");
                
                // Update the fit parameter and chisq
                fitval = fitFunction->GetParameter(i);
                //                chisq = fitsnr->GetChisquare();
                //                ndf = fitsnr->GetNDF();
            } else {
                // If the parameter is not at its limit, exit the loop
                break;
            }
        }
        if (fitval == fitvalUpperLimit) {
            printf("Fit parameteris still at its upper limit after iterations.\n");
        } else {
            printf("Fit parameter is within its limits.\n");
        }
        
//        CheckFitStatus(fitFunction, histogram, numFitParameters);
        
        // Draw the histogram
        histogram->Draw();
        
        //    c1->Update();
    }
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
 
 
 
TF1 *langaufit(TH1F *his, double *fitrange, double *startvalues, double *parlimitslo, double *parlimitshi, double *fitparams, double *fiterrors, double *ChiSqr, int *NDF)
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
 
   his->Fit(FunName,"RB0");   // fit within specified range, use ParLimits, do not plot
 
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

void LanGauData(const char* datafile, int runnumber, TString outputDir    = "LanGauFitData/")
{
    
    
    outputDir = Form("%s/Run%05d",outputDir.Data(), runnumber );
    gSystem->Exec("mkdir -p "+outputDir);
    // Create a ROOT canvas to display the histogram and fit results
    TCanvas *c1 = new TCanvas("c1", "LanGau Fit Example", 800, 600);
    
    //Get the data file with subtracted data
    TFile* fdatsub = new TFile(datafile,"READ");
    
    // Create a 2D array of histograms
//    TH2F *histHGLG[gMaxBoard][gMaxChannels];
    TH1F* histLG[gMaxBoard][gMaxChannels];
    TH1F* histHG[gMaxBoard][gMaxChannels];
    TH1F* newhistHG[gMaxBoard][gMaxChannels];
    
    //Get the noise subtracted files at the right voltage
    for (Int_t j = 0; j < gMaxBoard; j++){
        for (Int_t i = 0; i < gMaxChannels; i++){
            //noise histogram
            newhistHG[j][i] = (TH1F*)fdatsub->Get(Form("sh_HG_B%d_C%02d",j,i));
            newhistHG[j][i]->RebinX(5);
            histHG[j][i] = (TH1F*)newhistHG[j][i]->Clone();
            for(int k=1; k < 701; k++)
            {
                histHG[j][i]->SetBinContent(k,newhistHG[j][i]->GetBinContent(k+10));
            }
            histLG[j][i] = (TH1F*)fdatsub->Get(Form("sh_LG_B%d_C%02d",j,i));
            
        }
    }
    
    //Now we want to fit these with Landau-Gauss fit and store the fit values in a TGraph with the errors representing the fit errors.
    //The TGraph will have 64 values corresponding to the channels on one digitizer board
    //The parameters of the LG fit are
        //   par[0]=Width (scale) parameter of Landau density
        //   par[1]=Most Probable (MP, location) parameter of Landau density
        //   par[2]=Total area (integral -inf to inf, normalization constant)
        //   par[3]=Width (sigma) of convoluted Gaussian function
    //Additionally, we want to keep track of the chisq and ndf for the fits
    TGraphAsymmErrors* mpv_gr = new TGraphAsymmErrors();;
    TGraphAsymmErrors* scale_gr = new TGraphAsymmErrors();;
    TGraphAsymmErrors* area_gr = new TGraphAsymmErrors();;
    TGraphAsymmErrors* sigma_gr = new TGraphAsymmErrors();;
    TGraph* chisq_gr = new TGraph();
    TGraph* ndf_gr = new TGraph();
    
    mpv_gr->GetYaxis()->SetTitle("MPV");
    mpv_gr->SetName("MPV");
    scale_gr->GetYaxis()->SetTitle("Scale");
    scale_gr->SetName("Scale");
    area_gr->GetYaxis()->SetTitle("Area");
    area_gr->SetName("Area");
    sigma_gr->GetYaxis()->SetTitle("Sigma");
    sigma_gr->SetName("Sigma");
    
    mpv_gr->GetXaxis()->SetTitle("Channel");
    scale_gr->GetXaxis()->SetTitle("Channel");
    area_gr->GetXaxis()->SetTitle("Channel");
    sigma_gr->GetXaxis()->SetTitle("Channel");
    
    TH1D* mpv_hist = new TH1D("mpv_hist","MPV fit values",500,0,500);
    TH1* scale_hist = new TH1D("scale_hist","Scale fit values",500,0,500);
    TH1* area_hist = new TH1D("area_hist","Area fit values",500,0,1000);
    TH1* sigma_hist = new TH1D("sigma_hist","Sigma fit values",100,0,100);
    TH1* chisq_hist = new TH1D("chisq_hist","ChiSQ",100,0,100);
    TH1* ndf_hist=new TH1D("ndf_hist","NDF",100,0,100);
//
    //Defining fit parameter names
    Double_t mpv;
    Double_t mpv_err;
    float_t chisq;
    Int_t ndf;
    float_t area;
    float_t area_err;
    float_t sigma;
    float_t sigma_err;
    float_t scale;
    float_t scale_err;
    
    //Defining the x array of channels
//    Double_t x[gMaxChannels];
//    for(int i =0; i < 64; i++)
//    {
//      x[i]=i;
//    }
//
    //Now loop over histograms in the file and run the fitter
     for (Int_t j = 0; j < gMaxBoard; j++)
    {
        for (Int_t i = 0; i < gMaxChannels; i++)
        {
            
            double fr[2];
            double sv[4], pllo[4], plhi[4], fp[4], fpe[4];
            double chisqr;
            int    ndf;
            int binmax = histHG[j][i]->GetMaximumBin(); double x = histHG[j][i]->GetXaxis()->GetBinCenter(binmax);
            
            //Set low and high parameters for the fit
            fr[0]=0.15*histHG[j][i]->GetMean();
            fr[1]=2*histHG[j][i]->GetMean();

            pllo[0] = 0;
            pllo[1] = x - histHG[j][i]->GetMean();//MPV
            pllo[2] = 0.9*(histHG[j][i]->Integral());
            pllo[3] = 2;

            plhi[0] = 0.5*histHG[j][i]->GetStdDev(1);
            plhi[1] = x + histHG[j][i]->GetMean();//MPV
            plhi[2] = 1000*(histHG[j][i]->Integral());
            plhi[3] = 18;

            //Fix start values
            sv[0]=0.25*histHG[j][i]->GetStdDev(1) ; sv[1]=x; sv[2]=1.5*(histHG[j][i]->Integral()); sv[3]=(pllo[3]+plhi[3])/2;

            //Perform the fit first time
            TF1 *fitsnr = langaufit(histHG[j][i],fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);
            
            scale = fp[0]; scale_err = fpe[0];
            mpv = fp[1]; mpv_err = fpe[1];
            area = fp[2]; area_err = fpe[2];
            sigma = fp[3]; sigma_err = fpe[3];
            
            FitWithParameterLimitExpansion(30, 0.1, fitsnr, histHG[j][i]);
            
            
            
            chisq = fitsnr->GetChisquare();
            ndf = fitsnr->GetNDF();
 
            //Add values to the graph
            int pointIndex = mpv_gr->GetN(); // Get the current number of points
            mpv_gr->SetPoint(pointIndex, i, mpv);
            mpv_gr->SetPointError(pointIndex, 0, 0, mpv_err, mpv_err);

            scale_gr->SetPoint(pointIndex, i, scale);
            scale_gr->SetPointError(pointIndex, 0, 0, scale_err, scale_err);

            area_gr->SetPoint(pointIndex, i, area);
            area_gr->SetPointError(pointIndex, 0, 0, area_err, area_err);

            sigma_gr->SetPoint(pointIndex, i, sigma);
            sigma_gr->SetPointError(pointIndex, 0, 0, sigma_err, sigma_err);
        
            //Fill histogram with fit values
            mpv_hist->Fill(mpv);
            scale_hist->Fill(scale);
            area_hist->Fill(area);
            sigma_hist->Fill(sigma);
            chisq_hist->Fill(chisq);
            ndf_hist->Fill(ndf);

        }
    }

//histHG[0][1]->Draw();
////            fitsnr->Draw("lsame");
//            gStyle->SetOptStat(1111);
//            gStyle->SetOptFit(111);
//            gStyle->SetLabelSize(0.03,"x");
//            gStyle->SetLabelSize(0.03,"y");
 
    TFile* fileOutput = new TFile(Form("%s/%d.root","LanGauFitData/",runnumber),"RECREATE");
    for (Int_t j = 0; j < gMaxBoard; j++)
    {
            mpv_hist->Write();
            scale_hist->Write();
            area_hist->Write();
            sigma_hist->Write();
            chisq_hist->Write();
            ndf_hist->Write();
            
            //Write TGraphs to output file
            mpv_gr->Write();
            scale_gr->Write();
            area_gr->Write();
            sigma_gr->Write();
            
    }
    
    histHG[0][24]->Draw();
}
   




