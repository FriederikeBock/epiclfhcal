#ifndef COMMONHELPERFUNCTIONS_H
#define COMMONHELPERFUNCTIONS_H

  #include <iostream>
  #include <fstream>
  #include "TString.h"
  #include "TObjString.h"
  #include <vector>
  #include <map>
  #include <utility>
  
  struct RunInfo{
    RunInfo(): runNr(0), species(""), pdg(0), energy(0), vop(0), vbr(0), lgSet(0), hgSet(0), posX(0), posY(0), assemblyNr(0){}
    int runNr;
    TString species;
    int pdg;
    float energy;
    float vop;
    float vbr;
    int lgSet;
    int hgSet;
    float posX;
    float posY;
    int assemblyNr;
  } ;

  TString GetStringFromRunInfo(RunInfo, Int_t);

  //__________________________________________________________________________________________________________
  //__________________ Read run infos from text file _________________________________________________________
  //__________________________________________________________________________________________________________    
  // specialData: 0 - std. TB, 1 - SPE data ORNL
  inline std::map<int,RunInfo> readRunInfosFromFile(TString runListFileName, int debug, int specialData = 0 ){
    std::map<int,RunInfo> runs;
    //std::cout << "INFO: You have given the following run list file: " << runListFileName.Data() << std::endl;
    std::ifstream runListFile;
    runListFile.open(runListFileName,std::ios_base::in);
    if (!runListFile) {
      std::cout << "ERROR: file " << runListFileName.Data() << " not found!" << std::endl;
      return runs;
    }

    for( TString tempLine; tempLine.ReadLine(runListFile, kTRUE); ) {
      // check if line should be considered
      if (tempLine.BeginsWith("%") || tempLine.BeginsWith("#")){
        continue;
      }
      if (debug > 1) std::cout << tempLine.Data() << std::endl;

      // Separate the string according to tabulators
      TObjArray *tempArr  = tempLine.Tokenize(",");
      if(tempArr->GetEntries()<1){
        if (debug > 1) std::cout << "nothing to be done" << std::endl;
        delete tempArr;
        continue;
      } 

      // Put them to the correct variables    
      RunInfo tempRun;
      tempRun.runNr    = ((TString)((TObjString*)tempArr->At(0))->GetString()).Atoi();
      tempRun.species  =  (TString)((TObjString*)tempArr->At(1))->GetString();
      tempRun.pdg      = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atoi();
      tempRun.energy   = ((TString)((TObjString*)tempArr->At(3))->GetString()).Atof();
      tempRun.vop      = ((TString)((TObjString*)tempArr->At(4))->GetString()).Atof();
      tempRun.vbr      = ((TString)((TObjString*)tempArr->At(5))->GetString()).Atof();
      tempRun.hgSet    = ((TString)((TObjString*)tempArr->At(6))->GetString()).Atoi();
      tempRun.lgSet    = ((TString)((TObjString*)tempArr->At(7))->GetString()).Atoi();
      tempRun.posX    = ((TString)((TObjString*)tempArr->At(8))->GetString()).Atoi();
      tempRun.posY    = ((TString)((TObjString*)tempArr->At(9))->GetString()).Atoi();
      if (specialData == 1) tempRun.assemblyNr = ((TString)((TObjString*)tempArr->At(10))->GetString()).Atoi();
                  
      if (debug > 1) std::cout << "Run " << tempRun.runNr << "\t species: " << tempRun.species << "\t energy: "  << tempRun.energy << "\t Vop: " << tempRun.vop << "\t Vov: " << tempRun.vop-tempRun.vbr << "\t Xbeam: " << tempRun.posX<< "\t Ybeam: " << tempRun.posY<< std::endl;
      runs[tempRun.runNr]=tempRun;
    }
    std::cout << "registered " << runs.size() << std::endl;
    return runs;
  };

#endif
