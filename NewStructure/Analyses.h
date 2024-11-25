#ifndef ANALYSES_H
#define ANALYSES_H

#include <iostream>
#include <fstream>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TObjString.h"

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
  inline TString GetPlotOutputDir()         const {return OutputNameDirPlots;};
  
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
  inline void EnableDebug(int i)                 {debug=i;};
  
  
  inline void SetYear(int year)                  {yearData=year;};
  inline void SetASCIIinput(TString name)        {ASCIIinputName=name;};
  inline void SetMapInput(TString name)          {MapInputName=name;};
  inline void SetRunListInput(TString name)      {RunListInputName=name;};
  inline void SetRootCalibInput(TString name)    {RootCalibInputName=name;};
  inline void SetRootInput(TString name)         {RootInputName=name;};
  inline void SetRootPedestalInput(TString name) {RootPedestalInputName=name;};
  inline void SetRootOutput(TString name)        {RootOutputName =name;};
  inline void SetRootOutputHists(TString name)   {RootOutputNameHist =name;};
  inline void SetPlotOutputDir(TString name)     {OutputNameDirPlots =name;};
  
  
  
  //General methods
  bool CreateOutputRootFile(void);
  bool CheckAndOpenIO(void);
  bool Process(void);

  //Variable members
  TString ASCIIinputName;                 // CAEN ASCII file input name (only needed for converting data)
  TString RootOutputName;                 // file name of root output with tree
  TString RootOutputNameHist;             // file name of root output with additional histograms & fits
  TString OutputNameDirPlots;             // directory name of output for plots
  TString RootInputName;                  // file name of input root file 
  TString RootCalibInputName;             // file name of calibration root file (mip calib)
  TString RootPedestalInputName;          // file name of pedestal root file (pedestal values)
  TString MapInputName;                   // file name geometry mapping
  TString RunListInputName;               // file name run list 
  TFile* RootOutput         =nullptr;     // root file output tree
  TFile* RootOutputHist     =nullptr;     // root file output histos
  TFile* RootInput          =nullptr;     // root file input 
  TFile* RootPedestalInput  =nullptr;     // root file pedestal values
  TFile* RootCalibInput     =nullptr;     // root file calib values (mip)
  std::fstream ASCIIinput;                // ASCII file streamer for CAEN data
  std::fstream MapInput;                  // file streamer for geometry mapping
  bool Convert                =false;     // Flag for data conversion 
  bool ExtractPedestal        =false;     // Flag for pedestal extraction
  bool ExtractScaling         =false;     // Flag for mip scaling extraction
  bool ApplyPedestalCorrection=false;     // Flag for application of pedestals
  bool ApplyCalibration       =false;     // Flag for aplication of calibration
  bool Overwrite              =false;     // Flag to overwrite outputs
  int debug                   =0;         // debug level 
  int yearData                =-1;        // data taking year externally set
  RootSetupWrapper rsw;                   // Wrapper singleton class for setup
  RootSetupWrapper* rswptr;               // Pointer to wrapper for singleton class for setup
  Setup* setup;                           // geometry setup
  Calib calib;                            // calibration object
  Calib* calibptr;                        // pointer to calib object
  Event event;
  Event* eventptr;
  
  TTree* TsetupIn=nullptr;
  TTree* TsetupOut=nullptr;
  TTree* TdataIn=nullptr;
  TTree* TdataOut=nullptr;
  TTree* TcalibIn=nullptr;
  TTree* TcalibOut=nullptr;

 protected:


 private:
  bool ConvertASCII2Root(void);
  bool ConvertOldRootFile2Root(void);
  bool GetPedestal(void);
  bool CorrectPedestal(void);
  bool GetScaling(void);
  bool Calibrate(void);
};


#endif
