#! /bin/bash
baseDir=/media/fbock/T7/202309_SPSH4


if [ $1 == "single" ]; then 
  runs='145'
  dataDir=$baseDir/pionsEnergyScan
  for runNr in $runs; do   
    ./Analyse -c $dataDir/Run$runNr\_list.root -o $dataDir/raw_$runNr.root -d 3 -f -m ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
    ./Analyse -d 1 -p -i $dataDir/raw_$runNr.root -f -o $dataDir/PedestalCalib_$runNr.root
  done;
else 
  runs='47 48 50 51 52 53 54 74 75 76 77 78 79 80 81 82 142 143 144 145 146 147 148 149' 
  dataDir=$baseDir/pionsEnergyScan
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.root -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
  done;
  runs='30 31 32 33 34 35 36 150 152 153' 
  dataDir=$baseDir/pion300GeV_positionScan
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.root -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
  done;
  runs='120 121' 
  dataDir=$baseDir/pions360GeV_PositionScan
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.root -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
  done;
  runs='99 100 101 102 104 124 125' 
  dataDir=$baseDir/electron60GeV-VoltageScan
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.root -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
  done;
  runs='105 106 107 108' 
  dataDir=$baseDir/electron60GeV_GainScan
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.root -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
  done;
  runs='55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 83 85 86 87 88 89 90 91 140 141' 
  dataDir=$baseDir/electron_EnergyScan
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.root -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
  done;
  runs='3 20 21 22 23 24 150 152 153'
  dataDir=$baseDir/electrons100GeV_positionScan
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.root -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
  done;
fi
