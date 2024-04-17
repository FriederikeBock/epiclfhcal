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
#include <TSpectrum.h>
#include <TVirtualFitter.h>

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
void makeSinglePhotonSpectraFitsFromJanusTree(  TString fileName     = "", 
                                                TString outputDir    = "ProcessedData/",
                                                Int_t runnumber      = -1,
                                                ULong_t minNEvent    = 0,
                                                ULong_t maxNEvent    = 0,
                                                Int_t verbosity      = 0,
                                                Int_t dataType       = 0,
                                                TString mappingFile  = "",
                                                Bool_t bDetPlot      = kTRUE,
                                                TString runListFileName = "configs/SPS_RunNumbers.txt"
                                              ){
                               
    //*******************************************************
    //********************Running modes:*********************
    // dataType = 0       // local testing and parsing
    // dataType = 1       // PS test beam 2023
    // dataType = 2       // local test setups in small stack
  
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
    Int_t mapping[128][4]                     = {{-1}};      // linear mapping 
    Int_t backwardMapping[9][gMaxLayers]      = {{-1}};     // backward associations channels
    Int_t backwardMappingBoard[9][gMaxLayers] = {{-1}};     // backward associations board
    Bool_t lActive[gMaxLayers]                = {0};        // boolean to check whether layer is active
    Bool_t lActiveCh[9]                       = {0};        // boolean to check whether channel on board is active
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t c = 0; c < 9 ; c++){
        backwardMapping[c][l]       = -1;
        backwardMappingBoard[c][l]  = -1;
        lActiveCh[c]                = kFALSE;
      }
      lActive[l] = kFALSE;
    }
    Int_t nChmapped = 0;
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
        if (verbosity > 0) cout << tempLine.Data() << endl;

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
        
        if (verbosity > 0) std::cout << "-->" << chCAEN << "\t" << layerMod << "\t"<< chBoard << "\t" << rowBoard << "\t" << colBoard << std::endl;
        mapping[chCAEN][0]    = layerMod; 
        mapping[chCAEN][1]    = chBoard; 
        mapping[chCAEN][2]    = rowBoard; 
        mapping[chCAEN][3]    = colBoard; 
        nChmapped++;
        backwardMapping[chBoard][layerMod] = chCAEN;
        backwardMappingBoard[chBoard][layerMod] = ((TString)((TObjString*)tempArr->At(0))->GetString()).Atoi();
        if (verbosity > 0) std::cout << backwardMapping[chBoard][layerMod] << std::endl;
        delete tempArr;
    }
    
    for (Int_t ch = 0; ch< 64; ch++){
      if (verbosity > 0) std::cout  << "channel: " << ch << " location plane: " << mapping[ch][0] << "\t tile: " << mapping[ch][1] << std::endl;
    }
  
  
    Int_t nActiveLayers   = 0;
    Int_t minActiveLayer  = -1;
    Int_t maxActiveLayer  = -1;
    Int_t nActiveRBCh     = 0;
    Int_t minActiveRBCh   = -1;
    Int_t maxActiveRBCh   = -1;

    std::cout << "===================================================================" << std::endl;
    std::cout << "================ Visualization of layers ==========================" << std::endl;
    std::cout << "===================================================================" << std::endl;
    for (Int_t l = 0; l < gMaxLayers; l++){
        for (Int_t c = 1; c < 9; c++){
            std::cout << backwardMapping[c][l] << "\t || \t" ;
            if (lActive[l] == kFALSE ){
                if ( backwardMapping[c][l] != -1 ) lActive[l] = kTRUE;
            }
            if (lActiveCh[c] == kFALSE ){
              if ( backwardMapping[c][l] != -1 ) lActiveCh[c] = kTRUE;
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
    // count total number of active channel according to mapping file and determine maximum RBchannel
    for (Int_t c = 0; c<9; c++){
      if (lActiveCh[c]){
        nActiveRBCh++; 
        maxActiveRBCh = c;
        if (minActiveRBCh == -1) minActiveRBCh = c;
      }
    }
    std::cout << "There are " << nActiveLayers << " active layers. The lowest layer is " << minActiveLayer << "  and the highest layer is " << maxActiveLayer << std::endl;
    std::cout << "There are " << nActiveRBCh << " active read-outboard channels. The lowest channel is " << minActiveRBCh << "  and the highest channel is " << maxActiveRBCh << std::endl;
    
    // ********************************************************************************************************
    // RAW data monitoring histos per CAEN board and channel
    // ********************************************************************************************************    
    TH1D* histLG[gMaxBoard][gMaxChannels];                      // low gain all triggers
    TH1D* histHG[gMaxBoard][gMaxChannels];                      // high gain all triggers
    TH2D* hist_T_HG[gMaxBoard][gMaxChannels];                   // time dependent high gain all triggers
    TH2D* histLGHG[gMaxBoard][gMaxChannels];                    // LG - HG correlation
    TH2D* histHGLG[gMaxBoard][gMaxChannels];                    // HG - LG correlation
    //***************************************************
    // mapped channels - layer & read-out board channels
    //***************************************************
    // channel counting in layer starts with 1, easier acces make array go to 8
    TH1D* histLG_mapped[gMaxLayers][9];                         // low gain all triggers
    TH1D* histHG_mapped[gMaxLayers][9];                         // high gain all triggers
    
    //***************************************************
    // map of triggers above threshold
    //***************************************************
    TH3D* hist3DMap = new TH3D("h_map_z_x_y", "; layer; col; row", 14, -0.5, 13.5, 4, -0.5, 3.5, 2, -0.5, 1.5);
    TH2D* hist2DMap = new TH2D("h_map_z_channel", "; channel; layer", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH1D* hist1DMap = new TH1D("h_map_channel", "; channel", 8, 0.5, 8.5);
    
    //***************************************************
    TH1D* histNChAboveNoise   = new TH1D("h_NchannelAboveNoise", "; N_{channels}; counts", (gMaxBoard*gMaxChannels+1), -0.5, (gMaxBoard*gMaxChannels-0.5));
    
    //***************************************************
    // Init histograms
    //***************************************************
    for (Int_t j = 0; j < gMaxBoard; j++){
      for (Int_t i = 0; i < gMaxChannels; i++){
        hist_T_HG[j][i]    = new TH2D(Form("h_T_HG_B%d_C%02d",j,i),"; t (min); HG (adc); counts",2000,0,100,4002,-1,2000);
        histHG[j][i]    = new TH1D(Form("h_HG_B%d_C%02d",j,i),"; HG (adc); counts",4201,-200,4001);
        histLG[j][i]    = new TH1D(Form("h_LG_B%d_C%02d",j,i),"; LG (adc); counts",4201,-200,4001);
        histLGHG[j][i]  = new TH2D(Form("h_LGHG_B%d_C%02d",j,i),"; LG (adc); HG (adc)",4200/5,-200,4001,4200/5,-200,4001);
        histHGLG[j][i]  = new TH2D(Form("h_HGLG_B%d_C%02d",j,i),"; LG (adc); HG (adc)",4200/5,-200,4001,4200/5,-200,4001);
      }
    }
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t c = 0; c < 9; c++){
        histLG_mapped[l][c]             = nullptr;
        histHG_mapped[l][c]             = nullptr; 
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
        if(i>0 && nEntriesTree>100 && i%(nEntriesTree/(20))==0) std::cout << "//processed " << 100*(i)/nEntriesTree << "%"  << std::endl;
        if(verbosity>1){
          std::cout << "***********************************************************************************************************" << std::endl;
          std::cout << "event " << i << std::endl;
          std::cout << "***********************************************************************************************************" << std::endl;
        }
        // temporary mapping of channels
        Int_t signal[2][64][3] = {{{0}}}; // [board][channel][HG, LG, trigg HG]
        // readEvent
        if (verbosity > 1)std::cout << "------------- Event -------------------------" << std::endl;
        if (verbosity > 1)std::cout << gTrID << "\t" << gTRtimeStamp << std::endl;
        for(Int_t ch=0; ch<gMaxChannels && ch < nChmapped ; ch++){   
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
          if (verbosity > 1)std::cout << "--> board: "<< gBoard[ch] << "\t ch:"<< gChannel[ch] << "\t LG:" << gLG[ch] << "\t HG:" << gHG[ch] << std::endl;
          if (gHG[ch] > adcThreshold){
            nChNoNoise++;
            if (verbosity > 1) std::cout << "not noise" << std::endl;
            hist3DMap->Fill(mapping[gChannel[ch]][0],mapping[gChannel[ch]][3],mapping[gChannel[ch]][2]);
            hist2DMap->Fill(mapping[gChannel[ch]][1],mapping[gChannel[ch]][0]);
            hist1DMap->Fill(mapping[gChannel[ch]][1]);
          }
        }
        // fill channels above noise
        histNChAboveNoise->Fill(nChNoNoise);
        if (verbosity > 1) std::cout << "Channels above noise: "<< nChNoNoise << std::endl;        
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

        if (! ((histHG[j][i] && histHG[j][i]->GetEntries() > 0)|| (histLG[j][i] && histLG[j][i]->GetEntries() > 0)) ){
          fitGausLG_BG[j][i]  = nullptr;
          fitGausHG_BG[j][i]  = nullptr;
          fitLGHGCorr[j][i]   = nullptr;
          fitHGLGCorr[j][i]   = nullptr;
          continue;
        }
        Int_t chMap     = j*64 + i;
        Int_t chBoard   = mapping[chMap][1];
        Int_t layer     = mapping[chMap][0];
        Int_t channelBin1D = hist1DNoiseSigma_HG->FindBin(layer*10+chBoard);          
          
        if (verbosity > 0)std::cout << j << "\t" << i << "\t" << chMap << "\t L: " << layer  << "\t C:" <<  chBoard << "\t bin ID "<< channelBin1D << std::endl;
        
        // ********************************************************
        // map raw and trigger histos from CAEN numbering to 
        // physical layers & channels
        // ********************************************************
        histHG_mapped[layer][chBoard] = (TH1D*)histHG[j][i]->Clone(Form("h_HG_mapped_L%d_C%02d",layer,chBoard));
        histLG_mapped[layer][chBoard] = (TH1D*)histLG[j][i]->Clone(Form("h_LG_mapped_L%d_C%02d",layer,chBoard));
        // ********************************************************
        // Fit & plot noise histos
        // -> map noise histos from CAEN numb. -> physical
        // -> fill monitoring plots
        // ********************************************************
        Bool_t bFit = kFALSE;
        // HG noise fits
        bFit = FitNoiseWithBG (histHG[j][i], fitGausHG_BG[j][i], mean[0][j][i], mean[1][j][i], sigma[0][j][i], sigma[1][j][i], j, i, "f_GaussBG_HG", "HG", verbosity);
        if (bFit && bDetPlot) PlotNoiseSingle (canvas1DNoise, histHG[j][i], fitGausHG_BG[j][i], mean[0][j][i], mean[1][j][i], sigma[0][j][i], sigma[1][j][i], j, i, layer, chBoard, 
                                                Form("%s/HG_NoiseWithFit", outputDirPlotsDet.Data()), currentRunInfo, 0.04);
        if (bFit){
          fitGausHG_BG_mapped[layer][chBoard] = (TF1*)fitGausHG_BG[j][i]->Clone(Form("f_GaussBG_HG_mapped_L%d_C%02d",layer,chBoard));
        }
        if (bFit == kFALSE) fitGausHG_BG[j][i] = nullptr;
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
        bFit = FitNoiseWithBG (histLG[j][i], fitGausLG_BG[j][i], mean[2][j][i], mean[3][j][i], sigma[2][j][i], sigma[3][j][i], j, i, "f_GaussBG_LG", "LG");
        if (bFit && bDetPlot) PlotNoiseSingle (canvas1DNoise, histLG[j][i], fitGausLG_BG[j][i], mean[2][j][i], mean[3][j][i], sigma[2][j][i], sigma[3][j][i], j, i, layer, chBoard, 
                                                Form("%s/LG_NoiseWithFit", outputDirPlotsDet.Data()), currentRunInfo, 0.04);
        if (bFit){
          fitGausLG_BG_mapped[layer][chBoard] = (TF1*)fitGausLG_BG[j][i]->Clone(Form("f_GaussBG_LG_mapped_L%d_C%02d",layer,chBoard));
        }
        if (bFit == kFALSE) fitGausLG_BG[j][i] = nullptr;
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
        if (hist_T_HG[j][i]  && hist_T_HG[j][i]->GetEntries() > 0 && bDetPlot){
          canvas2DCorr->cd();
            SetStyleHistoTH2ForGraphs( hist_T_HG[j][i], hist_T_HG[j][i]->GetXaxis()->GetTitle(), hist_T_HG[j][i]->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 1.25);  
            // find max x bin
            hist_T_HG[j][i]->GetXaxis()->SetRangeUser(0,tdiff+0.1);
            hist_T_HG[j][i]->Draw("colz");
            
            TLatex *labelChannel                     = new TLatex(0.85,0.92,Form("CAEN: B %d, C %d, Stack: L %d, C%d",j, i, layer, chBoard));
            SetStyleTLatex( labelChannel, textSizeRel,4,1,42,kTRUE,31);
            labelChannel->Draw();

          canvas2DCorr->SaveAs(Form("%s/T_HG_B%d_C%02d.pdf", outputDirPlotsDet.Data(), j,i));
          delete labelChannel;
        }
        // ********************************************************
        // Fit & Plot 2D LG vs HG
        // ********************************************************
        if (histLGHG[j][i] && histLGHG[j][i]->GetEntries() > 0){
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
        } else {
          fitLGHGCorr[j][i] = nullptr;
        }
        if (histHGLG[j][i] && histLGHG[j][i]->GetEntries() > 0){
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
        } else {
          fitHGLGCorr[j][i] = nullptr;
        }        
        
        // ********************************************************
        // Plot different triggers together
        // ********************************************************
        Int_t hgmax = 1050;
        if (bDetPlot)PlotOverlayDiffTriggers( canvas1DDiffTrigg, histHG[j][i], nullptr, nullptr, fitGausHG_BG[j][i],
                                              0, hgmax, Form("%s/HG_DiffTriggers", outputDirPlotsDet.Data()),
                                              j, i, layer, chBoard, currentRunInfo, 0.04);
        if (bDetPlot)PlotOverlayDiffTriggers( canvas1DDiffTrigg, histLG[j][i], nullptr, nullptr, fitGausLG_BG[j][i],
                                              0, 1050, Form("%s/LG_DiffTriggers", outputDirPlotsDet.Data()),
                                              j, i, layer, chBoard, currentRunInfo, 0.04);
      }
    }
    
    for (Int_t l = 0; l < gMaxLayers; l++){
      if (!lActive[l]) continue;      
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                histHG_mapped[l], nullptr, nullptr, fitGausHG_BG_mapped[l], 
                                0, 1000, 1.2, l , Form("%s/TriggerOverlay_HG_Zoomed_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                histLG_mapped[l], nullptr, nullptr, fitGausLG_BG_mapped[l], 
                                0, 800, 1.2, l , Form("%s/TriggerOverlay_LG_Zoomed_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
    // ********************************************************************************************************
    // Overlay in same layer
    // ********************************************************************************************************
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histHG_mapped[l], 1, 9,
                                  -100, 1000, 1./5, Form("%s/HG", outputDirPlots.Data()), l, currentRunInfo, 0.04,"hist");
      PlotChannelOverlaySameLayerWithFitsBG( canvas1DDiffTrigg, histHG_mapped[l], fitGausHG_BG_mapped[l], 1, 9,
                                  -100, 1000, 1./5, Form("%s/HGWithFits", outputDirPlots.Data()), l, currentRunInfo, 0.04,"hist");
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histLG_mapped[l], 1, 9,
                                  -100, 500, 1, Form("%s/LG", outputDirPlots.Data()), l, currentRunInfo, 0.04);        
    
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
    TH1D* histNSHG_BG[gMaxBoard][gMaxChannels];             // high gain all triggers backgrounds
    TH1D* histNSHG_Sub[gMaxBoard][gMaxChannels];            // high gain all triggers BG sub
    TH2D* histNSLGHG[gMaxBoard][gMaxChannels];              // LG vs HG correlation
    TH2D* histNSHGLG[gMaxBoard][gMaxChannels];              // HG vs LG correlation
    TSpectrum* spectrumFitter[gMaxBoard][gMaxChannels];     // HG spectrum fittergHeader
    TF1* fitMultGaussNSLG[gMaxBoard][gMaxChannels];         // fit HG multi gauss
    
    //***************************************************
    // mapped channels - layer & read-out board channels
    //***************************************************
    // channel counting in layer starts with 1, easier acces make array go to 8
    TH1D* histNSLG_mapped[gMaxLayers][9];                   // low gain all triggers
    TH1D* histNSHG_mapped[gMaxLayers][9];                   // high gain all triggers
    TH1D* histNSHG_BG_mapped[gMaxLayers][9];                // high gain all triggers background mapped
    TH1D* histNSHG_Sub_mapped[gMaxLayers][9];                // high gain all triggers background sub
    
    //***************************************************
    // mapped channels - layer & read-out board channels - rebinned
    //***************************************************
    // channel counting in layer starts with 1, easier acces make array go to 8
    TH1D* histNSHG_mappedReb[gMaxLayers][9];                   // high gain all triggers
     
    //***************************************************
    // map of triggers above threshold  
    //***************************************************
    TH3D* histNS3DMap = new TH3D("h_map_NS_z_x_y", "; layer; col; row", 14, -0.5, 13.5, 4, -0.5, 3.5, 2, -0.5, 1.5);
    TH2D* histNS2DMap = new TH2D("h_map_NS_z_channel", "; channel; layer", 8, 0.5, 8.5, 14, -0.5, 13.5);
    TH1D* histNS1DMap = new TH1D("h_map_NS_channel", "; channel", 8, 0.5, 8.5);
    //***************************************************
    // Init histograms
    //***************************************************    
    for (Int_t j = 0; j < gMaxBoard; j++){
      for (Int_t i = 0; i < gMaxChannels; i++){
        histNSHG[j][i]    = new TH1D(Form("h_NS_HG_B%d_C%02d",j,i),";HG (adc); counts",4201,-200,4001);
        histNSLG[j][i]    = new TH1D(Form("h_NS_LG_B%d_C%02d",j,i),";LG (adc); counts",4201,-200,4001);
        histNSLGHG[j][i]  = new TH2D(Form("h_NS_LGHG_B%d_C%02d",j,i),";LG (adc); HG (adc)",420,-200,4001,420,-200,4001);
        histNSHGLG[j][i]  = new TH2D(Form("h_NS_HGLG_B%d_C%02d",j,i),";HG (adc); LG (adc)",420,-200,4001,420,-200,4001);
        spectrumFitter[j][i]  = new TSpectrum();
        histNSHG_BG[j][i] = nullptr;
      }
    }
    for (Int_t l = 0; l < gMaxLayers; l++){
      for (Int_t c = 0; c < 9; c++){
        histNSLG_mapped[l][c]             = nullptr;
        histNSHG_mapped[l][c]             = nullptr; 
        histNSHG_BG_mapped[l][c]          = nullptr; 
        histNSHG_mappedReb[l][c]          = nullptr;
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
    TRandom3* rand    = new TRandom3();
    nEventsProcessed  = 0;
    for (Long64_t i=startEvent; i<nEntriesTree;i++) {
        // load current event
        tt_event->GetEntry(i);
        nEventsProcessed++;

        Int_t nChNoNoise = 0;
        // processing progress info
        if(i>0 && nEntriesTree>100 && i%(nEntriesTree/(20))==0) std::cout << "//processed " << 100*(i)/nEntriesTree << "%"  << std::endl;
        if(verbosity>1){
          std::cout << "***********************************************************************************************************" << std::endl;
          std::cout << "event " << i << std::endl;
          std::cout << "***********************************************************************************************************" << std::endl;
        }
        // temporary mapping of channels after noise subtractions
        Int_t signal[2][64][4] = {{{0}}};         // [board][channel][HG, LG, trigg HG,comb HG&LG]
        Float_t signalNS[2][64][4] = {{{0}}};       // [board][channel][HG, LG, trigg HG,comb HG&LG]
        Double_t summed[2] = {0};                 // total channel sum
        // readEvent
        if (verbosity > 1)std::cout << "------------- Event -------------------------" << std::endl;
        if (verbosity > 1)std::cout << gTrID << "\t" << gTRtimeStamp << std::endl;
        if (i == startEvent) tstapMin = gTRtimeStamp;
        tstapMax = gTRtimeStamp;
        Double_t tCurr= (tstapMax-tstapMin)/1e6/60; // elapsed time in min
        
        for(Int_t ch=0; ch<gMaxChannels && ch < nChmapped; ch++){
          Double_t meanNHG = 0;
          if (mean[0][gBoard[ch]][gChannel[ch]] != -1)
            meanNHG = mean[0][gBoard[ch]][gChannel[ch]];
          Double_t meanNLG = 0;
          if (mean[2][gBoard[ch]][gChannel[ch]] != -1)
            meanNLG = mean[2][gBoard[ch]][gChannel[ch]];

          if (i == 0){
            std::cout << ch << "\t means: " <<  meanNHG << "\t" << meanNLG << std::endl;
          }
          
          signal[gBoard[ch]][gChannel[ch]][0] = gHG[ch];
          signal[gBoard[ch]][gChannel[ch]][1] = gLG[ch];
          
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
          histNSLGHG[gBoard[ch]][gChannel[ch]]->Fill(signalNS[gBoard[ch]][gChannel[ch]][1], signalNS[gBoard[ch]][gChannel[ch]][0]); 
          histNSHGLG[gBoard[ch]][gChannel[ch]]->Fill(signalNS[gBoard[ch]][gChannel[ch]][0], signalNS[gBoard[ch]][gChannel[ch]][1]); 
          if (signalNS[gBoard[ch]][gChannel[ch]][0] > (adcThreshold-meanNHG)){
            nChNoNoise++;
            if (verbosity > 1) std::cout << "not noise" << std::endl;
            histNS3DMap->Fill(mapping[gChannel[ch]][0],mapping[gChannel[ch]][3],mapping[gChannel[ch]][2]);
            histNS2DMap->Fill(mapping[gChannel[ch]][1],mapping[gChannel[ch]][0]);
            histNS1DMap->Fill(mapping[gChannel[ch]][1]);
          }
          // calculate summed signal
          summed[gBoard[ch]] = summed[gBoard[ch]]+signalNS[gBoard[ch]][gChannel[ch]][3]; 
        }
        // fill summed signal
        if (verbosity > 1)std::cout << "Channels above noise: "<< nChNoNoise << std::endl;        
    }
    std::cout << "Total events processed:" << nEventsProcessed << std::endl;

    // ********************************************************************************************************
    // Plotting of single channels after noise subtraction
    // -> remapping of CAEN board+channels to physicsal layers and readout-channels
    // ********************************************************************************************************    
    for (Int_t j = 0; j < gMaxBoard; j++){
      for (Int_t i = 0; i < gMaxChannels; i++){
        if (! ((histNSHG[j][i] && histNSHG[j][i]->GetEntries() > 0)|| (histNSLG[j][i] && histNSLG[j][i]->GetEntries() > 0)) ){
          fitMultGaussNSLG[j][i] = nullptr;
          continue;
        }
        Int_t chMap     = j*64 + i;
        Int_t chBoard   = mapping[chMap][1];
        Int_t layer     = mapping[chMap][0];
        if (verbosity > 0)std::cout << j << "\t" << i << "\t" << chMap << "\t L: " << layer  << "\t C:" <<  chBoard << std::endl;
        
        
        histNSHG_BG[j][i]                   = (TH1D*)spectrumFitter[j][i]->Background(histNSHG[j][i], 20, "smoothing");
        histNSHG_BG_mapped[layer][chBoard]  = (TH1D*)histNSHG_BG[j][i]->Clone(Form("h_NS_HG_BG_mapped_L%d_C%02d",layer,chBoard));
        histNSHG_Sub[j][i]                  = (TH1D*)histNSHG[j][i]->Clone(Form("h_NS_HG_Sub_B%d_C%02d",j,i));
        histNSHG_Sub[j][i]->Sumw2();
        histNSHG_Sub[j][i]->Add(histNSHG_BG[j][i], -1);
        histNSHG_Sub_mapped[layer][chBoard]  = (TH1D*)histNSHG_Sub[j][i]->Clone(Form("h_NS_HG_Sub_mapped_L%d_C%02d",layer,chBoard));
        
        Int_t nfound = spectrumFitter[j][i]->Search(histNSHG_Sub_mapped[layer][chBoard], 7., "", 0.05); // Search for peaks
        if (verbosity > 0) std::cout << "found " << nfound << "\t different peaks in spectrum" << std::endl;
        
        if (nfound > 2){
          nPeaksMultGauss = 0;
          Double_t *xpeaks;
          Double_t par[3000];
          xpeaks = spectrumFitter[j][i]->GetPositionX();
          
          // invert order of peaks
          std::sort(xpeaks, xpeaks + nfound, [&](Double_t a, Double_t b) {
              return a > b;
          });
          
          std::cout << "Differences between consecutive entries:" << std::endl;
          Double_t sum_diff = 0.0;
          for (size_t f = 1; f < (size_t)nfound-1; ++f) {
            Double_t diff = xpeaks[f - 1] - xpeaks[f];
            std::cout << diff << " ";
            sum_diff += diff;
          }
          std::cout << std::endl;
          
          Double_t average_diff = sum_diff / (nfound - 1);
          std::cout << "Average of differences: " << average_diff << std::endl;
          par[0]  = 0;
          par[1]  = 0;
          for (Int_t p = 0; p < (Int_t)nfound; p++){
            Double_t xp   = xpeaks[p];

            Int_t bin     = histNSHG_Sub[j][i]->GetXaxis()->FindBin(xp);
            Double_t yp   = histNSHG_Sub[j][i]->GetBinContent(bin);
            std::cout << p << "\t" << xp << "\t" << bin << "\t"<< yp << std::endl;            
            par[3 * nPeaksMultGauss + 2] = yp;     // "height"
            par[3 * nPeaksMultGauss + 3] = xp; // "mean"
            par[3 * nPeaksMultGauss + 4] = 3;  // "sigma"
            
            std::cout<<xp<<std::endl;
            nPeaksMultGauss++;
          }
          
          fitMultGaussNSLG[j][i] = new TF1(Form("fitMultGauss_NS_HG_Sub_B%d_C%02d",j,i), multGauss, 0, 4096, 3 * nPeaksMultGauss);
          fitMultGaussNSLG[j][i]->SetParameters(par);
          fitMultGaussNSLG[j][i]->SetNpx(1000);
          for (Int_t n = 0; n < fitMultGaussNSLG[j][i]->GetNpar(); n++){
              std::cout << n << "\t" << fitMultGaussNSLG[j][i]->GetParameter(n) << std::endl;
          }
          histNSHG_Sub[j][i]->Fit(fitMultGaussNSLG[j][i]);
        
        } else {
          std::cout << "no peaks from different pixels found" << std::endl;
          fitMultGaussNSLG[j][i] = nullptr;
        }
        
        
        histNSHG_mapped[layer][chBoard] = (TH1D*)histNSHG[j][i]->Clone(Form("h_NS_HG_mapped_L%d_C%02d",layer,chBoard));
        histNSLG_mapped[layer][chBoard] = (TH1D*)histNSLG[j][i]->Clone(Form("h_NS_LG_mapped_L%d_C%02d",layer,chBoard));
        
        histNSHG_mapped[layer][chBoard]->Sumw2();
        histNSHG_mappedReb[layer][chBoard] = (TH1D*)histNSHG_mapped[layer][chBoard]->Rebin(2100,Form("%sReb",(histNSHG_mapped[layer][chBoard]->GetName())), binningADC);
        histNSHG_mappedReb[layer][chBoard]->Scale(1,"width");
      
        if (bDetPlot){
          // ********************************************************
          // Plot different triggers together
          // ********************{************************************
          PlotOverlayDiffTriggers( canvas1DDiffTrigg, histNSHG[j][i], nullptr, nullptr, NULL,
                                -100, 1100, Form("%s/HG_NS_DiffTriggers", outputDirPlotsDet.Data()),
                                j, i, layer, chBoard, currentRunInfo, 0.04);
          PlotOverlayDiffTriggers( canvas1DDiffTrigg, histNSLG[j][i], nullptr, nullptr, NULL,
                                -100, 500, Form("%s/LG_NS_DiffTriggers", outputDirPlotsDet.Data()),
                                j, i, layer, chBoard, currentRunInfo, 0.04);
          PlotOverlaySinglePhoton( canvas1DDiffTrigg, histNSHG[j][i], histNSHG_BG[j][i], histNSHG_Sub[j][i],  fitMultGaussNSLG[j][i],
                                -100, 1100, Form("%s/HG_NS_WithSpectrumBG_DiffTriggers", outputDirPlotsDet.Data()),
                                j, i, layer, chBoard, currentRunInfo, 0.04);
        }
      }
    }

    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DNoiseMean_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HG_Noise_Mean.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DNoiseSigma_HG, maxActiveLayer, maxActiveRBCh, textSizeRel,Form("%s/HG_Noise_Sigma.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DNoiseMean_LG, maxActiveLayer, maxActiveRBCh, textSizeRel,Form("%s/LG_Noise_Mean.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DNoiseSigma_LG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/LG_Noise_Sigma.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DLGHG_slope, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/LGHG_Slope.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DLGHG_offset, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/LGHG_Offset.pdf", outputDirPlots.Data()), currentRunInfo, kTRUE);
    
    for (Int_t l = 0; l < gMaxLayers; l++){
      if (!lActive[l]) continue; 
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                histNSHG_mapped[l], nullptr, nullptr, nullptr, 
                                -80, 1000, 1.2, l , Form("%s/TriggerOverlay_HG_NS_Zoomed_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
      PlotDiffTriggersFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                histNSLG_mapped[l], nullptr, nullptr, nullptr, 
                                -80, 500, 1.2, l , Form("%s/TriggerOverlay_LG_NS_Zoomed_Layer%02d.pdf" ,outputDirPlots.Data(), l), currentRunInfo);
    }
    
    // ********************************************************************************************************
    // Overlay noise in same layer
    // ********************************************************************************************************
    for (Int_t l = 0; l < gMaxLayers; l++){
      if (!lActive[l]) continue;      
//       for (Int_t c = 1; c < 9; c++){
//         
//         PlotMIPSingle (canvas1DNoise, histNSHG_mappedReb[l][c],nullptr, chisqr[l][c], ndf[l][c], maxLandG[l][c], fwhmLandG[l][c],
//                         l, c, Form("%s/HG_TriggMipWithFit", outputDirPlotsDet.Data()), currentRunInfo, 0.04);
//       }
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSHG_mappedReb[l], 1, 9,
                                  -100, 1000, 1./5, Form("%s/HGNS", outputDirPlots.Data()), l, currentRunInfo, 0.04,"hist");
      PlotChannelOverlaySameLayer( canvas1DDiffTrigg, histNSLG_mapped[l], 1, 9,
                                  -100, 500, 1, Form("%s/LGNS", outputDirPlots.Data()), l, currentRunInfo, 0.04);        
    }
    
    // ********************************************************************************************************
    // Overlay noise for same readout channel
    // ********************************************************************************************************
    for (Int_t c = 1; c < 9; c++){      
      PlotChannelOverlaySameReadoutChannel( canvas1DDiffTrigg, histNSHG_mappedReb, lActive, 0, gMaxLayers, 
                                            -100,1000, 1./5, Form("%s/HGNS", outputDirPlots.Data()), c, currentRunInfo,0.04,"hist");    
    }

    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DMPV_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HGTrigg_MPV.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DMPVErr_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HGTrigg_MPVErr.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DMax_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HGTrigg_Max.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DFWHM_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HGTrigg_FWHM.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DWidth_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HGTrigg_Width.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DWidthErr_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HGTrigg_WidthErr.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DGWidth_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HGTrigg_GWidth.pdf", outputDirPlots.Data()), currentRunInfo);
    PlotSimpleMultiLayer2D( canvas2DCorr, hist2DGWidthErr_HG, maxActiveLayer, maxActiveRBCh, textSizeRel, Form("%s/HGTrigg_GWidthErr.pdf", outputDirPlots.Data()), currentRunInfo);
       
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
    stringRunInfo->SetString(GetStringFromRunInfo(currentRunInfo,4));
    
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
        WriteOnlyIfFilled(histHG[j][i]);
        WriteOnlyIfFilled(hist_T_HG[j][i]);
        WriteOnlyIfFilled(histLG[j][i]);
        // noise subtracted
        fileOutput->cd("NoiseSubtracted");
        WriteOnlyIfFilled(histNSHG[j][i]);
        WriteOnlyIfFilled(histNSLG[j][i]);
        if (fitMultGaussNSLG[j][i]) fitMultGaussNSLG[j][i]->Write();
        // correlation histograms
        fileOutput->cd("CorrelationLGHG");
        WriteOnlyIfFilled(histLGHG[j][i]);
        WriteOnlyIfFilled(histNSLGHG[j][i]);
        if (fitLGHGCorr[j][i]) fitLGHGCorr[j][i]->Write();
        fileOutput->cd("CorrelationHGLG");
        WriteOnlyIfFilled(histHGLG[j][i]);
        WriteOnlyIfFilled(histNSHGLG[j][i]);
        if (fitHGLGCorr[j][i]) fitHGLGCorr[j][i]->Write();
      }
      fileOutput->cd();
    }
    std::cout << __LINE__ << std::endl;
    // *******************************************************
    // run over all physical layers and readout board channels
    // *******************************************************
    for (Int_t l = 0; l  < gMaxLayers; l++){
      for (Int_t c = 1; c < 9; c++){
        fileOutput->cd("RawDataMapped");
        WriteOnlyIfFilled(histHG_mapped[l][c]);
        if (fitGausHG_BG_mapped[l][c]) fitGausHG_BG_mapped[l][c]->Write();
        WriteOnlyIfFilled(histLG_mapped[l][c]);
        if (fitGausLG_BG_mapped[l][c]) fitGausLG_BG_mapped[l][c]->Write();
        fileOutput->cd("NoiseSubtractedMapped");
        WriteOnlyIfFilled(histNSHG_mapped[l][c]);
        WriteOnlyIfFilled(histNSHG_mappedReb[l][c]);
        WriteOnlyIfFilled(histNSLG_mapped[l][c]);
      }
    }
    // *******************************************************
    // write summary histograms
    // *******************************************************
    fileOutput->cd();
    stringRunInfo->Write();
    histNChAboveNoise->Write();
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
        
    hist3DMap->Write();
    hist2DMap->Write();
    hist1DMap->Write();
    histNS3DMap->Write();
    histNS2DMap->Write();
    histNS1DMap->Write();
     // write output file
    fileOutput->Write();
    fileOutput->Close();
    // ********************************************************************************************************
    // DONE
    // ********************************************************************************************************

}                                  
                                  
  
