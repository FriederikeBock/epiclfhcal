#include "Calib.h"

ClassImp(Calib);

double Calib::GetPedestalMeanH(int cellID) const{
  //std::map<int, double>::const_iterator it= PedestalMeanH.find(cellID);
  //if(it!=PedestalMeanH.end()) return it->second;
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.PedestalMeanH;
  else return 9999;
}
double Calib::GetPedestalMeanL(int cellID) const{
  //std::map<int, double>::const_iterator it= PedestalMeanL.find(cellID);
  //if(it!=PedestalMeanL.end()) return it->second;
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.PedestalMeanL;
  else return 9999;
}

double Calib::GetPedestalSigH(int cellID) const{
  //std::map<int, double>::const_iterator it= PedestalSigH.find(cellID);
  //if(it!=PedestalSigH.end()) return it->second;
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.PedestalSigH;
  else return 9999;
}
double Calib::GetPedestalSigL(int cellID) const{
  //std::map<int, double>::const_iterator it= PedestalSigL.find(cellID);
  //if(it!=PedestalSigL.end()) return it->second;
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.PedestalSigL;
  else return 9999;
}

double Calib::GetPedestalMeanH(int row, int col, int lay, int mod=0) const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetPedestalMeanH(key);
}

double Calib::GetPedestalMeanL(int row, int col, int lay, int mod=0) const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetPedestalMeanL(key);
}

double Calib::GetPedestalSigH(int row, int col, int lay, int mod=0) const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetPedestalSigH(key);
}

double Calib::GetPedestalSigL(int row, int col, int lay, int mod=0) const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetPedestalSigL(key);
}

double Calib::GetScaleHigh(int cellID)const {
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.ScaleH;
  else return -1.;
}

double Calib::GetScaleHigh(int row, int col, int lay, int mod=0)const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col,lay,mod);
  return GetScaleHigh(key);
}

double Calib::GetAverageScaleHigh()const{
  double avSc = 0;
  std::map<int, TileCalib>::const_iterator it;
  for(it=CaloCalib.begin(); it!=CaloCalib.end(); ++it){
    avSc += it->second.ScaleH;
  }
  return avSc/CaloCalib.size();
}
double Calib::GetAverageScaleLow()const{
  double avSc = 0;
  std::map<int, TileCalib>::const_iterator it;
  for(it=CaloCalib.begin(); it!=CaloCalib.end(); ++it){
    avSc += it->second.ScaleL;
  }
  return avSc/CaloCalib.size();
}

double Calib::GetAverageLGHGCorr()const{
  double avSc = 0;
  std::map<int, TileCalib>::const_iterator it;
  for(it=CaloCalib.begin(); it!=CaloCalib.end(); ++it){
    avSc += it->second.LGHGCorr;
  }
  return avSc/CaloCalib.size();
}

double Calib::GetAverageHGLGCorr()const{
  double avSc = 0;
  std::map<int, TileCalib>::const_iterator it;
  for(it=CaloCalib.begin(); it!=CaloCalib.end(); ++it){
    avSc += it->second.HGLGCorr;
  }
  return avSc/CaloCalib.size();
}

double Calib::GetScaleWidthHigh(int cellID)const {
  //std::map<int, double>::const_iterator it=ScaleH.find(cellID);
  //if(it!=ScaleH.end()) return it->second;
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.ScaleWidthH;
  else return -1.;
}

double Calib::GetScaleWidthHigh(int row, int col, int lay, int mod=0)const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col,lay,mod);
  return GetScaleWidthHigh(key);
}


double Calib::GetScaleLow(int cellID) const {
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.ScaleL;
  else return -1.;
}

double Calib::GetScaleLow(int row, int col, int lay, int mod=0)const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetScaleLow(key);
}

double Calib::GetScaleWidthLow(int cellID) const {
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.ScaleWidthL;
  else return -1.;
}

double Calib::GetScaleWidthLow(int row, int col, int lay, int mod=0)const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetScaleWidthLow(key);
}

double Calib::GetScaleLGHGCorr(int cellID) const {
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.LGHGCorr;
  else return -1.;
}

double Calib::GetScaleLGHGCorr(int row, int col, int lay, int mod=0)const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetScaleLGHGCorr(key);
}

double Calib::GetScaleHGLGCorr(int cellID) const {
  std::map<int, TileCalib>::const_iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()) return it->second.HGLGCorr;
  else return -1.;
}

double Calib::GetScaleHGLGCorr(int row, int col, int lay, int mod=0)const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetScaleHGLGCorr(key);
}

TileCalib* Calib::GetTileCalib(int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    return &(it->second);
  }
  else {
    TileCalib acal;
    CaloCalib[cellID]=acal;
    return &(CaloCalib[cellID]);
  }
}

TileCalib* Calib::GetTileCalib(int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetTileCalib(key);
}

void Calib::SetPedestalMeanH(double ped, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.PedestalMeanH=ped;
    CaloCalib[cellID]=acal;
  }
  else it->second.PedestalMeanH=ped;
    //PedestalMeanH[cellID]=ped;
}

void Calib::SetPedestalMeanL(double ped, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.PedestalMeanL=ped;
    CaloCalib[cellID]=acal;
  }
  else it->second.PedestalMeanL=ped;
  //PedestalMeanL[cellID]=ped;
}

void Calib::SetPedestalSigH(double ped, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.PedestalSigH=ped;
    CaloCalib[cellID]=acal;
  }
  else it->second.PedestalSigH=ped;
  //PedestalSigH[cellID]=ped;
}

void Calib::SetPedestalSigL(double ped, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.PedestalSigL=ped;
    CaloCalib[cellID]=acal;
  }
  else it->second.PedestalSigL=ped;
  //PedestalSigL[cellID]=ped;
}


void Calib::SetPedestalMeanH(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetPedestalMeanH(ped,key);
  //PedestalMeanH[key]=ped;
}
void Calib::SetPedestalMeanL(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetPedestalMeanL(ped,key);
  //PedestalMeanL[key]=ped;
}
void Calib::SetPedestalSigH(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetPedestalSigH(ped,key);
  //PedestalSigH[key]=ped;
}
void Calib::SetPedestalSigL(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetPedestalSigL(ped,key);
  //PedestalSigL[key]=ped;
}

void Calib::SetScaleHigh(double s, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.ScaleH=s;
    CaloCalib[cellID]=acal;
  }
  else it->second.ScaleH=s;
  //ScaleH[cellID]=s;
}

void Calib::SetScaleHigh(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetScaleHigh(s,key);
  //ScaleH[key]=s;
}

void Calib::SetScaleWidthHigh(double s, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.ScaleWidthH=s;
    CaloCalib[cellID]=acal;
  }
  else it->second.ScaleH=s;
  //ScaleH[cellID]=s;
}

void Calib::SetScaleWidthHigh(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetScaleWidthHigh(s,key);
  //ScaleH[key]=s;
}



void Calib::SetScaleLow(double s, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.ScaleL=s;
    CaloCalib[cellID]=acal;
  }
  else it->second.ScaleL=s;
  //ScaleL[cellID]=s;
}

void Calib::SetScaleLow(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetScaleLow(s,key);
  //ScaleL[key]=s;
}

void Calib::SetScaleWidthLow(double s, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.ScaleWidthL=s;
    CaloCalib[cellID]=acal;
  }
  else it->second.ScaleH=s;
  //ScaleH[cellID]=s;
}

void Calib::SetScaleWidthLow(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetScaleWidthLow(s,key);
  //ScaleH[key]=s;
}

void Calib::SetScaleLGHGCorr(double s, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.LGHGCorr=s;
    CaloCalib[cellID]=acal;
  }
  else it->second.ScaleH=s;
  //ScaleH[cellID]=s;
}

void Calib::SetScaleLGHGCorr(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetScaleLGHGCorr(s,key);
  //ScaleH[key]=s;
}

void Calib::SetScaleHGLGCorr(double s, int cellID){
  std::map<int, TileCalib>::iterator it= CaloCalib.find(cellID);
  if(it!=CaloCalib.end()){
    TileCalib acal;
    acal.HGLGCorr=s;
    CaloCalib[cellID]=acal;
  }
  else it->second.ScaleH=s;
  //ScaleH[cellID]=s;
}

void Calib::SetScaleHGLGCorr(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  SetScaleHGLGCorr(s,key);
  //ScaleH[key]=s;
}

int Calib::GetRunNumber(void){
  return RunNumber;
}

const TTimeStamp* Calib::GetBeginRunTime(void) const{
  return &BeginRunTime;
}

double Calib::GetVop(void){
  return Vop;
}

double Calib::GetVov(void){
  return Vov;
}

void Calib::SetRunNumber(int r){
  RunNumber=r;
}

void Calib::SetBeginRunTime(TTimeStamp t){
  BeginRunTime=t;
}

void Calib::SetVop(double v){
  Vop=v;
}

void Calib::SetVov(double v){
  Vov=v;
}
