#include "Tile.h"

ClassImp(Tile);

double Tile::GetE(void)const {
  return E;
}

int Tile::GetCellID(void)const{
  return CellID;
}

int Tile::GetCol(void) const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return -999;
  else return setup->GetColumn(CellID);
}

int Tile::GetLayer(void) const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return -999;
  else return setup->GetLayer(CellID);
}

int Tile::GetRU() const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return -999;
  else return setup->GetROunit(CellID);
}
TString Tile::GetROClassName()const{
  switch(ROtype){
  case ReadOut::Type::Hgcroc:
    return "Tile::Hgcroc";
  case ReadOut::Type::Caen:
    return "Tile::Caen";
  default:
    return "Tile::Undefined";
  }
}

int Tile::GetRch() const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return -999;
  else return setup->GetROchannel(CellID);
}

int Tile::GetRow() const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return -999;
  else return setup->GetRow(CellID);
}

TString Tile::GetAssemblyID() const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return "Undefined";
  else return setup->GetAssemblyID(CellID);
}

double Tile::GetX()const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return -999.;
  else return setup->GetX(CellID);
}

double Tile::GetY() const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return -999.;
  else return setup->GetY(CellID);
}

double Tile::GetZ() const{
  Setup* setup=Setup::GetInstance();
  if(!setup->IsInit()) return -999.;
  else return setup->GetZ(CellID);
}

double Tile::GetTOA()const{
  return TOA;
}

void Tile::SetE(double e){
  E=e;
}

void Tile::SetCellID(int i){
  //Shall we make some checks against setup?
  CellID=i;
}

void Tile::SetROtype(ReadOut::Type i){
  ROtype=i;
}

void Tile::SetTOA(double t){
  TOA=t;
}
