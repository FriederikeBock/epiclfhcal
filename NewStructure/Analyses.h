#ifndef ANALYSES_H
#define ANALYSES_H

#include <iostream>
#include <fstream>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "Setup.h"
#include "RootSetupWrapper.h"
#include "Calib.h"
#include "Event.h"
#include "Tile.h"
#include "HGCROC.h"
#include "Caen.h"

class Analyses{

 public:
  Analyses(){
    eventptr=&event;
    rswptr=&rsw;
    calibptr=&calib;
  }
  ~Analyses(){}

  //Getter methods
  inline TString GetASCIIinputName()        const {return ASCIIinputName;};
  inline TString GetMapInputName()          const {return MapInputName;};
  inline TString GetRootCalibInputName()    const {return RootCalibInputName;};
  inline TString GetRootInputName()         const {return RootInputName;};
  inline TString GetRootPedestalInputName() const {return RootPedestalInputName;};
  inline TString GetRootOutputName()        const {return RootOutputName;};
  
  inline std::fstream* GetASCIIinput() {return &ASCIIinput;};
  inline std::fstream* GetMapInput()   {return &MapInput;};

  inline TFile* GetRootCalibInput()    {return RootCalibInput;}
  inline TFile* GetRootInput()         {return RootInput;}
  inline TFile* GetRootPedestalInput() {return RootPedestalInput;}
  inline TFile* GetRootOutput()        {return RootOutput;}

  inline bool CanOverWrite(void)                const {return Overwrite;};
  inline bool IsToApplyPedestalCorrection(void) const {return ApplyPedestalCorrection;};
  inline bool IsToApplyCalibration(void)        const {return ApplyCalibration;};
  inline bool IsToConvert(void)                 const {return Convert;};
  inline bool IsToExtractPedestal(void)         const {return ExtractPedestal;};
  inline bool IsToExtractScaling(void)          const {return ExtractScaling;};

  //setter methods
  //Overload method for boolean...or is it too dangerous?
  inline void CanOverWrite(bool b)               {Overwrite=b;};
  inline void IsToApplyPedestalCorrection(bool b){ApplyPedestalCorrection=b;};
  inline void IsToApplyCalibration(bool b)       {ApplyCalibration=b;};
  inline void IsToConvert(bool b)                {Convert=b;};
  inline void IsToExtractPedestal(bool b)        {ExtractPedestal=b;};
  inline void IsToExtractScaling(bool b)         {ExtractScaling=b;};
  

  inline void SetASCIIinput(TString name)        {ASCIIinputName=name;};
  inline void SetMapInput(TString name)          {MapInputName=name;};
  inline void SetRootCalibInput(TString name)    {RootCalibInputName=name;};
  inline void SetRootInput(TString name)         {RootInputName=name;};
  inline void SetRootPedestalInput(TString name) {RootPedestalInputName=name;};
  inline void SetRootOutput(TString name)        {RootOutputName =name;};

  //General methods
  bool CheckAndOpenIO(void);
  bool Process(void);

  //Variable members
  TString ASCIIinputName;
  TString RootOutputName;
  TString RootInputName;
  TString RootCalibInputName;
  TString RootPedestalInputName;
  TString MapInputName;
  TFile* RootOutput=nullptr;
  TFile* RootInput=nullptr;
  TFile* RootPedestalInput=nullptr;
  TFile* RootCalibInput=nullptr;
  std::fstream ASCIIinput;
  std::fstream MapInput;
  bool Convert=false;
  bool ExtractPedestal=false;
  bool ExtractScaling=false;
  bool ApplyPedestalCorrection=false;
  bool ApplyCalibration=false;
  bool Overwrite=false;
  RootSetupWrapper rsw;
  RootSetupWrapper* rswptr;
  Setup* setup;
  Calib calib;
  Calib* calibptr;
  Calib* calibextra;
  Event event;
  Event* eventptr;
  Event* eventextra;
  
  TTree* TsetupIn=nullptr;
  TTree* TsetupOut=nullptr;
  TTree* TdataIn=nullptr;
  TTree* TdataOut=nullptr;
  TTree* TcalibIn=nullptr;
  TTree* TcalibOut=nullptr;

 protected:


 private:
  bool ConvertASCII2Root(void);
  bool GetPedestal(void);
  bool CorrectPedestal(void);
  bool GetScaling(void);
  bool Calibrate(void);
};





#endif
