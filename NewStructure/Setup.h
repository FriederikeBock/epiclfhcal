#ifndef SETUP_H
#define SETUP_H

#include <cstddef>
#include <map>
#include "TString.h"

class RootSetupWrapper;

class Setup{
  
 public:
  // deleting copy constructor. 
  Setup(const Setup &)=delete;
  Setup& operator=(const Setup&)=delete;
  
  static Setup *GetInstance(){
    if(instancePtr == NULL) instancePtr = new Setup();
    return instancePtr;
  }
  
  TString GetAssemblyID(int /**/) const;
  TString GetAssemblyID(int /**/, int /**/, int /**/, int /**/) const;
  int     GetCellID    (int /**/, int /**/) const;
  int     GetCellID    (int /**/, int /**/, int /**/, int /**/)const;
  int     GetColumn    (int /**/) const;
  int     GetLayer     (int /**/) const;
  int     GetModule    (int /**/) const;
  double  GetModuleX   (int /**/) const;
  double  GetModuleY   (int /**/) const;
  int     GetROchannel (int /**/) const;
  int     GetROchannel (int /**/, int /**/, int /**/, int /**/) const;
  int     GetROunit    (int /**/) const;
  int     GetROunit    (int /**/, int /**/, int /**/, int /**/) const;
  int     GetRow       (int /**/) const;
  int     GetTotalNbChannels(void) const;
  int     GetNMaxLayer  (void) const;
  int     GetNMaxRow    (void) const;
  int     GetNMaxColumn (void) const;
  int     GetNMaxModule (void) const;
  int     GetNMaxROUnit (void) const;
  int     GetMaxCellID (void) const;
  TString DecodeCellID(int /**/) const;
  double  GetX         (int /**/) const;
  double  GetY         (int /**/) const;
  double  GetZ         (int /**/) const;
  bool    Initialize   (TString, int);
  bool    Initialize   (RootSetupWrapper&);
  bool    IsInit       (void) const;

  friend class RootSetupWrapper;
  
 private:
  static Setup* instancePtr;
  Setup(){}
  ~Setup() {}
  
  
  bool isInit=false;
  //key is CellID
  std::map<int, TString> assemblyID;
  std::map<int, int>     ROunit;
  std::map<int, int>     ROchannel;
  std::map<int, int>     Board;
  //Inverse mapping
  std::map< std::pair<int, int>, int> CellIDfromRO;
  int nMaxLayer;
  int nMaxRow;
  int nMaxColumn;
  int nMaxModule;
  int nMaxROUnit;
  int maxCellID;
  ClassDef(Setup,1)
};


#endif
