#! /bin/bash

if [ $1 == "allEScan" ]; then
  for i in 47 48 50 51 52 53 54 74 75 76 77 78 79 80 81 82 142 143 144 145 146 147 148 149 ; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pionsEnergyScan/Run$i\_list.root\"\,\"ProcessedData/PionEnergyScan\"\,$i\,0\,0\,0\,1\,\"configs/mappingTBSetup_SPS1stdays_extended.txt\"\)
  done
#position scan
elif [ $1 == "position" ]; then
  for i in 30 31 32 33 34 35 36 150 152 153; do
#   for i in 150 152 153; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pions360GeV_PositionScan/Run$i\_list.root\"\,\"ProcessedData/PionPositionScan\"\,$i\,0\,0\,0\,1\,\"configs/mappingTBSetup_SPS1stdays_extended.txt\"\)
  done
  for i in 120 121; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pion300GeV_positionScan/Run$i\_list.root\"\,\"ProcessedData/PionPositionScan\"\,$i\,0\,0\,0\,1\,\"configs/mappingTBSetup_SPS1stdays_extended.txt\"\)
  done
elif [ $1 == "compVov" ]; then
  
  root -b -x -q -l 'CompareDifferentRuns.C+("configs/gainScan2.txt","CompareGain/",0,1,"configs/mappingTBSetup_SPS1stdays_extended.txt","configs/SPS_RunNumbers.txt")'
  
else 
  root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pionsEnergyScan/Run$1\_list.root\"\,\"ProcessedData/PionEnergyScan\"\,$1\,0\,0\,0\,1\,\"configs/mappingTBSetup_SPS1stdays_extended.txt\"\,kFALSE\)
fi
