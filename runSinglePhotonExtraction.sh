#! /bin/bash

if [ $1 == "ORNLMulti" ]; then
    for i in 442 ; do
#   for i in 438 444 450 439 445 451 440 446 452 441 447 453 442 448 454 443 449 ; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"TestingORNLLocal/04_11_2024_SinglePhotonTest/Run$i\_list.root\"\,\"ProcessedData/SinglePhotonORNL\"\,$i\,0\,0\,0\,0\,\"configs/mappingA16_SinglePhoton.txt\"\,kTRUE\,\"configs/ORNL_RunNumbers.txt\"\)
  done
elif [ $1 == "new" ]; then
#   for i in 438 ; do
  for i in 438 439 440 441 442 443 444 450 445 446 447 448 449 451 452 453 454 ; do
    root -b -x -q -l makeSinglePhotonSpectraFitsFromJanusTree.C\+\(\"TestingORNLLocal/04_11_2024_SinglePhotonTest/Run$i\_list.root\"\,\"ProcessedData/SinglePhotonORNLNew\"\,$i\,0\,0\,1\,0\,\"configs/mappingA16_SinglePhoton.txt\"\,kTRUE\,\"configs/ORNL_RunNumbers.txt\"\)
  done
elif [ $1 == "FullA18" ]; then
#   for i in 438 ; do
  for i in 442 ; do
    root -b -x -q -l makeSinglePhotonSpectraFitsFromJanusTree.C\+\(\"TestingORNLLocal/05_27_2024_SinglePhoton/Run$i\_list.root\"\,\"ProcessedData/SinglePhotonORNL_May2024\"\,$i\,0\,0\,1\,0\,\"configs/mappingA18_SinglePhoton.txt\"\,kTRUE\,\"configs/ORNL_RunNumbers_May2024.txt\"\)
  done  
fi
