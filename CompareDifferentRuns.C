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

const int gMaxChannels = 64;
const int gMaxBoard    = 1;
const int gMaxLayers   = 16;
// Define tree variables
Long64_t gTrID;
Double_t gTRtimeStamp;
Long64_t* gBoard          = new Long64_t[gMaxChannels];
Long64_t* gChannel        = new Long64_t[gMaxChannels];
Long64_t* gLG             = new Long64_t[gMaxChannels];
Long64_t* gHG             = new Long64_t[gMaxChannels];

// Definition of color/marker arrays for plotting
Color_t colorLayer[16]    = { kRed-7, kBlue+1, kCyan-3, kMagenta+1, kPink-1, kViolet-9, kOrange, kGreen+1,  
                              kRed+1, kBlue-9, kCyan+1, kMagenta-7, kOrange+7, kSpring+5, kPink+5, kViolet+8 };
Color_t colorReadBoard[8] = { kRed+1, kBlue+1, kCyan+1, kMagenta+1, kOrange, kGreen+1, kPink+5, kViolet-9};
Style_t markerLayer[16]     = { 20, 25, 33, 28, 29, 37, 41, 46,
                                24, 21, 27, 34, 30, 39, 40, 47};
Style_t markerReadBoard[8]  = { 20, 21, 33, 34, 29, 39, 40, 46};                             

// #include "makeSimplePlotsFromJanusTree.h"



struct mipDataPoint{
    mipDataPoint(): layer(0), channel(0), mpvL(0), empvL(0), max(0), emax(0), vop(0), runnr(0), beame(0), isHad (true) {}
    int layer;
    int channel;
    float mpvL;
    float empvL;
    float max;
    float emax;
    float vop;
    int runnr;
    float beame;
    bool isHad;
} ;


//__________________________________________________________________________________________________________
//_____________________MAIN function !!! ___________________________________________________________________
//__________________________________________________________________________________________________________
void CompareDifferentRuns( TString configFileName     = "", 
                           TString outputDir          = "Compare/",
                            Int_t verbosity           = 0,
                            Int_t isTBdata            = 0,
                            TString mappingFile       = "",
                            TString runListFileName   = "configs/SPS_RunNumbers.txt"
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
    std::vector<TGraphErrors*>graphsMPV;
    std::vector<TGraphErrors*>graphsMax;
    
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
      TGraphErrors* tempGraph = (TGraphErrors*)tempFile->Get("graphMPV_HG_channels");
      graphsMPV.push_back(tempGraph);
      tempGraph = (TGraphErrors*)tempFile->Get("graphMax_HG_channels");
      graphsMax.push_back(tempGraph);
    }
    std::cout<<"=============================================================="<<std::endl;

    std::vector<mipDataPoint> mipPoints;
    for (Int_t r = 0; r < runsToComp; r++){
      std::cout <<  runnumbers.at(r) << "\t" << fileNames.at(r).Data() << "\t" << compRuns.at(r).species.Data() << "\t" << compRuns.at(r).vop << std::endl;
      if (graphsMPV.at(r)->GetN() !=graphsMax.at(r)->GetN() ){
        std::cout << "MPV and Max graph don't have same size ... something is wrong!" << std::endl;
        continue;
      }
      for (Int_t e = 0; e < (Int_t)graphsMPV.at(r)->GetN(); e++){
        mipDataPoint currMip;
        currMip.layer   = (int)(graphsMPV.at(r)->GetX()[e]/10);
        currMip.channel = (int)(graphsMPV.at(r)->GetX()[e]-10*currMip.layer);
        currMip.mpvL    = (float)(graphsMPV.at(r)->GetY()[e]);
        currMip.empvL   = (float)(graphsMPV.at(r)->GetEY()[e]);
        currMip.max     = (float)(graphsMax.at(r)->GetY()[e]);
        currMip.emax    = (float)(graphsMax.at(r)->GetEY()[e]);
        currMip.vop     = (float)(compRuns.at(r).vop);
        currMip.runnr   = (int)(compRuns.at(r).runNr);
        currMip.beame   = (float)(compRuns.at(r).energy);
        if (compRuns.at(r).species.CompareTo("h") == 0)
          currMip.isHad   = true;
        else 
          currMip.isHad   = false;
//         std::cout << graphsMPV.at(r)->GetX()[e] << "\t l: " << currMip.layer << "\t c: " << currMip.channel << "\t vop: " << currMip.vop << std::endl;
        mipPoints.push_back(currMip);
      }      
    }
    
    Float_t tempYVal[5000];
    Float_t tempXVal[5000];
    Float_t tempEYVal[5000];
    Float_t tempEXVal[5000];
 
    for (Int_t p = 0; p< (Int_t)mipPoints.size(); p++){
      tempYVal[p]     = mipPoints.at(p).max;
      tempEYVal[p]    = mipPoints.at(p).emax;
      tempXVal[p]     = mipPoints.at(p).vop;
      tempEXVal[p]    = 0;
    }
    TGraphErrors* graphVoVvsMipMax = new TGraphErrors((Int_t)mipPoints.size(),tempXVal,tempYVal,tempEXVal,tempEYVal );
    graphVoVvsMipMax->Sort();
    for (Int_t p = 0; p< (Int_t)mipPoints.size(); p++){
      tempYVal[p]     = mipPoints.at(p).max;
      tempEYVal[p]    = mipPoints.at(p).emax;
      tempXVal[p]     = mipPoints.at(p).runnr;
      tempEXVal[p]    = 0;
    }
    TGraphErrors* graphRunNrvsMipMax = new TGraphErrors((Int_t)mipPoints.size(),tempXVal,tempYVal,tempEXVal,tempEYVal );
    graphRunNrvsMipMax->Sort();
    
    
    TGraphErrors* graphPerChannelVoVvsMipMax[gMaxLayers][9];
    Int_t nEntriesGraphsPerChannel[gMaxLayers][9];
    Float_t Vop[gMaxLayers][9][300];
    Float_t eVop[gMaxLayers][9][300];
    Float_t mipMax[gMaxLayers][9][300];
    Float_t emipMax[gMaxLayers][9][300];
    bool layerExists[gMaxLayers];
    for (Int_t l = 0; l < gMaxLayers; l++){
      layerExists[l] = false;
      for (Int_t cb = 0; cb < 9; cb++){
        graphPerChannelVoVvsMipMax[l][cb] = nullptr;
        nEntriesGraphsPerChannel[l][cb]   = 0;
        for (Int_t n = 0; n < 300; n++){
          Vop[l][cb][n]       = 0;
          eVop[l][cb][n]      = 0;
          mipMax[l][cb][n]    = 0;
          emipMax[l][cb][n]   = 0;
        }
      }
    }
    
    // filter for hadrons only
    for (Int_t p = 0; p< (Int_t)mipPoints.size(); p++){
      if (mipPoints.at(p).isHad){
        Int_t currEnt = nEntriesGraphsPerChannel[mipPoints.at(p).layer][mipPoints.at(p).channel];
        Vop[mipPoints.at(p).layer][mipPoints.at(p).channel][currEnt]      = mipPoints.at(p).vop;
        mipMax[mipPoints.at(p).layer][mipPoints.at(p).channel][currEnt]   = mipPoints.at(p).max;
        emipMax[mipPoints.at(p).layer][mipPoints.at(p).channel][currEnt]  = mipPoints.at(p).emax;
        
        nEntriesGraphsPerChannel[mipPoints.at(p).layer][mipPoints.at(p).channel]++;
      }
    }
    
    
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t cb = 0; cb < 9; cb++){
        if (nEntriesGraphsPerChannel[l][cb] != 0){
          layerExists[l] = true;
          graphPerChannelVoVvsMipMax[l][cb] = new TGraphErrors(nEntriesGraphsPerChannel[l][cb], Vop[l][cb], mipMax[l][cb], eVop[l][cb], emipMax[l][cb] );
          std::cout << "layer: " << l << "\t channel:\t" << cb << std::endl;
          graphPerChannelVoVvsMipMax[l][cb]->Sort();
//           graphPerChannelVoVvsMipMax[l][cb]->Print();
        }
      }
    }
    
    
    
    TCanvas* canvas1DSimple = new TCanvas("canvas1DSimple","",0,0,700,500);  // gives the page size
    DefaultCancasSettings( canvas1DSimple, 0.08, 0.02, 0.02, 0.08);
    
    TH1D* histDummyVvsMPV = new TH1D("histDummyVvsMPV","", 70, 0, 7 );
    SetStyleHistoTH1ForGraphs( histDummyVvsMPV, "#it{V}_{ov} (V)", "MPV_{lan-gaus} (HG ADC)",0.85* textSizeRel, textSizeRel, 0.85* textSizeRel, textSizeRel, 0.9, 0.95);
    histDummyVvsMPV->GetYaxis()->SetRangeUser(100, 1000);
    histDummyVvsMPV->Draw();
      SetMarkerDefaultsTGraphErr(  graphVoVvsMipMax, 24, 0.5, kBlue+1, kBlue+1, 1);
      
      graphVoVvsMipMax->Draw("same,p,e");
      DrawLatex(0.95, 0.92, "SPS hadron-beam, LFHCal-Prototype 1", true, textSizeRel, 42);
    canvas1DSimple->SaveAs(Form("%s/VopVsMPVlangaus.pdf",outputDirPlots.Data()));
    
    TH1D* histDummyRunNrvsMPV = new TH1D("histDummyRunNrvsMPV","", 201, -0.5,200.5  );
    SetStyleHistoTH1ForGraphs( histDummyRunNrvsMPV, "Run Nr.", "MPV_{lan-gaus} (HG ADC)",0.85* textSizeRel, textSizeRel, 0.85* textSizeRel, textSizeRel, 0.9, 0.95);
    histDummyRunNrvsMPV->GetYaxis()->SetRangeUser(100, 1000);
    histDummyRunNrvsMPV->Draw();
      SetMarkerDefaultsTGraphErr(  graphRunNrvsMipMax, 24, 0.5, kBlue+1, kBlue+1, 1);      
      graphRunNrvsMipMax->Draw("same,p,e");

      DrawLatex(0.95, 0.92, "SPS hadron-beam, LFHCal-Prototype 1", true, textSizeRel, 42);
    canvas1DSimple->SaveAs(Form("%s/RunNrVsMPVlangaus.pdf",outputDirPlots.Data()));

    for (Int_t l = 0; l < gMaxLayers; l++ ){
      if (layerExists[l]){
        canvas1DSimple->cd();
        canvas1DSimple->Clear();
        histDummyVvsMPV->Draw();
        
        TLegend* legend = GetAndSetLegend2( 0.12, 0.93-3*textSizeRel, 0.4, 0.93,textSizeRel, 4, Form("Layer %d, channel:",l), 42,0.2);
        for (Int_t cb = 1; cb < 9; cb++){
          if (graphPerChannelVoVvsMipMax[l][cb]){
            SetMarkerDefaultsTGraphErr( graphPerChannelVoVvsMipMax[l][cb], markerReadBoard[cb-1], 0.8, colorReadBoard[cb-1],colorReadBoard[cb-1]);
            graphPerChannelVoVvsMipMax[l][cb]->Draw("p,e,same");
            legend->AddEntry(graphPerChannelVoVvsMipMax[l][cb], Form("%d",cb), "p");
            
          }
        }
        DrawLatex(0.95, 0.92, "SPS hadron-beam, LFHCal-Prototype 1", true, textSizeRel, 42);

        
        legend->Draw();
        canvas1DSimple->SaveAs(Form("%s/VopVsMPVlangaus_Layer%02d.pdf",outputDirPlots.Data(), l));
      }
    }
    
}
