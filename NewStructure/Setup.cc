#include "Setup.h"
#include "RootSetupWrapper.h"
#include <iostream>
#include <fstream>

ClassImp(Setup);

bool Setup::Initialize(TString file){
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
  int Aboard,Achannel,Alayer,Arolayer,Arow,Acolumn;
  TString Anassembly;
  int Akey;
  while(!input.eof()){
    input>>Aboard>>Achannel>>Alayer>>Anassembly>>Arolayer>>Arow>>Acolumn;
    if(!input.good())break;
    Akey=(0<<9)+(Arow<<8)+(Acolumn<<6)+(Alayer);
    assemblyID[Akey] = Anassembly;
    ROunit    [Akey] = Arolayer;
    ROchannel [Akey] = Achannel;
    Board     [Akey] = Aboard;
    CellIDfromRO[std::make_pair(Aboard,Achannel)]=Akey;
  }
  input.close();
  isInit=true;
  return isInit;
}

bool Setup::Initialize(RootSetupWrapper& rsw){
  std::cout<<rsw.isInit<<"\t"<<rsw.assemblyID.size()<<std::endl;
  isInit      =rsw.isInit;
  assemblyID  =rsw.assemblyID;
  ROunit      =rsw.ROunit;
  ROchannel   =rsw.ROchannel;
  Board       = rsw.Board;
  CellIDfromRO=rsw.CellIDfromRO;
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
