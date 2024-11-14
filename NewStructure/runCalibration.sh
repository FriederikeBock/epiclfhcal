#! /bin/bash

dataDir=/home/fbock/EIC/Analysis/LFHCalTB2024/CAENdata/MuonRuns


# pedestal runs 
pedestalRuns='303 306 308 311 315 271 277 420 454 528 552 553 332 369 377 404 465 476 492 505 521'

if [ $1 = "pedestal" ]; then
	for runNr in $pedestalRuns; do
		./Analyse -d 1 -p -i $dataDir/raw_$runNr.root -f -o $dataDir/PedestalCalib_$runNr.root -O ../PlotsCalib/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
	done;
fi
	
pedHVScan_44V='303'
pedHVScan_43V='306'
pedHVScan_42V='308'
pedHVScan_41V='311'
pedHVScan_40V='315'

pedScanA_45V='271 277'
pedScanD1_45V='420'
pedScanD2_45V='454'
pedScanH1_45V='528'
pedScanH2_45V='552 553'
pedScanB1_42V='332'
pedScanB2_42V='369'
pedScanC1_43_5V='377'
pedScanC2_43_5V='404'
pedScanE1_40V='465'
pedScanE2_40V='476'
pedScanF1_41V='492'
pedScanF2_41V='505'
pedScanG_46V='521'

# ./Analyse -d 1 -p -i $dataDir/raw_271.root -f -o $dataDir/PedestalCalib_271.root -O ../PlotsCalib/Run271 -r ../configs/DataTakingDB_202409_CAEN.csv
# ./Analyse -c $dataDir/Run$pedScanD1_45V\_list.txt -o $dataDir/raw_$pedScanD1_45V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDir/raw_$pedScanD1_45V.root -f -o $dataDir/PedestalCalib_$pedScanD1_45V.root
# ./Analyse -c $dataDir/Run$pedScanG_46V\_list.txt -o $dataDir/raw_$pedScanG_46V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDir/raw_$pedScanG_46V.root -f -o $dataDir/PedestalCalib_$pedScanG_46V.root
# ./Analyse -c $dataDir/Run$pedHVScan_40V\_list.txt -o $dataDir/raw_$pedHVScan_40V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDir/raw_$pedHVScan_40V.root -f -o $dataDir/PedestalCalib_$pedHVScan_40V.root

if [ $1 == "mergemuons" ]; then
	hadd -f $dataDir/raw_muonScanA1_45V.root $dataDir/raw_244.root $dataDir/raw_250.root
	hadd -f $dataDir/raw_muonScanA2_45V.root $dataDir/raw_283.root $dataDir/raw_282.root
	hadd -f $dataDir/raw_muonScanD1_45V.root $dataDir/raw_412.root $dataDir/raw_417.root
	hadd -f $dataDir/raw_muonScanD2_45V.root $dataDir/raw_460.root $dataDir/raw_456.root $dataDir/raw_457.root
	hadd -f $dataDir/raw_muonScanH1_45V.root $dataDir/raw_526.root $dataDir/raw_527.root
	hadd -f $dataDir/raw_muonScanH2_45V.root $dataDir/raw_554.root $dataDir/raw_559.root
	hadd -f $dataDir/raw_muonScanB1_42V.root $dataDir/raw_331.root $dataDir/raw_322.root
	hadd -f $dataDir/raw_muonScanB2_42V.root $dataDir/raw_370.root $dataDir/raw_371.root $dataDir/raw_374.root
	hadd -f $dataDir/raw_muonScanC1_43_5V.root $dataDir/raw_376.root $dataDir/raw_375.root
	hadd -f $dataDir/raw_muonScanC2_43_5V.root $dataDir/raw_405.root $dataDir/raw_510.root $dataDir/raw_408.root
	hadd -f $dataDir/raw_muonScanE1_40V.root $dataDir/raw_464.root $dataDir/raw_463.root
	hadd -f $dataDir/raw_muonScanE2_40V.root $dataDir/raw_481.root $dataDir/raw_478.root
	hadd -f $dataDir/raw_muonScanF1_41V.root $dataDir/raw_486.root $dataDir/raw_489.root
	hadd -f $dataDir/raw_muonScanF2_41V.root $dataDir/raw_507.root $dataDir/raw_506.root
	hadd -f $dataDir/raw_muonScanG_46V.root $dataDir/raw_508.root $dataDir/raw_510.root $dataDir/raw_511.root $dataDir/raw_525.root
fi


# muon runs different scans in groups with separate pedestal
muonHVScan_44V='305'
muonHVScan_43V='307'
muonHVScan_42V='309'
muonHVScan_41V='312'
muonHVScan_40V='316'

muonScanA_45V='244 250 282 283'
# ./Analyse -d 1 -p -i $dataDir/raw_244.root -f -o $dataDir/PedestalCalib_244.root -O ../PlotsCalib/Run244 -r ../configs/DataTakingDB_202409_CAEN.csv

if [ $1 == "muoncalib" ]; then
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_271.root -i $dataDir/raw_muonScanA1_45V.root -o $dataDir/rawPed_muonScanA1_45V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
	./Analyse -f -d 1 -s -i $dataDir/rawPed_muonScanA1_45V.root -o $dataDir/rawPedAndMuon_muonScanA1_45V.root -O ../PlotsCalibMuon/muonScanA1_45V -r ../configs/DataTakingDB_202409_CAEN.csv

	
fi


muonScanD1_45V='412 417'
muonScanD2_45V='460 456 457'
muonScanH1_45V='526 527'
muonScanH2_45V='554 559'
muonScanB1_42V='331 322'
muonScanB2_42V='370 371 374'
muonScanC1_43_5V='376 375'
muonScanC2_43_5V='405 510 408'
muonScanE1_40V='463 464'
muonScanE2_40V='481 478'
muonScanF1_41V='486 489'
muonScanF2_41V='507 506'
muonScanG_46V='508 510 511 525'

