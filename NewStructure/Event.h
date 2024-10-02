#ifndef EVENT_H
#define EVENT_H

#include "TString.h"
#include <map>
#include <iostream>
#include "Caen.h"
#include "HGCROC.h"

class Event{

 public:

  void    AddTile      (Tile*);
  void    ClearTiles   (void);
  
  double  GetBeamEnergy(void) const;
  int     GetBeamID    (void) const;
  TString GetBeamName  (void) const;
  int     GetEventID   (void) const;
  int     GetNTiles    (void) const;
  int     GetROtype    (void) const;
  int     GetRunNumber (void) const;
  Tile*   GetTile      (int);
  int     GetTimeStamp (void) const;

  
  void    SetBeamEnergy(double);
  void    SetBeamID    (int);
  void    SetBeamName  (TString);
  void    SetEventID   (int);
  void    SetROtype    (int);
  void    SetRunNumber (int);
  void    SetTimeStamp (int);

 private:

  double                BeamEnergy;
  int                   BeamID;
  TString               BeamName;
  int                   EventID;
  int                   ROtype;
  int                   RunNumber;
  std::map<int, Tile* > Tiles;
  int                   TimeStamp;
  
 protected:
  

};


#endif
