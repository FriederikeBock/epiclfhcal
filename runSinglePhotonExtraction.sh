#! /bin/bash

if [ $1 == "ORNLMulti" ]; then
    for i in 442 ; do
#   for i in 438 444 450 439 445 451 440 446 452 441 447 453 442 448 454 443 449 ; do
    root -b -x -q -l makeSimplePlotsFromJanusTree.C\+\(\"TestingORNLLocal/04_11_2024_SinglePhotonTest/Run$i\_list.root\"\,\"ProcessedData/SinglePhotonORNL\"\,$i\,0\,0\,0\,0\,\"configs/mappingA16_SinglePhoton.txt\"\,kTRUE\,\"configs/ORNL_RunNumbers.txt\"\)
  done
fi
