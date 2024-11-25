#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TSpectrum.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TMath.h"
#include "TLeaf.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TVirtualFitter.h"
#include <iostream>

typedef double Double_t;

using namespace std;
using std::ofstream;

Int_t npeaks = 0;

const int gNumSipm = 4;
const int gNumOv = 1;

Double_t fpeaks(Double_t * x, Double_t * par)
{
  Double_t result = par[0] + par[1] * x[0];
  for (Int_t p = 0; p < npeaks; p++)
    {
      Double_t norm = par[3 * p +2]; // "height" or "area"
      Double_t mean = par[3 * p + 3];
      Double_t sigma = par[3 * p + 4];
#if defined(__PEAKS_C_FIT_AREAS__)
      norm /= sigma * (TMath::Sqrt(TMath::TwoPi())); // "area"
#endif                                               /* defined(__PEAKS_C_FIT_AREAS__) */
      result += norm * TMath::Gaus(x[0], mean, sigma);
    }
  return result;
}


void MultiMultiphoton(){
    
    const int ch[4] = {00, 01, 32, 33};
    TH1D *h[4];
    TH1 *bg[4];
    TSpectrum *s[4];
    
    TCanvas *c = new TCanvas("c", "c",515,81,837,763);
    c->Divide(4, 2); // The pad is 1 coloumn and two lines
    
    std::string filenames[gNumOv];
    filenames[0] = "Truncated_Dummy.txt";
//    filenames[1] = "/Users/ar2545/Downloads/S14160_1315_Scans/Trunc_Run1_list_45.txt";
//    filenames[2] = "/Users/ar2545/Downloads/S14160_1315_Scans/Trunc_Run1_list_44.txt";
    double Vop[gNumOv];
    double yAvg[gNumSipm][gNumOv];
    
    for(int i=0; i < gNumOv; i++)
    {
        //        if(i>0)break;
        TTree *t = new TTree(Form("ntuple_%d",i),"data from ascii file"); // A temporary tree
        
        t->ReadFile(filenames[i].c_str(), "a:b:c"); // Read the data file
        
        
        for (Int_t k = 0; k < gNumSipm; k++) {
            
            h[k] = new TH1D(Form("h_%d_%d",k,i), "Multiphoton Spectrum; ADC ; Counts", t->GetEntries(), 0., (Double_t)t->GetEntries()); // A new histogram
            h[k]->Sumw2(kFALSE);
            h[k]->ResetStats();
            
            // Fill histogram with values from column c where column a is equal to 0
            for (Int_t i = 0; i < t->GetEntries(); ++i) {
                t->GetEntry(i);
                if (t->GetLeaf("a")->GetValue() == ch[k]) {
                    //                    	cout<< "check 1 ======>"<<ch[k]<< endl;
                    //                    cout<< "check 2 ======>" << t->GetLeaf("c")->GetValue()<<endl;
                    
                    
                    h[k]->Fill(t->GetLeaf("c")->GetValue());
                }
            }
            cout<< "check 3 ---------->"<<endl;
            
            //delete t; // No longer needed
            
            s[k] = new TSpectrum(); // Use TSpectrum to find the peak candidates
            
            bg[k] = s[k]->Background(h[k], 20, "smoothing"); // Estimate the background
            
            bg[k]->SetLineColor(kRed); // Set the color
            
            c->cd(k+5);        // Move to line one
            bg[k]->Draw("SAME"); // Draw the bg in the first line
            
            //h->SetTitleFont(82);
            
            //h->GetYaxis()->SetTitleFont(82);
            h[k]->GetYaxis()->SetTitleSize(0.05);
            h[k]->GetXaxis()->SetTitleSize(0.05);
            h[k]->GetYaxis()->SetTitleOffset(1);
            h[k]->GetXaxis()->SetTitleOffset(1);
            h[k]->GetXaxis()->SetLabelSize(0.05);
            h[k]->GetYaxis()->SetLabelSize(0.05);
            h[k]->GetXaxis()->SetRange(0,2000);
            h[k]->SetStats(0);
            h[k]->SetTitle("S1416-1315 @  V_{op} = 7 V (High-Gain)");
            h[k]->DrawCopy("");
            TLatex *   tex1 = new TLatex();
            
//            bg[k]->Draw("SAME"); // Draw the bg in the first line
            
            //if (gPad)
            //gPad->SetGrid(1, 1);
            
            c->cd(k+1); // Move to line two
            
            h[k]->Add(bg[k], -1.); // Subtract the estimated background
            
            Int_t nfound = s[k]->Search(h[k], 7., "", 0.05); // Search for peaks
            
            printf("We found %d candidate peaks to fit.\n", nfound);
            
            Double_t *xpeaks;
            Double_t par[3000];
            xpeaks = s[k]->GetPositionX();
            
            std::sort(xpeaks, xpeaks + nfound, [&](Double_t a, Double_t b) {
                return a > b;
            });
            
            std::cout << "Differences between consecutive entries:" << std::endl;
            Double_t sum_diff = 0.0;
            for (size_t i = 1; i < (Int_t)nfound; ++i) {
                Double_t diff = xpeaks[i - 1] - xpeaks[i];
                std::cout << diff << " ";
                sum_diff += diff;
            }
            std::cout << std::endl;
            
            Double_t average_diff = sum_diff / (nfound - 1);
            std::cout << "Average of differences: " << average_diff << std::endl;
            yAvg[k][i] = average_diff;
      
            for (Int_t p = 0; p < (Int_t)nfound; p++)
            {
                Double_t xp = xpeaks[p];
                Int_t bin = h[k]->GetXaxis()->FindBin(xp);
                Double_t yp = h[k]->GetBinContent(bin);
                
                par[3 * npeaks + 2] = yp;     // "height"
                par[3 * npeaks + 3] = xp; // "mean"
                par[3 * npeaks + 4] = 3;  // "sigma"
                
                
                cout<<xp<<endl;
                npeaks++;
                
            }
            
            
            printf("Howdy, now fitting: Be patient.\n");
            
            TF1 *fit = new TF1("fit", fpeaks, 0, 4096, 3 * npeaks);
            
            TVirtualFitter::Fitter(h[k], 3 * npeaks);
            fit->SetParameters(par);
            fit->SetNpx(1000);
            
            h[k]->Fit("fit");
            
            //h->GetYaxis()->SetTitleFont(82);
            //h->GetXaxis()->SetTitleFont(82);
            
            h[k]->Draw("E"); // Draw the plot without bg
           
            
            //if (gPad)
            //gPad->SetGrid(1, 1);
            
            /* c->cd(2); */
            
            /* TLatex *   tex = new TLatex(); */
            /* tex->DrawLatex(1400,2225.444,"Background Subtracted"); */
            /* tex->DrawLatex(1030.066,1693.444,Form("Average peak-to-peak value = %0.2f",average_diff)); */
            /* tex->SetTextFont(82); */
            /* tex->Draw("SAME"); */
            
            printf("There, done! You happy?\n");
            
            
        }
    }
    c->SaveAs("fittedPeak.pdf");
//    cout<<yAvg[0][0]<<endl;
//    cout<<yAvg[3][2]<<endl;
    
    double y0[gNumOv];
//    double y1[gNumOv];
//    double y2[gNumOv];
//    double y3[gNumOv];
    
    for (int i = 0; i < gNumOv; ++i) {
        y0[i] = yAvg[0][i];
        cout<<y0[i]<<endl;
//        y1[i] = yAvg[1][i];
//        cout<<y1[i]<<endl;
//        y2[i] = yAvg[2][i];
//        cout<<y2[i]<<endl;
//        y3[i] = yAvg[3][i];
        
    }

    TGraph* g = new TGraph(gNumOv,y0,Vop);
//    TGraph* g1 = new TGraph(gNumOv,y1,Vop);
//    TGraph* g2 = new TGraph(gNumOv,y2,Vop);
//    TGraph* g3 = new TGraph(gNumOv,y3,Vop);
//    
    TMultiGraph *multiGraph = new TMultiGraph();
    multiGraph->Add(g);
//    multiGraph->Add(g1);
    
    
    TCanvas *canvas = new TCanvas("canvas", "Multiple Graphs", 800, 600);
    multiGraph->Draw("APL"); // "APL" stands for axes, points, and line
}
