#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
//#include <unistd.h> // Add for use on Mac OS -> Same goes for Analyses.cc
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1D.h"
#include "TObjArray.h"
#include "TObjString.h"

#include "Setup.h"
#include "Calib.h"
#include "Event.h"
#include "Tile.h"
#include "HGCROC.h"
#include "Caen.h"
#include "ComparisonCalib.h"

Setup* Setup::instancePtr=nullptr; // Remove for use on Mac OS -> Add to Setup.cc instead
//Calib* Calib::instancePtr=nullptr;

void PrintHelp(char* exe){
  std::cout<<"Usage:"<<std::endl;
  std::cout<<exe<<" [-option (arguments)]"<<std::endl;
  std::cout<<"Options:"<<std::endl;
  std::cout<<"-i uuu   Input file in root format"<<std::endl;
  std::cout<<"-o vvv   Output file name (mandatory)"<<std::endl;
  std::cout<<"-O kkk   Output directory name for plots (mandatory)"<<std::endl;
  std::cout<<"-f       Force to write output if already exist"<<std::endl;
  std::cout<<"-r rrr   Name of run list file  2024 PS TB [../configs/DataTakingDB_202409_CAEN.csv] "<<std::endl;
  std::cout<<"-d [0-3] switch on debug info with debug level 0 to 3"<<std::endl;
  std::cout<<"-h       this help"<<std::endl<<std::endl;
  std::cout<<"Examples:"<<std::endl;
  std::cout<<exe<<" -c input.txt -o output.root (Convert ASCII to root)"<<std::endl;
  std::cout<<exe<<" -p (-f) -o OutputWithExtractedPedestal.root -i input.root (-f to overwrite existing output)"<<std::endl;
  std::cout<<exe<<" -s (-f) -o OutputWithMIPscaling.root -i input.root (-f to overwrite existing output)"<<std::endl;
  std::cout<<exe<<" -s (-f) -o OutputWithMIPscaling.root -P PedestalInput.root -i RawInput.root (-f to overwrite existing output)"<<std::endl;
  std::cout<<exe<<" -C Calibration.root (-f) -o CalibratedOutput.root -i Input.root (-f to overwrite existing output)"<<std::endl;
}
  

int main(int argc, char* argv[]){
  if(argc<4) {
    PrintHelp(argv[0]);
    return 0;
  }
  ComparisonCalib CompAnalysis;
  int c;
  while((c=getopt(argc,argv,"fo:O:d:i:r:h"))!=-1){
    switch(c){
    case 'f':
      std::cout<<"If output already exists it will be overwritten"<<std::endl;
      CompAnalysis.CanOverWrite(true);
      break;
    case 'o':
      std::cout<<"Output to be saved in: "<<optarg<<std::endl;
      CompAnalysis.SetRootOutput(Form("%s",optarg));
      break;
    case 'O':
      std::cout<<"Outputdir plots to be saved in: "<<optarg<<std::endl;
      CompAnalysis.SetPlotOutputDir(Form("%s",optarg));
      break;
    case 'r':
      std::cout<<"run list file from: "<<optarg<<std::endl;
      CompAnalysis.SetRunListInput(Form("%s",optarg));
      break;
    case 'i':
      std::cout<<"Root input file is: "<<optarg<<std::endl;
      CompAnalysis.SetRootInput(Form("%s",optarg));
      break;
    case 'd':
      std::cout<<"enable debug " << optarg <<std::endl;
      CompAnalysis.EnableDebug(atoi(optarg));
      break;
    case '?':
      std::cout<<"Option "<<optarg <<" not supported, will be ignored "<<std::endl;
      break;
    case 'h':
      PrintHelp(argv[0]);
      return 0;
    }
  }
  if(!CompAnalysis.CheckAndOpenIO()){
    std::cout<<"Check input and configurations, inconsistency or error with I/O detected"<<std::endl;
    PrintHelp(argv[0]);
    return -1;
  }

  CompAnalysis.Process();
  std::cout<<"Exiting"<<std::endl;
  return 0;
}
