#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
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
#include "Analyses.h"

Setup* Setup::instancePtr=nullptr;
//Calib* Calib::instancePtr=nullptr;

void PrintHelp(char* exe){
  std::cout<<"Usage:"<<std::endl;
  std::cout<<exe<<" [-option (arguments)]"<<std::endl;
  std::cout<<"Options:"<<std::endl;
  std::cout<<"-c xxx   Convert ASCII input file xxx into root format output"<<std::endl;
  std::cout<<"-C yyy   Apply calibrations stored in yyy root file to the input uncalibrated file"<<std::endl;
  std::cout<<"-p       Extract pedestal from input file and store it in output file"<<std::endl;
  std::cout<<"-P zzz   Correct pedestals stored in zzz root file and applied to input file"<<std::endl;
  std::cout<<"-i uuu   Input file in root format"<<std::endl;
  std::cout<<"-o vvv   Output file name (mandatory)"<<std::endl;
  std::cout<<"-O kkk   Output directory name for plots (mandatory)"<<std::endl;
  std::cout<<"-f       Force to write output if already exist"<<std::endl;
  std::cout<<"-m www   Name of mapping file  2024 PS TB [../configs/mappingFile_202409_CAEN.txt] "<<std::endl;
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
  Analyses AnAnalysis;
  int c;
  while((c=getopt(argc,argv,"c:psP:C:fo:O:m:d:i:r:h"))!=-1){
    switch(c){
    case 'c':
      std::cout<<"Convert ASCII input '"<<optarg<<"' to root format"<<std::endl;
      AnAnalysis.SetASCIIinput(Form("%s",optarg));
      AnAnalysis.IsToConvert(true);
      break;
    case 'p':
      std::cout<<"Extract pedestal from input data"<<std::endl;
      AnAnalysis.IsToExtractPedestal(true);
      break;
    case 's':
      std::cout<<"Extract scaling constant from input data"<<std::endl;
      AnAnalysis.IsToExtractScaling(true);
      break;
    case 'P':
      std::cout<<"Apply pedestal correction from: "<<optarg<<std::endl;
      AnAnalysis.SetRootPedestalInput(Form("%s",optarg));
      AnAnalysis.IsToApplyPedestalCorrection(true);
      break;
    case 'C':
      std::cout<<"Apply calibration (pedestal correction and scaling factor) from: "<<optarg<<std::endl;
      AnAnalysis.SetRootCalibInput(Form("%s",optarg));
      AnAnalysis.IsToApplyCalibration(true);
      break;
    case 'f':
      std::cout<<"If output already exists it will be overwritten"<<std::endl;
      AnAnalysis.CanOverWrite(true);
      break;
    case 'o':
      std::cout<<"Output to be saved in: "<<optarg<<std::endl;
      AnAnalysis.SetRootOutput(Form("%s",optarg));
      break;
    case 'O':
      std::cout<<"Outputdir plots to be saved in: "<<optarg<<std::endl;
      AnAnalysis.SetPlotOutputDir(Form("%s",optarg));
      break;
    case 'm':
      std::cout<<"Mapping file from: "<<optarg<<std::endl;
      AnAnalysis.SetMapInput(Form("%s",optarg));
      break;
    case 'r':
      std::cout<<"run list file from: "<<optarg<<std::endl;
      AnAnalysis.SetRunListInput(Form("%s",optarg));
      break;
    case 'i':
      std::cout<<"Root input file is: "<<optarg<<std::endl;
      AnAnalysis.SetRootInput(Form("%s",optarg));
      break;
    case 'd':
      std::cout<<"enable debug " << optarg <<std::endl;
      AnAnalysis.EnableDebug(atoi(optarg));
      break;
    case '?':
      std::cout<<"Option "<<optarg <<" not supported, will be ignored "<<std::endl;
      break;
    case 'h':
      PrintHelp(argv[0]);
      return 0;
    }
  }
  if(!AnAnalysis.CheckAndOpenIO()){
    std::cout<<"Check input and configurations, inconsistency or error with I/O detected"<<std::endl;
    PrintHelp(argv[0]);
    return -1;
  }

  AnAnalysis.Process();
  std::cout<<"Exiting"<<std::endl;
  return 0;
}
