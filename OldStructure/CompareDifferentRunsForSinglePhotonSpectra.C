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
    spectraFitDataPoint(): layer(0), channel(0), vop(0), runnr(0), lgSet(0), hgSet(0), assemNr (0) {}
    int layer;
    int channel;
    float vop;
    int runnr;
    int lgSet;
    int hgSet;
    int nSPEPeaks;
    double avDiffSPEPeaks;
    double avDiffSPEPeaksFit;
    int assemNr;
} ;
//__________________________________________________________________________________________________________
//_____________________MAIN function !!! ___________________________________________________________________
//__________________________________________________________________________________________________________
void CompareDifferentRunsForSinglePhotonSpectra( TString configFileName   = "configs/BoardA16_RunNumbers",         // list of runs to analyze, filename + runNumber, should be run for one board at the time
                                                TString outputDir         = "Compare/A16/",
                                                Int_t verbosity           = 0,
                                                TString mappingFile       = "configs/mappingA16_SinglePhoton.txt", // board name here 
                                                TString runListFileName   = "configs/ORNL_RunNumbers.txt",         // general list of the runs, no board name
                                                Int_t specialData         = 0                                      // specialData: 0- std. TB data, 1 - local ORNL SPE
                                  ){
    StyleSettingsThesis("pdf");
    SetPlotStyle();
    Double_t textSizeRel = 0.04;
    
    Color_t colorReadBoard[8] = { kRed+1, kBlue+1, kCyan+1, kMagenta+1, kOrange, kGreen+1, kPink+5, kViolet-9};
    Style_t markerReadBoard[8]  = { 20, 21, 33, 34, 29, 39, 40, 46};                             

    
      // make output directory
    TString dateForOutput = ReturnDateStr();
    TString outputDirPlots = Form("%s/%s",outputDir.Data(), dateForOutput.Data());
    gSystem->Exec("mkdir -p "+outputDir);
    gSystem->Exec("mkdir -p "+outputDirPlots);

    // ********************************************************************************************************
    // read run list and corresponding settings
    // ********************************************************************************************************
    std::vector<runInfo> allRuns = readRunInfosFromFile(runListFileName, 1, specialData);
    
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
      if (specialData == 1){
        std::cout <<  runnumbers.at(r) << "\t" << fileNames.at(r).Data() << "\t" << compRuns.at(r).vop << "\t"<< compRuns.at(r).assemblyNr << std::endl;
      } else {
        std::cout <<  runnumbers.at(r) << "\t" << fileNames.at(r).Data() << "\t" << compRuns.at(r).vop  << std::endl;
      }
      
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

    Double_t rangeDiffPeaks[2] = {0.};
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
        if (specialData == 1) currFit.assemNr = (int)(compRuns.at(r).assemblyNr);
//         std::cout << graphsMPV.at(r)->GetX()[e] << "\t l: " << currMip.layer << "\t c: " << currMip.channel << "\t vop: " << currMip.vop << std::endl;
        spectraFitPoints.push_back(currFit);
        if (currFit.avDiffSPEPeaks < rangeDiffPeaks[0]) rangeDiffPeaks[0] = currFit.avDiffSPEPeaks;
        if (currFit.avDiffSPEPeaks > rangeDiffPeaks[1]) rangeDiffPeaks[1] = currFit.avDiffSPEPeaks;
      }      
    }

    Int_t gMaxAssemblies = 100;
    TGraphErrors* graphAvSPEDiffVsVop[gMaxAssemblies][gMaxChannels];
    TGraphErrors* graphAvSPEDiffFitVsVop[gMaxAssemblies][gMaxChannels];

    for (Int_t a = 0; a < gMaxAssemblies; a++){
      for (Int_t ch=0; ch<gMaxChannels; ch++){
        graphAvSPEDiffVsVop[a][ch]     = new TGraphErrors();
        graphAvSPEDiffFitVsVop[a][ch]  = new TGraphErrors();
      }
    }
    for(Int_t f = 0; f<(Int_t)spectraFitPoints.size(); f++){
      Int_t ch      = spectraFitPoints.at(f).channel-1;
      Int_t nPeaks  = spectraFitPoints.at(f).nSPEPeaks;
      Int_t assem   = spectraFitPoints.at(f).layer;
      if (specialData == 1){
        assem = spectraFitPoints.at(f).assemNr;
      }
      graphAvSPEDiffVsVop[assem][ch]->SetPoint(graphAvSPEDiffVsVop[assem][ch]->GetN(), spectraFitPoints.at(f).vop, spectraFitPoints.at(f).avDiffSPEPeaks);
      graphAvSPEDiffFitVsVop[assem][ch]->SetPoint(graphAvSPEDiffFitVsVop[assem][ch]->GetN(), spectraFitPoints.at(f).vop, spectraFitPoints.at(f).avDiffSPEPeaksFit);
    }

    for (Int_t a = 0; a < gMaxAssemblies; a++){
      for(Int_t ch=0; ch<gMaxChannels; ch++){
//         std::cout << "A" << a <<" channel: " << ch << std::endl;
        if( graphAvSPEDiffVsVop[a][ch]->GetN() !=0)      graphAvSPEDiffVsVop[a][ch]->Sort();
        if( graphAvSPEDiffFitVsVop[a][ch]->GetN() !=0)   graphAvSPEDiffFitVsVop[a][ch]->Sort();
      }
    }

    // ********************************************************************************************************
    // plotting
    // ********************************************************************************************************

    TCanvas* canvas1DSimple = new TCanvas("canvas1DSimple","",0,0,700,500);  // gives the page size
    DefaultCancasSettings( canvas1DSimple, 0.08, 0.02, 0.02, 0.08);


    TH1D* histDummyAvDiffSPEvsVop = new TH1D("histDummyAvDiffSPEvsVop","", 55, 1.5, 7 );
    SetStyleHistoTH1ForGraphs( histDummyAvDiffSPEvsVop, "#it{V}_{ov} (V)", "#mu(#Delta_{SPE}) (HG ADC)",0.85* textSizeRel, textSizeRel, 0.85* textSizeRel, textSizeRel, 0.9, 0.95);
    histDummyAvDiffSPEvsVop->GetYaxis()->SetRangeUser(rangeDiffPeaks[0], rangeDiffPeaks[1]*1.5);
    histDummyAvDiffSPEvsVop->Draw();
    Bool_t lenteredAll[2]  = {0, 0};
    Bool_t plottedA      = 0; 
    TLegend* legend = GetAndSetLegend2( 0.12, 0.95-3*textSizeRel, 0.7, 0.95,textSizeRel, 4, "RB-Channel:", 42,0.1);

    for (Int_t a = 0; a < gMaxAssemblies; a++){
      for(Int_t ch=0; ch<gMaxChannels; ch++){
        if(graphAvSPEDiffVsVop[a][ch]->GetN() != 0 ) {
          SetMarkerDefaultsTGraphErr( graphAvSPEDiffVsVop[a][ch], markerReadBoard[ch], 0.8, colorReadBoard[ch],markerReadBoard[ch]);
          graphAvSPEDiffVsVop[a][ch]->Draw("p,e,same");
          legend->AddEntry(graphAvSPEDiffVsVop[a][ch], Form("A%02d, %d",a, ch+1), "p");
        }
      }
    }
    // DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
    legend->Draw();
    canvas1DSimple->SaveAs(Form("%s/AvDiffSPEPeaks.pdf",outputDirPlots.Data()));
    
    TH1D* histDummyAvDiffSPEvsVopF = new TH1D("histDummyAvDiffSPEvsVopF","", 55, 1.5, 7 );
    SetStyleHistoTH1ForGraphs( histDummyAvDiffSPEvsVopF, "#it{V}_{ov} (V)", "#mu(#Delta_{SPE,fit}) (HG ADC)",0.85* textSizeRel, textSizeRel, 0.85* textSizeRel, textSizeRel, 0.9, 0.95);
    histDummyAvDiffSPEvsVopF->GetYaxis()->SetRangeUser(rangeDiffPeaks[0], rangeDiffPeaks[1]*1.5);
    histDummyAvDiffSPEvsVopF->Draw();
    
    for (Int_t a = 0; a < gMaxAssemblies; a++){
      for(Int_t ch=0; ch<gMaxChannels; ch++){
        if( graphAvSPEDiffFitVsVop[a][ch]->GetN() != 0 ){
          SetMarkerDefaultsTGraphErr( graphAvSPEDiffFitVsVop[a][ch], markerReadBoard[ch], 0.8, colorReadBoard[ch],markerReadBoard[ch]);
          graphAvSPEDiffFitVsVop[a][ch]->Draw("p,e,same");
        }

      }
    }
    // DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
    legend->Draw();
    canvas1DSimple->SaveAs(Form("%s/AvDiffSPEPeaksFits.pdf",outputDirPlots.Data()));
}
