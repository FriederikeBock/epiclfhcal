#include "HGCROC.h"
#include <cassert>
ClassImp(Hgcroc)
std::vector<int> Hgcroc::GetWaveform(void) const{
  return adc;
}

int Hgcroc::GetNsample(void) const{
  return Nsample;
}

double Hgcroc::GetTOT(void) const{
  return TOT;
}

double Hgcroc::GetTOA(void) const{
  return TOA;
}

void Hgcroc::SetWaveform(std::vector<int> v){
  adc=v;
}

void Hgcroc::AppendWaveformADC(int a){
  adc.push_back(a);
}

void Hgcroc::ResetWaveformPoint(int s, int a){
  assert(0<=s && s<(int)adc.size());
  adc.at(s)=a;
}

void Hgcroc::SetTOT(double tot){
  TOT=tot;
}

void Hgcroc::SetTOA(double toa){
  TOA=toa;
}
