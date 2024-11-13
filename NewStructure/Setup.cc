#include "Setup.h"
#include "RootSetupWrapper.h"
#include <iostream>
#include <fstream>

//Setup* Setup::instancePtr = nullptr; // Add for use on Mac OS

ClassImp(Setup);

bool Setup::Initialize(TString file, int debug){
  if(isInit){
    std::cout<<"Already initialized, bailing out without action"<<std::endl;
    return true;
  }
  std::fstream input;
  input.open(file.Data(),std::ios::in);
  if(!input.is_open()){
    std::cout<<"Could not open "<<file<<std::endl;
    std::cout<<"Leaving uninitialized"<<std::endl;
    return false;
  }
  nMaxLayer   = -1;
  nMaxRow     = -1;
  nMaxColumn  = -1;
  nMaxModule  = -1;
  nMaxROUnit  = -1;
  maxCellID   = -1;
  int AROunit,AROchannel,Alayer,AROlayer,Arow,Acolumn,Amod;
  TString Anassembly;
  int Akey;
  while(!input.eof()){
    input>>AROunit>>AROchannel>>Alayer>>Anassembly>>AROlayer>>Arow>>Acolumn >> Amod;
    if(!input.good())break;
    Akey=(Amod<<9)+(Arow<<8)+(Acolumn<<6)+(Alayer);
    assemblyID[Akey] = Anassembly;
    ROunit    [Akey] = AROunit;
    ROchannel [Akey] = AROchannel;
    Board     [Akey] = AROlayer;
    CellIDfromRO[std::make_pair(AROunit,AROchannel)]=Akey;
    if (nMaxLayer < Alayer)   nMaxLayer   = Alayer;
    if (nMaxRow < Arow)       nMaxRow     = Arow;
    if (nMaxColumn < Acolumn) nMaxColumn  = Acolumn;
    if (nMaxModule < Amod)    nMaxModule  = Amod;
    if (nMaxROUnit < AROunit) nMaxROUnit  = AROunit;
    if (maxCellID < Akey)     maxCellID   = Akey;
    if (debug > 10)std::cout <<AROunit<< "\t" << AROchannel << "\t"<< Alayer << "\t"<< Anassembly << "\t"<< AROlayer << "\t"<< Arow << "\t"<< Acolumn << "\t"<< Amod << std::endl;
    if (debug > 1)std::cout << "registered cell: " << DecodeCellID(Akey).Data() << std::endl;
  }
  input.close();
  isInit=true;
  return isInit;
}

bool Setup::Initialize(RootSetupWrapper& rsw){
  std::cout<<rsw.isInit<<"\t"<<rsw.assemblyID.size()<<std::endl;
  isInit          =rsw.isInit;
  assemblyID      =rsw.assemblyID;
  ROunit          =rsw.ROunit;
  ROchannel       =rsw.ROchannel;
  Board           =rsw.Board;
  CellIDfromRO    =rsw.CellIDfromRO;
  nMaxLayer       =rsw.nMaxLayer;
  nMaxRow         =rsw.nMaxRow;
  nMaxColumn      =rsw.nMaxColumn;
  nMaxModule      =rsw.nMaxModule;
  nMaxROUnit      =rsw.nMaxROUnit;
  maxCellID       =rsw.maxCellID;
  return isInit;
}

bool Setup::IsInit() const{
  return isInit;
}

TString Setup::GetAssemblyID(int cellID) const{
  std::map<int, TString>::const_iterator it=assemblyID.find(cellID);
  if(it!=assemblyID.end()) return it->second;
  else return "";
}

TString Setup::GetAssemblyID(int row, int col, int lay, int mod=0) const{
  return GetAssemblyID((mod<<9)+(row<<8)+(col<<6)+lay);
}

int Setup::GetCellID(int roboard, int roch) const{
  std::map<std::pair<int,int>,int>::const_iterator it=CellIDfromRO.find(std::make_pair(roboard,roch));
  if(it!=CellIDfromRO.cend()) return it->second;
  else return -1;
}

int Setup::GetCellID(int row, int col, int lay, int mod=0)const {
  return (mod<<9)+(row<<8)+(col<<6)+lay;
}

int Setup::GetColumn(int cellID) const{
  return (cellID&(3<<6))>>6;
}

int Setup::GetLayer(int cellID) const{
  return (cellID&((1<<6)-1));
}

int Setup::GetModule(int cellID) const{
  return cellID>>9;
}

int Setup::GetROchannel(int cellID) const{
  std::map<int, int>::const_iterator it=ROchannel.find(cellID);
  if(it!=ROchannel.end()) return it->second;
  else return -999;
}

int Setup::GetROchannel(int row, int col, int lay, int mod=0) const{
  return GetROchannel((mod<<9)+(row<<8)+(col<<6)+lay);
}

int Setup::GetROunit(int cellID) const{
  std::map<int,int>::const_iterator it=ROunit.find(cellID);
  if(it!=ROunit.end()) return it->second;
  else return -999;
}

int Setup::GetROunit(int row, int col, int lay, int mod=0) const{
  return GetROunit((mod<<9)+(row<<8)+(col<<6)+lay);
}

int Setup::GetRow(int cellID) const{
  return (cellID&(1<<8))>>8;
}

double Setup::GetModuleX(int mod=0)const {
  return 0.;
}

double Setup::GetModuleY(int mod=0)const {
  return 0.;
}

int Setup::GetTotalNbChannels(void) const {
  return (int) ROunit.size();
}

double Setup::GetX(int cellID) const{
  int col=GetColumn(cellID);
  return -7.5/*cm*/+col*5./*cm*/ /*+GetModuleX()*/;
}

double Setup::GetY(int cellID) const{
  int row=GetRow(cellID);
  return (2*row-1)*2.5/*cm*/ /* +GetModuleY()*/;
}

double Setup::GetZ(int cellID) const{
  int lay=GetLayer(cellID);
  return lay*2.0/*cm*/;
}

TString Setup::DecodeCellID(int cellID) const{
  TString out = Form("cell ID: %d ==> RO unit %d RO channel %d  module %d  layer %d  column %d row %d", cellID, GetROunit(cellID), GetROchannel(cellID), GetModule(cellID), GetLayer(cellID), GetColumn(cellID), GetRow(cellID));
  return out;
}


int Setup::GetNMaxLayer() const{
  return nMaxLayer;
}

int Setup::GetNMaxRow() const{
  return nMaxRow;
}

int Setup::GetNMaxColumn() const{
  return nMaxColumn;
}

int Setup::GetNMaxModule() const{
  return nMaxModule;
}

int Setup::GetNMaxROUnit() const{
  return nMaxROUnit;
}

int Setup::GetMaxCellID() const{
  return maxCellID;
}

int Setup::GetChannelInLayer(int cellID) const{
  int row     = GetRow(cellID);
  int column  = GetColumn(cellID);
  int absChL  = row*(nMaxColumn+1)+column;
  return absChL;
}
