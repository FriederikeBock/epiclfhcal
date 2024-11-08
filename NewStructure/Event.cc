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
