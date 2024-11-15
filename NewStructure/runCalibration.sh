#! /bin/bash
dataDir=""
if [ $1 = "fbock" ]; then 
	dataDir=/home/fbock/EIC/Analysis/LFHCalTB2024/CAENdata/MuonRuns
else
	echo "Please select a known user name, otherwise I don't know where the data is"
	exit
fi

# pedestal runs 
pedestalRuns='303 306 308 311 315 271 277 420 454 528 552 553 332 369 377 404 465 476 492 505 521'

if [ $2 = "pedestal" ]; then
	for runNr in $pedestalRuns; do
		./Analyse -d 1 -p -i $dataDir/raw_$runNr.root -f -o $dataDir/PedestalCalib_$runNr.root -O ../PlotsCalib/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
	done;
fi
	
# ./Analyse -d 1 -p -i $dataDir/raw_271.root -f -o $dataDir/PedestalCalib_271.root -O ../PlotsCalib/Run271 -r ../configs/DataTakingDB_202409_CAEN.csv
# ./Analyse -c $dataDir/Run$pedScanD1_45V\_list.txt -o $dataDir/raw_$pedScanD1_45V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDir/raw_$pedScanD1_45V.root -f -o $dataDir/PedestalCalib_$pedScanD1_45V.root
# ./Analyse -c $dataDir/Run$pedScanG_46V\_list.txt -o $dataDir/raw_$pedScanG_46V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDir/raw_$pedScanG_46V.root -f -o $dataDir/PedestalCalib_$pedScanG_46V.root
# ./Analyse -c $dataDir/Run$pedHVScan_40V\_list.txt -o $dataDir/raw_$pedHVScan_40V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDir/raw_$pedHVScan_40V.root -f -o $dataDir/PedestalCalib_$pedHVScan_40V.root

if [ $2 == "mergemuons" ]; then
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
pedHVScan_44V='303'

if [ $2 == "muoncalib" ]; then
	echo "running muon calib for 44V runs"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedHVScan_44V.root -i $dataDir/raw_$muonHVScan_44V.root -o $dataDir/rawPed_$muonHVScan_44V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
	./Analyse -f -d 1 -s -i $dataDir/rawPed_$muonHVScan_44V.root -o $dataDir/rawPedAndMuon_$muonHVScan_44V.root -O ../PlotsCalibMuon/muonHVScan_44V -r ../configs/DataTakingDB_202409_CAEN.csv

fi

# muonHVScan_43V='307'
# pedHVScan_43V='306'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 43V runs"
# # 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedHVScan_43V.root -i $dataDir/raw_$muonHVScan_43V.root -o $dataDir/rawPed_$muonHVScan_43V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 1 -s -i $dataDir/rawPed_$muonHVScan_43V.root -o $dataDir/rawPedAndMuon_$muonHVScan_43V.root -O ../PlotsCalibMuon/muonHVScan_43V -r ../configs/DataTakingDB_202409_CAEN.csv
# # 
# fi
# muonHVScan_42V='309'
# pedHVScan_42V='308'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 42V runs"
# # 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedHVScan_42V.root -i $dataDir/raw_$muonHVScan_42V.root -o $dataDir/rawPed_$muonHVScan_42V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 1 -s -i $dataDir/rawPed_$muonHVScan_42V.root -o $dataDir/rawPedAndMuon_$muonHVScan_42V.root -O ../PlotsCalibMuon/muonHVScan_42V -r ../configs/DataTakingDB_202409_CAEN.csv
# # 
# fi
# 
# muonHVScan_41V='312'
# pedHVScan_41V='311'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 41V runs"
# # 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedHVScan_41V.root -i $dataDir/raw_$muonHVScan_41V.root -o $dataDir/rawPed_$muonHVScan_41V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 1 -s -i $dataDir/rawPed_$muonHVScan_41V.root -o $dataDir/rawPedAndMuon_$muonHVScan_41V.root -O ../PlotsCalibMuon/muonHVScan_41V -r ../configs/DataTakingDB_202409_CAEN.csv
# # 
# fi
# muonHVScan_40V='316'
# pedHVScan_40V='315'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 40V runs"
# # 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedHVScan_40V.root -i $dataDir/raw_$muonHVScan_40V.root -o $dataDir/rawPed_$muonHVScan_40V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 1 -s -i $dataDir/rawPed_$muonHVScan_40V.root -o $dataDir/rawPedAndMuon_$muonHVScan_40V.root -O ../PlotsCalibMuon/muonHVScan_40V -r ../configs/DataTakingDB_202409_CAEN.csv
# # 
# fi
# muonScanA_45V='244 250 282 283'
# pedScanA_45V='271 277'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 45V runs, campaing A1"
# # 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_271.root -i $dataDir/raw_muonScanA1_45V.root -o $dataDir/rawPed_muonScanA1_45V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanA1_45V.root -o $dataDir/rawPedAndMuon_muonScanA1_45V.root -O ../PlotsCalibMuon/muonScanA1_45V -r ../configs/DataTakingDB_202409_CAEN.csv
# 	echo "running muon calib for 45V runs, campaing A2"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_277.root -i $dataDir/raw_muonScanA2_45V.root -o $dataDir/rawPed_muonScanA2_45V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanA2_45V.root -o $dataDir/rawPedAndMuon_muonScanA2_45V.root -O ../PlotsCalibMuon/muonScanA2_45V -r ../configs/DataTakingDB_202409_CAEN.csv
# 	
# fi

# muonScanD1_45V='412 417'
# pedScanD1_45V='420'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 45V runs, campaing D1"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanD1_45V.root -i $dataDir/raw_muonScanD1_45V.root -o $dataDir/rawPed_muonScanD1_45V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanD1_45V.root -o $dataDir/rawPedAndMuon_muonScanD1_45V.root -O ../PlotsCalibMuon/muonScanD1_45V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanD2_45V='460 456 457'
# pedScanD2_45V='454'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 45V runs, campaing D2"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanD2_45V.root -i $dataDir/raw_muonScanD2_45V.root -o $dataDir/rawPed_muonScanD2_45V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanD2_45V.root -o $dataDir/rawPedAndMuon_muonScanD2_45V.root -O ../PlotsCalibMuon/muonScanD2_45V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanH1_45V='526 527'
# pedScanH1_45V='528'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 45V runs, campaing H1"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanH1_45V.root -i $dataDir/raw_muonScanH1_45V.root -o $dataDir/rawPed_muonScanH1_45V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanH1_45V.root -o $dataDir/rawPedAndMuon_muonScanH1_45V.root -O ../PlotsCalibMuon/muonScanH1_45V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanH2_45V='554 559'
# pedScanH2_45V='552 553'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 45V runs, campaing H2"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanH2_45V.root -i $dataDir/raw_muonScanH2_45V.root -o $dataDir/rawPed_muonScanH2_45V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanH2_45V.root -o $dataDir/rawPedAndMuon_muonScanH2_45V.root -O ../PlotsCalibMuon/muonScanH2_45V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# 
# muonScanB1_42V='331 322'
# pedScanB1_42V='332'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 42V runs, campaing B1"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanB1_42V.root -i $dataDir/raw_muonScanB1_42V.root -o $dataDir/rawPed_muonScanB1_42V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanB1_42V.root -o $dataDir/rawPedAndMuon_muonScanB1_42V.root -O ../PlotsCalibMuon/muonScanB1_42V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanB2_42V='370 371 374'
# pedScanB2_42V='369'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 42V runs, campaing B2"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanB2_42V.root -i $dataDir/raw_muonScanB2_42V.root -o $dataDir/rawPed_muonScanB2_42V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanB2_42V.root -o $dataDir/rawPedAndMuon_muonScanB2_42V.root -O ../PlotsCalibMuon/muonScanB2_42V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanC1_43_5V='376 375'
# pedScanC1_43_5V='377'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 43.5V runs, campaing C1"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanC1_43_5V.root -i $dataDir/raw_muonScanC1_43_5V.root -o $dataDir/rawPed_muonScanC1_43_5V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanC1_43_5V.root -o $dataDir/rawPedAndMuon_muonScanC1_43_5V.root -O ../PlotsCalibMuon/muonScanC1_43_5V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanC2_43_5V='405 510 408'
# pedScanC2_43_5V='404'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 43.5V runs, campaing C2"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanC2_43_5V.root -i $dataDir/raw_muonScanC2_43_5V.root -o $dataDir/rawPed_muonScanC2_43_5V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanC2_43_5V.root -o $dataDir/rawPedAndMuon_muonScanC2_43_5V.root -O ../PlotsCalibMuon/muonScanC2_43_5V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanE1_40V='463 464'
# pedScanE1_40V='465'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 40V runs, campaing E1"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanE1_40V.root -i $dataDir/raw_muonScanE1_40V.root -o $dataDir/rawPed_muonScanE1_40V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanE1_40V.root -o $dataDir/rawPedAndMuon_muonScanE1_40V.root -O ../PlotsCalibMuon/muonScanE1_40V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanE2_40V='481 478'
# pedScanE2_40V='476'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 40V runs, campaing E2"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanE2_40V.root -i $dataDir/raw_muonScanE2_40V.root -o $dataDir/rawPed_muonScanE2_40V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanE2_40V.root -o $dataDir/rawPedAndMuon_muonScanE2_40V.root -O ../PlotsCalibMuon/muonScanE2_40V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanF1_41V='486 489'
# pedScanF1_41V='492'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 41V runs, campaing F1"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanF1_41V.root -i $dataDir/raw_muonScanF1_41V.root -o $dataDir/rawPed_muonScanF1_41V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanF1_41V.root -o $dataDir/rawPedAndMuon_muonScanF1_41V.root -O ../PlotsCalibMuon/muonScanF1_41V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# muonScanF2_41V='507 506'
# pedScanF2_41V='505'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 41V runs, campaing F2"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanF2_41V.root -i $dataDir/raw_muonScanF2_41V.root -o $dataDir/rawPed_muonScanF2_41V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanF2_41V.root -o $dataDir/rawPedAndMuon_muonScanF2_41V.root -O ../PlotsCalibMuon/muonScanF2_41V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# 
# muonScanG_46V='508 510 511 525'
# pedScanG_46V='521'
# if [ $2 == "muoncalib" ]; then
# 	echo "running muon calib for 46V runs, campaing B2"
# 	./Analyse -d 1 -f -P $dataDir/PedestalCalib_$pedScanG_46V.root -i $dataDir/raw_muonScanG_46V.root -o $dataDir/rawPed_muonScanG_46V.root -O ../PlotsCalibMuon/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
# 	./Analyse -f -d 2 -s -i $dataDir/rawPed_muonScanG_46V.root -o $dataDir/rawPedAndMuon_muonScanG_46V.root -O ../PlotsCalibMuon/muonScanG_46V -r ../configs/DataTakingDB_202409_CAEN.csv
# fi
# 
