#include "RootCalibWrapper.h"
#include "Calib.h"

RootCalibWrapper::RootCalibWrapper(Calib* c){
  Pedestal = c->Pedestal;
  ScaleH   = c->ScaleH  ;
  ScaleL   = c->ScaleL  ;
}
