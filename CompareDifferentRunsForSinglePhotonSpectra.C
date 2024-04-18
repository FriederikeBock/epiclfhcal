#include <TROOT.h>
#include <TString.h>
#include <TObject.h>
#include <TObjString.h>
#include <TSystem.h>
#include <TChain.h>
#include <TMath.h>
#include <TVector3.h>
#include <iostream>
#include <fstream>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>
#include <TRandom3.h>

#include <TCanvas.h>
#include <TPad.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TString.h>
#include <TDatime.h>
#include <TF1.h>
#include <TF2.h>
#include <THStack.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TLatex.h>
#include <TArrow.h>
#include <TGraphErrors.h>
#include <TGaxis.h>
#include <TLegend.h>
#include <TFrame.h>
#include <TLorentzVector.h>

#include "PlottingHeader.h"
#include "FittingHeader.h"
#include "CommonVariables.h"

const int gMaxLayers    = 16;
const int gMaxSpecies   = 2;
const int gMaxVoltages  = 10;
const int gMaxBEn       = 11;
const int gMaxGainS     = 7;
const int gMaxChannels  = 8;

struct spectraFitDataPoint{
    spectraFitDataPoint(): layer(0), channel(0), vop(0), runnr(0), lgSet(0), hgSet(0) {}
    int layer;
    int channel;
    float vop;
    int runnr;
    int lgSet;
    int hgSet;
    int nSPEPeaks;
    double avDiffSPEPeaks;
    double avDiffSPEPeaksFit;
} ;
//__________________________________________________________________________________________________________
//_____________________MAIN function !!! ___________________________________________________________________
//__________________________________________________________________________________________________________
void CompareDifferentRunsForSinglePhotonSpectra( TString configFileName     = "configs/BoardA16_RunNumbers",                // list of runs to analyze, filename + runNumber, should be run for one board at the time
                                                TString outputDir          = "Compare/A16/",
                                                Int_t verbosity           = 0,
                                                TString mappingFile       = "configs/mappingA16_SinglePhoton.txt",       // board name here 
                                                TString runListFileName   = "configs/ORNL_RunNumbers.txt"  // general list of the runs, no board name
                                  ){
    StyleSettingsThesis("pdf");
    SetPlotStyle();
    Double_t textSizeRel = 0.04;
    
      // make output directory
    TString dateForOutput = ReturnDateStr();
    TString outputDirPlots = Form("%s/%s",outputDir.Data(), dateForOutput.Data());
    gSystem->Exec("mkdir -p "+outputDir);
    gSystem->Exec("mkdir -p "+outputDirPlots);

    // ********************************************************************************************************
    // read run list and corresponding settings
    // ********************************************************************************************************
    std::vector<runInfo> allRuns = readRunInfosFromFile(runListFileName, 1 );
    
    // ********************************************************************************************************    
    // read folder and name from file
    // ********************************************************************************************************
    std::vector<Int_t> runnumbers;
    std::vector<TString> fileNames;
    ifstream in;
    in.open(configFileName,ios_base::in);
    if (!in) {
        std::cout << "ERROR: file " << configFileName.Data() << " not found!" << std::endl;
        return;
    }

    for( TString tempLine; tempLine.ReadLine(in, kTRUE); ) {
      // check if line should be considered
      if (tempLine.BeginsWith("%") || tempLine.BeginsWith("#")){
          continue;
      }
      if (verbosity > 0) std::cout << tempLine.Data() << std::endl;

      // Separate the string according to tabulators
      TObjArray *tempArr  = tempLine.Tokenize("\t");
      if(tempArr->GetEntries()<1){
          if (verbosity > 1) std::cout << "nothing to be done" << std::endl;
          delete tempArr;
          continue;
      } else if (tempArr->GetEntries() == 1 ){
          // Separate the string according to space
          tempArr       = tempLine.Tokenize(" ");
          if(tempArr->GetEntries()<1){
              if (verbosity > 1) std::cout << "nothing to be done" << std::endl;
              delete tempArr;
              continue;
          } else if (tempArr->GetEntries() == 1  ) {
              if (verbosity > 1) std::cout << ((TString)((TObjString*)tempArr->At(0))->GetString()).Data() << " has not been reset, no value given!" << std::endl;
              delete tempArr;
              continue;
          }
      }

      TString tempname  = ((TString)((TObjString*)tempArr->At(1))->GetString());
      Int_t temprun     = ((TString)((TObjString*)tempArr->At(0))->GetString()).Atoi();
      runnumbers.push_back(temprun);
      fileNames.push_back(tempname);
    } 

    std::cout<<"=============================================================="<<std::endl;
    std::vector<runInfo> compRuns;
    std::vector<TGraphErrors*>graphsNSPEPeaks;
    std::vector<TGraphErrors*>graphsAvDiffSPEPeaks;
    std::vector<TGraphErrors*>graphsAvDiffSPEPeaksFit;
    
    Int_t runsToComp = (Int_t)runnumbers.size();
    
    for (Int_t r = 0; r < runsToComp; r++){
      
      Int_t indexCRun = findCurrentRun(allRuns, runnumbers.at(r));
      runInfo currentRunInfo;
      if (indexCRun < 0){
        std::cout << "run not in current list of runs, provided" << std::endl;
        return;
      } else {
        std::cout << "Run "<< runnumbers.at(r) << "\t found at index " << indexCRun << std::endl;
        currentRunInfo = GetRunInfoObject(allRuns,indexCRun);
      }
      compRuns.push_back(currentRunInfo);
      std::cout <<  runnumbers.at(r) << "\t" << fileNames.at(r).Data() << "\t" << compRuns.at(r).vop << std::endl;
      
      TFile* tempFile = new TFile(fileNames.at(r).Data(), "OPEN");
      if (tempFile->IsZombie()){
          std::cout << fileNames.at(r).Data() << " is broken, please remove from list or fix!" << std::endl;
      }
      TGraphErrors* tempGraph = (TGraphErrors*)tempFile->Get("graphnSPEPeaks_HG_channels");
      graphsNSPEPeaks.push_back(tempGraph);
      tempGraph = (TGraphErrors*)tempFile->Get("graphAvDiffSPEPeaks_HG_channels");
      graphsAvDiffSPEPeaks.push_back(tempGraph);
      tempGraph = (TGraphErrors*)tempFile->Get("graphAvDiffSPEPeaksFit_HG_channels");
      graphsAvDiffSPEPeaksFit.push_back(tempGraph);
    }
    std::cout<<"=============================================================="<<std::endl;

    std::vector<spectraFitDataPoint> spectraFitPoints;
    for (Int_t r = 0; r < runsToComp; r++){
      std::cout <<  runnumbers.at(r) << "\t" << fileNames.at(r).Data() << "\t" << compRuns.at(r).species.Data() << "\t" << compRuns.at(r).vop << std::endl;
      // if (graphsNSPEPeaks.at(r)->GetN() !=graphsAvDiffSPEPeaks.at(r)->GetN() || graphsNSPEPeaks.at(r)->GetN() !=graphsAvDiffSPEPeaksFit.at(r)->GetN() ){
      //   std::cout << "Graphs don't have same size ... something is wrong!" << std::endl;
      //   continue;
      // }
      for (Int_t e = 0; e < (Int_t)graphsNSPEPeaks.at(r)->GetN(); e++){
        spectraFitDataPoint currFit;
        currFit.layer   = (int)(graphsNSPEPeaks.at(r)->GetX()[e]/10);
        currFit.channel = (int)(graphsNSPEPeaks.at(r)->GetX()[e]-10*currFit.layer);
        currFit.vop     = (float)(compRuns.at(r).vop);
        currFit.runnr   = (int)(compRuns.at(r).runNr);
        currFit.lgSet   = (float)(compRuns.at(r).lgSet);
        currFit.hgSet   = (float)(compRuns.at(r).hgSet);
        currFit.nSPEPeaks          = (int)(graphsNSPEPeaks.at(r)->GetY()[e]);
        currFit.avDiffSPEPeaks     = (double)(graphsAvDiffSPEPeaks.at(r)->GetY()[e]);
        currFit.avDiffSPEPeaksFit  = (double)(graphsAvDiffSPEPeaksFit.at(r)->GetY()[e]);

//         std::cout << graphsMPV.at(r)->GetX()[e] << "\t l: " << currMip.layer << "\t c: " << currMip.channel << "\t vop: " << currMip.vop << std::endl;
        spectraFitPoints.push_back(currFit);
      }      
    }

    TGraphErrors* graphAvSPEDiffVsVop[gMaxChannels];
    TGraphErrors* graphAvSPEDiffFitVsVop[gMaxChannels];

    for (Int_t ch=0; ch<gMaxChannels; ch++){
      graphAvSPEDiffVsVop[ch]     = new TGraphErrors();
      graphAvSPEDiffFitVsVop[ch]  = new TGraphErrors();
    }

    for(Int_t f = 0; f<(Int_t)spectraFitPoints.size(); f++){
      Int_t   ch  = spectraFitPoints.at(f).channel-1;
      Int_t   nPeaks  = spectraFitPoints.at(f).nSPEPeaks;

      if( nPeaks < 3) continue;
      cout << "channel: " << ch << endl;
      cout << "nPeaks: " << spectraFitPoints.at(f).nSPEPeaks << endl;
      graphAvSPEDiffVsVop[ch]->SetPoint(graphAvSPEDiffVsVop[ch]->GetN(), spectraFitPoints.at(f).vop, spectraFitPoints.at(f).avDiffSPEPeaks);
      cout << "av diff " << spectraFitPoints.at(f).avDiffSPEPeaks << endl;
      graphAvSPEDiffFitVsVop[ch]->SetPoint(graphAvSPEDiffFitVsVop[ch]->GetN(), spectraFitPoints.at(f).vop, spectraFitPoints.at(f).avDiffSPEPeaksFit);
      cout << endl;    
    }

    for(Int_t ch=0; ch<gMaxChannels; ch++){
      if( graphAvSPEDiffVsVop[ch]->GetN() !=0)      graphAvSPEDiffVsVop[ch]->Sort();
      if( graphAvSPEDiffFitVsVop[ch]->GetN() !=0)   graphAvSPEDiffFitVsVop[ch]->Sort();
    }

    // ********************************************************************************************************
    // plotting
    // ********************************************************************************************************

    TCanvas* canvas1DSimple = new TCanvas("canvas1DSimple","",0,0,700,500);  // gives the page size
    DefaultCancasSettings( canvas1DSimple, 0.08, 0.02, 0.02, 0.08);


    TH1D* histDummyAvDiffSPEvsVop = new TH1D("histDummyAvDiffSPEvsVop","", 70, 0, 7 );
    SetStyleHistoTH1ForGraphs( histDummyAvDiffSPEvsVop, "#it{V}_{ov} (V)", "av diff SPE (HG ADC)",0.85* textSizeRel, textSizeRel, 0.85* textSizeRel, textSizeRel, 0.9, 0.95);
    // histDummyAvDiffSPEvsVop->GetYaxis()->SetRangeUser(rangeMaxMIP[0], rangeMaxMIP[1]);
    histDummyAvDiffSPEvsVop->Draw();
    Bool_t lenteredAll[2]  = {0, 0};
    Bool_t plottedA      = 0; 
    TLegend* legend = GetAndSetLegend2( 0.12, 0.95-2*textSizeRel, 0.25, 0.95,textSizeRel, 2, "Channel:", 42,0.4);

    for(Int_t ch=0; ch<gMaxChannels; ch++){
      if(graphAvSPEDiffVsVop[ch]->GetN() != 0 ) {
        SetMarkerDefaultsTGraphErr( graphAvSPEDiffVsVop[ch], 24, 0.8, kRed+1,kRed+1);
        graphAvSPEDiffVsVop[ch]->Draw("p,e,same");
        legend->AddEntry(graphAvSPEDiffVsVop[ch], Form("%d",ch), "p");
      }

      if( graphAvSPEDiffFitVsVop[ch]->GetN() != 0 ){
        SetMarkerDefaultsTGraphErr( graphAvSPEDiffFitVsVop[ch], 25, 0.8, kBlue+1,kBlue+1);
        graphAvSPEDiffFitVsVop[ch]->Draw("p,e,same");
        legend->AddEntry(graphAvSPEDiffFitVsVop[ch], Form("%d, fit",ch), "p");
      }

    }
    // DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
    legend->Draw();
    canvas1DSimple->SaveAs(Form("%s/AvDiffSPEPeaks.pdf",outputDirPlots.Data()));
}