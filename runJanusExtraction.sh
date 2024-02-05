#! /bin/bash

if [ $1 == "all" ]; then
  # 142 147 50 74 79 143 148 51 75 80 144 149 52 76 81 145 47 53 77 82 146 48 54 78
  for i in 142 147 50 74 79 143 148 51 75 80 144 149 52 76 81 145 47 53 77 82 146 48 54 78 ; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pionsEnergyScan/Run$i\_list.root\"\,\"ProcessedData/PionEnergyScan\"\,$i\,0\,0\,0\,1\,\"mappingTBSetup_SPS1stdays_extended.txt\"\)
  done
#position scan
elif [ $1 == "position" ]; then
  for i in 30 31 32 33 34 35 36; do
#   for i in 150 152 153; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pions360GeV_PositionScan/Run$i\_list.root\"\,\"ProcessedData/PionPositionScan\"\,$i\,0\,0\,0\,1\,\"mappingTBSetup_SPS1stdays_extended.txt\"\)
  done
#   for i in 120 121 ; do
#     root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pion300GeV_positionScan/Run$i\_list.root\"\,\"ProcessedData/PionPositionScan\"\,$i\,0\,0\,0\,1\,\"mappingTBSetup_SPS1stdays_extended.txt\"\)
#   done
else 
  root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pionsEnergyScan/Run$1\_list.root\"\,\"ProcessedData/PionEnergyScan\"\,$1\,0\,0\,0\,1\,\"mappingTBSetup_SPS1stdays_extended.txt\"\,kFALSE\)
fi
