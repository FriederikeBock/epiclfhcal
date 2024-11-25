#! /bin/bash
dataDirRaw=""
dataDirOut=""
baseDir=""
if [ $1 = "fbock" ]; then 
  baseDir=/media/fbock/Samsung_T5/LFHCAL_TB/202309_SPSH4
else
	echo "Please select a known user name, otherwise I don't know where the data is"
	exit
fi

#mapping file ../configs/mappingTBSetupNewStructure_Sept2023_SPSfirstdays.txt 
#run list file  ../configs/DataTakingDB_TBSept2023_SPSH4.csv

# pedestal runs 
if [ $2 = "pedestal" ]; then

  dataDirOut=$baseDir/Calibrations
#   pedestalRuns='47 48 50 51 52 53 54 74 75 76 77 78 79 80 81 82 142 143 144 145 146 147 148 149' 
#   dataDirRaw=$baseDir/pionsEnergyScan
#   for runNr in $pedestalRuns; do
#     ./Analyse -d 1 -y 2023 -p -i $dataDirRaw/raw_$runNr.root -f -o $dataDirOut/PedestalCalib_$runNr.root -O ../PlotsCalib_Sept2023/Run$runNr -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
#   done;
#   
#   pedestalRuns='120 121' 
#   dataDirRaw=$baseDir/pions300GeV_positionScan
#   for runNr in $pedestalRuns; do
#     ./Analyse -d 1 -y 2023 -p -i $dataDirRaw/raw_$runNr.root -f -o $dataDirOut/PedestalCalib_$runNr.root -O ../PlotsCalib_Sept2023/Run$runNr -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
#   done;
#   pedestalRuns='30 31 32 33 34 35 36 ' 
#   dataDirRaw=$baseDir/pions360GeV_PositionScan
#   for runNr in $pedestalRuns; do
#     ./Analyse -d 1 -y 2023 -p -i $dataDirRaw/raw_$runNr.root -f -o $dataDirOut/PedestalCalib_$runNr.root -O ../PlotsCalib_Sept2023/Run$runNr -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
#   done;

#   pedestalRuns='99 100 101 102 104 124 125' 
#   dataDirRaw=$baseDir/electron60GeV-VoltageScan
#   for runNr in $pedestalRuns; do
#     ./Analyse -d 1 -y 2023 -p -i $dataDirRaw/raw_$runNr.root -f -o $dataDirOut/PedestalCalib_$runNr.root -O ../PlotsCalib_Sept2023/Run$runNr -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
#   done;

#   pedestalRuns='105 106 107 108' 
#   dataDirRaw=$baseDir/electron60GeV_GainScan
#   for runNr in $pedestalRuns; do
#     ./Analyse -d 1 -y 2023 -p -i $dataDirRaw/raw_$runNr.root -f -o $dataDirOut/PedestalCalib_$runNr.root -O ../PlotsCalib_Sept2023/Run$runNr -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
#   done;
# 
#   pedestalRuns='55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 83 85 86 87 88 89 90 91 140 141' 
#   dataDirRaw=$baseDir/electron_EnergyScan
#   for runNr in $pedestalRuns; do
#     ./Analyse -d 1 -y 2023 -p -i $dataDirRaw/raw_$runNr.root -f -o $dataDirOut/PedestalCalib_$runNr.root -O ../PlotsCalib_Sept2023/Run$runNr -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
#   done;

  pedestalRuns='3 20 21 22 23 24 150 152 153'
  dataDirRaw=$baseDir/electrons100GeV_positionScan
  for runNr in $pedestalRuns; do
    ./Analyse -d 1 -y 2023 -p -i $dataDirRaw/raw_$runNr.root -f -o $dataDirOut/PedestalCalib_$runNr.root -O ../PlotsCalib_Sept2023/Run$runNr -r ../configs/DataTakingDB_TBSept2023_SPSH4.csv
  done;

fi


