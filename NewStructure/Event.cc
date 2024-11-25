#include "Event.h"

int Event::GetRunNumber(void) const{
  return RunNumber;
}

int Event::GetEventID(void) const{
  return EventID;
}

int Event::GetTimeStamp(void) const{
  return TimeStamp;
}

int Event::GetROtype(void) const{
  return ROtype;
}

void Event::SetRunNumber(int r){
  RunNumber=r;
}

void Event::SetEventID(int ev){
  EventID=ev;
}

void Event::SetTimeStamp(int t){
  TimeStamp=t;
}

void Event::SetROtype(int ro){
  ROtype=ro;
}

TString Event::GetBeamName(void)const{
  return BeamName;
}

int Event::GetBeamID(void) const{
  return BeamID;
}

double Event::GetBeamEnergy(void) const{
  return BeamEnergy;
}

void Event::SetBeamName(TString n){
  BeamName=n;
}

void Event::SetBeamID(int id){
  BeamID=id;
}

void Event::SetBeamEnergy(double e){
  BeamEnergy=e;
}

void Event::AddTile(Tile* t){
  int id=t->GetCellID();
  std::map<int, Tile*>::iterator it=Tiles.find(id);
  if(it!=Tiles.end()){
    delete it->second;
    it->second=t;
  }
  else{
    Tiles[id]=t;
    TileIDs.push_back(id);
  }
}

Tile* Event::GetTile(int index){
  int tempID = TileIDs.at(index);
  return GetTileFromID(tempID);
}

Tile* Event::GetTileFromID(int id){
  std::map<int, Tile*>::iterator it=Tiles.find(id);
  if(it!=Tiles.end()) return it->second;
  else return nullptr;
}

int Event::GetNTiles(void)const{
  return (int)Tiles.size();
}

void Event::ClearTiles(void){
  std::map<int, Tile*>::iterator it;
  for(it=Tiles.begin(); it!=Tiles.end(); ++it){
    delete it->second;
    it->second=NULL;
  }
  Tiles.clear();
  TileIDs.clear();
}

double Event::GetVov() const{
  return Vov;
}

double Event::GetVop()const{
  return Vop;
}

double Event::GetBeamPosX(void) {
  return BeamPosX;
}

double Event::GetBeamPosY(void){
  return BeamPosY;
}

const TTimeStamp* Event::GetBeginRunTime(void) const{
  return &BeginRun;
}

void Event::SetVov(double v){
  Vov=v;
}

void Event::SetVop(double v){
  Vop=v;
}
void Event::SetBeamPosX(double x){
  BeamPosX=x;
}
void Event::SetBeamPosY(double y){
  BeamPosY=y;
}
void Event::SetBeginRunTime(TTimeStamp t){
  BeginRun=t;
}

bool Event::InspectIfLocalMuonTrigg( Setup* setup, 
                                     Calib calib, 
                                     double &avsurr, 
                                     int currTileID, 
                                     double averageScale,
                                     double minThrSc = 0.9, 
                                     double maxThrSc = 3,  
                                     int nTiles = 4 
                                    ){
  
  // figure out surrounding tiles for local mip selection
  long ids[6]  = {-1, -1, -1, -1, -1, -1};
  int layer     = setup->GetLayer(currTileID);
  int row       = setup->GetRow(currTileID);
  int col       = setup->GetColumn(currTileID);
  int mod       = setup->GetModule(currTileID);
  if (nTiles == 4){
    if (layer == 0){
      ids[0] = setup->GetCellID(row, col, layer+1,mod);
      ids[1] = setup->GetCellID(row, col, layer+2,mod);
      ids[2] = setup->GetCellID(row, col, layer+3,mod);
      ids[3] = setup->GetCellID(row, col, layer+4,mod);
    }else if (layer == 1){
      ids[0] = setup->GetCellID(row, col, layer-1,mod);
      ids[1] = setup->GetCellID(row, col, layer+1,mod);
      ids[2] = setup->GetCellID(row, col, layer+2,mod);
      ids[3] = setup->GetCellID(row, col, layer+3,mod);
    } else if (layer == setup->GetNMaxLayer()-1){
      ids[0] = setup->GetCellID(row, col, layer-3,mod);
      ids[1] = setup->GetCellID(row, col, layer-2,mod);
      ids[2] = setup->GetCellID(row, col, layer-1,mod);
      ids[3] = setup->GetCellID(row, col, layer+1,mod);      
    } else if (layer == setup->GetNMaxLayer()){
      ids[0] = setup->GetCellID(row, col, layer-4,mod);
      ids[1] = setup->GetCellID(row, col, layer-3,mod);
      ids[2] = setup->GetCellID(row, col, layer-2,mod);
      ids[3] = setup->GetCellID(row, col, layer-1,mod);      
    } else {
      ids[0] = setup->GetCellID(row, col, layer-2,mod);
      ids[1] = setup->GetCellID(row, col, layer-1,mod);
      ids[2] = setup->GetCellID(row, col, layer+1,mod);
      ids[3] = setup->GetCellID(row, col, layer+2,mod);
    }
  } else if (nTiles == 6){  
    if (layer == 0){
      ids[0] = setup->GetCellID(row, col, layer+1,mod);
      ids[1] = setup->GetCellID(row, col, layer+2,mod);
      ids[2] = setup->GetCellID(row, col, layer+3,mod);
      ids[3] = setup->GetCellID(row, col, layer+4,mod);
      ids[4] = setup->GetCellID(row, col, layer+5,mod);
      ids[5] = setup->GetCellID(row, col, layer+6,mod);
    }else if (layer == 1){
      ids[0] = setup->GetCellID(row, col, layer-1,mod);
      ids[1] = setup->GetCellID(row, col, layer+1,mod);
      ids[2] = setup->GetCellID(row, col, layer+2,mod);
      ids[3] = setup->GetCellID(row, col, layer+3,mod);
      ids[4] = setup->GetCellID(row, col, layer+4,mod);
      ids[5] = setup->GetCellID(row, col, layer+5,mod);
    }else if (layer == 2){
      ids[0] = setup->GetCellID(row, col, layer-2,mod);
      ids[1] = setup->GetCellID(row, col, layer-1,mod);
      ids[2] = setup->GetCellID(row, col, layer+1,mod);
      ids[3] = setup->GetCellID(row, col, layer+2,mod);
      ids[4] = setup->GetCellID(row, col, layer+3,mod);
      ids[5] = setup->GetCellID(row, col, layer+4,mod);
    } else if (layer == setup->GetNMaxLayer()-2){
      ids[0] = setup->GetCellID(row, col, layer-4,mod);
      ids[1] = setup->GetCellID(row, col, layer-3,mod);
      ids[2] = setup->GetCellID(row, col, layer-2,mod);
      ids[3] = setup->GetCellID(row, col, layer-1,mod);
      ids[4] = setup->GetCellID(row, col, layer+1,mod);
      ids[5] = setup->GetCellID(row, col, layer+2,mod);      
    } else if (layer == setup->GetNMaxLayer()-1){
      ids[0] = setup->GetCellID(row, col, layer-5,mod);
      ids[1] = setup->GetCellID(row, col, layer-4,mod);
      ids[2] = setup->GetCellID(row, col, layer-3,mod);
      ids[3] = setup->GetCellID(row, col, layer-2,mod);
      ids[4] = setup->GetCellID(row, col, layer-1,mod);
      ids[5] = setup->GetCellID(row, col, layer+1,mod);      
    } else if (layer == setup->GetNMaxLayer()){
      ids[0] = setup->GetCellID(row, col, layer-6,mod);
      ids[1] = setup->GetCellID(row, col, layer-5,mod);
      ids[2] = setup->GetCellID(row, col, layer-4,mod);
      ids[3] = setup->GetCellID(row, col, layer-3,mod);
      ids[4] = setup->GetCellID(row, col, layer-2,mod);
      ids[5] = setup->GetCellID(row, col, layer-1,mod);      
    } else {
      ids[0] = setup->GetCellID(row, col, layer-3,mod);
      ids[1] = setup->GetCellID(row, col, layer-2,mod);
      ids[2] = setup->GetCellID(row, col, layer-1,mod);
      ids[3] = setup->GetCellID(row, col, layer+1,mod);
      ids[4] = setup->GetCellID(row, col, layer+2,mod);
      ids[5] = setup->GetCellID(row, col, layer+3,mod);
    }
    
  } else if (nTiles == 2){
    if (layer == 0){
      ids[0] = setup->GetCellID(row, col, layer+1,mod);
      ids[1] = setup->GetCellID(row, col, layer+2,mod);            
    } else if (layer == setup->GetNMaxLayer()){
      ids[0] = setup->GetCellID(row, col, layer-2,mod);
      ids[1] = setup->GetCellID(row, col, layer-1,mod);          
    } else {
      ids[0] = setup->GetCellID(row, col, layer-1,mod);
      ids[1] = setup->GetCellID(row, col, layer+1,mod);      
    }
  }
  // calculate average sum of surrounding tiles (nominally 2 in the front + 2 in the back)
  avsurr = 0;
  for (Int_t t = 0; t < nTiles; t++){
    double tmpHG = ((Caen*)GetTileFromID(ids[t]))->GetADCHigh()-calib.GetPedestalMeanH(ids[t]); //needs protection
    if (tmpHG > 3*calib.GetPedestalSigH(ids[t]))
      avsurr +=tmpHG;
  }
  avsurr        = avsurr/nTiles;

  if (avsurr >  averageScale*minThrSc && avsurr < maxThrSc*averageScale)
    return true;
  else 
    return false;
    
}
