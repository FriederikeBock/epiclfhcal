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

struct mipDataPoint{
    mipDataPoint(): layer(0), channel(0), mpvL(0), empvL(0), max(0), emax(0), vop(0), runnr(0), beame(0), isHad (true), lgSet(0), hgSet(0) {}
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
    int lgSet;
    int hgSet;
} ;


//__________________________________________________________________________________________________________
//_____________________MAIN function !!! ___________________________________________________________________
//__________________________________________________________________________________________________________
void CompareDifferentRuns( TString configFileName     = "", 
                           TString outputDir          = "Compare/",
                            Int_t verbosity           = 0,
                            Int_t tbdata              = 0,        // 1: SPS 2023, 2: PS 2023
                            TString mappingFile       = "",
                            TString runListFileName   = "configs/SPS_RunNumbers.txt"
                            
                                  ){
    StyleSettingsThesis("pdf");
    SetPlotStyle();
    Double_t textSizeRel = 0.04;
    
      // make output directory
    TString dateForOutput = ReturnDateStr();
    TString outputDirPlots = Form("%s/%s",outputDir.Data(), dateForOutput.Data());
    TString outputDirPlotsH = Form("%s/%s/Hadron",outputDir.Data(), dateForOutput.Data());
    TString outputDirPlotsE = Form("%s/%s/Electron",outputDir.Data(), dateForOutput.Data());
    gSystem->Exec("mkdir -p "+outputDir);
    gSystem->Exec("mkdir -p "+outputDirPlots);
    gSystem->Exec("mkdir -p "+outputDirPlotsH);
    std::cout << outputDirPlotsH.Data() << std::endl;
    gSystem->Exec("mkdir -p "+outputDirPlotsE);
    TString labelTB = "";
    Double_t rangeMaxMIP[2] = {0,1000};
    if (tbdata == 1){
      labelTB = "SPS, LFHCal-Prototype 1";
      rangeMaxMIP[1]  = 1200;
    }
    
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
        currMip.lgSet   = (float)(compRuns.at(r).lgSet);
        currMip.hgSet   = (float)(compRuns.at(r).hgSet);
        
//         std::cout << graphsMPV.at(r)->GetX()[e] << "\t l: " << currMip.layer << "\t c: " << currMip.channel << "\t vop: " << currMip.vop << std::endl;
        mipPoints.push_back(currMip);
      }      
    }
    
    Float_t tempYVal[5000];
    Float_t tempXVal[5000];
    Float_t tempEYVal[5000];
    Float_t tempEXVal[5000];
 
    TGraphErrors* graphVoVvsMipMax[gMaxSpecies][gMaxBEn][gMaxGainS];
    TGraphErrors* graphRunNrvsMipMax[gMaxSpecies][gMaxBEn][gMaxGainS][gMaxVoltages];
    for (Int_t s = 0; s < gMaxSpecies; s++){
      for (Int_t gs = 0; gs < gMaxGainS; gs++){
        for (Int_t be = 0; be < gMaxBEn; be++){
          graphVoVvsMipMax[s][be][gs]   = new TGraphErrors();
          for (Int_t v = 0; v < gMaxVoltages; v++){
            graphRunNrvsMipMax[s][be][gs][v] = new TGraphErrors();
          }
        }
      }
    }
    for (Int_t p = 0; p< (Int_t)mipPoints.size(); p++){
      Int_t s   = mipPoints.at(p).isHad; // will need to be adjusted if we don't have h & e
      Int_t be = GetBeamEnergyIndex(mipPoints.at(p).beame, tbdata);
      Int_t gs = GetGainSetIndex(mipPoints.at(p).hgSet, mipPoints.at(p).lgSet, tbdata);
      Int_t v  = GetOverVoltageIndex(mipPoints.at(p).vop, tbdata);
      
      if (be == -1 || gs == -1){
        std::cout << "beam E: "<< mipPoints.at(p).beame << "\t or HG set: " << mipPoints.at(p).hgSet << "\t or LG set: " << mipPoints.at(p).hgSet << std::endl;
        continue;
      }
      if (v == -1){
        std::cout << "Vov: "<< mipPoints.at(p).vop << std::endl;
        continue;
      }
      graphVoVvsMipMax[s][be][gs]->SetPoint(graphVoVvsMipMax[s][be][gs]->GetN(),mipPoints.at(p).vop, mipPoints.at(p).max );
      graphVoVvsMipMax[s][be][gs]->SetPointError(graphVoVvsMipMax[s][be][gs]->GetN()-1,0, mipPoints.at(p).emax );
      graphRunNrvsMipMax[s][be][gs][v]->SetPoint(graphRunNrvsMipMax[s][be][gs][v]->GetN(),mipPoints.at(p).runnr, mipPoints.at(p).max );
      graphRunNrvsMipMax[s][be][gs][v]->SetPointError(graphRunNrvsMipMax[s][be][gs][v]->GetN()-1,0, mipPoints.at(p).emax );
    }

    for (Int_t s = 0; s < gMaxSpecies; s++){
      for (Int_t gs = 0; gs < gMaxGainS; gs++){
        for (Int_t be = 0; be < gMaxBEn; be++){
          if (graphVoVvsMipMax[s][be][gs]->GetN() != 0){
            graphVoVvsMipMax[s][be][gs]->Sort();
          }
          for (Int_t v = 0; v < gMaxVoltages; v++){
            if (graphRunNrvsMipMax[s][be][gs][v]->GetN() != 0){
              graphRunNrvsMipMax[s][be][gs][v]->Sort();
            }
          }
        }
      }
    }

    
    
                                            // beam type, beam energy, gain setting,  layer,   channel
    TGraphErrors* graphPerChannelVoVvsMipMax[gMaxSpecies] [gMaxBEn]  [gMaxGainS]   [gMaxLayers][9];
                                            // beam type, voltage, gain setting,  layer,   channel
    TGraphErrors* graphPerChannelBeamEvsMipMax[gMaxSpecies] [gMaxVoltages]  [gMaxGainS]   [gMaxLayers][9];
    bool layerExists[gMaxLayers];
    for (Int_t l = 0; l < gMaxLayers; l++){
      layerExists[l] = false;
      for (Int_t cb = 0; cb < 9; cb++){
        for (Int_t s = 0; s < gMaxSpecies; s++){
          for (Int_t gs = 0; gs < gMaxGainS; gs++){
            for (Int_t be = 0; be < gMaxBEn; be++){
              graphPerChannelVoVvsMipMax[s][be][gs][l][cb] = new TGraphErrors();
            }
            for (Int_t v = 0; v < gMaxVoltages; v++){
              graphPerChannelBeamEvsMipMax[s][v][gs][l][cb] = new TGraphErrors();
            }
          }
        }
      }
    }
    
    // filter for different conditions
    for (Int_t p = 0; p< (Int_t)mipPoints.size(); p++){
      Int_t l = mipPoints.at(p).layer;
      Int_t c = mipPoints.at(p).channel;
      Int_t s = mipPoints.at(p).isHad; // will need to be adjusted if we don't have h & e
      Int_t be = GetBeamEnergyIndex(mipPoints.at(p).beame, tbdata);
      Int_t gs = GetGainSetIndex(mipPoints.at(p).hgSet, mipPoints.at(p).lgSet, tbdata);
      Int_t v  = GetOverVoltageIndex(mipPoints.at(p).vop, tbdata);
      if (be == -1 || gs == -1){
        std::cout << "beam E: "<< mipPoints.at(p).beame << "\t or HG set: " << mipPoints.at(p).hgSet << "\t or LG set: " << mipPoints.at(p).hgSet << std::endl;
        continue;
      }
      if (v == -1){
        std::cout << "Vov: "<< mipPoints.at(p).vop << std::endl;
        continue;
      }
      if (!layerExists[l]) layerExists[l] = true;        
      // add point to channel graph vs voltage
      graphPerChannelVoVvsMipMax[s][be][gs][l][c]->SetPoint(graphPerChannelVoVvsMipMax[s][be][gs][l][c]->GetN(),mipPoints.at(p).vop, mipPoints.at(p).max );
      graphPerChannelVoVvsMipMax[s][be][gs][l][c]->SetPointError(graphPerChannelVoVvsMipMax[s][be][gs][l][c]->GetN()-1,0, mipPoints.at(p).emax );
      // add point to channel graph vs beamE
      graphPerChannelBeamEvsMipMax[s][v][gs][l][c]->SetPoint(graphPerChannelBeamEvsMipMax[s][v][gs][l][c]->GetN(),mipPoints.at(p).beame, mipPoints.at(p).max );
      graphPerChannelBeamEvsMipMax[s][v][gs][l][c]->SetPointError(graphPerChannelBeamEvsMipMax[s][v][gs][l][c]->GetN()-1,0, mipPoints.at(p).emax );
    }
        
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t cb = 0; cb < 9; cb++){
        for (Int_t s = 0; s < gMaxSpecies; s++){
          for (Int_t gs = 0; gs < gMaxGainS; gs++){        
            for (Int_t be = 0; be < gMaxBEn; be++){
              if (graphPerChannelVoVvsMipMax[s][be][gs][l][cb]->GetN() != 0){
                graphPerChannelVoVvsMipMax[s][be][gs][l][cb]->Sort();
              }
            }
            for (Int_t v = 0; v < gMaxVoltages; v++){
              if (graphPerChannelBeamEvsMipMax[s][v][gs][l][cb]->GetN() != 0){
                graphPerChannelBeamEvsMipMax[s][v][gs][l][cb]->Sort();
              }
            }
          }
        }
      }
    }
    
    
    TCanvas* canvas1DSimple = new TCanvas("canvas1DSimple","",0,0,700,500);  // gives the page size
    DefaultCancasSettings( canvas1DSimple, 0.08, 0.02, 0.02, 0.08);
    
    TH1D* histDummyVvsMPV = new TH1D("histDummyVvsMPV","", 70, 0, 7 );
    SetStyleHistoTH1ForGraphs( histDummyVvsMPV, "#it{V}_{ov} (V)", "MPV_{lan-gaus} (HG ADC)",0.85* textSizeRel, textSizeRel, 0.85* textSizeRel, textSizeRel, 0.9, 0.95);
    histDummyVvsMPV->GetYaxis()->SetRangeUser(rangeMaxMIP[0], rangeMaxMIP[1]);
    histDummyVvsMPV->Draw();
    Bool_t lenteredAll[2]  = {0, 0};
    Bool_t plottedA      = 0; 
    TLegend* legend = GetAndSetLegend2( 0.12, 0.95-2*textSizeRel, 0.25, 0.95,textSizeRel, 2, "Beam:", 42,0.4);
    for (Int_t gs = 0; gs < gMaxGainS; gs++){
      for (Int_t be = 0; be < gMaxBEn; be++){
        if (graphVoVvsMipMax[1][be][gs]->GetN() > 0){
          SetMarkerDefaultsTGraphErr( graphVoVvsMipMax[1][be][gs], 24, 0.8, kRed+1,kRed+1);
          graphVoVvsMipMax[1][be][gs]->Draw("p,e,same");
          if (lenteredAll[1] == 0){
            legend->AddEntry(graphVoVvsMipMax[1][be][gs], "h", "p");
            lenteredAll[1]  = 1;
            plottedA       = 1;
          }
        }
        if (graphVoVvsMipMax[0][be][gs]->GetN() > 0){
          SetMarkerDefaultsTGraphErr( graphVoVvsMipMax[0][be][gs], 25, 0.8, kBlue+1,kBlue+1);
          graphVoVvsMipMax[0][be][gs]->Draw("p,e,same");
          if (lenteredAll[0] == 0){
            legend->AddEntry(graphVoVvsMipMax[0][be][gs], "e", "p");
            lenteredAll[0]  = 1;
            plottedA       = 1;
          }
        }
      }
    }
    DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
    legend->Draw();
    if (plottedA) canvas1DSimple->SaveAs(Form("%s/VopVsMPVlangaus.pdf",outputDirPlots.Data()));
    
    TH1D* histDummyRunNrvsMPV = new TH1D("histDummyRunNrvsMPV","", 201, -0.5,200.5  );
    SetStyleHistoTH1ForGraphs( histDummyRunNrvsMPV, "Run Nr.", "MPV_{lan-gaus} (HG ADC)",0.85* textSizeRel, textSizeRel, 0.85* textSizeRel, textSizeRel, 0.9, 0.95);
    histDummyRunNrvsMPV->GetYaxis()->SetRangeUser(rangeMaxMIP[0], rangeMaxMIP[1]);
    histDummyRunNrvsMPV->Draw();
    for (Int_t gs = 0; gs < gMaxGainS; gs++){
      for (Int_t be = 0; be < gMaxBEn; be++){
        for (Int_t v = 0; v < gMaxVoltages; v++){
          if (graphRunNrvsMipMax[1][be][gs][v]->GetN() > 0){
            SetMarkerDefaultsTGraphErr( graphRunNrvsMipMax[1][be][gs][v], 24, 0.8, kRed+1,kRed+1);
            graphRunNrvsMipMax[1][be][gs][v]->Draw("p,e,same");
          }
          if (graphRunNrvsMipMax[0][be][gs][v]->GetN() > 0){
            SetMarkerDefaultsTGraphErr( graphRunNrvsMipMax[0][be][gs][v], 25, 0.8, kBlue+1,kBlue+1);
            graphRunNrvsMipMax[0][be][gs][v]->Draw("p,e,same");
          }
        }
      }
    }
    DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
    legend->Draw();
    if (plottedA) canvas1DSimple->SaveAs(Form("%s/RunNrVsMPVlangaus.pdf",outputDirPlots.Data()));

    
    
    histDummyVvsMPV->Draw();
    Bool_t lenteredBE[gMaxBEn][gMaxSpecies];
    for (Int_t be = 0; be < gMaxBEn; be++ ){
      for (Int_t s = 0; s < gMaxSpecies; s++ ){
        lenteredBE[be][s] = 0;
      }
    }
    
    plottedA      = 0; 
    TLegend* legendE = GetAndSetLegend2( 0.12, 0.945-5*textSizeRel, 0.7, 0.945,textSizeRel, 4, "Beam species, E (GeV):", 42,0.15);
    for (Int_t gs = 0; gs < gMaxGainS; gs++){
      for (Int_t be = 0; be < gMaxBEn; be++){
        Float_t beamEnergy = GetBeamEnergyFromIndex(be, tbdata);
        for (Int_t s = 0; s < gMaxSpecies; s++){
            if (graphVoVvsMipMax[s][be][gs]->GetN() > 0){
              SetMarkerDefaultsTGraphErr( graphVoVvsMipMax[s][be][gs], (s==1) ? 24 : 25, 0.8, colorBeamE[be],colorBeamE[be]);
              graphVoVvsMipMax[s][be][gs]->Draw("p,e,same");
              if (lenteredBE[be][s] == 0){
                legendE->AddEntry(graphVoVvsMipMax[s][be][gs], Form("%s, %d",(s==1) ? "h" : "e", (Int_t)beamEnergy), "p");
                lenteredBE[be][s]  = 1;
                plottedA          = 1;
              }
            }
        }
      }
    }
    DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
    legendE->Draw();
    if (plottedA) canvas1DSimple->SaveAs(Form("%s/VopVsMPVlangaus_withBeamEnergy.pdf",outputDirPlots.Data()));
    
    histDummyRunNrvsMPV->Draw();
    for (Int_t gs = 0; gs < gMaxGainS; gs++){
      for (Int_t be = 0; be < gMaxBEn; be++){
        Float_t beamEnergy = GetBeamEnergyFromIndex(be, tbdata);
        for (Int_t s = 0; s < gMaxSpecies; s++){
          for (Int_t v = 0; v < gMaxVoltages; v++){
            if (graphRunNrvsMipMax[s][be][gs][v]->GetN() > 0){
              SetMarkerDefaultsTGraphErr( graphRunNrvsMipMax[s][be][gs][v], (s==1) ? 24 : 25, 0.8, colorBeamE[be],colorBeamE[be]);
              graphRunNrvsMipMax[s][be][gs][v]->Draw("p,e,same");
            }
          }
        }
      }
    }
    DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
    legendE->Draw();
    if (plottedA) canvas1DSimple->SaveAs(Form("%s/RunNrVsMPVlangaus_withBeamEnergy.pdf",outputDirPlots.Data()));


    Bool_t lenteredV[gMaxBEn][gMaxSpecies];
    for (Int_t v = 0; v < gMaxBEn; v++ ){
      for (Int_t s = 0; s < gMaxSpecies; s++ ){
        lenteredV[v][s] = 0;
      }
    }

    histDummyRunNrvsMPV->Draw();
    plottedA      = 0; 
    TLegend* legendV = GetAndSetLegend2( 0.12, 0.945-4.5*textSizeRel, 0.7, 0.945,textSizeRel, 4, "Beam species, V_{ov} (V):", 42,0.15);
    for (Int_t v = 0; v < gMaxVoltages; v++){
      for (Int_t gs = 0; gs < gMaxGainS; gs++){
        Float_t voltage = GetOverVoltageFromIndex(v, tbdata);
        for (Int_t s = 0; s < gMaxSpecies; s++){
          for (Int_t be = 0; be < gMaxBEn; be++){
            if (graphRunNrvsMipMax[s][be][gs][v]->GetN() > 0){
              SetMarkerDefaultsTGraphErr( graphRunNrvsMipMax[s][be][gs][v], (s==1) ? 24 : 25, 0.8, colorVoltage[v],colorVoltage[v]);
              graphRunNrvsMipMax[s][be][gs][v]->Draw("p,e,same");
              if (lenteredV[v][s] == 0){
                legendV->AddEntry(graphRunNrvsMipMax[s][be][gs][v], Form("%s, %1.1f",(s==1) ? "h" : "e", voltage), "p");
                lenteredV[v][s]  = 1;
                plottedA          = 1;
              }
            }
          }
        }
      }
    }
    DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
    legendV->Draw();
    if (plottedA) canvas1DSimple->SaveAs(Form("%s/RunNrVsMPVlangaus_withVoltage.pdf",outputDirPlots.Data()));
    

    
    TH1D* histDummyBeamEvsMPV = new TH1D("histDummyBeamEvsMPV","", 361, -0.5, 360.5 );
    SetStyleHistoTH1ForGraphs( histDummyBeamEvsMPV, "#it{E}_{beam} (GeV)", "MPV_{lan-gaus} (HG ADC)",0.85* textSizeRel, textSizeRel, 0.85* textSizeRel, textSizeRel, 0.9, 0.95);
    histDummyBeamEvsMPV->GetYaxis()->SetRangeUser(rangeMaxMIP[0], rangeMaxMIP[1]);
    
    // ****************************************************************************************
    // Create electron beam plots
    // ****************************************************************************************    
    for (Int_t l = 0; l < gMaxLayers; l++ ){
      if (layerExists[l]){
        for (Int_t gs = 0; gs < gMaxGainS; gs++){
            for (Int_t be = 0; be < gMaxBEn; be++){
            canvas1DSimple->cd();
            canvas1DSimple->Clear();
            histDummyVvsMPV->Draw();
            Bool_t lentered[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            Bool_t plotted      = 0; 
            TLegend* legend = GetAndSetLegend2( 0.12, 0.95-3*textSizeRel, 0.4, 0.95,textSizeRel, 4, Form("Layer %d, channel:",l), 42,0.2);
          
            for (Int_t cb = 1; cb < 9; cb++){
              if (graphPerChannelVoVvsMipMax[1][be][gs][l][cb]->GetN() > 0){
                SetMarkerDefaultsTGraphErr( graphPerChannelVoVvsMipMax[1][be][gs][l][cb], markerReadBoard[cb-1], 0.8, colorReadBoard[cb-1],colorReadBoard[cb-1]);
                graphPerChannelVoVvsMipMax[1][be][gs][l][cb]->Draw("p,e,same");
                if (lentered[cb] == 0){
                  legend->AddEntry(graphPerChannelVoVvsMipMax[1][be][gs][l][cb], Form("%d",cb), "p");
                  lentered[cb]  = 1;
                  plotted       = 1;
                }
              }
            }
            
            Int_t lowGainSet  = GetLGGainSetIndex(gs, tbdata);
            Int_t highGainSet = GetHGGainSetIndex(gs, tbdata);
            Float_t beamEnergy = GetBeamEnergyFromIndex(be, tbdata);
            DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
            DrawLatex(0.95, 0.92-textSizeRel, Form("h-beam E =%.0f GeV", beamEnergy), true, textSizeRel, 42);        
            DrawLatex(0.95, 0.92-2*textSizeRel, Form("HG amp: %d, LG amp: %d", highGainSet, lowGainSet), true, textSizeRel, 42);
            legend->Draw();
            
            if (plotted) canvas1DSimple->SaveAs(Form("%s/VopVsMPVlangaus_Ebeam%03.0fGeV_Layer%02d_HG%02d_LG%02d.pdf",outputDirPlotsH.Data(), beamEnergy,  l, highGainSet, lowGainSet));
          }
        }
      }
    } 
    for (Int_t l = 0; l < gMaxLayers; l++ ){
      if (layerExists[l]){
        for (Int_t gs = 0; gs < gMaxGainS; gs++){
          for (Int_t v = 0; v < gMaxVoltages; v++){
            canvas1DSimple->cd();
            canvas1DSimple->Clear();
            histDummyBeamEvsMPV->Draw();
            Bool_t lentered[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            Bool_t plotted      = 0; 
            TLegend* legend = GetAndSetLegend2( 0.12, 0.95-3*textSizeRel, 0.4, 0.95,textSizeRel, 4, Form("Layer %d, channel:",l), 42,0.2);
          
            for (Int_t cb = 1; cb < 9; cb++){
              if (graphPerChannelBeamEvsMipMax[1][v][gs][l][cb]->GetN() > 0){
                SetMarkerDefaultsTGraphErr( graphPerChannelBeamEvsMipMax[1][v][gs][l][cb], markerReadBoard[cb-1], 0.8, colorReadBoard[cb-1],colorReadBoard[cb-1]);
                graphPerChannelBeamEvsMipMax[1][v][gs][l][cb]->Draw("p,e,same");
                if (lentered[cb] == 0){
                  legend->AddEntry(graphPerChannelBeamEvsMipMax[1][v][gs][l][cb], Form("%d",cb), "p");
                  lentered[cb]  = 1;
                  plotted       = 1;
                }
              }
            }
            
            Int_t lowGainSet  = GetLGGainSetIndex(gs, tbdata);
            Int_t highGainSet = GetHGGainSetIndex(gs, tbdata);
            Float_t overV     = GetOverVoltageFromIndex(v, tbdata);
            if (lowGainSet == -1 || highGainSet == -1 || overV == -1) continue;
            DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
            DrawLatex(0.95, 0.92-textSizeRel, Form("V_{ov} =%.1f V", overV), true, textSizeRel, 42);        
            DrawLatex(0.95, 0.92-2*textSizeRel, Form("HG amp: %d, LG amp: %d", highGainSet, lowGainSet), true, textSizeRel, 42);
            legend->Draw();
            
            if (plotted) canvas1DSimple->SaveAs(Form("%s/BeamEvsMPVlangaus_V%03.0fmV_Layer%02d_HG%02d_LG%02d.pdf",outputDirPlotsH.Data(), overV*1000,  l, highGainSet, lowGainSet));
          }
        }
      }
    }

    
    
    // ****************************************************************************************
    // Create electron beam plots
    // ****************************************************************************************
    for (Int_t l = 0; l < gMaxLayers; l++ ){
      if (layerExists[l]){
        for (Int_t gs = 0; gs < gMaxGainS; gs++){
            for (Int_t be = 0; be < gMaxBEn; be++){
            canvas1DSimple->cd();
            canvas1DSimple->Clear();
            histDummyVvsMPV->Draw();
            Bool_t lentered[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            Bool_t plotted      = 0; 
            TLegend* legend = GetAndSetLegend2( 0.12, 0.95-3*textSizeRel, 0.4, 0.95,textSizeRel, 4, Form("Layer %d, channel:",l), 42,0.2);
          
            for (Int_t cb = 1; cb < 9; cb++){
              if (graphPerChannelVoVvsMipMax[0][be][gs][l][cb]->GetN() > 0){
                SetMarkerDefaultsTGraphErr( graphPerChannelVoVvsMipMax[0][be][gs][l][cb], markerReadBoard[cb-1], 0.8, colorReadBoard[cb-1],colorReadBoard[cb-1]);
                graphPerChannelVoVvsMipMax[0][be][gs][l][cb]->Draw("p,e,same");
                if (lentered[cb] == 0){
                  legend->AddEntry(graphPerChannelVoVvsMipMax[0][be][gs][l][cb], Form("%d",cb), "p");
                  lentered[cb]  = 1;
                  plotted       = 1;
                }
              }
            }
            
            Int_t lowGainSet  = GetLGGainSetIndex(gs, tbdata);
            Int_t highGainSet = GetHGGainSetIndex(gs, tbdata);
            Float_t beamEnergy = GetBeamEnergyFromIndex(be, tbdata);
            DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
            DrawLatex(0.95, 0.92-textSizeRel, Form("e-beam E =%.0f GeV", beamEnergy), true, textSizeRel, 42);        
            DrawLatex(0.95, 0.92-2*textSizeRel, Form("HG amp: %d, LG amp: %d", highGainSet, lowGainSet), true, textSizeRel, 42);
            legend->Draw();
            
            if (plotted) canvas1DSimple->SaveAs(Form("%s/VopVsMPVlangaus_Ebeam%03.0fGeV_Layer%02d_HG%02d_LG%02d.pdf",outputDirPlotsE.Data(), beamEnergy,  l, highGainSet, lowGainSet));
          }
        }
      }
    }
    for (Int_t l = 0; l < gMaxLayers; l++ ){
      if (layerExists[l]){
        for (Int_t gs = 0; gs < gMaxGainS; gs++){
          for (Int_t v = 0; v < gMaxVoltages; v++){
            canvas1DSimple->cd();
            canvas1DSimple->Clear();
            histDummyBeamEvsMPV->Draw();
            Bool_t lentered[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            Bool_t plotted      = 0; 
            TLegend* legend = GetAndSetLegend2( 0.12, 0.95-3*textSizeRel, 0.4, 0.95,textSizeRel, 4, Form("Layer %d, channel:",l), 42,0.2);
          
            for (Int_t cb = 1; cb < 9; cb++){
              if (graphPerChannelBeamEvsMipMax[0][v][gs][l][cb]->GetN() > 0){
                SetMarkerDefaultsTGraphErr( graphPerChannelBeamEvsMipMax[0][v][gs][l][cb], markerReadBoard[cb-1], 0.8, colorReadBoard[cb-1],colorReadBoard[cb-1]);
                graphPerChannelBeamEvsMipMax[0][v][gs][l][cb]->Draw("p,e,same");
                if (lentered[cb] == 0){
                  legend->AddEntry(graphPerChannelBeamEvsMipMax[0][v][gs][l][cb], Form("%d",cb), "p");
                  lentered[cb]  = 1;
                  plotted       = 1;
                }
              }
            }
            
            Int_t lowGainSet  = GetLGGainSetIndex(gs, tbdata);
            Int_t highGainSet = GetHGGainSetIndex(gs, tbdata);
            Float_t overV     = GetOverVoltageFromIndex(v, tbdata);
            if (lowGainSet == -1 || highGainSet == -1 || overV == -1) continue;
            DrawLatex(0.95, 0.92, Form("%s", labelTB.Data()), true, textSizeRel, 42);        
            DrawLatex(0.95, 0.92-textSizeRel, Form("V_{ov} =%.1f V", overV), true, textSizeRel, 42);        
            DrawLatex(0.95, 0.92-2*textSizeRel, Form("HG amp: %d, LG amp: %d", highGainSet, lowGainSet), true, textSizeRel, 42);
            legend->Draw();
            
            if (plotted) canvas1DSimple->SaveAs(Form("%s/BeamEvsMPVlangaus_V%03.0fmV_Layer%02d_HG%02d_LG%02d.pdf",outputDirPlotsE.Data(), overV*1000,  l, highGainSet, lowGainSet));
          }
        }
      }
    }

    
}
