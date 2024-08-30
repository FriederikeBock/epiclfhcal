#! /bin/bash

if [ $1 == "allEScan" ]; then
  for i in 52 ; do
#   for i in 47 48 50 51 52 53 54 74 75 76 77 78 79 80 81 82 142 143 144 145 146 147 148 149 ; do
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
  
elif [ $1 == "electron" ]; then  
#   for i in 99 100 101 102 104 124 125 ; do
#     root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"/media/fbock/FrediExt4TB/LFHCAL_TB/202309_SPSH4/electron60GeV-VoltageScan/Run$i\_list.root\"\,\"ProcessedData/ElectronVoltageScan\"\,$i\,0\,0\,0\,1\,\"configs/mappingTBSetup_SPS1stdays_extended.txt\"\)
#   done

#   for i in 105 106 107 108 ; do
#     root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"/media/fbock/FrediExt4TB/LFHCAL_TB/202309_SPSH4/electron60GeV_GainScan/Run$i\_list.root\"\,\"ProcessedData/ElectronVoltageScan\"\,$i\,0\,0\,0\,1\,\"configs/mappingTBSetup_SPS1stdays_extended.txt\"\)
#   done

#   for i in 55 56 57 58 59 ; do  
#   for i in 60 61 62 63 64 ; do
#   for i in 65 66 67 68 69 ; do
#   for i in 70 71 72 73 83; do
#   for i in 85 86 87 88 89; do
#   for i in 90 91 140 141; do
#   for i in 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 83 85 86 87 88 89 90 91 140 141 ; do
#     root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"/media/fbock/FrediExt4TB/LFHCAL_TB/202309_SPSH4/electron_EnergyScan/Run$i\_list.root\"\,\"ProcessedData/ElectronEnergyScan\"\,$i\,0\,0\,0\,1\,\"configs/mappingTBSetup_SPS1stdays_extended.txt\"\,kFALSE\)
#   done
# 
  for i in 3 20 21 22 23 24 150 152 153 ; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"/media/fbock/FrediExt4TB/LFHCAL_TB/202309_SPSH4/electrons100GeV_positionScan/Run$i\_list.root\"\,\"ProcessedData/ElectronEnergyScan\"\,$i\,0\,0\,0\,1\,\"configs/mappingTBSetup_SPS1stdays_extended.txt\"\,kFALSE\)
  done
elif [ $1 == "ORNLstack" ]; then
  for i in 525 ; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"TestingORNLLocal/SingleTileCosmicsStack/Run$i\_list.root\"\,\"ProcessedData/ORNLSingleStackCosmics\"\,$i\,0\,0\,1\,2\,\"configs/mappingORNLTestSetup_SingleTIle_240617.txt\"\,kTRUE\,\"configs/ORNL_RunNumbers_May2024.txt\"\)
  done

  
elif [ $1 == "compVov" ]; then
  
  root -b -x -q -l 'CompareDifferentRuns.C+("configs/gainScan2.txt","CompareGain/",0,1,"configs/mappingTBSetup_SPS1stdays_extended.txt","configs/SPS_RunNumbers.txt")'
elif [ $1 == "compAll" ]; then
  
  root -b -x -q -l 'CompareDifferentRuns.C+("configs/gainAll.txt","CompareAll/",0,1,"configs/mappingTBSetup_SPS1stdays_extended.txt","configs/SPS_RunNumbers.txt")'
elif [ $1 == "compElec" ]; then
  
  root -b -x -q -l 'CompareDifferentRuns.C+("configs/allElectron.txt","CompareElec/",0,1,"configs/mappingTBSetup_SPS1stdays_extended.txt","configs/SPS_RunNumbers.txt")'
  root -b -x -q -l 'CompareDifferentRuns.C+("configs/electrons100GeV.txt","CompareElec100GeV/",0,1,"configs/mappingTBSetup_SPS1stdays_extended.txt","configs/SPS_RunNumbers.txt")'
else 
  root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"202309_SPSH4/pionsEnergyScan/Run$1\_list.root\"\,\"ProcessedData/PionEnergyScan\"\,$1\,0\,0\,0\,1\,\"configs/mappingORNLTestSetup_SingleTIle_240617.txt\"\,kFALSE\)
fi
