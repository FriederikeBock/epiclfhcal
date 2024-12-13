#ifndef TILE_H
#define TILE_H

#include "TString.h"
#include "Setup.h"

namespace ReadOut{
  enum Type {Undef, Hgcroc, Caen};
}

class Tile{

 public:
  Tile():E(0.),CellID(-1),ROtype(ReadOut::Type::Undef),TOA(0.),lTPr(0.),lTrBit(0){}
  Tile(ReadOut::Type RO):E(0.),CellID(-1),ROtype(RO),TOA(0.),lTPr(0.),lTrBit(0){}
  virtual ~Tile(){}
  
  TString       GetAssemblyID             (void) const;
  int           GetCellID                 (void) const;
  int           GetCol                    (void) const;
  double        GetE                      (void) const;
  int           GetLayer                  (void) const;
  int           GetRch                    (void) const;
  TString       GetROClassName            (void) const;
  int           GetRow                    (void) const;
  int           GetRU                     (void) const;
  double        GetTOA                    (void) const;
  double        GetX                      (void) const;
  double        GetY                      (void) const;
  double        GetZ                      (void) const;
  double        GetLocalTriggerPrimitive  (void) const;
  unsigned char GetLocalTriggerBit        (void) const;
  
  void    SetCellID                 (int);
  void    SetE                      (double);
  void    SetROtype                 (ReadOut::Type);
  void    SetTOA                    (double);
  void    SetLocalTriggerPrimitive  (double);
  void    SetLocalTriggerBit        (unsigned char);
  
  
 protected:
  double           E;//energy deposited in MIP equivalent
  int              CellID;
  ReadOut::Type    ROtype;    //Readout unit
  double           TOA;
  double           lTPr;        // local trigger primitive
  unsigned char    lTrBit;      // local trigger bit
  
  ClassDef(Tile,2)

};


#endif
