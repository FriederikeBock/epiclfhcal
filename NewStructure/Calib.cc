#include "Calib.h"
//#include "RootCalibWrapper.h"

ClassImp(Calib);

double Calib::GetPedestal(int cellID) const{
  std::map<int, double>::const_iterator it= Pedestal.find(cellID);
  if(it!=Pedestal.end()) return it->second;
  else return 9999;
}

double Calib::GetPedestal(int row, int col, int lay, int mod=0) const{
  Setup* setup = Setup::GetInstance();
  int key=setup->GetCellID(row, col, lay, mod);
  return GetPedestal(key);
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

void Calib::SetPedestal(double ped, int cellID){
  Pedestal[cellID]=ped;
}

void Calib::SetPedestal(double ped, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  //if(setup==NULL) setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  Pedestal[key]=ped;
}

void Calib::SetScaleHigh(double s, int cellID){
  ScaleH[cellID]=s;
}

void Calib::SetScaleHigh(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  //if(setup==NULL) setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  ScaleH[key]=s;
}

void Calib::SetScaleLow(double s, int cellID){
  ScaleL[cellID]=s;
}

void Calib::SetScaleLow(double s, int row, int col, int lay, int mod=0){
  Setup* setup = Setup::GetInstance();
  //if(setup==NULL) setup = Setup::GetInstance();
  int key=setup->GetCellID(row,col,lay,mod);
  ScaleL[key]=s;
}
