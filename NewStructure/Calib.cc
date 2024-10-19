#include "Calib.h"
//#include "RootCalibWrapper.h"

ClassImp(Calib);

double Calib::GetPedestalMeanH(int cellID) const{
  std::map<int, double>::const_iterator it= PedestalMeanH.find(cellID);
  if(it!=PedestalMeanH.end()) return it->second;
  else return 9999;
}
double Calib::GetPedestalMeanL(int cellID) const{
  std::map<int, double>::const_iterator it= PedestalMeanL.find(cellID);
  if(it!=PedestalMeanL.end()) return it->second;
  else return 9999;
}

double Calib::GetPedestalSigH(int cellID) const{
  std::map<int, double>::const_iterator it= PedestalSigH.find(cellID);
  if(it!=PedestalSigH.end()) return it->second;
  else return 9999;
}
double Calib::GetPedestalSigL(int cellID) const{
  std::map<int, double>::const_iterator it= PedestalSigL.find(cellID);
  if(it!=PedestalSigL.end()) return it->second;
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
  std::map<int, double>::const_iterator it=ScaleH.find(cellID);
  if(it!=ScaleH.end()) return it->second;
  else return -1.;
}

double Calib::GetScaleHigh(int row, int col, int lay, int mod=0)const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col,lay,mod);
  return GetScaleHigh(key);
}

double Calib::GetScaleLow(int cellID) const {
  std::map<int, double>::const_iterator it=ScaleL.find(cellID);
  if(it!=ScaleL.end()) return it->second;
  else return -1.;
}

double Calib::GetScaleLow(int row, int col, int lay, int mod=0)const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetScaleLow(key);
}

//void Calib::Restore(RootCalibWrapper& c){
//  Pedestal = c.Pedestal;
//  ScaleH   = c.ScaleH  ;
//  ScaleL   = c.ScaleL  ;
//}

void Calib::SetPedestalMeanH(double ped, int cellID){
  PedestalMeanH[cellID]=ped;
}

void Calib::SetPedestalMeanL(double ped, int cellID){
  PedestalMeanL[cellID]=ped;
}

void Calib::SetPedestalSigH(double ped, int cellID){
  PedestalSigH[cellID]=ped;
}

void Calib::SetPedestalSigL(double ped, int cellID){
  PedestalSigL[cellID]=ped;
}


void Calib::SetPedestalMeanH(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  PedestalMeanH[key]=ped;
}
void Calib::SetPedestalMeanL(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  PedestalMeanL[key]=ped;
}
void Calib::SetPedestalSigH(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  PedestalSigH[key]=ped;
}
void Calib::SetPedestalSigL(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  PedestalSigL[key]=ped;
}

void Calib::SetScaleHigh(double s, int cellID){
  ScaleH[cellID]=s;
}

void Calib::SetScaleHigh(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  ScaleH[key]=s;
}

void Calib::SetScaleLow(double s, int cellID){
  ScaleL[cellID]=s;
}

void Calib::SetScaleLow(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  ScaleL[key]=s;
}
