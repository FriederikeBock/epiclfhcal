#! /bin/bash

dataDir=""
if [ $1 = "fbock" ]; then 
	dataDir=/home/fbock/EIC/Analysis/LFHCalTB2024/CAENdata/MuonRuns
else
	echo "Please select a known user name, otherwise I don't know where the data is"
	exit
fi

if [ $2 == "single" ]; then 
#   runs='244'
  runs='271'
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.txt -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingFile_202409_CAEN.txt -r ../configs/DataTakingDB_202409_CAEN.csv
  done;
else 
  runs='244	250	271	277	282	283	303	305	306	307	308	309	311	312	315	316	322	331	332	369	370	371	374	375	376	377	404	405	408	410	412	417	420	454	456	457	460	463	464	465	476	478	481	486	489	492	505	506	507	508	510	511	521	524	525	526	527	528	552	553	554	559' 
  for runNr in $runs; do 
    ./Analyse -c $dataDir/Run$runNr\_list.txt -o $dataDir/raw_$runNr.root -d 1 -f -m ../configs/mappingFile_202409_CAEN.txt -r ../configs/DataTakingDB_202409_CAEN.csv
  done;
fi
