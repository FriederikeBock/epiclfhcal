#! /bin/bash
dataDirRaw=""
dataDirOut=""
if [ $1 = "fbock" ]; then 
	dataDirRaw=/media/fbock/Samsung_T5/LFHCAL_TB/202408_PST09/CAENdata/MuonRuns
	dataDirOut=/home/fbock/EIC/Analysis/LFHCalTB2024/CAENdata/MuonRuns
else
	echo "Please select a known user name, otherwise I don't know where the data is"
	exit
fi

# pedestal runs 
pedestalRuns='303 306 308 311 315 271 277 420 454 528 552 553 332 369 377 404 465 476 492 505 521'

if [ $2 = "pedestal" ]; then
	for runNr in $pedestalRuns; do
		./Analyse -d 1 -p -i $dataDirRaw/raw_$runNr.root -f -o $dataDirOut/PedestalCalib_$runNr.root -O ../PlotsCalib_2024/Run$runNr -r ../configs/DataTakingDB_202409_CAEN.csv
	done;
fi
	
# ./Analyse -d 1 -p -i $dataDirRaw/raw_271.root -f -o $dataDirOut/PedestalCalib_271.root -O ../PlotsCalib_2024/Run271 -r ../configs/DataTakingDB_202409_CAEN.csv
# ./Analyse -c $dataDirRaw/Run$pedScanD1_45V\_list.txt -o $dataDirRaw/raw_$pedScanD1_45V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDirRaw/raw_$pedScanD1_45V.root -f -o $dataDirOut/PedestalCalib_$pedScanD1_45V.root
# ./Analyse -c $dataDirRaw/Run$pedScanG_46V\_list.txt -o $dataDirRaw/raw_$pedScanG_46V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDirRaw/raw_$pedScanG_46V.root -f -o $dataDirOut/PedestalCalib_$pedScanG_46V.root
# ./Analyse -c $dataDirRaw/Run$pedHVScan_40V\_list.txt -o $dataDirRaw/raw_$pedHVScan_40V.root -d 1 -f
# ./Analyse -d 1 -p -i $dataDirRaw/raw_$pedHVScan_40V.root -f -o $dataDirOut/PedestalCalib_$pedHVScan_40V.root

if [ $2 == "mergemuons" ]; then
	hadd -f $dataDirRaw/raw_muonScanA1_45V.root $dataDirRaw/raw_244.root $dataDirRaw/raw_250.root
	hadd -f $dataDirRaw/raw_muonScanA2_45V.root $dataDirRaw/raw_283.root $dataDirRaw/raw_282.root
	hadd -f $dataDirRaw/raw_muonScanD1_45V.root $dataDirRaw/raw_412.root $dataDirRaw/raw_417.root
	hadd -f $dataDirRaw/raw_muonScanD2_45V.root $dataDirRaw/raw_460.root $dataDirRaw/raw_456.root $dataDirRaw/raw_457.root
	hadd -f $dataDirRaw/raw_muonScanH1_45V.root $dataDirRaw/raw_526.root $dataDirRaw/raw_527.root
	hadd -f $dataDirRaw/raw_muonScanH2_45V.root $dataDirRaw/raw_554.root $dataDirRaw/raw_559.root
	hadd -f $dataDirRaw/raw_muonScanB1_42V.root $dataDirRaw/raw_331.root $dataDirRaw/raw_322.root
	hadd -f $dataDirRaw/raw_muonScanB2_42V.root $dataDirRaw/raw_370.root $dataDirRaw/raw_371.root $dataDirRaw/raw_374.root
	hadd -f $dataDirRaw/raw_muonScanC1_43_5V.root $dataDirRaw/raw_376.root $dataDirRaw/raw_375.root
	hadd -f $dataDirRaw/raw_muonScanC2_43_5V.root $dataDirRaw/raw_405.root $dataDirRaw/raw_510.root $dataDirRaw/raw_408.root
	hadd -f $dataDirRaw/raw_muonScanE1_40V.root $dataDirRaw/raw_464.root $dataDirRaw/raw_463.root
	hadd -f $dataDirRaw/raw_muonScanE2_40V.root $dataDirRaw/raw_481.root $dataDirRaw/raw_478.root
	hadd -f $dataDirRaw/raw_muonScanF1_41V.root $dataDirRaw/raw_486.root $dataDirRaw/raw_489.root
	hadd -f $dataDirRaw/raw_muonScanF2_41V.root $dataDirRaw/raw_507.root $dataDirRaw/raw_506.root
	hadd -f $dataDirRaw/raw_muonScanG_46V.root $dataDirRaw/raw_508.root $dataDirRaw/raw_510.root $dataDirRaw/raw_511.root $dataDirRaw/raw_525.root
fi


# muon runs different scans in groups with separate pedestal
muonHVScan_44V='305'
pedHVScan_44V='303'

if [ $2 == "muoncalib" ] || [ $2 == "muoncalibHV44" ]; then
	echo "running muon calib for 44V runs"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedHVScan_44V.root -i $dataDirRaw/raw_$muonHVScan_44V.root -o $dataDirRaw/rawPed_$muonHVScan_44V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_$muonHVScan_44V.root -o $dataDirOut/rawPedAndMuon_$muonHVScan_44V.root -O ../PlotsCalibMuon_2024/muonHVScan_44V -r ../configs/DataTakingDB_202409_CAEN.csv

fi

muonHVScan_43V='307'
pedHVScan_43V='306'
if [ $2 == "muoncalib" ] || [ $2 == "muoncalibHV43" ]; then
	echo "running muon calib for 43V runs"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedHVScan_43V.root -i $dataDirRaw/raw_$muonHVScan_43V.root -o $dataDirRaw/rawPed_$muonHVScan_43V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_$muonHVScan_43V.root -o $dataDirOut/rawPedAndMuon_$muonHVScan_43V.root -O ../PlotsCalibMuon_2024/muonHVScan_43V -r ../configs/DataTakingDB_202409_CAEN.csv
# 
fi
muonHVScan_42V='309'
pedHVScan_42V='308'
if [ $2 == "muoncalib" ] || [ $2 == "muoncalibHV42" ]; then
	echo "running muon calib for 42V runs"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedHVScan_42V.root -i $dataDirRaw/raw_$muonHVScan_42V.root -o $dataDirRaw/rawPed_$muonHVScan_42V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_$muonHVScan_42V.root -o $dataDirOut/rawPedAndMuon_$muonHVScan_42V.root -O ../PlotsCalibMuon_2024/muonHVScan_42V -r ../configs/DataTakingDB_202409_CAEN.csv
# 
fi

muonHVScan_41V='312'
pedHVScan_41V='311'
if [ $2 == "muoncalib" ] || [ $2 == "muoncalibHV41" ]; then
	echo "running muon calib for 41V runs"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedHVScan_41V.root -i $dataDirRaw/raw_$muonHVScan_41V.root -o $dataDirRaw/rawPed_$muonHVScan_41V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_$muonHVScan_41V.root -o $dataDirOut/rawPedAndMuon_$muonHVScan_41V.root -O ../PlotsCalibMuon_2024/muonHVScan_41V -r ../configs/DataTakingDB_202409_CAEN.csv
fi

muonHVScan_40V='316'
pedHVScan_40V='315'
if [ $2 == "muoncalib" ] || [ $2 == "muoncalibHV40" ]; then
	echo "running muon calib for 40V runs"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedHVScan_40V.root -i $dataDirRaw/raw_$muonHVScan_40V.root -o $dataDirRaw/rawPed_$muonHVScan_40V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_$muonHVScan_40V.root -o $dataDirOut/rawPedAndMuon_$muonHVScan_40V.root -O ../PlotsCalibMuon_2024/muonHVScan_40V -r ../configs/DataTakingDB_202409_CAEN.csv
fi

muonScanA_45V='244 250 282 283'
pedScanA_45V='271 277'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibA" ]; then
	echo "running muon calib for 45V runs, campaing A1"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_271.root -i $dataDirRaw/raw_muonScanA1_45V.root -o $dataDirRaw/rawPed_muonScanA1_45V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanA1_45V.root -o $dataDirOut/rawPedAndMuon_muonScanA1_45V.root -O ../PlotsCalibMuon_2024/muonScanA1_45V -r ../configs/DataTakingDB_202409_CAEN.csv
	echo "running muon calib for 45V runs, campaing A2"
	./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_277.root -i $dataDirRaw/raw_muonScanA2_45V.root -o $dataDirRaw/rawPed_muonScanA2_45V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanA2_45V.root -o $dataDirOut/rawPedAndMuon_muonScanA2_45V.root -O ../PlotsCalibMuon_2024/muonScanA2_45V -r ../configs/DataTakingDB_202409_CAEN.csv
	
fi

muonScanD1_45V='412 417'
pedScanD1_45V='420'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibD" ]; then
	echo "running muon calib for 45V runs, campaing D1"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanD1_45V.root -i $dataDirRaw/raw_muonScanD1_45V.root -o $dataDirRaw/rawPed_muonScanD1_45V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanD1_45V.root -o $dataDirOut/rawPedAndMuon_muonScanD1_45V.root -O ../PlotsCalibMuon_2024/muonScanD1_45V -r ../configs/DataTakingDB_202409_CAEN.csv
fi
muonScanD2_45V='460 456 457'
pedScanD2_45V='454'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibD" ]; then
	echo "running muon calib for 45V runs, campaing D2"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanD2_45V.root -i $dataDirRaw/raw_muonScanD2_45V.root -o $dataDirRaw/rawPed_muonScanD2_45V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanD2_45V.root -o $dataDirOut/rawPedAndMuon_muonScanD2_45V.root -O ../PlotsCalibMuon_2024/muonScanD2_45V -r ../configs/DataTakingDB_202409_CAEN.csv
fi
muonScanH1_45V='526 527'
pedScanH1_45V='528'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibH" ]; then
	echo "running muon calib for 45V runs, campaing H1"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanH1_45V.root -i $dataDirRaw/raw_muonScanH1_45V.root -o $dataDirRaw/rawPed_muonScanH1_45V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanH1_45V.root -o $dataDirOut/rawPedAndMuon_muonScanH1_45V.root -O ../PlotsCalibMuon_2024/muonScanH1_45V -r ../configs/DataTakingDB_202409_CAEN.csv
fi

muonScanH2_45V='554 559'
# pedScanH2_45V='552 553'
pedScanH2_45V='552'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibH" ]; then
	echo "running muon calib for 45V runs, campaing H2"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanH2_45V.root -i $dataDirRaw/raw_muonScanH2_45V.root -o $dataDirRaw/rawPed_muonScanH2_45V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanH2_45V.root -o $dataDirOut/rawPedAndMuon_muonScanH2_45V.root -O ../PlotsCalibMuon_2024/muonScanH2_45V -r ../configs/DataTakingDB_202409_CAEN.csv
fi


muonScanB1_42V='331 322'
pedScanB1_42V='332'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibB" ]; then
	echo "running muon calib for 42V runs, campaing B1"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanB1_42V.root -i $dataDirRaw/raw_muonScanB1_42V.root -o $dataDirRaw/rawPed_muonScanB1_42V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanB1_42V.root -o $dataDirOut/rawPedAndMuon_muonScanB1_42V.root -O ../PlotsCalibMuon_2024/muonScanB1_42V -r ../configs/DataTakingDB_202409_CAEN.csv
fi

muonScanB2_42V='370 371 374'
pedScanB2_42V='369'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibB" ]; then
	echo "running muon calib for 42V runs, campaing B2"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanB2_42V.root -i $dataDirRaw/raw_muonScanB2_42V.root -o $dataDirRaw/rawPed_muonScanB2_42V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanB2_42V.root -o $dataDirOut/rawPedAndMuon_muonScanB2_42V.root -O ../PlotsCalibMuon_2024/muonScanB2_42V -r ../configs/DataTakingDB_202409_CAEN.csv
fi

muonScanC1_43_5V='376 375'
pedScanC1_43_5V='377'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibC" ]; then
	echo "running muon calib for 43.5V runs, campaing C1"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanC1_43_5V.root -i $dataDirRaw/raw_muonScanC1_43_5V.root -o $dataDirRaw/rawPed_muonScanC1_43_5V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanC1_43_5V.root -o $dataDirOut/rawPedAndMuon_muonScanC1_43_5V.root -O ../PlotsCalibMuon_2024/muonScanC1_43_5V -r ../configs/DataTakingDB_202409_CAEN.csv
fi
muonScanC2_43_5V='405 510 408'
pedScanC2_43_5V='404'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibC" ]; then
	echo "running muon calib for 43.5V runs, campaing C2"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanC2_43_5V.root -i $dataDirRaw/raw_muonScanC2_43_5V.root -o $dataDirRaw/rawPed_muonScanC2_43_5V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanC2_43_5V.root -o $dataDirOut/rawPedAndMuon_muonScanC2_43_5V.root -O ../PlotsCalibMuon_2024/muonScanC2_43_5V -r ../configs/DataTakingDB_202409_CAEN.csv
fi
muonScanE1_40V='463 464'
pedScanE1_40V='465'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibE" ]; then
	echo "running muon calib for 40V runs, campaing E1"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanE1_40V.root -i $dataDirRaw/raw_muonScanE1_40V.root -o $dataDirRaw/rawPed_muonScanE1_40V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanE1_40V.root -o $dataDirOut/rawPedAndMuon_muonScanE1_40V.root -O ../PlotsCalibMuon_2024/muonScanE1_40V -r ../configs/DataTakingDB_202409_CAEN.csv
fi
muonScanE2_40V='481 478'
pedScanE2_40V='476'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibE" ]; then
	echo "running muon calib for 40V runs, campaing E2"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanE2_40V.root -i $dataDirRaw/raw_muonScanE2_40V.root -o $dataDirRaw/rawPed_muonScanE2_40V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanE2_40V.root -o $dataDirOut/rawPedAndMuon_muonScanE2_40V.root -O ../PlotsCalibMuon_2024/muonScanE2_40V -r ../configs/DataTakingDB_202409_CAEN.csv
fi
muonScanF1_41V='486 489'
pedScanF1_41V='492'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibF" ]; then
	echo "running muon calib for 41V runs, campaing F1"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanF1_41V.root -i $dataDirRaw/raw_muonScanF1_41V.root -o $dataDirRaw/rawPed_muonScanF1_41V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanF1_41V.root -o $dataDirOut/rawPedAndMuon_muonScanF1_41V.root -O ../PlotsCalibMuon_2024/muonScanF1_41V -r ../configs/DataTakingDB_202409_CAEN.csv
fi
muonScanF2_41V='507 506'
pedScanF2_41V='505'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibF" ]; then
	echo "running muon calib for 41V runs, campaing F2"
	if [ $3 == "transfer" ]; then 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanF2_41V.root -i $dataDirRaw/raw_muonScanF2_41V.root -o $dataDirRaw/rawPed_muonScanF2_41V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanF2_41V.root -o $dataDirOut/rawPedAndMuon_muonScanF2_41V.root -O ../PlotsCalibMuon_2024/muonScanF2_41V -r ../configs/DataTakingDB_202409_CAEN.csv
fi
# 
muonScanG_46V='508 510 511 525'
pedScanG_46V='521'
if [ $2 == "muoncalibAll" ] || [ $2 == "muoncalibG" ]; then
	echo "running muon calib for 46V runs, campaing G"
	if 
		./Analyse -d 1 -f -P $dataDirOut/PedestalCalib_$pedScanG_46V.root -i $dataDirRaw/raw_muonScanG_46V.root -o $dataDirRaw/rawPed_muonScanG_46V.root -r ../configs/DataTakingDB_202409_CAEN.csv
	fi
	./Analyse -f -d 1 -s -i $dataDirRaw/rawPed_muonScanG_46V.root -o $dataDirOut/rawPedAndMuon_muonScanG_46V.root -O ../PlotsCalibMuon_2024/muonScanG_46V -r ../configs/DataTakingDB_202409_CAEN.csv
fi

