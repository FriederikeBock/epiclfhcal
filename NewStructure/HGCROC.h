#ifndef HGCROC_H
#define HGCROC_H

#include <vector>
#include "Tile.h"

class Hgcroc: public Tile {

 public:
  Hgcroc():Tile(ReadOut::Type::Hgcroc){}
  ~Hgcroc(){}
  std::vector<int> GetWaveform(void) const;
  int GetNsample(void) const ;
  double GetTOT(void) const;
  double GetTOA(void) const;

  void SetWaveform(std::vector<int>);
  void AppendWaveformADC(int);
  void ResetWaveformPoint(int, int);
  void SetNsample(int);
  void SetTOT(double);
  void SetTOA(double);
  
 protected:
  int Nsample;
  std::vector<int> adc;
  double TOT;
  double TOA;
  
 private:

  ClassDef(Hgcroc,1)
};


#endif
