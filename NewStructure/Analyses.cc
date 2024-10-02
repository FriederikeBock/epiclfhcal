#include "Analyses.h"


bool Analyses::CheckAndOpenIO(void){
  int matchingbranch;
  if(!ASCIIinputName.IsNull()){
    ASCIIinput.open(ASCIIinputName.Data(),std::ios::in);
    if(!ASCIIinput.is_open()){
      std::cout<<"Could not open input file: "<<optarg<<std::endl;
      return false;
    }
  }

  
  //Need to check first input to get the setup...I do not think it is necessary
  if(!RootInputName.IsNull()){
    //File exist?
    RootInput=new TFile(RootInputName.Data(),"READ");
    if(RootInput->IsZombie()){
      std::cout<<"Error opening '"<<RootInputName<<"', does the file exist?"<<std::endl;
      return false;
    }

    //Retrieve info, start with setup
    TsetupIn = (TTree*) RootInput->Get("Setup");
    if(!TsetupIn){
      std::cout<<"Could not retrieve the Setup tree, leaving"<<std::endl;
      return false;
    }
    setup=Setup::GetInstance();
    std::cout<<"Setup add "<<setup<<std::endl;
    //matchingbranch=TsetupIn->SetBranchAddress("setup",&setup);
    matchingbranch=TsetupIn->SetBranchAddress("setup",&rswptr);
    if(matchingbranch<0){
      std::cout<<"Error retrieving Setup info from the tree"<<std::endl;
      return false;
    }
    std::cout<<"Entries "<<TsetupIn->GetEntries()<<std::endl;
    TsetupIn->GetEntry(0);
    setup->Initialize(*rswptr);
    std::cout<<"Reading "<<RootInput->GetName()<<std::endl;
    std::cout<<"Setup Info "<<setup->IsInit()<<"  and  "<<setup->GetCellID(0,0)<<std::endl;
    std::cout<<"Setup add now "<<setup<<std::endl;

    //Retrieve info, existing data?
    TdataIn = (TTree*) RootInput->Get("Data");
    if(!TdataIn){
      std::cout<<"Could not retrieve the Data tree, leaving"<<std::endl;
      return false;
    }
    matchingbranch=TdataIn->SetBranchAddress("event",&eventptr);
    if(matchingbranch<0){
      std::cout<<"Error retrieving Event info from the tree"<<std::endl;
      return false;
    }
    //We want to retrieve also calibration if do not specify ApplyPedestalCorrection from external file
    //In other words, the pedestal was potentially already done and we have an existing calib object
    if(!ApplyPedestalCorrection && ExtractScaling){
      TcalibIn = (TTree*) RootInput->Get("Calib");
      if(!TcalibIn){
	std::cout<<"Could not retrieve Calib tree, leaving"<<std::endl;
	return false;
      }
      //calib=Calib::GetInstance();
      matchingbranch=TcalibIn->SetBranchAddress("calib",&calibptr);
      if(matchingbranch<0){
	std::cout<<"Error retrieving calibration info from the tree"<<std::endl;
	return false;
      }
    }
    //All good
  }
  else if(!Convert){
    std::cout<<"Explicit Input option mandatory except for convertion ASCII -> ROOT"<<std::endl;
    return false;
  }
  
  //Setup Output files
  if(!RootOutputName.IsNull()){
    if(Overwrite) RootOutput=new TFile(RootOutputName.Data(),"RECREATE");
    else RootOutput = new TFile(RootOutputName.Data(),"CREATE");
    if(RootOutput->IsZombie()){
      std::cout<<"Error opening '"<<RootOutput<<"'no reachable path? Exist without force mode to overwrite?..."<<std::endl;
      return false;
    }
    TsetupOut = new TTree("Setup","Setup");
    setup=Setup::GetInstance();
    //TsetupOut->Branch("setup",&setup);
    TsetupOut->Branch("setup",&rsw);
    TdataOut = new TTree("Data","Data");
    TdataOut->Branch("event",&event);
    //if(!calib) calib=new Calib();
    //if(!calib)
    //calib=Calib::GetInstance();
    //Calib* calib=Calib::GetInstance();
    std::cout<<"Calib pointer is "<<calibptr<<std::endl;
    TcalibOut = new TTree("Calib","Calib");
    TcalibOut->Branch("calib",&calib);
  }
  else {
    std::cout<<"Output option mandatory except when converting"<<std::endl;
    return false;
  }
  
  if(!RootCalibInputName.IsNull()){
    RootCalibInput=new TFile(RootCalibInputName.Data(),"READ");
    if(RootCalibInput->IsZombie()){
      std::cout<<"Error opening '"<<RootCalibInputName<<"', does the file exist?"<<std::endl;
      return false;
    }
    TcalibIn = (TTree*) RootCalibInput->Get("Calib");
    if(!TcalibIn){
      std::cout<<"Could not retrieve Calib tree, leaving"<<std::endl;
      return false;
    }
    matchingbranch=TcalibIn->SetBranchAddress("calib",&calibextra);
    if(matchingbranch<0){
      std::cout<<"Error retrieving calibration info from the tree"<<std::endl;
      return false;
    }
    
  }

  if(!RootPedestalInputName.IsNull()){
    RootPedestalInput = new TFile(RootPedestalInputName.Data(),"READ");
    if(RootPedestalInput->IsZombie()){
      std::cout<<"Error opening '"<<RootPedestalInputName<<"', does the file exist?"<<std::endl;
      return false;
    }
    TcalibIn = (TTree*) RootPedestalInput->Get("Calib");
    if(!TcalibIn){
      std::cout<<"Could not retrieve Calib tree, leaving"<<std::endl;
      return false;
    }
    matchingbranch=TcalibIn->SetBranchAddress("calib",&calib);
    if(matchingbranch<0){
      std::cout<<"Error retrieving calibration info from the tree"<<std::endl;
      return false;
    }

  }
  if(!MapInputName.IsNull()){
    MapInput.open(MapInputName.Data(),std::ios::in);
    if(!MapInput.is_open()){
      std::cout<<"Could not open mapping file: "<<MapInputName<<std::endl;
      return false;
    }	
  }
  return true;    
}


bool Analyses::Process(void){
  bool status;
  if(Convert){
    status=ConvertASCII2Root();
  }
  if(ExtractPedestal){
    status=GetPedestal();
  }
  if(ApplyPedestalCorrection){
    status=CorrectPedestal();
  }
  if(ExtractScaling){
    status=GetScaling();
  }
  if(ApplyCalibration){
    status=Calibrate();
  }
  return status;
}

bool Analyses::ConvertASCII2Root(void){
  //Init first
  setup->Initialize("../configs/MyMapping.txt");
  event.SetRunNumber(300);
  event.SetROtype(2);
  event.SetBeamName("toto");
  event.SetBeamID(2212);
  event.SetBeamEnergy(5.);
  for(int toto=0; toto<100; toto++){
    Caen aTile;
    aTile.SetADCHigh(42);
    aTile.SetADCLow(-1);
    aTile.SetCellID(0);
    event.AddTile(new Caen(aTile));
    TdataOut->Fill();
    aTile.SetADCHigh(421);
    aTile.SetADCLow(43);
    aTile.SetCellID(0);
    event.AddTile(new Caen(aTile));
    TdataOut->Fill();
    event.ClearTiles();
  }
  //CaenTiles->push_back(aTile);
  RootOutput->cd();
  RootSetupWrapper rswtmp=RootSetupWrapper(setup);
  rsw=rswtmp;
  TsetupOut->Fill();
  TcalibOut->Fill();
  TcalibOut->Write();
  TdataOut->Fill();
  TsetupOut->Write();
  TdataOut->Write();

  RootOutput->Close();
  return true;
}

bool Analyses::GetPedestal(void){
  std::cout<<"GetPedestal"<<std::endl;
  int evts=TdataIn->GetEntries();
  for(int i=0; i<evts; i++){
    TdataIn->GetEntry(i);
    //std::cout<<event.GetRunNumber()<<std::endl;
    //std::cout<<"Which has "<<event.GetNTiles()<<std::endl;
    //for(int j=0; j<event.GetNTiles(); j++){
    //  Caen* aTile=(Caen*)event.GetTile(j);
    //  std::cout<<aTile->GetADCHigh()<<"\t"<<aTile->GetADCLow()<<std::endl;
    //}
    RootOutput->cd();
    TdataOut->Fill();
  }
  TdataOut->Write();
  //TsetupIn->GetEntry(1);
  std::cout<<"Setup add fin "<<setup<<std::endl;
  std::cout<<"Setup Info"<<setup->GetCellID(0,0)<<std::endl;
  //TsetupOut->Fill();
  //TsetupOut->Write();
  TsetupIn->CloneTree()->Write();
  RootOutput->Close();
  RootInput->Close();
  return true;
}
bool Analyses::CorrectPedestal(void){
  return true;
}
bool Analyses::GetScaling(void){
  return true;
}
bool Analyses::Calibrate(void){
  return true;
}
