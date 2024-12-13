#ifndef COMPARISONCALIB_H
#define COMPARISONCALIB_H

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
    
class ComparisonCalib{

 public:
  ComparisonCalib(){
    rswptr=&rsw;
    calibptr=&calib;
  }
  ~ComparisonCalib(){}

  //Getter methods
  inline TString GetRootCalibInputName()    const {return RootCalibInputName;};
  inline TString GetInputListName()         const {return InputListName;};
  inline TString GetRootPedestalInputName() const {return RootPedestalInputName;};
  inline TString GetRootOutputName()        const {return RootOutputName;};
  inline TString GetPlotOutputDir()         const {return OutputNameDirPlots;};
  
  inline TFile* GetRootCalibInput()    {return RootCalibInput;}
  inline TFile* GetRootInput()         {return RootInput;}
  inline TFile* GetRootPedestalInput() {return RootPedestalInput;}
  inline TFile* GetRootOutput()        {return RootOutput;}

  inline bool CanOverWrite(void)                const {return Overwrite;};
  
  //setter methods
  //Overload method for boolean...or is it too dangerous?
  inline void CanOverWrite(bool b)               {Overwrite=b;};
  inline void EnableDebug(int i)                 {debug=i;};
    
  inline void SetYear(int year)                  {yearData=year;};
  inline void SetRunListInput(TString name)      {RunListInputName=name;};
  inline void SetRootCalibInput(TString name)    {RootCalibInputName=name;};
  inline void SetInputList(TString name)         {InputListName=name;};
  inline void SetRootPedestalInput(TString name) {RootPedestalInputName=name;};
  inline void SetRootOutput(TString name)        {RootOutputName =name;};
  inline void SetRootOutputHists(TString name)   {RootOutputNameHist =name;};
  inline void SetPlotOutputDir(TString name)     {OutputNameDirPlots =name;};
  
  //General methods
  bool CreateOutputRootFile(void);
  bool CheckAndOpenIO(void);
  bool Process(void);

  //Variable members
  TString RootOutputName;                 // file name of root output with tree
  TString RootOutputNameHist;             // file name of root output with additional histograms & fits
  TString OutputNameDirPlots;             // directory name of output for plots
  TString InputListName;                  // file name of input file list 
  TString RootCalibInputName;             // file name of calibration root file (mip calib)
  TString RootPedestalInputName;          // file name of pedestal root file (pedestal values)
  TString RunListInputName;               // file name run list 
  TFile* RootOutput         =nullptr;     // root file output tree
  TFile* RootOutputHist     =nullptr;     // root file output histos
  TFile* RootInput          =nullptr;     // root file input 
  TFile* RootPedestalInput  =nullptr;     // root file pedestal values
  TFile* RootCalibInput     =nullptr;     // root file calib values (mip)
  bool Overwrite              =false;     // Flag to overwrite outputs
  int debug                   =0;         // debug level 
  int yearData                =-1;        // data taking year externally set
  RootSetupWrapper rsw;                   // Wrapper singleton class for setup
  RootSetupWrapper* rswptr;               // Pointer to wrapper for singleton class for setup
  Setup* setup;                           // geometry setup
  Calib calib;                            // calibration object
  Calib* calibptr;                        // pointer to calib object
  
  TTree* TsetupIn=nullptr;
  TTree* TcalibIn=nullptr;

 protected:


 private:
   
};

#endif
