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

#include "makeSimplePlotsFromJanusTree.h"



//__________________________________________________________________________________________________________
//_____________________MAIN function !!! ___________________________________________________________________
//__________________________________________________________________________________________________________
void makeSimplePlotsFromJanusTree( TString fileName     = "", 
                                   TString outputDir    = "ProcessedData/",
                                   Int_t runnumber      = -1,
                                   ULong_t minNEvent    = 0,
                                   ULong_t maxNEvent    = 0,
                                   Int_t verbosity      = 0,
                                   Int_t isTBdata       = 0,
                                   TString mappingFile  = "",
                                   Bool_t bDetPlot      = kTRUE,
                                   TString runListFileName = "configs/SPS_RunNumbers.txt"
                                  ){
                               
    StyleSettingsThesis("pdf");
    SetPlotStyle();
    Double_t textSizeRel = 0.04;
    
      // make output directory
    TString dateForOutput = ReturnDateStr();
    if (runnumber > -1)
      outputDir = Form("%s/Run%05d",outputDir.Data(), runnumber );
    TString outputDirPlots = Form("%s/Plots",outputDir.Data());
    TString outputDirPlotsDet = Form("%s/Plots/Detailed",outputDir.Data());
    gSystem->Exec("mkdir -p "+outputDir);
    gSystem->Exec("mkdir -p "+outputDirPlots);
    gSystem->Exec("mkdir -p "+outputDirPlotsDet);

    // load tree
    TChain *const tt_event = new TChain("tree");
    if (fileName.EndsWith(".root")) {                     // are we loading a single root tree?
        std::cout << "loading a single root file" << std::endl;
        tt_event->AddFile(fileName);
    } else {
        std::cout << "please try again this isn't a root file" << std::endl;
    } 
    if(!tt_event){ std::cout << "tree not found... returning!"<< std::endl; return;}

    // load all branches (see header)
    SetBranchAddressesTree(tt_event);

    
    Long64_t nEntriesTree                 = tt_event->GetEntries();
    std::cout << "Number of events in tree: " << nEntriesTree << std::endl;
    if(maxNEvent>0 && maxNEvent<(ULong_t)nEntriesTree){
      nEntriesTree = maxNEvent;
      std::cout << "Will only analyze first " << maxNEvent << " events in the tree..." << std::endl;
    }

    // ********************************************************************************************************
    // read run list and corresponding settings
    // ********************************************************************************************************
    std::vector<runInfo> runs = readRunInfosFromFile(runListFileName, 0 );
    Int_t indexCRun = findCurrentRun(runs, runnumber);
    runInfo currentRunInfo;
    if (indexCRun < 0){
      std::cout << "run not in current list of runs, provided" << std::endl;
      return;
    } else {
      std::cout << "Run "<< runnumber << "\t found at index " << indexCRun << std::endl;
      currentRunInfo = GetRunInfoObject(runs,indexCRun);
      std::cout << "Run " << currentRunInfo.runNr << "\t species: " << currentRunInfo.species << "\t energy: "  << currentRunInfo.energy << "\t Vop: " << currentRunInfo.vop << std::endl;
    }
    
    // ********************************************************************************************************
    // Read mapping
    //  * association of CAEN board channels with actuall layers in the detectors
    //  * CAEN board Nr. 0-2
    //  * CAEN board channels 0-64
    //  * Layers during TB (0-14)
    //  * Readout board channel (1-8)
    //  * Readout board column (0-3)
    //  * Readout board row (0-1)
    //  * Assembly Nr TB (0-19)
    // ********************************************************************************************************
    Int_t mapping[128][4]                     = {{0}};      // linear mapping 
    Int_t backwardMapping[9][gMaxLayers]      = {{-1}};     // backward associations channels
    Int_t backwardMappingBoard[9][gMaxLayers] = {{-1}};     // backward associations board
    Bool_t lActive[gMaxLayers]                = {0};        // boolean to check whether layer is active
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t c = 0; c < 9 ; c++){
        backwardMapping[c][l]       = -1;
        backwardMappingBoard[c][l]  = -1;
      }
      lActive[l] = kFALSE;
    }
    if (isTBdata > 0){
      cout << "INFO: You have chosen the given the following config file: " << mappingFile.Data() << endl;
      ifstream fileMapping;
      fileMapping.open(mappingFile,ios_base::in);
      if (!fileMapping) {
          cout << "ERROR: settings " << mappingFile.Data() << " not found!" << endl;
          return;
      }

      // read settings from file
      for( TString tempLine; tempLine.ReadLine(fileMapping, kTRUE); ) {
          // check if line should be considered
          if (tempLine.BeginsWith("%") || tempLine.BeginsWith("#")){
              continue;
          }
          cout << tempLine.Data() << endl;

          // Separate the string according to tabulators
          TObjArray *tempArr  = tempLine.Tokenize("\t");
          if(tempArr->GetEntries()<1){
              cout << "nothing to be done" << endl;
              delete tempArr;
              continue;
          } else if (tempArr->GetEntries() == 1 ){
              // Separate the string according to space
              tempArr       = tempLine.Tokenize(" ");
              if(tempArr->GetEntries()<1){
                  cout << "nothing to be done" << endl;
                  delete tempArr;
                  continue;
              } else if (tempArr->GetEntries() == 1 ) {
                  cout << ((TString)((TObjString*)tempArr->At(0))->GetString()).Data() << " has not be reset, no value given!" << endl;
                  delete tempArr;
                  continue;
              }
          }
          Int_t chCAEN    = ((TString)((TObjString*)tempArr->At(0))->GetString()).Atoi()*64 + ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();
          Int_t layerMod  = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atoi();
          Int_t assemblyMod  = ((TString)((TObjString*)tempArr->At(3))->GetString()).Atoi();
          Int_t chBoard   = ((TString)((TObjString*)tempArr->At(4))->GetString()).Atoi();
          Int_t rowBoard  = ((TString)((TObjString*)tempArr->At(5))->GetString()).Atoi();
          Int_t colBoard  = ((TString)((TObjString*)tempArr->At(6))->GetString()).Atoi();
          
          std::cout << "-->" << chCAEN << "\t" << layerMod << "\t"<< chBoard << "\t" << rowBoard << "\t" << colBoard << std::endl;
          mapping[chCAEN][0]    = layerMod; 
          mapping[chCAEN][1]    = chBoard; 
          mapping[chCAEN][2]    = rowBoard; 
          mapping[chCAEN][3]    = colBoard; 
          
          backwardMapping[chBoard][layerMod] = chCAEN;
          backwardMappingBoard[chBoard][layerMod] = ((TString)((TObjString*)tempArr->At(0))->GetString()).Atoi();
          std::cout << backwardMapping[chBoard][layerMod] << std::endl;
          delete tempArr;
      }
      
      for (Int_t ch = 0; ch< 64; ch++){
        std::cout  << "channel: " << ch << " location plane: " << mapping[ch][0] << "\t tile: " << mapping[ch][1] << std::endl;
      }
    }
  
    Int_t nActiveLayers   = 0;
    Int_t minActiveLayer  = -1;
    Int_t maxActiveLayer  = -1;
    if (isTBdata > 0){
      std::cout << "===================================================================" << std::endl;
      std::cout << "================ Visualization of layers ==========================" << std::endl;
      std::cout << "===================================================================" << std::endl;
      for (Int_t l = 0; l < gMaxLayers; l++){
          for (Int_t c = 1; c < 9; c++){
              std::cout << backwardMapping[c][l] << "\t || \t" ;
              if (lActive[l] == kFALSE ){
                  if ( backwardMapping[c][l] != -1 ) lActive[l] = kTRUE;
              }
          }
          std::cout << std::endl;
      }
      std::cout << "===================================================================" << std::endl;
      
      // count total number of active layers according to mapping file and determine maximum layer
      for (Int_t l = 0; l<gMaxLayers; l++){
        if (lActive[l]){
          nActiveLayers++; 
          maxActiveLayer = l;
          if (minActiveLayer == -1) minActiveLayer = l;
        }
      }
      std::cout << "There are " << nActiveLayers << " active layers. The lowest layer is " << minActiveLayer << "  and the highest layer is " << maxActiveLayer << std::endl;
    }
    
    // ********************************************************************************************************
    // RAW data monitoring histos per CAEN board and channel
    // ********************************************************************************************************    
    TH1D* histLG[gMaxBoard][gMaxChannels];                      // low gain all triggers
    TH1D* histHG[gMaxBoard][gMaxChannels];                      // high gain all triggers
    TH2D* hist_T_HG[gMaxBoard][gMaxChannels];                   // time dependent high gain all triggers
    TH2D* histLGHG[gMaxBoard][gMaxChannels];                    // LG - HG correlation
    TH2D* histHGLG[gMaxBoard][gMaxChannels];                    // HG - LG correlation
    TH1D* histLGTrig[gMaxBoard][gMaxChannels];                  // low gain straight line triggers
    TH1D* histHGTrig[gMaxBoard][gMaxChannels];                  // high gain straight line triggers
    TH1D* histLGNoise[gMaxBoard][gMaxChannels];                 // low gain !(straight line triggers)
    TH1D* histHGNoise[gMaxBoard][gMaxChannels];                 // high gain !(straight line triggers)
    TH1D* histLGTriggNoise[gMaxBoard][gMaxChannels];            // low gain noise triggers
    TH1D* histHGTriggNoise[gMaxBoard][gMaxChannels];            // high gain noise triggers
    
    //***************************************************
    // mapped channels - layer & read-out board channels
    //***************************************************
    // channel counting in layer starts with 1, easier acces make array go to 8
    TH1D* histLG_mapped[gMaxLayers][9];                         // low gain all triggers
    TH1D* histHG_mapped[gMaxLayers][9];                         // high gain all triggers
    TH1D* histLGTrig_mapped[gMaxLayers][9];                     // low gain straight line triggers
    TH1D* histHGTrig_mapped[gMaxLayers][9];                     // high gain straight line triggers
    TH1D* histLGTriggNoise_mapped[gMaxLayers][9];               // low gain noise triggers
    TH1D* histHGTriggNoise_mapped[gMaxLayers][9];               // high gain noise triggers
    
    //***************************************************
    // map of triggers above threshold
    //***************************************************
    TH3D* hist3DMap = new TH3D("h_map_z_x_y", "; layer; col; row", 14, -0.5, 13.5, 4, -0.5, 3.5, 2, -0.5, 1.5);
    TH2D* hist2DMap = new TH2D("h_map_z_channel", "; channel; layer", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH1D* hist1DMap = new TH1D("h_map_channel", "; channel", 8, 0.5, 8.5);
    
    //***************************************************
    TH1D* histNChAboveNoise   = new TH1D("h_NchannelAboveNoise", "; N_{channels}; counts", (gMaxBoard*gMaxChannels+1), -0.5, (gMaxBoard*gMaxChannels-0.5));
    TH1D* histNTrig           = new TH1D("h_NTriggStraigthLine", "; Channel_{board}; counts", 8, 0.5, 8.5);
    TH1D* histNTrigNoise      = new TH1D("h_NTriggNoise", "; Channel_{board}; counts", 8, 0.5, 8.5);
    
    //***************************************************
    // Init histograms
    //***************************************************
    for (Int_t j = 0; j < gMaxBoard; j++){
      for (Int_t i = 0; i < gMaxChannels; i++){
        if (isTBdata)
          hist_T_HG[j][i]    = new TH2D(Form("h_T_HG_B%d_C%02d",j,i),"; t (min); HG (adc); counts",1000,0,120,4201,-200,4001);
        else
          hist_T_HG[j][i]    = new TH2D(Form("h_T_HG_B%d_C%02d",j,i),"; t (min); HG (adc); counts",10000,0,1000,4201,-200,4001);
        histHG[j][i]    = new TH1D(Form("h_HG_B%d_C%02d",j,i),"; HG (adc); counts",4201,-200,4001);
        histLG[j][i]    = new TH1D(Form("h_LG_B%d_C%02d",j,i),"; LG (adc); counts",4201,-200,4001);
        histLGHG[j][i]  = new TH2D(Form("h_LGHG_B%d_C%02d",j,i),"; LG (adc); HG (adc)",4200/5,-200,4001,4200/5,-200,4001);
        histHGLG[j][i]  = new TH2D(Form("h_HGLG_B%d_C%02d",j,i),"; LG (adc); HG (adc)",4200/5,-200,4001,4200/5,-200,4001);
        if (isTBdata > 0){
          histLGTrig[j][i]        = new TH1D(Form("h_LGTrigg_B%d_C%02d",j,i),"coincidence trigg. front & back ; LG (adc); counts",4201,-200,4001);   // triggered 
          histHGTrig[j][i]        = new TH1D(Form("h_HGTrigg_B%d_C%02d",j,i),"coincidence trigg. front & back ; HG (adc); counts",4201,-200,4001);
          histLGNoise[j][i]       = new TH1D(Form("h_LGNoise_B%d_C%02d",j,i),"!(coincidence trigg. front & back) ; LG (adc); counts",4201,-200,4001);
          histHGNoise[j][i]       = new TH1D(Form("h_HGNoise_B%d_C%02d",j,i),"!(coincidence trigg. front & back) ; HG (adc); counts",4201,-200,4001);
          histLGTriggNoise[j][i]  = new TH1D(Form("h_LGTriggeredNoise_B%d_C%02d",j,i),"veto front & back ; LG (adc); counts",4201,-200,4001);
          histHGTriggNoise[j][i]  = new TH1D(Form("h_HGTriggeredNoise_B%d_C%02d",j,i),"veto front & back ; HG (adc); counts",4201,-200,4001);
        }
      }
    }
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t c = 0; c < 9; c++){
        histLG_mapped[l][c]             = nullptr;
        histHG_mapped[l][c]             = nullptr; 
        histLGTrig_mapped[l][c]         = nullptr;
        histHGTrig_mapped[l][c]         = nullptr;
        histLGTriggNoise_mapped[l][c]   = nullptr;
        histHGTriggNoise_mapped[l][c]   = nullptr;
      }
    }
    
    // ********************************************************************************************************
    // Setup of global variables for parsing tree
    // ********************************************************************************************************
    Long64_t adcThreshold = 100;                    // minimum ADC count for triggers
    Double_t scaledThr        = adcThreshold*1.5;   // scaled ADC trigger threshold
    Double_t tstapMin         = 0;                  // min time stamp after start of trigger (#mus)
    Double_t tstapMax         = 0;                  // current max time stamp after start of trigger (#mus)
    
    Long64_t nEventsProcessed = 0;              // running counter
    // use this if you wanna start at a specific event for debug
    Long64_t startEvent = 0;
    if (minNEvent > 0) startEvent = minNEvent;
    
    // ********************************************************************************************************
    // First loop over full tree to obtain RAW histograms
    // ********************************************************************************************************
    for (Long64_t i=startEvent; i<nEntriesTree;i++) {
        // load current event
        tt_event->GetEntry(i);
        if (i == startEvent) tstapMin = gTRtimeStamp;
        tstapMax = gTRtimeStamp;
        Double_t tCurr= (tstapMax-tstapMin)/1e6/60; // elapsed time in min
        nEventsProcessed++;

        Int_t nChNoNoise = 0;             // number of channels above noise level
        // processing progress info
        if(i>0 && nEntriesTree>100 && i%(nEntriesTree/(50))==0) std::cout << "//processed " << 100*(i)/nEntriesTree << "%"  << std::endl;
        if(verbosity>0){
          std::cout << "***********************************************************************************************************" << std::endl;
          std::cout << "event " << i << std::endl;
          std::cout << "***********************************************************************************************************" << std::endl;
        }
        // temporary mapping of channels
        Int_t signal[2][64][3] = {{{0}}}; // [board][channel][HG, LG, trigg HG]
        // readEvent
        if (verbosity > 0)std::cout << "------------- Event -------------------------" << std::endl;
        if (verbosity > 0)std::cout << gTrID << "\t" << gTRtimeStamp << std::endl;
        for(Int_t ch=0; ch<gMaxChannels; ch++){
          signal[gBoard[ch]][gChannel[ch]][0] = gHG[ch];
          signal[gBoard[ch]][gChannel[ch]][1] = gLG[ch];
          if (gHG[ch] > scaledThr)  // fill potential trigger signals
            signal[gBoard[ch]][gChannel[ch]][2] = gHG[ch]-scaledThr;
          
          // filling hists
          histHG[gBoard[ch]][gChannel[ch]]->Fill(gHG[ch]); 
          hist_T_HG[gBoard[ch]][gChannel[ch]]->Fill(tCurr,gHG[ch]); 
          histLG[gBoard[ch]][gChannel[ch]]->Fill(gLG[ch]); 
          histLGHG[gBoard[ch]][gChannel[ch]]->Fill(gLG[ch], gHG[ch]); 
          histHGLG[gBoard[ch]][gChannel[ch]]->Fill(gHG[ch], gLG[ch]); 
          if (verbosity > 0)std::cout << "--> board: "<< gBoard[ch] << "\t ch:"<< gChannel[ch] << "\t LG:" << gLG[ch] << "\t HG:" << gHG[ch] << std::endl;
          if (gHG[ch] > adcThreshold){
            nChNoNoise++;
            if (verbosity > 0) std::cout << "not noise" << std::endl;
            if (isTBdata > 0){
              hist3DMap->Fill(mapping[gChannel[ch]][0],mapping[gChannel[ch]][3],mapping[gChannel[ch]][2]);
              hist2DMap->Fill(mapping[gChannel[ch]][1],mapping[gChannel[ch]][0]);
              hist1DMap->Fill(mapping[gChannel[ch]][1]);
            }
          }
        }
        // fill channels above noise
        histNChAboveNoise->Fill(nChNoNoise);
        
        // *****************************************************************
        // check for conincidence between first and last channel 
        // above noise or pure noise triggers
        // *****************************************************************
        if (isTBdata == 1){
        // only for first CAEN board!!!!!
          for (Int_t chCAEN = 0; chCAEN < 64; chCAEN++){            
            Int_t chBoard = mapping[chCAEN][1];
            Int_t nNeigh  = 0;
            Int_t* neighborCh = new Int_t[5];
            GetNeighborWithinBoard(chBoard, nNeigh, neighborCh);
            if (verbosity > 2)std::cout << "channel in board "<< chBoard <<" found " << nNeigh << " neighbors " << std::endl;
          
            Int_t chCAENFront = backwardMapping[chBoard][minActiveLayer];
            Int_t chCAENBack  = backwardMapping[chBoard][maxActiveLayer];
            
            // Determine straight line trigger
            Bool_t  trigg         = (signal[0][chCAENFront][2] > 0 && signal[0][chCAENBack][2] > 0);
            Float_t triggSignal   = (signal[0][chCAENFront][2] + signal[0][chCAENBack][2])/2;
            
            // Determine if its triggered noise
            Bool_t triggNoise     = kFALSE;
            Float_t triggSigFN     = signal[0][chCAENFront][2];
            Float_t triggSigBN     = signal[0][chCAENBack][2];
            for (Int_t n = 0; n < nNeigh; n++){
                triggSigFN = triggSigFN + signal[0][backwardMapping[neighborCh[n]][minActiveLayer]][2];
                triggSigBN = triggSigBN + signal[0][backwardMapping[neighborCh[n]][maxActiveLayer]][2];
            }
            if (triggSigFN == 0 && triggSigBN == 0)
              triggNoise  = kTRUE;
            
            // Fill triggered histograms
            if (verbosity > 0) std::cout << "channel board " << chCAEN << "\t" << chBoard << "\t" << chCAENFront << "\t" << chCAENBack << ""<< std::endl; 
            if (trigg){
              if(mapping[chCAEN][0] == 0) histNTrig->Fill(chBoard);
              histHGTrig[0][chCAEN]->Fill(signal[0][chCAEN][0]); 
              histLGTrig[0][chCAEN]->Fill(signal[0][chCAEN][1]); 
            } else {
              histHGNoise[0][chCAEN]->Fill(signal[0][chCAEN][0]); 
              histLGNoise[0][chCAEN]->Fill(signal[0][chCAEN][1]);               
            }
            if (triggNoise){
              if(mapping[chCAEN][0] == 0) histNTrigNoise->Fill(chBoard);
              histHGTriggNoise[0][chCAEN]->Fill(signal[0][chCAEN][0]); 
              histLGTriggNoise[0][chCAEN]->Fill(signal[0][chCAEN][1]);               
            }
          }
        }
        if (verbosity > 0) std::cout << "Channels above noise: "<< nChNoNoise << std::endl;        
    }
    
    //**********************************************************************
    // Monitoring infos
    //**********************************************************************
    std::cout << "Total events processed:" << nEventsProcessed << std::endl;
    Double_t tdiff = (tstapMax-tstapMin)/1e6/60; //time in min
    std::cout << "times: " << tstapMin << "\t" << tstapMax <<"\t elapsed: " <<  tdiff  << " min"<< std::endl;
    
    //**********************************************************************
    // Create canvases for single channel plotting
    //**********************************************************************
    TCanvas* canvas2DCorr = new TCanvas("canvasCorrPlots","",0,0,1350,1200);  // gives the page size
    DefaultCancasSettings( canvas2DCorr, 0.1, 0.1, 0.02, 0.08);
    canvas2DCorr->SetLogz();
    TCanvas* canvas1DNoise = new TCanvas("canvas1DNoise","",0,0,700,500);  // gives the page size
    DefaultCancasSettings( canvas1DNoise, 0.07, 0.02, 0.02, 0.08);
    canvas1DNoise->SetLogy();
    TCanvas* canvas1DDiffTrigg = new TCanvas("canvas1DDiffTrigg","",0,0,700,500);  // gives the page size
    DefaultCancasSettings( canvas1DDiffTrigg, 0.07, 0.02, 0.02, 0.08);
    canvas1DDiffTrigg->SetLogy();
    
    //***********************************************************************************************************
    //********************************* 8 Panel overview plot  **************************************************
    //***********************************************************************************************************
    //*****************************************************************
    // Test beam geometry (beam coming from viewer)
    //==============================================
    //||    8    ||    7    ||    6    ||    5    ||
    //==============================================
    //||    1    ||    2    ||    3    ||    4    ||
    //==============================================
    // rebuild pad geom in similar way (numbering -1)
    //*****************************************************************
    TCanvas* canvas8Panel;
    TPad* pad8Panel[8];
    Double_t topRCornerX[8];
    Double_t topRCornerY[8];
    Int_t textSizePixel = 30;
    Double_t relSize8P[8];
    CreateCanvasAndPadsFor8PannelTBPlot(canvas8Panel, pad8Panel,  topRCornerX, topRCornerY, relSize8P, textSizePixel);
    
    //**********************************************************************
    // Fits for noise and LG/HG calibration
    //**********************************************************************
    TF1* fitGausHG_BG[gMaxBoard][gMaxChannels];
    TF1* fitGausLG_BG[gMaxBoard][gMaxChannels];
    TF1* fitGausHG_BG_mapped[gMaxLayers][9];
    TF1* fitGausLG_BG_mapped[gMaxLayers][9];
    TF1* fitLGHGCorr[gMaxBoard][gMaxChannels];
    TF1* fitHGLGCorr[gMaxBoard][gMaxChannels];
    Double_t mean[4][gMaxBoard][gMaxChannels];
    Double_t sigma[4][gMaxBoard][gMaxChannels];
    Double_t cslope[4][gMaxBoard][gMaxChannels];
    Double_t coffset[4][gMaxBoard][gMaxChannels];

    //**********************************************************************
    // Monitoring hists for fits to noise and slope LG-HG
    //**********************************************************************
    TH1D* histNoiseSigma_HG     = new TH1D("histNoiseSigma_HG", "; noise #sigma (HG ADC)", 400, 0, 20);
    TH1D* histNoiseMean_HG      = new TH1D("histNoiseMean_HG", "; noise #mu (HG ADC)", 400, 30, 70);
    TH1D* histNoiseSigma_LG     = new TH1D("histNoiseSigma_LG", "; noise #sigma (LG ADC)", 400, 0, 20);
    TH1D* histNoiseMean_LG      = new TH1D("histNoiseMean_LG", "; noise #mu (LG ADC)", 400, 30, 70);
    TH1D* histLGHG_slope        = new TH1D("histLGHGslope", "; slope (HG adc/LG adc)", 400, 0, 40);
    
    TH2D* hist2DNoiseSigma_HG   = new TH2D("hist2DNoiseSigma_HG_z_channel", "; channel; layer; noise #sigma (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DNoiseMean_HG    = new TH2D("hist2DNoiseMean_HG_z_channel", "; channel; layer; noise #mu (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DNoiseSigma_LG   = new TH2D("hist2DNoiseSigma_LG_z_channel", "; channel; layer; noise #sigma (LG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DNoiseMean_LG    = new TH2D("hist2DNoiseMean_LG_z_channel", "; channel; layer; noise #mu (LG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DLGHG_slope      = new TH2D("hist2D_LGHGslope_z_channel", "; channel; layer; slope (HG adc/LG adc)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DLGHG_offset     = new TH2D("hist2D_LGHGoffset_z_channel", "; channel; layer; offset (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    
    // 1D channel representation of fit values, x axis scales as 10x layer count + channel within one assembley, 
    // - layer 3 channel 3: 33
    // - layer 0 channel 2: 2
    TH1D* hist1DNoiseSigma_HG   = new TH1D("hist1DNoiseSigma_HG_channels", "; 10x layer + board channel; noise #sigma (HG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DNoiseMean_HG    = new TH1D("hist1DNoiseMean_HG_channels", "; 10x layer + board channel; noise #mu (HG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DNoiseSigma_LG   = new TH1D("hist1DNoiseSigma_LG_channels", "; 10x layer + board channel; noise #sigma (LG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DNoiseMean_LG    = new TH1D("hist1DNoiseMean_LG_channels", "; 10x layer + board channel; noise #mu (LG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DLGHG_slope      = new TH1D("hist1DLGHG_slope_channels", "; 10x layer + board channel; slope (HG adc/LG adc)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DLGHG_offset     = new TH1D("hist1DLGHG_slope_channels", "; 10x layer + board channel; offset (HG adc/LG adc)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DHGLG_slope      = new TH1D("hist1DHGLG_slope_channels", "; 10x layer + board channel; slope (LG adc/HG adc)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DHGLG_offset     = new TH1D("hist1DHGLG_slope_channels", "; 10x layer + board channel; offset (LG adc/HG adc)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    
    // 1D channel representation of fit values, x axis scales as CAEN board channelns 64x CAEN board # + CAEN channel
    TH1D* hist1DCAEN_NoiseSigma_HG   = new TH1D("hist1DCAEN_NoiseSigma_HG_channels", "; 64x CAEN board + CAEN channel; noise #sigma (HG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_NoiseMean_HG    = new TH1D("hist1DCAEN_NoiseMean_HG_channels", "; 64x CAEN board + CAEN channel; noise #mu (HG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_NoiseSigma_LG   = new TH1D("hist1DCAEN_NoiseSigma_LG_channels", "; 64x CAEN board + CAEN channel; noise #sigma (LG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_NoiseMean_LG    = new TH1D("hist1DCAEN_NoiseMean_LG_channels", "; 64x CAEN board + CAEN channel; noise #mu (LG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_LGHG_slope      = new TH1D("hist1DCAEN_LGHG_slope_channels", "; 64x CAEN board + CAEN channel; slope (HG adc/LG adc)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_LGHG_offset     = new TH1D("hist1DCAEN_LGHG_slope_channels", "; 64x CAEN board + CAEN channel; offset (HG adc/LG adc)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_HGLG_slope      = new TH1D("hist1DCAEN_HGLG_slope_channels", "; 10x layer + board channel; slope (LG adc/HG adc)", 64*gMaxBoard+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DCAEN_HGLG_offset     = new TH1D("hist1DCAEN_HGLG_slope_channels", "; 10x layer + board channel; offset (LG adc/HG adc)", 64*gMaxBoard+1, -0.5, 10*maxActiveLayer+0.5 );

    //**********************************************************************
    // Initialize fits for all layers and channels to nullptr
    //**********************************************************************    
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t c = 0; c< 9;c++){
        fitGausHG_BG_mapped[l][c] = nullptr;
        fitGausLG_BG_mapped[l][c] = nullptr;
      }
    }
    
    // ********************************************************************************************************
    // Fitting Noise and correlation HG-LG, LG-HG
    // -> detailed plotting per channel
    // -> remapping of CAEN board+channels to physicsal layers and readout-channels
    // ********************************************************************************************************    
    for (Int_t j = 0; j < gMaxBoard; j++){
      for (Int_t i = 0; i < gMaxChannels; i++){
        mean[0][j][i] = -1;
        mean[1][j][i] = -1;
        mean[2][j][i] = -1;
        mean[3][j][i] = -1;
        sigma[0][j][i] = -1;
        sigma[1][j][i] = -1;
        sigma[2][j][i] = -1;
        sigma[3][j][i] = -1;

        Int_t chMap     = j*64 + i;
        Int_t chBoard   = mapping[chMap][1];
        Int_t layer     = mapping[chMap][0];
        Int_t channelBin1D = hist1DNoiseSigma_HG->FindBin(layer*10+chBoard);          
          
        if (verbosity > 0)std::cout << j << "\t" << i << "\t" << chMap << "\t L: " << layer  << "\t C:" <<  chBoard << "\t bin ID"<< channelBin1D << std::endl;
        
        // ********************************************************
        // map raw and trigger histos from CAEN numbering to 
        // physical layers & channels
        // ********************************************************
        histHG_mapped[layer][chBoard] = (TH1D*)histHG[j][i]->Clone(Form("h_HG_mapped_L%d_C%02d",layer,chBoard));
        histLG_mapped[layer][chBoard] = (TH1D*)histLG[j][i]->Clone(Form("h_LG_mapped_L%d_C%02d",layer,chBoard));
        if (histHGTrig[j][i]) histHGTrig_mapped[layer][chBoard] = (TH1D*)histHGTrig[j][i]->Clone(Form("h_HGTrigg_mapped_L%d_C%02d",layer,chBoard));
        if (histLGTrig[j][i]) histLGTrig_mapped[layer][chBoard] = (TH1D*)histLGTrig[j][i]->Clone(Form("h_LGTrigg_mapped_L%d_C%02d",layer,chBoard));
        
        // ********************************************************
        // Fit & plot noise histos
        // -> map noise histos from CAEN numb. -> physical
        // -> fill monitoring plots
        // ********************************************************
        Bool_t bFit = kFALSE;
        // HG noise fits
        if (histHGTriggNoise[j][i]){
          bFit = FitNoise (histHGTriggNoise[j][i], fitGausHG_BG[j][i], mean[0][j][i], mean[1][j][i], sigma[0][j][i], sigma[1][j][i], j, i, "f_GaussBG_HG", "HG");
          if (bFit && bDetPlot) PlotNoiseSingle (canvas1DNoise, histHGTriggNoise[j][i], fitGausHG_BG[j][i], mean[0][j][i], mean[1][j][i], sigma[0][j][i], sigma[1][j][i], j, i, layer, chBoard, 
                                                  Form("%s/HG_NoiseWithFit", outputDirPlotsDet.Data()), currentRunInfo, 0.04);
          histHGTriggNoise_mapped[layer][chBoard] = (TH1D*)histHGTriggNoise[j][i]->Clone(Form("h_HGTriggeredNoise_mapped_L%d_C%02d",layer,chBoard));
          if (bFit){
            fitGausHG_BG_mapped[layer][chBoard] = (TF1*)fitGausHG_BG[j][i]->Clone(Form("f_GaussBG_HG_mapped_L%d_C%02d",layer,chBoard));
          }
        } else {
          bFit = FitNoise (histHG[j][i], fitGausHG_BG[j][i], mean[0][j][i], mean[1][j][i], sigma[0][j][i], sigma[1][j][i], j, i, "f_GaussBG_HG", "HG");
          if (bFit && bDetPlot) PlotNoiseSingle (canvas1DNoise, histHG[j][i], fitGausHG_BG[j][i], mean[0][j][i], mean[1][j][i], sigma[0][j][i], sigma[1][j][i], j, i, layer, chBoard, 
                                                  Form("%s/HG_NoiseWithFit", outputDirPlotsDet.Data()), currentRunInfo, 0.04);
          if (bFit){
            fitGausHG_BG_mapped[layer][chBoard] = (TF1*)fitGausHG_BG[j][i]->Clone(Form("f_GaussBG_HG_mapped_L%d_C%02d",layer,chBoard));
          }
        }
        // fill fit monitoring hists HG
        if (bFit){
          hist2DNoiseMean_HG->Fill(chBoard,layer,mean[0][j][i]);
          hist2DNoiseSigma_HG->Fill(chBoard,layer,sigma[0][j][i]);
          histNoiseMean_HG->Fill(mean[0][j][i]);
          histNoiseSigma_HG->Fill(sigma[0][j][i]);
          
          hist1DNoiseSigma_HG->SetBinContent(channelBin1D, sigma[0][j][i]);
          hist1DNoiseSigma_HG->SetBinError(channelBin1D, sigma[1][j][i]);
          hist1DNoiseMean_HG->SetBinContent(channelBin1D, mean[0][j][i]);
          hist1DNoiseMean_HG->SetBinError(channelBin1D, mean[1][j][i]);
          hist1DCAEN_NoiseSigma_HG->SetBinContent(chMap, sigma[0][j][i]);
          hist1DCAEN_NoiseSigma_HG->SetBinError(chMap, sigma[1][j][i]);
          hist1DCAEN_NoiseMean_HG->SetBinContent(chMap, mean[0][j][i]);          
          hist1DCAEN_NoiseMean_HG->SetBinError(chMap, mean[1][j][i]);          
        }
        // reset boolean for fit success monitoring
        bFit = kFALSE;
        // LG noise fits
        if (histLGTriggNoise[j][i]){
          Bool_t bFit = FitNoise (histLGTriggNoise[j][i], fitGausLG_BG[j][i], mean[2][j][i], mean[3][j][i], sigma[2][j][i], sigma[3][j][i], j, i, "f_GaussBG_LG", "LG");
          if (bFit && bDetPlot) PlotNoiseSingle (canvas1DNoise, histLGTriggNoise[j][i], fitGausLG_BG[j][i],  mean[2][j][i], mean[3][j][i], sigma[2][j][i], sigma[3][j][i], j, i, layer, chBoard, 
                                                  Form("%s/LG_NoiseWithFit", outputDirPlotsDet.Data()), currentRunInfo, 0.04);
          histLGTriggNoise_mapped[layer][chBoard] = (TH1D*)histLGTriggNoise[j][i]->Clone(Form("h_LGTriggeredNoise_mapped_L%d_C%02d",layer,chBoard));
          if (bFit){
            fitGausLG_BG_mapped[layer][chBoard] = (TF1*)fitGausLG_BG[j][i]->Clone(Form("f_GaussBG_LG_mapped_L%d_C%02d",layer,chBoard));
          }
        } else {
          Bool_t bFit = FitNoise (histLG[j][i], fitGausLG_BG[j][i], mean[2][j][i], mean[3][j][i], sigma[2][j][i], sigma[3][j][i], j, i, "f_GaussBG_LG", "LG");
          if (bFit && bDetPlot) PlotNoiseSingle (canvas1DNoise, histLGTriggNoise[j][i], fitGausLG_BG[j][i], mean[2][j][i], mean[3][j][i], sigma[2][j][i], sigma[3][j][i], j, i, layer, chBoard, 
                                                  Form("%s/LG_NoiseWithFit", outputDirPlotsDet.Data()), currentRunInfo, 0.04);
          if (bFit){
            fitGausLG_BG_mapped[layer][chBoard] = (TF1*)fitGausLG_BG[j][i]->Clone(Form("f_GaussBG_LG_mapped_L%d_C%02d",layer,chBoard));
          }
        }
        // fill fit monitoring hists LG
        if (bFit){
          hist2DNoiseMean_LG->Fill(chBoard,layer,mean[2][j][i]);
          hist2DNoiseSigma_LG->Fill(chBoard,layer,sigma[2][j][i]);
          histNoiseMean_LG->Fill(mean[2][j][i]);
          histNoiseSigma_LG->Fill(sigma[2][j][i]);
       
          hist1DNoiseSigma_LG->SetBinContent(channelBin1D, sigma[2][j][i]);
          hist1DNoiseSigma_LG->SetBinError(channelBin1D, sigma[3][j][i]);
          hist1DNoiseMean_LG->SetBinContent(channelBin1D, mean[2][j][i]);
          hist1DNoiseMean_LG->SetBinError(channelBin1D, mean[3][j][i]);
          hist1DCAEN_NoiseSigma_LG->SetBinContent(chMap, sigma[2][j][i]);
          hist1DCAEN_NoiseSigma_LG->SetBinError(chMap, sigma[3][j][i]);
          hist1DCAEN_NoiseMean_LG->SetBinContent(chMap, mean[2][j][i]);          
          hist1DCAEN_NoiseMean_LG->SetBinError(chMap, mean[3][j][i]);          
        }
        
        // ********************************************************
        // Plot 2D distribution time vs HG for monitoring
        // ********************************************************
        if (hist_T_HG[j][i] && bDetPlot){
          canvas2DCorr->cd();
            SetStyleHistoTH2ForGraphs( hist_T_HG[j][i], hist_T_HG[j][i]->GetXaxis()->GetTitle(), hist_T_HG[j][i]->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 1.25);  
            // find max x bin
            hist_T_HG[j][i]->GetXaxis()->SetRangeUser(0,tdiff+0.1);
            hist_T_HG[j][i]->Draw("colz");
            
            TLatex *labelChannel                     = new TLatex(0.85,0.92,Form("CAEN: B %d, C %d, Stack: L %d, C%d",j, i, layer, chBoard));
            SetStyleTLatex( labelChannel, textSizeRel,4,1,42,kTRUE,31);
            labelChannel->Draw();

          canvas2DCorr->SaveAs(Form("%s/T_HG_B%d_C%02d.pdf", outputDirPlotsDet.Data(), j,i));
        }
        
        // ********************************************************
        // Fit & Plot 2D LG vs HG
        // ********************************************************
        if (histLGHG[j][i]){
          FitAndPlotGainCorr ( histLGHG[j][i], fitLGHGCorr[j][i], "f_LGHGCorr", 100, 380, 500, 4000,
                              cslope[0][j][i], cslope[1][j][i], coffset[0][j][i], coffset[1][j][i], 
                              j, i, layer, chBoard,
                              kTRUE, canvas2DCorr, Form("%s/LG_HG_Corr", outputDirPlotsDet.Data()), textSizeRel);
          hist2DLGHG_slope->Fill(chBoard,layer,cslope[0][j][i]);
          hist2DLGHG_offset->Fill(chBoard,layer,coffset[0][j][i]);
          histLGHG_slope->Fill(cslope[0][j][i]);
          hist1DLGHG_slope->SetBinContent(channelBin1D, cslope[0][j][i]);
          hist1DLGHG_slope->SetBinError(channelBin1D, cslope[1][j][i]);
          hist1DLGHG_offset->SetBinContent(channelBin1D, coffset[0][j][i]);
          hist1DLGHG_offset->SetBinError(channelBin1D, coffset[1][j][i]);
          hist1DCAEN_LGHG_slope->SetBinContent(chMap, cslope[0][j][i]);
          hist1DCAEN_LGHG_slope->SetBinError(chMap, cslope[1][j][i]);
          hist1DCAEN_LGHG_offset->SetBinContent(chMap, coffset[0][j][i]);
          hist1DCAEN_LGHG_offset->SetBinError(chMap, coffset[1][j][i]);

        }
        if (histLGHG[j][i]){
          FitAndPlotGainCorr ( histHGLG[j][i], fitHGLGCorr[j][i], "f_HGLGCorr", 100, 3800, 4000, 500,
                              cslope[2][j][i], cslope[3][j][i], coffset[2][j][i], coffset[3][j][i], 
                              j, i, layer, chBoard,
                              kFALSE, canvas2DCorr, Form("%s/HG_LG_Corr", outputDirPlotsDet.Data()), textSizeRel);
          hist1DHGLG_slope->SetBinContent(channelBin1D, cslope[2][j][i]);
          hist1DHGLG_slope->Fill(channelBin1D, cslope[3][j][i]);
          hist1DHGLG_offset->SetBinContent(channelBin1D, coffset[2][j][i]);
          hist1DHGLG_offset->SetBinError(channelBin1D, coffset[3][j][i]);
          hist1DCAEN_HGLG_slope->SetBinContent(chMap, cslope[2][j][i]);
          hist1DCAEN_HGLG_slope->SetBinError(chMap, cslope[3][j][i]);
          hist1DCAEN_HGLG_offset->SetBinContent(chMap, coffset[2][j][i]);
          hist1DCAEN_HGLG_offset->SetBinError(chMap, coffset[3][j][i]);
        }        
        
        
        // ********************************************************
        // Plot different triggers together
        // ********************************************************
        if (bDetPlot)PlotOverlayDiffTriggers( canvas1DDiffTrigg, histHG[j][i], histHGTrig[j][i], histHGTriggNoise[j][i], fitGausHG_BG[j][i],
                                              0, 1050, Form("%s/HG_DiffTriggers", outputDirPlotsDet.Data()),
                                              j, i, layer, chBoard, currentRunInfo, 0.04);
        if (bDetPlot)PlotOverlayDiffTriggers( canvas1DDiffTrigg, histLG[j][i], histLGTrig[j][i], histLGTriggNoise[j][i], fitGausLG_BG[j][i],
                                              0, 1050, Form("%s/LG_DiffTriggers", outputDirPlotsDet.Data()),
                                              j, i, layer, chBoard, currentRunInfo, 0.04);
      }
    }
    
    for (Int_t l = 0; l < gMaxLayers; l++){
      if (!lActive[l]) continue;      
      if (isTBdata == 1){
        PlotNoiseWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                    histHGTriggNoise_mapped[l], fitGausHG_BG_mapped[l], 0, 275, 1.2, l,
                                    Form("%s/Noise_HG_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
        PlotNoiseWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                    histLGTriggNoise_mapped[l], fitGausLG_BG_mapped[l], 0, 275, 1.2, l,
                                    Form("%s/Noise_LG_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      }
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                 histHG_mapped[l], histHGTrig_mapped[l], histHGTriggNoise_mapped[l], fitGausHG_BG_mapped[l], 
                                 0, 3800, 1.2, l , Form("%s/TriggerOverlay_HG_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                 histHG_mapped[l], histHGTrig_mapped[l], histHGTriggNoise_mapped[l], fitGausHG_BG_mapped[l], 
                                 0, 2100, 1.2, l , Form("%s/TriggerOverlay_HG_Zoomed_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                 histLG_mapped[l], histLGTrig_mapped[l], histLGTriggNoise_mapped[l], fitGausLG_BG_mapped[l], 
                                 0, 3800, 1.2, l , Form("%s/TriggerOverlay_LG_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                 histLG_mapped[l], histLGTrig_mapped[l], histLGTriggNoise_mapped[l], fitGausLG_BG_mapped[l], 
                                 0, 1525, 1.2, l , Form("%s/TriggerOverlay_LG_Zoomed_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
     
      
    }
    if (isTBdata == 1){
      PlotOverlayFullLayer ( canvas8Panel, pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                              histHGTriggNoise_mapped, lActive, 0, gMaxLayers,
                              0, 350, -10, 425, 3, Form("%s/HG_Noise_AllLayers.pdf", outputDirPlots.Data()), currentRunInfo, "pe");
    }

    
    
    // ********************************************************
    // Create graphs per board and channels with calib values
    // -> noise mean & sigma LG, HG
    // -> slope and offset of LG-HG & HG-LG correlation
    // ********************************************************
    TGraphErrors* gNoiseMeanHG      = CreateGraphFromHistAndCleanup(hist1DNoiseMean_HG, "graph_mean_Noise_HG_channels");
    TGraphErrors* gNoiseMeanLG      = CreateGraphFromHistAndCleanup(hist1DNoiseMean_LG, "graph_mean_Noise_LG_channels");
    TGraphErrors* gNoiseSigmaHG     = CreateGraphFromHistAndCleanup(hist1DNoiseSigma_HG, "graph_sigma_Noise_HG_channels");
    TGraphErrors* gNoiseSigmaLG     = CreateGraphFromHistAndCleanup(hist1DNoiseSigma_LG, "graph_sigma_Noise_LG_channels");
    TGraphErrors* gCorrLGHGSlope    = CreateGraphFromHistAndCleanup(hist1DLGHG_slope, "graph_slope_corr_LGHG_channels");
    TGraphErrors* gCorrLGHGOffset   = CreateGraphFromHistAndCleanup(hist1DLGHG_offset, "graph_offset_corr_LGHG_channels");
    TGraphErrors* gCorrHGLGSlope    = CreateGraphFromHistAndCleanup(hist1DHGLG_slope, "graph_slope_corr_HGLG_channels");
    TGraphErrors* gCorrHGLGOffset   = CreateGraphFromHistAndCleanup(hist1DHGLG_offset, "graph_offset_corr_HGLG_channels");

    TGraphErrors* gCAEN_NoiseMeanHG     = CreateGraphFromHistAndCleanup(hist1DCAEN_NoiseMean_HG, "graphCAEN_mean_Noise_HG_channels");
    TGraphErrors* gCAEN_NoiseMeanLG     = CreateGraphFromHistAndCleanup(hist1DCAEN_NoiseMean_LG, "graphCAEN_mean_Noise_LG_channels");
    TGraphErrors* gCAEN_NoiseSigmaHG    = CreateGraphFromHistAndCleanup(hist1DCAEN_NoiseSigma_HG, "graphCAEN_sigma_Noise_HG_channels");
    TGraphErrors* gCAEN_NoiseSigmaLG    = CreateGraphFromHistAndCleanup(hist1DCAEN_NoiseSigma_LG, "graphCAEN_sigma_Noise_LG_channels");
    TGraphErrors* gCAEN_CorrLGHGSlope   = CreateGraphFromHistAndCleanup(hist1DCAEN_LGHG_slope, "graphCAEN_slope_corr_LGHG_channels");
    TGraphErrors* gCAEN_CorrLGHGOffset  = CreateGraphFromHistAndCleanup(hist1DCAEN_LGHG_offset, "graphCAEN_offset_corr_LGHG_channels");
    TGraphErrors* gCAEN_CorrHGLGSlope   = CreateGraphFromHistAndCleanup(hist1DCAEN_HGLG_slope, "graphCAEN_slope_corr_HGLG_channels");
    TGraphErrors* gCAEN_CorrHGLGOffset  = CreateGraphFromHistAndCleanup(hist1DCAEN_HGLG_offset, "graphCAEN_offset_corr_HGLG_channels");
        
    // ********************************************************************************************************
    // Noise subtracted data monitoring histos per CAEN board and channel
    // ********************************************************************************************************    
    TH1D* histNSLG[gMaxBoard][gMaxChannels];                // low gain all triggers
    TH1D* histNSHG[gMaxBoard][gMaxChannels];                // high gain all triggers
    TH1D* histNSCombHG[gMaxBoard][gMaxChannels];            // combined high&low gain all triggers
    TH2D* histNSLGHG[gMaxBoard][gMaxChannels];              // LG vs HG correlation
    TH2D* histNSHGLG[gMaxBoard][gMaxChannels];              // HG vs LG correlation
    TH1D* histNSLGTrig[gMaxBoard][gMaxChannels];            // low gain straight line triggers
    TH1D* histNSHGTrig[gMaxBoard][gMaxChannels];            // high gain straight line triggers
    TH1D* histNSCombHGTrig[gMaxBoard][gMaxChannels];        // combined high&low gain straight line triggers
    TH1D* histNSLGNoise[gMaxBoard][gMaxChannels];           // low gain !(straight line triggers)
    TH1D* histNSHGNoise[gMaxBoard][gMaxChannels];           // high gain !(straight line triggers)
    TH1D* histNSLGTriggNoise[gMaxBoard][gMaxChannels];      // low gain noise triggered
    TH1D* histNSHGTriggNoise[gMaxBoard][gMaxChannels];      // high gain noise triggered
    TH2D* hist_T_NSCombHG[gMaxBoard];                       // time vs high&low gain all triggers
    TF1*  fitLandauG_NSHGTrig[gMaxBoard][gMaxChannels];     // Landau Gauss fits
    
    //***************************************************
    // mapped channels - layer & read-out board channels
    //***************************************************
    // channel counting in layer starts with 1, easier acces make array go to 8
    TH1D* histNSLG_mapped[gMaxLayers][9];                   // low gain all triggers
    TH1D* histNSHG_mapped[gMaxLayers][9];                   // high gain all triggers
    TH1D* histNSCombHG_mapped[gMaxLayers][9];               // combined high&low gain all triggers
    TH1D* histNSLGTrig_mapped[gMaxLayers][9];               // low gain straight line triggers
    TH1D* histNSHGTrig_mapped[gMaxLayers][9];               // high gain straight line triggers
    TH1D* histNSCombHGTrig_mapped[gMaxLayers][9];           // combined high&low gain straight line triggers
    TH1D* histNSLGTriggNoise_mapped[gMaxLayers][9];         // low gain noise triggered
    TH1D* histNSHGTriggNoise_mapped[gMaxLayers][9];         // high gain noise triggered

    //***************************************************
    // mapped channels - layer & read-out board channels - rebinned
    //***************************************************
    // channel counting in layer starts with 1, easier acces make array go to 8
    TH1D* histNSHG_mappedReb[gMaxLayers][9];                   // high gain all triggers
    TH1D* histNSCombHG_mappedReb[gMaxLayers][9];               // combined high&low gain all triggers
    TH1D* histNSHGTrig_mappedReb[gMaxLayers][9];               // high gain straight line triggers
    TH1D* histNSCombHGTrig_mappedReb[gMaxLayers][9];           // combined high&low gain straight line triggers
    TF1*  fitLandauG_NSHGTrig_mapped[gMaxLayers][9];           // Landau Gauss fits
     
    //***************************************************
    // map of triggers above threshold  
    //***************************************************
    TH3D* histNS3DMap = new TH3D("h_map_NS_z_x_y", "; layer; col; row", 14, -0.5, 13.5, 4, -0.5, 3.5, 2, -0.5, 1.5);
    TH2D* histNS2DMap = new TH2D("h_map_NS_z_channel", "; channel; layer", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH1D* histNS1DMap = new TH1D("h_map_NS_channel", "; channel", 8, 0.5, 8.5);
    TH2D* hist2DNSTrigg         = new TH2D("hist2D_NS_Trigg", "; channel; layer; counts", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DNSTriggNoise    = new TH2D("hist2D_NS_TriggeredNoise", "; channel; layer; counts", 8, 0.5, 8.5, 14, -0.5, 13.5);
    //***************************************************
    // Init histograms
    //***************************************************    
    for (Int_t j = 0; j < gMaxBoard; j++){
      for (Int_t i = 0; i < gMaxChannels; i++){
        histNSHG[j][i]    = new TH1D(Form("h_NS_HG_B%d_C%02d",j,i),";HG (adc); counts",4201,-200,4001);
        histNSCombHG[j][i]= new TH1D(Form("h_NS_CombHG_B%d_C%02d",j,i),";comb HG equivalent (adc); counts",40201,-200,40001);
        histNSLG[j][i]    = new TH1D(Form("h_NS_LG_B%d_C%02d",j,i),";LG (adc); counts",4201,-200,4001);
        histNSLGHG[j][i]  = new TH2D(Form("h_NS_LGHG_B%d_C%02d",j,i),";LG (adc); HG (adc)",420,-200,4001,420,-200,4001);
        histNSHGLG[j][i]  = new TH2D(Form("h_NS_HGLG_B%d_C%02d",j,i),";HG (adc); LG (adc)",420,-200,4001,420,-200,4001);
        if (isTBdata > 0){
          histNSLGTrig[j][i]        = new TH1D(Form("h_NS_LGTrigg_B%d_C%02d",j,i),"coincidence trigg. front & back ; LG (adc); counts",4201,-200,4001);   // triggered 
          histNSHGTrig[j][i]        = new TH1D(Form("h_NS_HGTrigg_B%d_C%02d",j,i),"coincidence trigg. front & back ; HG (adc); counts",4201,-200,4001);
          histNSCombHGTrig[j][i]    = new TH1D(Form("h_NS_CombHGTrigg_B%d_C%02d",j,i),"coincidence trigg. front & back ; comb HG equivalent (adc); counts",40201,-200,40001);
          histNSLGNoise[j][i]       = new TH1D(Form("h_NS_LGNoise_B%d_C%02d",j,i),"!(coincidence trigg. front & back) ;LG (adc); counts",4201,-200,4001);
          histNSHGNoise[j][i]       = new TH1D(Form("h_NS_HGNoise_B%d_C%02d",j,i),"!(coincidence trigg. front & back) ;HG (adc); counts",4201,-200,4001);
          histNSLGTriggNoise[j][i]  = new TH1D(Form("h_NS_LGTriggeredNoise_B%d_C%02d",j,i),"veto front & back; LG (adc); counts",4201,-200,4001);
          histNSHGTriggNoise[j][i]  = new TH1D(Form("h_NS_HGTriggeredNoise_B%d_C%02d",j,i),"veto front & back; HG (adc); counts",4201,-200,4001);
        }
        fitLandauG_NSHGTrig[j][i]     = nullptr;
      }
      if (isTBdata)
        hist_T_NSCombHG[j]= new TH2D(Form("h_T_NS_CombHG_B%d",j),"; t (min); comb HG equivalent (adc)",1000,0,120,10000,0,1e6);
      else 
        hist_T_NSCombHG[j]= new TH2D(Form("h_T_NS_CombHG_B%d",j),"; t (min); comb HG equivalent (adc)",10000,0,1000,10000,0,1e6);
      
      
    }
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t c = 0; c < 9; c++){
        histNSLG_mapped[l][c]             = nullptr;
        histNSHG_mapped[l][c]             = nullptr; 
        histNSLGTrig_mapped[l][c]         = nullptr;
        histNSHGTrig_mapped[l][c]         = nullptr;
        histNSLGTriggNoise_mapped[l][c]   = nullptr;
        histNSHGTriggNoise_mapped[l][c]   = nullptr;
        histNSCombHG_mapped[l][c]         = nullptr; 
        histNSCombHGTrig_mapped[l][c]     = nullptr;
        histNSHG_mappedReb[l][c]             = nullptr;
        histNSHGTrig_mappedReb[l][c]         = nullptr;
        histNSCombHG_mappedReb[l][c]         = nullptr; 
        histNSCombHGTrig_mappedReb[l][c]     = nullptr;
        fitLandauG_NSHGTrig_mapped[l][c]     = nullptr;
      }
    }

    // rebin array
    Double_t binningADC[3000];
    for (Int_t i = 0; i < 1200; i++) binningADC[i]                      = -200+i;
    for (Int_t i = 0; i < 500; i++) binningADC[i+1200]                  = 1000+i*2;   
    for (Int_t i = 0; i < 400; i++) binningADC[i+1200+500]              = 2000+i*5;   
    for (Int_t i = 0; i < 200; i++) binningADC[i+1200+500+400]          = 4000+i*10;   
    for (Int_t i = 0; i < 280; i++) binningADC[i+1200+500+400+200]      = 6000+i*50;   
    for (Int_t i = 0; i < 201; i++) binningADC[i+1200+500+400+200+280]  = 20000+i*100;   
    
    if (verbosity > 2)for (Int_t i = 0; i < 2781; i++) std::cout << binningADC[i] << "," ;
    if (verbosity > 2)std::cout<< std::endl; 
    
    // 1D channel representation of fit values, x axis scales as 10x layer count + channel within one assembley, 
    // - layer 3 channel 3: 33
    // - layer 0 channel 2: 2
    TH1D* hist1DMPV_HG          = new TH1D("hist1DMPV_HG_channels", "; 10x layer + board channel; MPV_{mip} (HG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DMax_HG          = new TH1D("hist1DMax_HG_channels", "; 10x layer + board channel; Max_{mip} (HG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DFWHM_HG         = new TH1D("hist1DFWHM_HG_channels", "; 10x layer + board channel; FWHM_{mip} (HG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DWidth_HG        = new TH1D("hist1DWidth_HG_channels", "; 10x layer + board channel; Width_{mip} (HG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );
    TH1D* hist1DGWidth_HG       = new TH1D("hist1DGWidth_HG_channels", "; 10x layer + board channel; Gauss Width_{mip} (HG ADC)", 10*maxActiveLayer+1, -0.5, 10*maxActiveLayer+0.5 );

    // 1D channel representation of fit values, x axis scales as CAEN board channelns 64x CAEN board # + CAEN channel
    TH1D* hist1DCAEN_MPV_HG     = new TH1D("hist1DCAEN_MPV_HG_channels", "; 64x CAEN board + CAEN channel; MPV_{mip} (HG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_Max_HG     = new TH1D("hist1DCAEN_Max_HG_channels", "; 64x CAEN board + CAEN channel; Max_{mip} (HG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_FWHM_HG    = new TH1D("hist1DCAEN_FWHM_HG_channels", "; 64x CAEN board + CAEN channel; FWHM_{mip} (HG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_Width_HG   = new TH1D("hist1DCAEN_Width_HG_channels", "; 64x CAEN board + CAEN channel; Width_{mip} (HG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    TH1D* hist1DCAEN_GWidth_HG  = new TH1D("hist1DCAEN_GWidth_HG_channels", "; 64x CAEN board + CAEN channel; Gauss Width_{mip} (HG ADC)", 64*gMaxBoard+1, -0.5, 64*gMaxBoard+0.5 );
    
    // 2D representation of fit values
    TH2D* hist2DMPV_HG          = new TH2D("hist2DMPV_HG_z_channel", "; channel; layer; MPV_{mip} (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DMPVErr_HG       = new TH2D("hist2DMPVErr_HG_z_channel", "; channel; layer; #Delta(MPV_{mip}) (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DMax_HG          = new TH2D("hist2DMax_HG_z_channel", "; channel; layer; Max_{mip} (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DFWHM_HG         = new TH2D("hist2DFWHM_HG_z_channel", "; channel; layer; FWHM_{mip} (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DWidth_HG        = new TH2D("hist2DWidth_HG_z_channel", "; channel; layer; Width_{mip} (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DWidthErr_HG     = new TH2D("hist2DWidthErr_HG_z_channel", "; channel; layer; #Delta(Width_{mip}) (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DGWidth_HG       = new TH2D("hist2DGWidth_HG_z_channel", "; channel; layer; Gauss Width_{mip} (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH2D* hist2DGWidthErr_HG    = new TH2D("hist2DGWidthErr_HG_z_channel", "; channel; layer; #Delta(Gauss Width_{mip}) (HG ADC)", 8, 0.5, 8.5, 14, -0.5, 13.5);
    
    Double_t mpL[gMaxLayers][9]       = {{0.}};
    Double_t mpLErr[gMaxLayers][9]    = {{0.}};
    Double_t sigmaG[gMaxLayers][9]    = {{0.}};
    Double_t sigmaGErr[gMaxLayers][9] = {{0.}};
    Double_t sigmaL[gMaxLayers][9]    = {{0.}};
    Double_t sigmaLErr[gMaxLayers][9] = {{0.}};
    Double_t maxLandG[gMaxLayers][9]  = {{0.}};
    Double_t fwhmLandG[gMaxLayers][9] = {{0.}};
    double chisqr[gMaxLayers][9]      = {{0.}};
    int ndf[gMaxLayers][9]            = {{0}};
  
    // ********************************************************************************************************
    // Second loop over full tree to obtain noise subtracted histograms
    // ********************************************************************************************************
    TRandom3* rand = new TRandom3();
    for (Long64_t i=startEvent; i<nEntriesTree;i++) {
        // load current event
        tt_event->GetEntry(i);
        nEventsProcessed++;

        Int_t nChNoNoise = 0;
        // processing progress info
        if(i>0 && nEntriesTree>100 && i%(nEntriesTree/(50))==0) std::cout << "//processed " << 100*(i)/nEntriesTree << "%"  << std::endl;
        if(verbosity>0){
          std::cout << "***********************************************************************************************************" << std::endl;
          std::cout << "event " << i << std::endl;
          std::cout << "***********************************************************************************************************" << std::endl;
        }
        // temporary mapping of channels after noise subtractions
        Int_t signalNS[2][64][4] = {{{0}}};       // [board][channel][HG, LG, trigg HG,comb HG&LG]
        Double_t summed[2] = {0};                 // total channel sum
        // readEvent
        if (verbosity > 0)std::cout << "------------- Event -------------------------" << std::endl;
        if (verbosity > 0)std::cout << gTrID << "\t" << gTRtimeStamp << std::endl;
        if (i == startEvent) tstapMin = gTRtimeStamp;
        tstapMax = gTRtimeStamp;
        Double_t tCurr= (tstapMax-tstapMin)/1e6/60; // elapsed time in min
        
        for(Int_t ch=0; ch<gMaxChannels; ch++){
          Double_t meanNHG = 0;
          if (mean[0][gBoard[ch]][ch] != -1)
            meanNHG = mean[0][gBoard[ch]][ch];
          Double_t meanNLG = 0;
          if (mean[2][gBoard[ch]][ch] != -1)
            meanNLG = mean[2][gBoard[ch]][ch];
          
          signalNS[gBoard[ch]][gChannel[ch]][0] = gHG[ch]-meanNHG;
          signalNS[gBoard[ch]][gChannel[ch]][1] = gLG[ch]-meanNLG;
          // set combined HG & LG signal
          if (signalNS[gBoard[ch]][gChannel[ch]][0] < 3800)
            signalNS[gBoard[ch]][gChannel[ch]][3] = signalNS[gBoard[ch]][gChannel[ch]][0];
          else 
            signalNS[gBoard[ch]][gChannel[ch]][3] = signalNS[gBoard[ch]][gChannel[ch]][1] *cslope[0][gBoard[ch]][gChannel[ch]] + rand->Rndm()*cslope[0][gBoard[ch]][gChannel[ch]];
          Double_t chThres = scaledThr-meanNHG;
          if (gHG[ch] > chThres)  // fill potential trigger signals
            signalNS[gBoard[ch]][gChannel[ch]][2] = gHG[ch]-chThres;
          // fill histos
          histNSHG[gBoard[ch]][gChannel[ch]]->Fill(signalNS[gBoard[ch]][gChannel[ch]][0]); 
          histNSLG[gBoard[ch]][gChannel[ch]]->Fill(signalNS[gBoard[ch]][gChannel[ch]][1]); 
          histNSCombHG[gBoard[ch]][gChannel[ch]]->Fill(signalNS[gBoard[ch]][gChannel[ch]][3]); 
          histNSLGHG[gBoard[ch]][gChannel[ch]]->Fill(signalNS[gBoard[ch]][gChannel[ch]][1], signalNS[gBoard[ch]][gChannel[ch]][0]); 
          histNSHGLG[gBoard[ch]][gChannel[ch]]->Fill(signalNS[gBoard[ch]][gChannel[ch]][0], signalNS[gBoard[ch]][gChannel[ch]][1]); 
          if (signalNS[gBoard[ch]][gChannel[ch]][0] > (adcThreshold-meanNHG)){
            nChNoNoise++;
            if (verbosity > 0) std::cout << "not noise" << std::endl;
            if (isTBdata > 0){
              histNS3DMap->Fill(mapping[gChannel[ch]][0],mapping[gChannel[ch]][3],mapping[gChannel[ch]][2]);
              histNS2DMap->Fill(mapping[gChannel[ch]][1],mapping[gChannel[ch]][0]);
              histNS1DMap->Fill(mapping[gChannel[ch]][1]);
            }
          }
          // calculate summed signal
          summed[gBoard[ch]] = summed[gBoard[ch]]+signalNS[gBoard[ch]][gChannel[ch]][3];
        }
        // fill summed signal
        for (Int_t j = 0; j < gMaxBoard; j++)
          hist_T_NSCombHG[j]->Fill(tCurr,summed[j]);
        
        // *****************************************************************
        // check for conincidence between first and last channel 
        // above noise or pure noise triggers
        // *****************************************************************
        if (isTBdata == 1){
        // only for first CAEN board!!!!!
          for (Int_t chCAEN = 0; chCAEN < 64; chCAEN++){            
            Int_t chBoard = mapping[chCAEN][1];
            Int_t nNeigh  = 0;
            Int_t* neighborCh = new Int_t[5];
            GetNeighborWithinBoard(chBoard, nNeigh, neighborCh);
            if (verbosity > 2)std::cout << "channel in board "<< chBoard <<" found " << nNeigh << " neighbors " << std::endl;
          
            Int_t chCAENFront = backwardMapping[chBoard][minActiveLayer];
            Int_t chCAENBack  = backwardMapping[chBoard][maxActiveLayer];
            
            // Determine straight line trigger
            Bool_t  trigg         = (signalNS[0][chCAENFront][2] > 0 && signalNS[0][chCAENBack][2] > 0);
            Float_t triggSignal   = (signalNS[0][chCAENFront][2] + signalNS[0][chCAENBack][2])/2;
            
            // Determine noise trigger
            Bool_t triggNoise     = kFALSE;
            Float_t triggSigFN     = signalNS[0][chCAENFront][2];
            Float_t triggSigBN     = signalNS[0][chCAENBack][2];
            for (Int_t n = 0; n < nNeigh; n++){
                triggSigFN = triggSigFN + signalNS[0][backwardMapping[neighborCh[n]][minActiveLayer]][2];
                triggSigBN = triggSigBN + signalNS[0][backwardMapping[neighborCh[n]][maxActiveLayer]][2];
            }
            if (triggSigFN == 0 && triggSigBN == 0)
              triggNoise  = kTRUE;
            
            // Fill triggered histograms
            if (verbosity > 0) std::cout << "channel board " << chCAEN << "\t" << chBoard << "\t" << chCAENFront << "\t" << chCAENBack << ""<< std::endl; 
            if (trigg){
              histNSHGTrig[0][chCAEN]->Fill(signalNS[0][chCAEN][0]); 
              histNSCombHGTrig[0][chCAEN]->Fill(signalNS[0][chCAEN][3]); 
              histNSLGTrig[0][chCAEN]->Fill(signalNS[0][chCAEN][1]); 
              if (signalNS[0][chCAEN][0] > 0) hist2DNSTrigg->Fill(mapping[chCAEN][1],mapping[chCAEN][0]);
            } else {
              histNSHGNoise[0][chCAEN]->Fill(signalNS[0][chCAEN][0]); 
              histNSLGNoise[0][chCAEN]->Fill(signalNS[0][chCAEN][1]);               
            }
            if (triggNoise){
              histNSHGTriggNoise[0][chCAEN]->Fill(signalNS[0][chCAEN][0]); 
              histNSLGTriggNoise[0][chCAEN]->Fill(signalNS[0][chCAEN][1]);       
              hist2DNSTriggNoise->Fill(mapping[chCAEN][1],mapping[chCAEN][0]);
            }
          }
        }
        if (verbosity > 0)std::cout << "Channels above noise: "<< nChNoNoise << std::endl;        
    }
    std::cout << "Total events processed:" << nEventsProcessed << std::endl;

    
    // ********************************************************************************************************
    // Plotting of single channels after noise subtraction
    // -> remapping of CAEN board+channels to physicsal layers and readout-channels
    // ********************************************************************************************************    
    for (Int_t j = 0; j < gMaxBoard; j++){
      for (Int_t i = 0; i < gMaxChannels; i++){
        Int_t chMap     = j*64 + i;
        Int_t chBoard   = mapping[chMap][1];
        Int_t layer     = mapping[chMap][0];
        if (verbosity > 0)std::cout << j << "\t" << i << "\t" << chMap << "\t L: " << layer  << "\t C:" <<  chBoard << std::endl;
        
        histNSHG_mapped[layer][chBoard] = (TH1D*)histNSHG[j][i]->Clone(Form("h_NS_HG_mapped_L%d_C%02d",layer,chBoard));
        histNSCombHG_mapped[layer][chBoard] = (TH1D*)histNSCombHG[j][i]->Clone(Form("h_NS_CombHG_mapped_L%d_C%02d",layer,chBoard));
        histNSLG_mapped[layer][chBoard] = (TH1D*)histNSLG[j][i]->Clone(Form("h_NS_LG_mapped_L%d_C%02d",layer,chBoard));
        if (histNSHGTrig[j][i]) histNSHGTrig_mapped[layer][chBoard] = (TH1D*)histNSHGTrig[j][i]->Clone(Form("h_NS_HGTrigg_mapped_L%d_C%02d",layer,chBoard));
        if (histNSCombHGTrig[j][i]) histNSCombHGTrig_mapped[layer][chBoard] = (TH1D*)histNSCombHGTrig[j][i]->Clone(Form("h_NS_CombHGTrigg_mapped_L%d_C%02d",layer,chBoard));
        if (histNSLGTrig[j][i]) histNSLGTrig_mapped[layer][chBoard] = (TH1D*)histNSLGTrig[j][i]->Clone(Form("h_NS_LGTrigg_mapped_L%d_C%02d",layer,chBoard));
        if (histNSHGTriggNoise[j][i]) histNSHGTriggNoise_mapped[layer][chBoard] = (TH1D*)histNSHGTriggNoise[j][i]->Clone(Form("h_NS_HGTriggeredNoise_mapped_L%d_C%02d",layer,chBoard));
        if (histNSLGTriggNoise[j][i]) histNSLGTriggNoise_mapped[layer][chBoard] = (TH1D*)histNSLGTriggNoise[j][i]->Clone(Form("h_NS_LGTriggeredNoise_mapped_L%d_C%02d",layer,chBoard));
        
        Bool_t fTrigg   = kFALSE;
        histNSHG_mapped[layer][chBoard]->Sumw2();
        histNSHG_mappedReb[layer][chBoard] = (TH1D*)histNSHG_mapped[layer][chBoard]->Rebin(2100,Form("%sReb",(histNSHG_mapped[layer][chBoard]->GetName())), binningADC);
        histNSHG_mappedReb[layer][chBoard]->Scale(1,"width");
        if (histNSHGTrig[j][i]){
          fTrigg    = kTRUE;
          histNSHGTrig_mapped[layer][chBoard]->Sumw2();
          histNSHGTrig_mappedReb[layer][chBoard] = (TH1D*)histNSHGTrig_mapped[layer][chBoard]->Rebin(2100,Form("%sReb",(histNSHGTrig_mapped[layer][chBoard]->GetName())), binningADC);
          histNSHGTrig_mappedReb[layer][chBoard]->Scale(1,"width");
        }
        histNSCombHG_mapped[layer][chBoard]->Sumw2();
        histNSCombHG_mappedReb[layer][chBoard] = (TH1D*)histNSCombHG_mapped[layer][chBoard]->Rebin(2780,Form("%sReb",(histNSCombHG_mapped[layer][chBoard]->GetName())), binningADC);
        histNSCombHG_mappedReb[layer][chBoard]->Scale(1,"width");
        if (histNSCombHGTrig[j][i]){
          histNSCombHGTrig_mapped[layer][chBoard]->Sumw2();
          histNSCombHGTrig_mappedReb[layer][chBoard] = (TH1D*)histNSCombHGTrig_mapped[layer][chBoard]->Rebin(2780,Form("%sReb",(histNSCombHGTrig_mapped[layer][chBoard]->GetName())), binningADC);
          histNSCombHGTrig_mappedReb[layer][chBoard]->Scale(1,"width");
        }
        
        if (fTrigg){
          // Setting fit range and start values
          double fr[2]    = {150, 2000};
          double sv[4]    = {50, 600, 5000, 5.0};
          double pllo[4]  = {0.5, 200, 1.0, 0.4};
          double plhi[4]  = {500, 1000, 10000, 10};
          double fp[4];
          double fpe[4];
          
          if (verbosity > 0) std::cout << "\t Int: " << histNSHGTrig_mapped[layer][chBoard]->Integral() << "\t" << histNSHGTrig_mapped[layer][chBoard]->GetRMS() << std::endl;
          if (histNSHGTrig_mapped[layer][chBoard]->Integral() < 5000){
            histNSHGTrig_mapped[layer][chBoard]->Sumw2();
            histNSHGTrig_mapped[layer][chBoard]->Rebin(2);
            histNSHGTrig_mapped[layer][chBoard]->Scale(1,"width");          
            histNSHGTrig_mappedReb[layer][chBoard]->Sumw2();
            histNSHGTrig_mappedReb[layer][chBoard]->Rebin(2);
            histNSHGTrig_mappedReb[layer][chBoard]->Scale(1./2);          
            
            histNSHGTrig[j][i]->Sumw2();
            histNSHGTrig[j][i]->Rebin(2);
            histNSHGTrig[j][i]->Scale(1,"width");          
          }
            
          plhi[2]=histNSHGTrig_mapped[layer][chBoard]->Integral()*2;
          plhi[3]=histNSHGTrig_mapped[layer][chBoard]->GetRMS()*2;
          sv[2]=histNSHGTrig_mapped[layer][chBoard]->Integral(); 

          fitLandauG_NSHGTrig_mapped[layer][chBoard]  = langaufit(histNSHGTrig_mappedReb[layer][chBoard],fr,sv,pllo,plhi,fp,fpe,&chisqr[layer][chBoard],&ndf[layer][chBoard], verbosity);
          fitLandauG_NSHGTrig_mapped[layer][chBoard]->SetName(Form("f_LandauG_NSHG_L%d_C%02d",layer,chBoard));
          fitLandauG_NSHGTrig[j][i]                   = (TF1*)fitLandauG_NSHGTrig_mapped[layer][chBoard]->Clone(Form("f_LandauG_NSHG_B%d_C%02d",j,i));
          mpL[layer][chBoard]           = fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(1);
          mpLErr[layer][chBoard]        = fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(1);
          sigmaG[layer][chBoard]        = fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(3);
          sigmaGErr[layer][chBoard]     = fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(3);
          sigmaL[layer][chBoard]        = fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(0);
          sigmaLErr[layer][chBoard]     = fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(0);
          
          hist2DMPV_HG->Fill(chBoard, layer,fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(1));
          hist2DMPVErr_HG->Fill(chBoard, layer,fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(1));
          hist2DWidth_HG->Fill(chBoard, layer,fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(0));
          hist2DWidthErr_HG->Fill(chBoard, layer,fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(0));
          hist2DGWidth_HG->Fill(chBoard, layer,fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(3));
          hist2DGWidthErr_HG->Fill(chBoard, layer,fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(3));
          
          /// get bin to fill for 1D representation of channels for fit values
          Int_t channelBin1D = hist1DMPV_HG->FindBin(layer*10+chBoard);          
          hist1DMPV_HG->SetBinContent(channelBin1D, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(1));
          hist1DMPV_HG->SetBinError(channelBin1D, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(1));
          hist1DWidth_HG->SetBinContent(channelBin1D, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(0));
          hist1DWidth_HG->SetBinError(channelBin1D, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(0));
          hist1DGWidth_HG->SetBinContent(channelBin1D, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(3));
          hist1DGWidth_HG->SetBinError(channelBin1D, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(3));
          
          hist1DCAEN_MPV_HG->SetBinContent(chMap+1, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(1));
          hist1DCAEN_MPV_HG->SetBinError(chMap+1, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(1));
          hist1DCAEN_Width_HG->SetBinContent(chMap+1, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(0));
          hist1DCAEN_Width_HG->SetBinError(chMap+1, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(0));
          hist1DCAEN_GWidth_HG->SetBinContent(chMap+1, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParameter(3));
          hist1DCAEN_GWidth_HG->SetBinError(chMap+1, fitLandauG_NSHGTrig_mapped[layer][chBoard]->GetParError(3));
          
          double SNRPeak, SNRFWHM;
          langaupro(fp,SNRPeak,SNRFWHM);

          maxLandG[layer][chBoard]      = (Double_t)SNRPeak;
          fwhmLandG[layer][chBoard]     = (Double_t)SNRFWHM;
          
          hist2DMax_HG->Fill(chBoard, layer,SNRPeak);
          hist2DFWHM_HG->Fill(chBoard, layer,SNRFWHM);
          hist1DMax_HG->SetBinContent(channelBin1D, SNRPeak);
          hist1DFWHM_HG->SetBinContent(channelBin1D, SNRFWHM);
          hist1DCAEN_Max_HG->SetBinContent(chMap+1, SNRPeak);
          hist1DCAEN_FWHM_HG->SetBinContent(chMap+1, SNRFWHM);
        }

        if (bDetPlot){
          // ********************************************************
          // Plot different triggers together
          // ********************{************************************
          PlotOverlayDiffTriggers( canvas1DDiffTrigg, histNSHG[j][i], histNSHGTrig[j][i], histNSHGTriggNoise[j][i], NULL,
                                -100, 4000, Form("%s/HG_NS_DiffTriggers", outputDirPlotsDet.Data()),
                                j, i, layer, chBoard, currentRunInfo, 0.04);
          PlotOverlayDiffTriggers( canvas1DDiffTrigg, histNSLG[j][i], histNSLGTrig[j][i], histNSLGTriggNoise[j][i], NULL,
                                -100, 4000, Form("%s/LG_NS_DiffTriggers", outputDirPlotsDet.Data()),
                                j, i, layer, chBoard, currentRunInfo, 0.04);
        }
      }
    }
    // ********************************************************************************************************
    // Plotting summed signals vs time per board
    // ********************************************************************************************************
    for (Int_t j = 0; j < gMaxBoard; j++){
      if (hist_T_NSCombHG[j] && bDetPlot){
        canvas2DCorr->cd();
        DefaultCancasSettings( canvas2DCorr, 0.1, 0.1, 0.04, 0.08);  
          SetStyleHistoTH2ForGraphs( hist_T_NSCombHG[j], hist_T_NSCombHG[j]->GetXaxis()->GetTitle(), hist_T_NSCombHG[j]->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 1.25);  
          hist_T_NSCombHG[j]->GetXaxis()->SetRangeUser(0,tdiff+0.1);
          hist_T_NSCombHG[j]->Draw("colz");
          
          TLatex *labelChannel                     = new TLatex(0.85,0.90,Form("CAEN: B %d",j));
          SetStyleTLatex( labelChannel, textSizeRel,4,1,42,kTRUE,31);
          labelChannel->Draw();

        canvas2DCorr->SaveAs(Form("%s/T_HGSummed_B%d.pdf", outputDirPlots.Data(), j));
      }
    }
    canvas2DCorr->cd();
    DefaultCancasSettings( canvas2DCorr, 0.1, 0.1, 0.02, 0.08);  
      SetStyleHistoTH2ForGraphs( hist2DNSTrigg, hist2DNSTrigg->GetXaxis()->GetTitle(), hist2DNSTrigg->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 1.25);  
      hist2DNSTrigg->GetYaxis()->SetRangeUser(-0.5,maxActiveLayer+1.5);
      hist2DNSTrigg->GetZaxis()->SetRangeUser(hist2DNSTrigg->GetMinimum(1),hist2DNSTrigg->GetMaximum());
      hist2DNSTrigg->Draw("colz,text");
      
      TLatex *labelTrigg                     = new TLatex(0.85,0.93,"straight line triggers");
      SetStyleTLatex( labelTrigg, textSizeRel,4,1,42,kTRUE,31);
      labelTrigg->Draw();

    canvas2DCorr->SaveAs(Form("%s/NTrigg_2D.pdf", outputDirPlots.Data()));

    canvas2DCorr->cd();
    DefaultCancasSettings( canvas2DCorr, 0.1, 0.1, 0.02, 0.08);  
      SetStyleHistoTH2ForGraphs( hist2DNSTriggNoise, hist2DNSTriggNoise->GetXaxis()->GetTitle(), hist2DNSTriggNoise->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 1.25);  
      hist2DNSTriggNoise->GetYaxis()->SetRangeUser(-0.5,maxActiveLayer+1.5);
      hist2DNSTriggNoise->GetZaxis()->SetRangeUser(hist2DNSTriggNoise->GetMinimum(1),hist2DNSTriggNoise->GetMaximum());
      hist2DNSTriggNoise->Draw("colz,text");
      
      labelTrigg->SetText(0.85,0.93,"noise triggers");
      labelTrigg->Draw();

    canvas2DCorr->SaveAs(Form("%s/NTriggNoise_2D.pdf", outputDirPlots.Data()));

    TH2D* hist2DNSTriggEffi = (TH2D*)hist2DNSTrigg->Clone("hist2DNSTrigg_effi");
    hist2DNSTriggEffi->Reset();
    hist2DNSTriggEffi->GetZaxis()->UnZoom();
    for (Int_t c = 1; c < 9; c++){
      for (Int_t l = 1; l < gMaxLayers+1; l++){
        Double_t content = (Double_t)hist2DNSTrigg->GetBinContent(c,l)/hist2DNSTrigg->GetBinContent(c,1)*100;
        hist2DNSTriggEffi->Fill(c,l-1,content );
      }        
    }
    canvas2DCorr->cd();
    canvas2DCorr->SetLogz(0);
    DefaultCancasSettings( canvas2DCorr, 0.08, 0.13, 0.02, 0.08);  
      SetStyleHistoTH2ForGraphs( hist2DNSTriggEffi, hist2DNSTriggEffi->GetXaxis()->GetTitle(), hist2DNSTriggEffi->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 0.9);  
      hist2DNSTriggEffi->GetZaxis()->SetTitle("#varepsilon (%)");
      hist2DNSTriggEffi->GetYaxis()->SetRangeUser(-0.5,maxActiveLayer+1.5);
      hist2DNSTriggEffi->GetZaxis()->SetRangeUser(hist2DNSTriggEffi->GetMinimum(0.5),hist2DNSTriggEffi->GetMaximum());
      if (verbosity > 0)std::cout << hist2DNSTriggEffi->GetMinimum(1) << "\t" << hist2DNSTriggEffi->GetMaximum() << std::endl;
      hist2DNSTriggEffi->Draw("colz,text");
      
      labelTrigg->SetText(0.8,0.93,"rec. effi");
      SetStyleTLatex( labelTrigg, textSizeRel,4,1,42,kTRUE,31);
      labelTrigg->Draw();

    canvas2DCorr->SaveAs(Form("%s/NTriggEff_2D.pdf", outputDirPlots.Data()));

    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DNoiseMean_HG, maxActiveLayer, textSizeRel, Form("%s/HG_Noise_Mean.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DNoiseSigma_HG, maxActiveLayer, textSizeRel, Form("%s/HG_Noise_Sigma.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DNoiseMean_LG, maxActiveLayer, textSizeRel, Form("%s/LG_Noise_Mean.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DNoiseSigma_LG, maxActiveLayer, textSizeRel, Form("%s/LG_Noise_Sigma.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DLGHG_slope, maxActiveLayer, textSizeRel, Form("%s/LGHG_Slope.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DLGHG_offset, maxActiveLayer, textSizeRel, Form("%s/LGHG_Offset.pdf", outputDirPlots.Data()), currentRunInfo, kTRUE);
    
    
    
    for (Int_t l = 0; l < gMaxLayers; l++){
      if (!lActive[l]) continue;      
      PlotNoiseWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                  histNSHGTriggNoise_mapped[l], nullptr, -120, 375, 1.2, l,
                                  Form("%s/Noise_NS_HG_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                 histNSHG_mapped[l], histNSHGTrig_mapped[l], histNSHGTriggNoise_mapped[l], nullptr, 
                                 -80, 3800, 1.2, l , Form("%s/TriggerOverlay_HG_NS_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                 histNSLG_mapped[l], histNSLGTrig_mapped[l], histNSLGTriggNoise_mapped[l], nullptr, 
                                 -80, 3800, 1.2, l , Form("%s/TriggerOverlay_LG_NS_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
    }
    
    // ********************************************************************************************************
    // Overlay noise in same layer
    // ********************************************************************************************************
    for (Int_t l = 0; l < gMaxLayers; l++){
      if (!lActive[l]) continue;      
      for (Int_t c = 1; c < 9; c++){
        
        PlotMIPSingle (canvas1DNoise, histNSHGTrig_mapped[l][c],fitLandauG_NSHGTrig_mapped[l][c], chisqr[l][c], ndf[l][c], maxLandG[l][c], fwhmLandG[l][c],
                        l, c, Form("%s/HG_TriggMipWithFit", outputDirPlotsDet.Data()), currentRunInfo, 0.04);
      }
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSHGTriggNoise_mapped[l], 1, 9,
                                  -100, 400, 1, Form("%s/HGNS_Noise", outputDirPlots.Data()), l, currentRunInfo, 0.04);
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSHGTrig_mappedReb[l], 1, 9,
                                  0, 4000, 1./5, Form("%s/HGNS_Trigg", outputDirPlots.Data()), l, currentRunInfo, 0.04,"hist");
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSHGTrig_mappedReb[l], 1, 9,
                                  0, 2000, 1./2, Form("%s/HGNS_TriggZoomed", outputDirPlots.Data()), l, currentRunInfo, 0.04,"hist");
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSHG_mappedReb[l], 1, 9,
                                  -100, 4000, 1./5, Form("%s/HGNS", outputDirPlots.Data()), l, currentRunInfo, 0.04,"hist");
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSLGTriggNoise_mapped[l], 1, 9,
                                  -100, 400, 1, Form("%s/LGNS_Noise", outputDirPlots.Data()), l, currentRunInfo, 0.04);
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSLGTrig_mapped[l], 1, 9,
                                  0, 4000, 1, Form("%s/LGNS_Trigg", outputDirPlots.Data()), l, currentRunInfo,0.04);
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSLG_mapped[l], 1, 9,
                                  -100, 4000, 1, Form("%s/LGNS", outputDirPlots.Data()), l, currentRunInfo, 0.04);
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSCombHGTrig_mappedReb[l], 1, 9,
                                  0, 40000, 1./100, Form("%s/CombHGNS_Trigg", outputDirPlots.Data()), l, currentRunInfo, 0.04,"hist");
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSCombHG_mappedReb[l], 1, 9,
                                  -100, 40000, 1./100, Form("%s/CombHGNS", outputDirPlots.Data()), l, currentRunInfo, 0.04,"hist");
      PlotStraigtLineTriggAndFitFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                          histNSHGTrig_mapped[l], fitLandauG_NSHGTrig_mapped[l],maxLandG[l],
                                          150, 2200, 0, 2200, 1.2, l, Form("%s/TriggerWithFit_HG_NS_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotStraigtLineTriggAndFitFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                          histNSHGTrig_mappedReb[l], fitLandauG_NSHGTrig_mapped[l],maxLandG[l],
                                          150, 2200, 0, 2200, 1.2, l, Form("%s/TriggerWithFit_HG_NSReb_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotStraigtLineTriggAndFitFullLayerLin (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                            histNSHGTrig_mappedReb[l], fitLandauG_NSHGTrig_mapped[l],maxLandG[l],
                                            150, 2200, 0, 2200, 1.05, l, Form("%s/TriggerWithFitLinY_HG_NS_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      
    }
    
    // ********************************************************************************************************
    // Overlay noise for same readout channel
    // ********************************************************************************************************
    for (Int_t c = 1; c < 9; c++){      
      PlotChannelOverlaySameReadoutChannel( canvas1DDiffTrigg, histNSHGTriggNoise_mapped, lActive, 0, gMaxLayers, 
                                            -200,400, 1, Form("%s/HGNS_Noise", outputDirPlots.Data()), c, currentRunInfo, 0.04);    
      PlotChannelOverlaySameReadoutChannel( canvas1DDiffTrigg, histNSHGTrig_mappedReb, lActive, 0, gMaxLayers, 
                                            0,4000, 1./5, Form("%s/HGNS_Trigg", outputDirPlots.Data()), c, currentRunInfo, 0.04,"hist");    
      PlotChannelOverlaySameReadoutChannel( canvas1DDiffTrigg, histNSHGTrig_mappedReb, lActive, 0, gMaxLayers, 
                                            0,2000, 1./2, Form("%s/HGNS_TriggZoomed", outputDirPlots.Data()), c, currentRunInfo, 0.04, "hist");    
      PlotChannelOverlaySameReadoutChannel( canvas1DDiffTrigg, histNSHG_mappedReb, lActive, 0, gMaxLayers, 
                                            -100,4000, 1./5, Form("%s/HGNS", outputDirPlots.Data()), c, currentRunInfo,0.04,"hist");    
      PlotChannelOverlaySameReadoutChannel( canvas1DDiffTrigg, histNSCombHGTrig_mappedReb, lActive, 0, gMaxLayers, 
                                            0,40000, 1./100, Form("%s/CombHGNS_Trigg", outputDirPlots.Data()), c, currentRunInfo, 0.04,"hist");    
      PlotChannelOverlaySameReadoutChannel( canvas1DDiffTrigg, histNSCombHG_mappedReb, lActive, 0, gMaxLayers, 
                                            -100,40000, 1./100, Form("%s/CombHGNS", outputDirPlots.Data()), c, currentRunInfo, 0.04, "hist");    
    }
    if (isTBdata == 1){
      PlotOverlayFullLayer ( canvas8Panel, pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                              histNSHGTriggNoise_mapped, lActive, 0, gMaxLayers,
                              -100, 350, -125, 425, 1.5, Form("%s/HGNS_Noise_AllLayers.pdf", outputDirPlots.Data()), currentRunInfo, "pe");
    }
    
    PlotOverlayFullLayer ( canvas8Panel, pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                            histNSHGTrig_mappedReb, lActive, 0, gMaxLayers,
                            200, 2200, -10, 2200, 1.5, Form("%s/HGNS_Trigg_AllLayers.pdf", outputDirPlots.Data()), currentRunInfo, "hist");


    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DMPV_HG, maxActiveLayer, textSizeRel, Form("%s/HGTrigg_MPV.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DMPVErr_HG, maxActiveLayer, textSizeRel, Form("%s/HGTrigg_MPVErr.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DMax_HG, maxActiveLayer, textSizeRel, Form("%s/HGTrigg_Max.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DFWHM_HG, maxActiveLayer, textSizeRel, Form("%s/HGTrigg_FWHM.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DWidth_HG, maxActiveLayer, textSizeRel, Form("%s/HGTrigg_Width.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DWidthErr_HG, maxActiveLayer, textSizeRel, Form("%s/HGTrigg_WidthErr.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DGWidth_HG, maxActiveLayer, textSizeRel, Form("%s/HGTrigg_GWidth.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DGWidthErr_HG, maxActiveLayer, textSizeRel, Form("%s/HGTrigg_GWidthErr.pdf", outputDirPlots.Data()), currentRunInfo);
        
    // *****************************************************************
    // Create graphs per board and channels with calib values MIP values
    // *****************************************************************
    TGraphErrors* graphMPV_HG     = CreateGraphFromHistAndCleanup(hist1DMPV_HG, "graphMPV_HG_channels");
    TGraphErrors* graphMax_HG     = CreateGraphFromHistAndCleanup(hist1DMax_HG, "graphMax_HG_channels");
    TGraphErrors* graphWidth_HG   = CreateGraphFromHistAndCleanup(hist1DWidth_HG, "graphWidth_HG_channels");
    TGraphErrors* graphGWidth_HG  = CreateGraphFromHistAndCleanup(hist1DGWidth_HG, "graphGWidth_HG_channels");
    TGraphErrors* graphFWHM_HG    = CreateGraphFromHistAndCleanup(hist1DFWHM_HG, "graphFWHM_HG_channels");

    TGraphErrors* graphCAEN_MPV_HG    = CreateGraphFromHistAndCleanup(hist1DCAEN_MPV_HG, "graphCAEN_MPV_HG_channels");
    TGraphErrors* graphCAEN_Max_HG    = CreateGraphFromHistAndCleanup(hist1DCAEN_Max_HG, "graphCAEN_Max_HG_channels");
    TGraphErrors* graphCAEN_Width_HG  = CreateGraphFromHistAndCleanup(hist1DCAEN_Width_HG, "graphCAEN_Width_HG_channels");
    TGraphErrors* graphCAEN_GWidth_HG = CreateGraphFromHistAndCleanup(hist1DCAEN_GWidth_HG, "graphCAEN_GWidth_HG_channels");
    TGraphErrors* graphCAEN_FWHM_HG   = CreateGraphFromHistAndCleanup(hist1DCAEN_FWHM_HG, "graphCAEN_FWHM_HG_channels");

    TObjString* stringRunInfo = new TObjString;
    stringRunInfo->SetString(Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V, HG = %1.0f, LG = %1.0f", currentRunInfo.species.Data(), currentRunInfo.energy, currentRunInfo.runNr, currentRunInfo.vop, currentRunInfo.hgSet, currentRunInfo.lgSet));
    
    // ********************************************************************************************************
    // write output to single file
    // ********************************************************************************************************
    TFile* fileOutput = new TFile(Form("%s/output_basics.root",outputDir.Data()),"RECREATE");
    // ******************************************************
    // create folders
    // ******************************************************
    fileOutput->mkdir("RawData");
    fileOutput->mkdir("RawDataMapped");
    fileOutput->mkdir("NoiseSubtracted");
    fileOutput->mkdir("NoiseSubtractedMapped");
    fileOutput->mkdir("CorrelationLGHG");
    fileOutput->mkdir("CorrelationHGLG");
    // ******************************************************
    // run over all CAEN boards and channels
    // ******************************************************
    for (Int_t j = 0; j < gMaxBoard; j++){
      for (Int_t i = 0; i < gMaxChannels; i++){
        // raw data
        fileOutput->cd("RawData");
        histHG[j][i]->Write();
        hist_T_HG[j][i]->Write();
        if (isTBdata){
          histHGTrig[j][i]->Write();
          histHGNoise[j][i]->Write();
          histHGTriggNoise[j][i]->Write();
          if (fitGausHG_BG[j][i]) fitGausHG_BG[j][i]->Write();
        }
        histLG[j][i]->Write();
        if (isTBdata){
          histLGTrig[j][i]->Write();
          histLGNoise[j][i]->Write();
          histLGTriggNoise[j][i]->Write();
          if (fitGausLG_BG[j][i]) fitGausLG_BG[j][i]->Write();          
        }
        // noise subtracted
        fileOutput->cd("NoiseSubtracted");
        histNSHG[j][i]->Write();
        histNSCombHG[j][i]->Write();
        if (isTBdata){
          histNSHGTrig[j][i]->Write();
          histNSCombHGTrig[j][i]->Write();
          histNSHGNoise[j][i]->Write();
          histNSHGTriggNoise[j][i]->Write();
          if(fitLandauG_NSHGTrig[j][i])fitLandauG_NSHGTrig[j][i]->Write();
        }
        histNSLG[j][i]->Write();
        if (isTBdata){
          histNSLGTrig[j][i]->Write();
          histNSLGNoise[j][i]->Write();
          histNSLGTriggNoise[j][i]->Write();
        }
        // correlation histograms
        fileOutput->cd("CorrelationLGHG");
        histLGHG[j][i]->Write();
        if (fitLGHGCorr[j][i]) fitLGHGCorr[j][i]->Write();
        histNSLGHG[j][i]->Write();        
        fileOutput->cd("CorrelationHGLG");
        histHGLG[j][i]->Write();
        if (fitHGLGCorr[j][i]) fitHGLGCorr[j][i]->Write();
        histNSHGLG[j][i]->Write();        
      }
      fileOutput->cd();
      hist_T_NSCombHG[j]->Write();
    }
    // *******************************************************
    // run over all physical layers and readout board channels
    // *******************************************************
    for (Int_t l = 0; l  < gMaxLayers; l++){
      for (Int_t c = 1; c < 9; c++){
        fileOutput->cd("RawDataMapped");
        if (histHG_mapped[l][c]) histHG_mapped[l][c]->Write();
        if (histHGTrig_mapped[l][c]) histHGTrig_mapped[l][c]->Write();
        if (histHGTriggNoise_mapped[l][c]) histHGTriggNoise_mapped[l][c]->Write();
        if (fitGausHG_BG_mapped[l][c]) fitGausHG_BG_mapped[l][c]->Write();
        if (histLG_mapped[l][c]) histLG_mapped[l][c]->Write();
        if (histLGTrig_mapped[l][c]) histLGTrig_mapped[l][c]->Write();
        if (histLGTriggNoise_mapped[l][c]) histLGTriggNoise_mapped[l][c]->Write();
        if (fitGausLG_BG_mapped[l][c]) fitGausLG_BG_mapped[l][c]->Write();
        fileOutput->cd("NoiseSubtractedMapped");
        if (histNSHG_mapped[l][c]) histNSHG_mapped[l][c]->Write();
        if (histNSHG_mappedReb[l][c]) histNSHG_mappedReb[l][c]->Write();
        if (histNSLG_mapped[l][c]) histNSLG_mapped[l][c]->Write();
        if (histNSCombHG_mapped[l][c]) histNSCombHG_mapped[l][c]->Write();
        if (histNSCombHG_mappedReb[l][c]) histNSCombHG_mappedReb[l][c]->Write();
        if (histNSHGTrig_mapped[l][c]) histNSHGTrig_mapped[l][c]->Write();
        if (fitLandauG_NSHGTrig_mapped[l][c]) fitLandauG_NSHGTrig_mapped[l][c]->Write(Form("LandauGauss_NSHGTrig_L%d_C%02d",l,c));
        if (histNSHGTrig_mappedReb[l][c]) histNSHGTrig_mappedReb[l][c]->Write();
        if (histNSLGTrig_mapped[l][c]) histNSLGTrig_mapped[l][c]->Write();
        if (histNSCombHGTrig_mapped[l][c]) histNSCombHGTrig_mapped[l][c]->Write();
        if (histNSCombHGTrig_mappedReb[l][c]) histNSCombHGTrig_mappedReb[l][c]->Write();
        if (histNSHGTriggNoise_mapped[l][c]) histNSHGTriggNoise_mapped[l][c]->Write();
        if (histNSLGTriggNoise_mapped[l][c]) histNSLGTriggNoise_mapped[l][c]->Write();
      }
    }
    // *******************************************************
    // write summary histograms
    // *******************************************************
    fileOutput->cd();
    stringRunInfo->Write();
    histNChAboveNoise->Write();
    histNTrig->Write();
    histNTrigNoise->Write();
    hist2DNoiseMean_HG->Write();
    histNoiseMean_HG->Write();
    hist2DNoiseSigma_HG->Write();
    histNoiseSigma_HG->Write();
    hist2DNoiseMean_LG->Write();
    histNoiseMean_LG->Write();
    hist2DNoiseSigma_LG->Write();
    histNoiseSigma_LG->Write();
    hist2DLGHG_slope->Write();
    histLGHG_slope->Write();
    hist2DLGHG_offset->Write();
    
    hist1DNoiseSigma_HG->Write();
    hist1DNoiseMean_HG->Write();
    hist1DNoiseSigma_LG->Write();
    hist1DNoiseMean_LG->Write();
    hist1DLGHG_slope->Write();
    hist1DLGHG_offset->Write();
    hist1DHGLG_slope->Write();
    hist1DHGLG_offset->Write();
    hist1DCAEN_NoiseSigma_HG->Write();
    hist1DCAEN_NoiseMean_HG->Write();
    hist1DCAEN_NoiseSigma_LG->Write();
    hist1DCAEN_NoiseMean_LG->Write();
    hist1DCAEN_LGHG_slope->Write();
    hist1DCAEN_LGHG_offset->Write();
    hist1DCAEN_HGLG_slope->Write();
    hist1DCAEN_HGLG_offset->Write();

    gNoiseMeanHG->Write();
    gNoiseSigmaHG->Write();
    gNoiseMeanLG->Write();
    gNoiseSigmaLG->Write();
    gCorrLGHGSlope->Write();
    gCorrLGHGOffset->Write();
    gCorrHGLGSlope->Write();
    gCorrHGLGOffset->Write();
    gCAEN_NoiseMeanHG->Write();
    gCAEN_NoiseSigmaHG->Write();
    gCAEN_NoiseMeanLG->Write();
    gCAEN_NoiseSigmaLG->Write();
    gCAEN_CorrLGHGSlope->Write();
    gCAEN_CorrLGHGOffset->Write();
    gCAEN_CorrHGLGSlope->Write();
    gCAEN_CorrHGLGOffset->Write();

    hist1DMPV_HG->Write();
    hist1DMax_HG->Write();
    hist1DWidth_HG->Write();
    hist1DGWidth_HG->Write();
    hist1DFWHM_HG->Write();
    hist1DCAEN_MPV_HG->Write();
    hist1DCAEN_Max_HG->Write();
    hist1DCAEN_Width_HG->Write();
    hist1DCAEN_GWidth_HG->Write();
    hist1DCAEN_FWHM_HG->Write();

    graphMPV_HG->Write();
    graphMax_HG->Write();
    graphWidth_HG->Write();
    graphGWidth_HG->Write();
    graphFWHM_HG->Write();
    graphCAEN_MPV_HG->Write();
    graphCAEN_Max_HG->Write();
    graphCAEN_Width_HG->Write();
    graphCAEN_GWidth_HG->Write();
    graphCAEN_FWHM_HG->Write();
   
    hist2DMPV_HG->Write();
    hist2DMPVErr_HG->Write();
    hist2DWidth_HG->Write();
    hist2DWidthErr_HG->Write();
    hist2DGWidth_HG->Write();
    hist2DGWidthErr_HG->Write();
    hist2DMax_HG->Write();
    hist2DFWHM_HG->Write();
        
    hist2DNSTrigg->Write();
    hist2DNSTriggNoise->Write();
    hist2DNSTriggEffi->Write();
    
    if (isTBdata){
      hist3DMap->Write();
      hist2DMap->Write();
      hist1DMap->Write();
      histNS3DMap->Write();
      histNS2DMap->Write();
      histNS1DMap->Write();
    }
     // write output file
    fileOutput->Write();
    fileOutput->Close();
    // ********************************************************************************************************
    // DONE
    // ********************************************************************************************************
    
}                                  
                                  
