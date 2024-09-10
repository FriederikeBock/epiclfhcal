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

struct layerInfo{
    layerInfo(): layerNrAbs(0), layerLabel(""), rUnit(0), layerUnit(0){}
    int layerNrAbs;       // absolute layer number
    TString layerLabel;   // assembly name
    int rUnit;            // readout unit number
    int layerUnit;        // layer within readout unit
} ;

struct unitInfo{
    unitInfo(): chRU(0), layerUnit(0), chAssembly(0){}
    int chRU;         //channel read-out unit
    int layerUnit;    // layer within readout unit
    int chAssembly;   // channel within one assembly
} ;

struct channelInfo{
    channelInfo(): chRU(0), rUnit(0), layer(0), layerLabel(""), chAssembly(0), rowAssembly(0), colAssembly(0), posX(0.), posY(0.), posZ(0.){}
    int chRU;
    int rUnit;
    int layer;
    TString layerLabel;
    int chAssembly;
    int rowAssembly;
    int colAssembly;
    float posX;
    float posY;
    float posZ;
};

//__________________________________________________________________________________________________________
//__________________________________________________________________________________________________________
//__________________________________________________________________________________________________________
Int_t findChannelInUnit(std::vector<unitInfo> lUnit, int layer, int chA ){
    Int_t currEntry = 0;
    // cout << "searching for: " << layer << "\t" << chA << endl;
    // while (lUnit.at(currEntry).layerUnit != layer && lUnit.at(currEntry).chAssembly != chA &&  currEntry < (Int_t)lUnit.size()){
    //   cout << "-> "<< lUnit.at(currEntry).chAssembly << "\t" << lUnit.at(currEntry).layerUnit << endl;
    //   currEntry++;
    // }
    Bool_t found = kFALSE;
    while (!found && currEntry < (Int_t)lUnit.size()){
      if (lUnit.at(currEntry).layerUnit == layer && lUnit.at(currEntry).chAssembly == chA){
        // cout << "-> "<< lUnit.at(currEntry).layerUnit<< "\t" << lUnit.at(currEntry).chAssembly  << endl;
        found = kTRUE;
      } else {
        // cout << currEntry << "/" << lUnit.size() << endl;
        if (currEntry < (Int_t)lUnit.size()) currEntry++;
      }
    }
    
    if (currEntry == (Int_t)lUnit.size()) return -1;
    else{
      // cout << "correct: "<< lUnit.at(currEntry).layerUnit<< "\t" << lUnit.at(currEntry).chAssembly  << endl;
      return lUnit.at(currEntry).chRU;
    }
}


//__________________________________________________________________________________________________________
//__________________________________________________________________________________________________________
//__________________________________________________________________________________________________________
Int_t ReturnRowBoard(Int_t ch){
    if (ch < 5) return 1;
    else return 0;
}
Int_t ReturnColumnBoard(Int_t ch){
    if (ch == 1 || ch == 8) return 0;
    if (ch == 2 || ch == 7) return 1;
    if (ch == 3 || ch == 6) return 2;
    if (ch == 4 || ch == 5) return 3;
    return -1;
}


void CreateMapping(   TString filenameUnitMapping,
                      TString filenameLayerMapping,
                      int debug = 0
                  ){

    // ********************************************************************************************************    
    // read folder and name from file
    // ********************************************************************************************************
    std::vector<layerInfo> layers;
    std::vector<unitInfo> uChannels;
    ifstream inUnit;
    inUnit.open(filenameUnitMapping,ios_base::in);
    if (!inUnit) {
        std::cout << "ERROR: file " << filenameUnitMapping.Data() << " not found!" << std::endl;
        return;
    }

    ifstream inLayerMap;
    inLayerMap.open(filenameLayerMapping,ios_base::in);
    if (!inLayerMap) {
        std::cout << "ERROR: file " << filenameLayerMapping.Data() << " not found!" << std::endl;
        return;
    }

    
    for( TString tempLine; tempLine.ReadLine(inUnit, kTRUE); ) {
        // check if line should be considered
        if (tempLine.BeginsWith("%") || tempLine.BeginsWith("#")){
            continue;
        }
        if (debug > 1) std::cout << tempLine.Data() << std::endl;

        // Separate the string according to tabulators
        TObjArray *tempArr  = tempLine.Tokenize("\t");
        if(tempArr->GetEntries()<1){
            if (debug > 1) std::cout << "nothing to be done" << std::endl;
            delete tempArr;
            continue;
        } else if (tempArr->GetEntries() == 1 ){
            // Separate the string according to space
            tempArr       = tempLine.Tokenize(" ");
            if(tempArr->GetEntries()<1){
                if (debug > 1) std::cout << "nothing to be done" << std::endl;
                delete tempArr;
                continue;
            } else if (tempArr->GetEntries() == 1  ) {
                if (debug > 1) std::cout << ((TString)((TObjString*)tempArr->At(0))->GetString()).Data() << " has not been reset, no value given!" << std::endl;
                delete tempArr;
                continue;
            }
        }

        // Put them to the correct variables    
        unitInfo tempUCh;
        tempUCh.chRU        = ((TString)((TObjString*)tempArr->At(0))->GetString()).Atoi();
        tempUCh.layerUnit   = ((TString)((TObjString*)tempArr->At(1))->GetString()).Atoi();;
        tempUCh.chAssembly  = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atoi();
            
        if (debug > 0) std::cout << "channel readout unit " << tempUCh.chRU << "\t layer in unit: " << tempUCh.layerUnit << "\t channel within assembly: "  << tempUCh.chAssembly  << std::endl;
        uChannels.push_back(tempUCh);
    }

    for( TString tempLine; tempLine.ReadLine(inLayerMap, kTRUE); ) {
        // check if line should be considered
        if (tempLine.BeginsWith("%") || tempLine.BeginsWith("#")){
            continue;
        }
        if (debug > 1) std::cout << tempLine.Data() << std::endl;

        // Separate the string according to tabulators
        TObjArray *tempArr  = tempLine.Tokenize("\t");
        if(tempArr->GetEntries()<1){
            if (debug > 1) std::cout << "nothing to be done" << std::endl;
            delete tempArr;
            continue;
        } else if (tempArr->GetEntries() == 1 ){
            // Separate the string according to space
            tempArr       = tempLine.Tokenize(" ");
            if(tempArr->GetEntries()<1){
                if (debug > 1) std::cout << "nothing to be done" << std::endl;
                delete tempArr;
                continue;
            } else if (tempArr->GetEntries() == 1  ) {
                if (debug > 1) std::cout << ((TString)((TObjString*)tempArr->At(0))->GetString()).Data() << " has not been reset, no value given!" << std::endl;
                delete tempArr;
                continue;
            }
        }

        // Put them to the correct variables    
        layerInfo tempLayer;
        tempLayer.layerNrAbs    = ((TString)((TObjString*)tempArr->At(0))->GetString()).Atoi();
        tempLayer.layerLabel  = (TString)((TObjString*)tempArr->At(1))->GetString();
        tempLayer.rUnit   = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atoi();
        tempLayer.layerUnit      = ((TString)((TObjString*)tempArr->At(3))->GetString()).Atoi();
            
        if (debug > 0) std::cout << "layer " << tempLayer.layerNrAbs << "\t assembly name: " << tempLayer.layerLabel << "\t CAEN unit Nr.: "  << tempLayer.rUnit << "\t layer in unit: " << tempLayer.layerUnit << std::endl;
        layers.push_back(tempLayer);
    }
  
    for (int l = 0; l < (int)layers.size();l++){
      for (int chA = 1; chA < 9; chA++){
            // cout << "\n" << endl;
            Int_t channel = findChannelInUnit(uChannels, layers.at(l).layerUnit, chA);
            cout << layers.at(l).rUnit << "\t" << channel << "\t" << layers.at(l).layerNrAbs << "\t" << layers.at(l).layerLabel << "\t" <<  chA << "\t" << ReturnRowBoard(chA) << "\t" << ReturnColumnBoard(chA) << endl;
      }
    }
    
  return;
}
