#include "RootSetupWrapper.h"
#include "Setup.h"

ClassImp(RootSetupWrapper);


RootSetupWrapper::RootSetupWrapper(Setup* s){
  isInit      =s->isInit;
  assemblyID  =s->assemblyID;
  ROunit      =s->ROunit;
  ROchannel   =s->ROchannel;
  Board       =s->Board;
  CellIDfromRO=s->CellIDfromRO;
}
