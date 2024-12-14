#include "Analyses.h"
#include <vector>
#include "TROOT.h"
//#include <unistd.h> // Add for use on Mac OS -> Same goes for Analyse.cc
#include "TF1.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TChain.h"
#include "TileSpectra.h"
#include "CommonHelperFunctions.h"
#include "PlottHelper.h"
#include "TRandom3.h"

// ****************************************************************************
// Checking and opening input and output files
// ****************************************************************************
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
    std::cout<<"Input name set to: '"<<RootInputName.Data() <<std::endl;
  
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
    //std::cout<<"Setup add now "<<setup<<std::endl;

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
    
    //Do I really want this?
    TcalibIn = (TTree*) RootInput->Get("Calib");
    if(!TcalibIn){
      std::cout<<"Could not retrieve Calib tree, leaving"<<std::endl;
      //return false;
    }
    else {
      matchingbranch=TcalibIn->SetBranchAddress("calib",&calibptr);
      if(matchingbranch<0){
        std::cout<<"Error retrieving calibration info from the tree"<<std::endl;
        TcalibIn=nullptr;
      }
    }
    //End of do I really want this?
    
    //We want to retrieve also calibration if do not specify ApplyPedestalCorrection from external file
    //In other words, the pedestal was potentially already done and we have an existing calib object
    if((!ApplyPedestalCorrection && ExtractScaling) || (!ApplyPedestalCorrection && ExtractScalingImproved)){
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
    if (!Convert){
      TString temp = RootOutputName;
      temp         = temp.ReplaceAll(".root","_Hists.root");
      SetRootOutputHists(temp);
      std::cout << "creating additional histo file: " << RootOutputNameHist.Data() << " tree in : "<< RootOutputName.Data() << std::endl;
    }
    
    bool sCOF = CreateOutputRootFile();
    if (!sCOF) return false;
    
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
    //std::cout<<"Calib pointer is "<<calibptr<<std::endl;
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
    matchingbranch=TcalibIn->SetBranchAddress("calib",&calibptr);
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
    matchingbranch=TcalibIn->SetBranchAddress("calib",&calibptr);
    if(matchingbranch<0){
      std::cout<<"Error retrieving calibration info from the tree"<<std::endl;
      return false;
    }
    //std::cout<<"Did the address changed? "<<&calib<<std::endl;
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

// ****************************************************************************
// Primary process function to start all calibrations
// ****************************************************************************
bool Analyses::Process(void){
  bool status;
  ROOT::EnableImplicitMT();
  
  if(Convert){
    if (!(GetASCIIinputName().EndsWith(".root"))){
      status=ConvertASCII2Root();
    } else {
      std::cout << "WARNING: This option should only be used for the 2023 SPS test beam for which the CAEN raw data was lost!" << std::endl;
      status=ConvertOldRootFile2Root();
    }
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
  if (ExtractScalingImproved){
    status=GetImprovedScaling();
  }
  
  if(ApplyCalibration){
    status=Calibrate();
  }
  return status;
}


// ****************************************************************************
// convert original ASCII file from CAEN output to root format
// ****************************************************************************
bool Analyses::ConvertASCII2Root(void){
  //============================================
  //Init first
  //============================================
  // initialize setup
  if (MapInputName.CompareTo("")== 0) {
      std::cerr << "ERROR: No mapping file has been provided, please make sure you do so! Aborting!" << std::endl;
      return false;
  }
  setup->Initialize(MapInputName.Data(),debug);
  // initialize run number file
  if (RunListInputName.CompareTo("")== 0) {
      std::cerr << "ERROR: No run list file has been provided, please make sure you do so! Aborting!" << std::endl;
      return false;
  }
  std::map<int,RunInfo> ri=readRunInfosFromFile(RunListInputName.Data(),debug,0);
  
  // Clean up file headers
  TObjArray* tok=ASCIIinputName.Tokenize("/");
  // get file name
  TString file=((TObjString*)tok->At(tok->GetEntries()-1))->String();
  delete tok;
  tok=file.Tokenize("_");
  TString header=((TObjString*)tok->At(0))->String();
  delete tok;
  
  // Get run number from file & find necessary run infos
  TString RunString=header(3,header.Sizeof());
  std::map<int,RunInfo>::iterator it=ri.find(RunString.Atoi());
  //std::cout<<RunString.Atoi()<<"\t"<<it->second.species<<std::endl;
  event.SetRunNumber(RunString.Atoi());
  event.SetROtype(2);
  event.SetBeamName(it->second.species);
  event.SetBeamID(it->second.pdg);
  event.SetBeamEnergy(it->second.energy);
  event.SetVop(it->second.vop);
  event.SetVov(it->second.vop-it->second.vbr);
  event.SetBeamPosX(it->second.posX);
  event.SetBeamPosY(it->second.posY);
  calib.SetRunNumber(RunString.Atoi());
  calib.SetVop(it->second.vop);
  calib.SetVov(it->second.vop-it->second.vbr);  
  
  //============================================
  // Start decoding file
  //============================================
  TString aline         = "";
  TString versionCAEN   = "";
  TObjArray* tokens;
  std::map<int,std::vector<Caen> > tmpEvent;
  std::map<int,double> tmpTime;
  std::map<int,std::vector<Caen> >::iterator itevent;
  long tempEvtCounter = 0;
  while(!ASCIIinput.eof()){                                                     // run till end of file is reached and read line by line
    aline.ReadLine(ASCIIinput);
    if(!ASCIIinput.good()) break;
    if(aline[0]=='/'){
      if (aline.Contains("File Format Version")){
        tokens      = aline.Tokenize(" ");
        versionCAEN = ((TObjString*)tokens->At(4))->String();
        std::cout << "File version: " << ((TObjString*)tokens->At(4))->String() << std::endl;
        
        if (!(versionCAEN.CompareTo("3.3") == 0 || versionCAEN.CompareTo("3.1") == 0)){
          std::cerr << "This version cannot be converted with the current code, please add the corresponding file format to the converter" << std::endl;
          tokens->Clear();
          delete tokens;
          return false;
        }  
        
        tokens->Clear();
        delete tokens;
      }
      else if(aline.Contains("Run start time")){
        tokens    = aline.Tokenize(" ");
        int year=((TObjString*)tokens->At(8))->String().Atoi();
        int month;
        TString Stringmonth=((TObjString*)tokens->At(5))->String();
        if(Stringmonth=="Jan") month=1;
        else if(Stringmonth=="Feb") month=2;
        else if(Stringmonth=="Mar") month=3;
        else if(Stringmonth=="Apr") month=4;
        else if(Stringmonth=="May") month=5;
        else if(Stringmonth=="Jun") month=6;
        else if(Stringmonth=="Jul") month=7;
        else if(Stringmonth=="Aug") month=8;
        else if(Stringmonth=="Sep") month=9;
        else if(Stringmonth=="Oct") month=10;
        else if(Stringmonth=="Nov") month=11;
        else if(Stringmonth=="Dec") month=12;
        int day=((TObjString*)tokens->At(6))->String().Atoi();
        int hour=((TString)((TObjString*)tokens->At(7))->String()(0,2)).Atoi();
        int min=((TString)((TObjString*)tokens->At(7))->String()(3,5)).Atoi();
        int sec=((TString)((TObjString*)tokens->At(7))->String()(6,8)).Atoi();
        TTimeStamp t=TTimeStamp(year,month,day,hour,min,sec);
        event.SetBeginRunTime(t);
        calib.SetBeginRunTime(t);
        tokens->Clear();
        delete tokens;
      }
      continue;
    }
    tokens=aline.Tokenize(" \t");
    tokens->SetOwner(true);
    
    if (versionCAEN.CompareTo("3.3") == 0){
      int Nfields=tokens->GetEntries();
      if(((TObjString*)tokens->At(0))->String()=="Brd") {
        tokens->Clear();
        delete tokens;
        continue;
      }
      //================================================================================
      // data format example
      // Brd  Ch       LG       HG        Tstamp_us        TrgID		NHits
      // 7  00       51       68     10203578.136            0		64
      // 7  01       55       75 
      //================================================================================
      if(Nfields!=7){
        std::cout<<"Unexpected number of fields"<<std::endl;
        std::cout<<"Expected 7, got: "<<Nfields<<", exit"<<std::endl;
        for(int j=0; j<Nfields;j++) std::cout<<j<<"  "<<((TObjString*)tokens->At(j))->String()<<std::endl;
        tokens->Clear();
        delete tokens;
        return -1;
      }
      int TriggerID=((TObjString*)tokens->At(5))->String().Atoi();
      int NHits=((TObjString*)tokens->At(6))->String().Atoi();
      double Time = ((TObjString*)tokens->At(4))->String().Atof();
      Caen aTile;
      int aBoard=((TObjString*)tokens->At(0))->String().Atoi();
      int achannel=((TObjString*)tokens->At(1))->String().Atoi();
      aTile.SetE(((TObjString*)tokens->At(3))->String().Atoi());//To Test
      aTile.SetADCHigh(((TObjString*)tokens->At(3))->String().Atoi());
      aTile.SetADCLow (((TObjString*)tokens->At(2))->String().Atoi());
      tokens->Clear();
      delete tokens;
      aTile.SetCellID(setup->GetCellID(aBoard,achannel));
      itevent=tmpEvent.find(TriggerID);
      
      if(itevent!=tmpEvent.end()){
        tmpTime[TriggerID]+=Time;
        if (aTile.GetCellID() != -1){
          itevent->second.push_back(aTile);
        } else {
          if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
        }
        for(int ich=1; ich<NHits; ich++){
            aline.ReadLine(ASCIIinput);
            tokens=aline.Tokenize(" ");
            tokens->SetOwner(true);
            Nfields=tokens->GetEntries();
            if(Nfields!=4){
              std::cout<<"Expecting 4 fields but read "<<Nfields<<std::endl;
              return -1;
            }
            achannel=((TObjString*)tokens->At(1))->String().Atoi();
            aTile.SetE(((TObjString*)tokens->At(3))->String().Atoi());//To Test
            aTile.SetADCHigh(((TObjString*)tokens->At(3))->String().Atoi());
            aTile.SetADCLow (((TObjString*)tokens->At(2))->String().Atoi());
            aTile.SetCellID(setup->GetCellID(aBoard,achannel));
            if (aTile.GetCellID() != -1){
              itevent->second.push_back(aTile);
            } else {
              if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
            }
            tokens->Clear();
            delete tokens;
        }
        if((int)itevent->second.size()==setup->GetTotalNbChannels()/*8*64*/){
          //Fill the tree the event is complete and erase from the map
          event.SetTimeStamp(tmpTime[TriggerID]/setup->GetNMaxROUnit());
          event.SetEventID(itevent->first);
          for(std::vector<Caen>::iterator itv=itevent->second.begin(); itv!=itevent->second.end(); ++itv){
            event.AddTile(new Caen(*itv));
          }
          TdataOut->Fill();
          tmpEvent.erase(itevent);
          tmpTime.erase(TriggerID);
        }
      } else {
        //This is a new event;
        tempEvtCounter++;                                                                   // in crease event counts for monitoring of progress
        if (tempEvtCounter%5000 == 0 && debug > 0) std::cout << "Converted " <<  tempEvtCounter << " events" << std::endl;
        std::vector<Caen> vCaen;
        if (aTile.GetCellID() != -1){
          vCaen.push_back(aTile);
        } else {
          if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
        }
        for(int ich=1; ich<NHits; ich++){
          aline.ReadLine(ASCIIinput);
          tokens=aline.Tokenize(" ");
          tokens->SetOwner(true);
          Nfields=tokens->GetEntries();
          if(Nfields!=4){
            std::cout<<"Expecting 4 fields but read "<<Nfields<<std::endl;
            return -1;
          }
          achannel=((TObjString*)tokens->At(1))->String().Atoi();
          aTile.SetE(((TObjString*)tokens->At(3))->String().Atoi());//To Test
          aTile.SetADCHigh(((TObjString*)tokens->At(3))->String().Atoi());
          aTile.SetADCLow (((TObjString*)tokens->At(2))->String().Atoi());
          aTile.SetCellID(setup->GetCellID(aBoard,achannel));
          if (aTile.GetCellID() != -1){
            vCaen.push_back(aTile);
          } else {
            if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
          }
          tokens->Clear();
          delete tokens;
        }
        tmpTime[TriggerID]=Time;
        tmpEvent[TriggerID]=vCaen;
      }
    } else if (versionCAEN.CompareTo("3.1") == 0){
      int Nfields=tokens->GetEntries();
      if(((TObjString*)tokens->At(0))->String()=="Tstamp_us") {
        tokens->Clear();
        delete tokens;
        continue;
      }
      
      //================================================================================
      // data format example
      //   Tstamp_us        TrgID   Brd  Ch       LG       HG
      // 4970514.360            0    01  00       49       46 
      //                             01  01       49       35 
      //================================================================================
      
      if(Nfields!=6){
        std::cout<<"Unexpected number of fields"<<std::endl;
        std::cout<<"Expected 6, got: "<<Nfields<<", exit"<<std::endl;
        for(int j=0; j<Nfields;j++) std::cout<<j<<"  "<<((TObjString*)tokens->At(j))->String()<<std::endl;
        tokens->Clear();
        delete tokens;
        return -1;
      }
      
      // std::cout << aline.Data() << std::endl;
      int TriggerID = ((TObjString*)tokens->At(1))->String().Atoi();
      int NHits     = 64;                                           // temporary fix
      double Time   = ((TObjString*)tokens->At(0))->String().Atof();
      Caen aTile;
      int aBoard    =((TObjString*)tokens->At(2))->String().Atoi();
      int achannel  =((TObjString*)tokens->At(3))->String().Atoi();
      aTile.SetE(((TObjString*)tokens->At(5))->String().Atoi());//To Test
      aTile.SetADCHigh(((TObjString*)tokens->At(5))->String().Atoi());
      aTile.SetADCLow (((TObjString*)tokens->At(4))->String().Atoi());
      tokens->Clear();
      delete tokens;
      aTile.SetCellID(setup->GetCellID(aBoard,achannel));
      itevent=tmpEvent.find(TriggerID);
      
      if(itevent!=tmpEvent.end()){
        tmpTime[TriggerID]+=Time;
        if (aTile.GetCellID() != -1){
          itevent->second.push_back(aTile);
        } else {
          if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
        }
        for(int ich=1; ich<NHits; ich++){
            aline.ReadLine(ASCIIinput);
            // std::cout << aline.Data() << std::endl;
            tokens=aline.Tokenize(" ");
            tokens->SetOwner(true);
            Nfields=tokens->GetEntries();
            
            if(Nfields!=4){
              std::cout<< "Current line :" << aline.Data() << std::endl;
              std::cout<<"Expecting 4 fields but read "<<Nfields<<std::endl;
              return -1;
            }
            achannel=((TObjString*)tokens->At(1))->String().Atoi();
            aTile.SetE(((TObjString*)tokens->At(3))->String().Atoi());//To Test
            aTile.SetADCHigh(((TObjString*)tokens->At(3))->String().Atoi());
            aTile.SetADCLow (((TObjString*)tokens->At(2))->String().Atoi());
            aTile.SetCellID(setup->GetCellID(aBoard,achannel));
            
            if (aTile.GetCellID() != -1){
              itevent->second.push_back(aTile);
            } else {
              if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
            }
            tokens->Clear();
            delete tokens;
        }
        if((int)itevent->second.size()==setup->GetTotalNbChannels()){
          //Fill the tree the event is complete and erase from the map
          event.SetTimeStamp(tmpTime[TriggerID]/setup->GetNMaxROUnit());
          event.SetEventID(itevent->first);
          for(std::vector<Caen>::iterator itv=itevent->second.begin(); itv!=itevent->second.end(); ++itv){
            event.AddTile(new Caen(*itv));
          }
          TdataOut->Fill();
          tmpEvent.erase(itevent);
          tmpTime.erase(TriggerID);
        }
      } else {
        //This is a new event;
        tempEvtCounter++;                                                                   // in crease event counts for monitoring of progress
        if (tempEvtCounter%5000 == 0 && debug > 0) std::cout << "Converted " <<  tempEvtCounter << " events" << std::endl;
        std::vector<Caen> vCaen;
        
        if (aTile.GetCellID() != -1){
          vCaen.push_back(aTile);
        } else {
          if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
        }
        for(int ich=1; ich<NHits; ich++){
          aline.ReadLine(ASCIIinput);
          // std::cout << aline.Data() << std::endl;
          tokens=aline.Tokenize(" ");
          tokens->SetOwner(true);
          Nfields=tokens->GetEntries();
          if(Nfields!=4){
            std::cout<< "Current line :" << aline.Data() << std::endl;
            std::cout<<"Expecting 4 fields but read "<<Nfields<<std::endl;
            return -1;
          }
          achannel=((TObjString*)tokens->At(1))->String().Atoi();
          aTile.SetE(((TObjString*)tokens->At(3))->String().Atoi());//To Test
          aTile.SetADCHigh(((TObjString*)tokens->At(3))->String().Atoi());
          aTile.SetADCLow (((TObjString*)tokens->At(2))->String().Atoi());
          aTile.SetCellID(setup->GetCellID(aBoard,achannel));
          if (aTile.GetCellID() != -1){
            vCaen.push_back(aTile);
          } else {
            if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
          }
          tokens->Clear();
          delete tokens;
        }
        tmpTime[TriggerID]=Time;
        tmpEvent[TriggerID]=vCaen;
      }      
    }
  } // finished reading in file
  // 
  if (debug > 0) std::cout << "Converted a total of " <<  tempEvtCounter << " events" << std::endl;
  
  //============================================
  // Fill & write all trees to output file 
  //============================================
  RootOutput->cd();
  // setup 
  RootSetupWrapper rswtmp=RootSetupWrapper(setup);
  rsw=rswtmp;
  TsetupOut->Fill();
  // calib
  TcalibOut->Fill();
  TcalibOut->Write();
  // event data
  TdataOut->Fill();
  TsetupOut->Write();
  TdataOut->Write();

  RootOutput->Close();
  return true;
}



// ****************************************************************************
// convert already processed root file from CAEN output to new root format
// ****************************************************************************
bool Analyses::ConvertOldRootFile2Root(void){
  //============================================
  //Init first
  //============================================
  // initialize setup
  if (MapInputName.CompareTo("")== 0) {
      std::cerr << "ERROR: No mapping file has been provided, please make sure you do so! Aborting!" << std::endl;
      return false;
  }
  setup->Initialize(MapInputName.Data(),debug);
  // initialize run number file
  if (RunListInputName.CompareTo("")== 0) {
      std::cerr << "ERROR: No run list file has been provided, please make sure you do so! Aborting!" << std::endl;
      return false;
  }
  std::map<int,RunInfo> ri=readRunInfosFromFile(RunListInputName.Data(),debug,0);
  
  // Clean up file headers
  TObjArray* tok=ASCIIinputName.Tokenize("/");
  // get file name
  TString file=((TObjString*)tok->At(tok->GetEntries()-1))->String();
  delete tok;
  tok=file.Tokenize("_");
  TString header=((TObjString*)tok->At(0))->String();
  delete tok;
  
  // Get run number from file & find necessary run infos
  TString RunString=header(3,header.Sizeof());
  std::map<int,RunInfo>::iterator it=ri.find(RunString.Atoi());
  //std::cout<<RunString.Atoi()<<"\t"<<it->second.species<<std::endl;
  event.SetRunNumber(RunString.Atoi());
  event.SetROtype(2);
  event.SetBeamName(it->second.species);
  event.SetBeamID(it->second.pdg);
  event.SetBeamEnergy(it->second.energy);
  event.SetVop(it->second.vop);
  event.SetVov(it->second.vop-it->second.vbr);
  event.SetBeamPosX(it->second.posX);
  event.SetBeamPosY(it->second.posY);
  calib.SetRunNumber(RunString.Atoi());
  calib.SetVop(it->second.vop);
  calib.SetVov(it->second.vop-it->second.vbr);  
  
    // load tree
  TChain *const tt_event = new TChain("tree");
  if (ASCIIinputName.EndsWith(".root")) {                     // are we loading a single root tree?
      std::cout << "loading a single root file" << std::endl;
      tt_event->AddFile(ASCIIinputName);
      TFile testFile(ASCIIinputName.Data());
      if (testFile.IsZombie()){
        std::cout << Form("The file %s is not a root file or doesn't exit, please fix the file path", ASCIIinputName.Data()) << std::endl;
        return false;
      }

  } else {
      std::cout << "please try again this isn't a root file" << std::endl;
  } 
  if(!tt_event){ std::cout << "tree not found... returning!"<< std::endl; return false;}

  // Define tree variables
  Long64_t gTrID;
  Double_t gTRtimeStamp;
  const int gMaxChannels = 64;
  Long64_t* gBoard          = new Long64_t[gMaxChannels];
  Long64_t* gChannel        = new Long64_t[gMaxChannels];
  Long64_t* gLG             = new Long64_t[gMaxChannels];
  Long64_t* gHG             = new Long64_t[gMaxChannels];

  if (tt_event->GetBranchStatus("t_stamp") ){
    tt_event->SetBranchAddress("trgid",            &gTrID);
    tt_event->SetBranchAddress("t_stamp",          &gTRtimeStamp);
    tt_event->SetBranchAddress("board",            gBoard);
    tt_event->SetBranchAddress("channel",          gChannel);
    tt_event->SetBranchAddress("LG",               gLG);
    tt_event->SetBranchAddress("HG",               gHG);
  }
  
  Long64_t nEntriesTree                 = tt_event->GetEntries();
  std::cout << "Number of events in tree: " << nEntriesTree << std::endl;

  std::map<int,std::vector<Caen> > tmpEvent;
  std::map<int,double> tmpTime;
  for (Long64_t i=0; i<nEntriesTree;i++) {
    // load current event
    tt_event->GetEntry(i);  
    if (i%5000 == 0 && debug > 0) std::cout << "Converted " <<  i << " events" << std::endl;    
    int TriggerID = gTrID;
    double Time   = gTRtimeStamp;
    std::vector<Caen> vCaen;
    
    for(Int_t ch=0; ch<gMaxChannels; ch++){   
      Caen aTile;
      int aBoard      = gBoard[ch];
      int achannel    = gChannel[ch];
      aTile.SetE(gHG[ch]);//To Test
      aTile.SetADCHigh(gHG[ch]);
      aTile.SetADCLow (gLG[ch]);
      aTile.SetCellID(setup->GetCellID(aBoard,achannel));
      if (aTile.GetCellID() != -1){
        vCaen.push_back(aTile);
      } else {
        if(debug ==10) std::cout << "cell " << aBoard << "\t" << achannel << " wasn't active according to mapping file!" << std::endl;
      }
    }
    
     if((int)vCaen.size()==setup->GetTotalNbChannels()){
      //Fill the tree the event is complete and erase from the map
      event.SetTimeStamp(Time);
      event.SetEventID(TriggerID);
      for(std::vector<Caen>::iterator itv=vCaen.begin(); itv!=vCaen.end(); ++itv){
        event.AddTile(new Caen(*itv));
      }
      TdataOut->Fill();
      vCaen.clear();
    }
  } // finished reading the file

  // 
  if (debug > 0) std::cout << "Converted a total of " <<  nEntriesTree << " events" << std::endl;
  
  //============================================
  // Fill & write all trees to output file 
  //============================================
  RootOutput->cd();
  // setup 
  RootSetupWrapper rswtmp=RootSetupWrapper(setup);
  rsw=rswtmp;
  TsetupOut->Fill();
  // calib
  TcalibOut->Fill();
  TcalibOut->Write();
  // event data
  TdataOut->Fill();
  TsetupOut->Write();
  TdataOut->Write();

  RootOutput->Close();
  return true;
}



// ****************************************************************************
// extract pedestral from dedicated data run, no other data in run 
// ****************************************************************************
bool Analyses::GetPedestal(void){
  std::cout<<"GetPedestal for maximium "<< setup->GetMaxCellID() << " cells" <<std::endl;
  
  std::map<int,RunInfo> ri=readRunInfosFromFile(RunListInputName.Data(),debug,0);
  
  int maxChannelPerLayer = (setup->GetNMaxColumn()+1)*(setup->GetNMaxRow()+1);
  
  // create HG and LG histo's per channel
  TH2D* hspectraHGvsCellID      = new TH2D( "hspectraHG_vsCellID","ADC spectrum High Gain vs CellID; cell ID; ADC_{HG} (arb. units) ",
                                            setup->GetMaxCellID()+1, -0.5, setup->GetMaxCellID()+1-0.5, 4000,0,4000);
  hspectraHGvsCellID->SetDirectory(0);
  TH2D* hspectraLGvsCellID      = new TH2D( "hspectraLG_vsCellID","ADC spectrum Low Gain vs CellID; cell ID; ADC_{LG} (arb. units)  ",
                                            setup->GetMaxCellID()+1, -0.5, setup->GetMaxCellID()+1-0.5, 4000,0,4000);
  hspectraLGvsCellID->SetDirectory(0);
  TH1D* hMeanPedHGvsCellID      = new TH1D( "hMeanPedHG_vsCellID","mean Ped High Gain vs CellID ; cell ID; #mu_{noise, HG} (arb. units) ",
                                            setup->GetMaxCellID()+1, -0.5, setup->GetMaxCellID()+1-0.5);
  hMeanPedHGvsCellID->SetDirectory(0);
  TH2D* hspectraHGMeanVsLayer   = new TH2D( "hspectraHGMeanVsLayer","Mean Ped High Gain vs CellID; layer; brd channel; #mu_{Ped HG} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGMeanVsLayer->SetDirectory(0);
  TH2D* hspectraHGSigmaVsLayer  = new TH2D( "hspectraHGSigmaVsLayer","Mean Ped High Gain vs CellID; layer; brd channel; #sigma_{Ped HG} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGSigmaVsLayer->SetDirectory(0);
  TH1D* hMeanPedLGvsCellID      = new TH1D( "hMeanPedLG_vsCellID","mean Ped Low Gain vs CellID ; cell ID; #mu_{noise, LG} (arb. units) ",
                                            setup->GetMaxCellID()+1, -0.5, setup->GetMaxCellID()+1-0.5);
  hMeanPedLGvsCellID->SetDirectory(0);
  TH2D* hspectraLGMeanVsLayer   = new TH2D( "hspectraLGMeanVsLayer","Mean Ped Low Gain vs CellID; layer; brd channel; #mu_{PED LG} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGMeanVsLayer->SetDirectory(0);
  TH2D* hspectraLGSigmaVsLayer  = new TH2D( "hspectraLGSigmaVsLayer","Mean Ped Low Gain vs CellID; layer; brd channel; #sigma_{Ped LG} (arb. units)",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGSigmaVsLayer->SetDirectory(0);
  
  std::map<int,TileSpectra> hSpectra;
  std::map<int, TileSpectra>::iterator ithSpectra;

  std::cout << "Additional Output with histos being created: " << RootOutputNameHist.Data() << std::endl;
  if(Overwrite){
    std::cout << "recreating file with hists" << std::endl;
    RootOutputHist = new TFile(RootOutputNameHist.Data(),"RECREATE");
  } else{
    std::cout << "newly creating file with hists" << std::endl;
    RootOutputHist = new TFile(RootOutputNameHist.Data(),"CREATE");
  }
  // entering histoOutput file
  RootOutputHist->mkdir("IndividualCells");
  RootOutputHist->cd("IndividualCells");

  RootOutput->cd();
  // Event loop to fill histograms & output tree
  std::cout << "N max layers: " << setup->GetNMaxLayer() << "\t columns: " <<  setup->GetNMaxColumn() << "\t row: " << setup->GetNMaxRow() << "\t module: " <<  setup->GetNMaxModule() << std::endl;
  if(TcalibIn) TcalibIn->GetEntry(0);
  int evts=TdataIn->GetEntries();
  int runNr = -1;
  for(int i=0; i<evts; i++){
    TdataIn->GetEntry(i);
    if (i == 0)runNr = event.GetRunNumber();
    if (i%5000 == 0&& i > 0 && debug > 0) std::cout << "Reading " <<  i << "/" << evts << " events"<< std::endl;
    for(int j=0; j<event.GetNTiles(); j++){
      Caen* aTile=(Caen*)event.GetTile(j);
      if (i == 0 && debug > 2){
        std::cout << ((TString)setup->DecodeCellID(aTile->GetCellID())).Data() << std::endl;
      }
      ithSpectra=hSpectra.find(aTile->GetCellID());
      if(ithSpectra!=hSpectra.end()){
        ithSpectra->second.Fill(aTile->GetADCLow(),aTile->GetADCHigh());
      } else {
        RootOutputHist->cd("IndividualCells");
        hSpectra[aTile->GetCellID()]=TileSpectra("1stExtraction",aTile->GetCellID(),calib.GetTileCalib(aTile->GetCellID()),debug);
        hSpectra[aTile->GetCellID()].Fill(aTile->GetADCLow(),aTile->GetADCHigh());
        RootOutput->cd();
      }
      hspectraHGvsCellID->Fill(aTile->GetCellID(), aTile->GetADCHigh());
      hspectraLGvsCellID->Fill(aTile->GetCellID(), aTile->GetADCLow());
    }
    RootOutput->cd();
    TdataOut->Fill();
  }
  
  // fit pedestal
  double* parameters=new double[8];
  bool isGood;
  for(ithSpectra=hSpectra.begin(); ithSpectra!=hSpectra.end(); ++ithSpectra){
    if ( debug > 2) std::cout << ((TString)setup->DecodeCellID(ithSpectra->second.GetCellID())).Data() << std::endl;
    isGood=ithSpectra->second.FitNoise(parameters, yearData);
    hMeanPedHGvsCellID->SetBinContent(hMeanPedHGvsCellID->FindBin(ithSpectra->second.GetCellID()), parameters[4]);
    hMeanPedHGvsCellID->SetBinError  (hMeanPedHGvsCellID->FindBin(ithSpectra->second.GetCellID()), parameters[6]);
    hMeanPedLGvsCellID->SetBinContent(hMeanPedLGvsCellID->FindBin(ithSpectra->second.GetCellID()), parameters[0]);
    hMeanPedLGvsCellID->SetBinError  (hMeanPedLGvsCellID->FindBin(ithSpectra->second.GetCellID()), parameters[2]);
    
    int layer     = setup->GetLayer(ithSpectra->second.GetCellID());
    int chInLayer = setup->GetChannelInLayer(ithSpectra->second.GetCellID());
  
    hspectraHGMeanVsLayer->SetBinContent(hspectraHGMeanVsLayer->FindBin(layer,chInLayer), parameters[4]);
    hspectraHGMeanVsLayer->SetBinError(hspectraHGMeanVsLayer->FindBin(layer,chInLayer), parameters[5]);
    hspectraHGSigmaVsLayer->SetBinContent(hspectraHGSigmaVsLayer->FindBin(layer,chInLayer), parameters[6]);
    hspectraHGSigmaVsLayer->SetBinError(hspectraHGSigmaVsLayer->FindBin(layer,chInLayer), parameters[7]);
    hspectraLGMeanVsLayer->SetBinContent(hspectraLGMeanVsLayer->FindBin(layer,chInLayer), parameters[0]);
    hspectraLGMeanVsLayer->SetBinError(hspectraLGMeanVsLayer->FindBin(layer,chInLayer), parameters[1]);
    hspectraLGSigmaVsLayer->SetBinContent(hspectraLGSigmaVsLayer->FindBin(layer,chInLayer), parameters[2]);
    hspectraLGSigmaVsLayer->SetBinError(hspectraLGSigmaVsLayer->FindBin(layer,chInLayer), parameters[3]);
  }
  
  RootOutput->cd();
  // write output tree (copy of raw data)
  TdataOut->Write();
  // write setup tree (copy of raw data)
  TsetupIn->CloneTree()->Write();
  
  TcalibOut->Fill();
  TcalibOut->Write();
  RootOutput->Write();
  RootOutput->Close();
  
  // entering histoOutput file
  //RootOutputHist->cd();
  //RootOutputHist->mkdir("IndividualCells");
  RootOutputHist->cd("IndividualCells");
  for(ithSpectra=hSpectra.begin(); ithSpectra!=hSpectra.end(); ++ithSpectra){
    ithSpectra->second.Write(true);
  }
  RootOutputHist->cd();
  hspectraHGvsCellID->Write();
  hMeanPedHGvsCellID->Write();
  hspectraLGvsCellID->Write();
  hMeanPedLGvsCellID->Write();
  hspectraHGMeanVsLayer->Write();
  hspectraLGMeanVsLayer->Write();
  hspectraHGSigmaVsLayer->Write();
  hspectraLGSigmaVsLayer->Write();
  
  // fill calib tree & write it
  // close open root files
  RootOutputHist->Write();
  RootOutputHist->Close();

  RootInput->Close();
  
  
  // Get run info object
  std::map<int,RunInfo>::iterator it=ri.find(runNr);
  
  // create directory for plot output
  TString outputDirPlots = GetPlotOutputDir();
  gSystem->Exec("mkdir -p "+outputDirPlots);
  
  //**********************************************************************
  // Create canvases for channel overview plotting
  //**********************************************************************
  Double_t textSizeRel = 0.035;
  StyleSettingsBasics("pdf");
  SetPlotStyle();
  
  TCanvas* canvas2DCorr = new TCanvas("canvasCorrPlots","",0,0,1450,1200);  // gives the page size
  DefaultCancasSettings( canvas2DCorr, 0.08, 0.13, 0.02, 0.07);
  canvas2DCorr->SetLogz();
  
  PlotSimple2D( canvas2DCorr, hspectraHGvsCellID, 300, setup->GetMaxCellID()+1, textSizeRel, Form("%s/HG_Noise.pdf", outputDirPlots.Data()), it->second, 5 );
  PlotSimple2D( canvas2DCorr, hspectraLGvsCellID, 300, setup->GetMaxCellID()+1, textSizeRel, Form("%s/LG_Noise.pdf", outputDirPlots.Data()), it->second, 5);

  canvas2DCorr->SetLogz(0);
  PlotSimple2D( canvas2DCorr, hspectraHGMeanVsLayer, -10000, -10000, textSizeRel, Form("%s/HG_NoiseMean.pdf", outputDirPlots.Data()), it->second, 5, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGSigmaVsLayer,-10000, -10000, textSizeRel, Form("%s/HG_NoiseSigma.pdf", outputDirPlots.Data()), it->second, 5,  kFALSE, "colz");

  canvas2DCorr->SetLogz(0);
  PlotSimple2D( canvas2DCorr, hspectraLGMeanVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_NoiseMean.pdf", outputDirPlots.Data()), it->second, 5, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGSigmaVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_NoiseSigma.pdf", outputDirPlots.Data()), it->second, 5, kFALSE, "colz");
  
  
  //***********************************************************************************************************
  //********************************* 8 Panel overview plot  **************************************************
  //***********************************************************************************************************
  //*****************************************************************
    // Test beam geometry (beam coming from viewer)
    //===========================================================
    //||    8 (4)    ||    7 (5)   ||    6 (6)   ||    5 (7)   ||  row 0
    //===========================================================
    //||    1 (0)    ||    2 (1)   ||    3 (2)   ||    4 (3)   ||  row 1
    //===========================================================
    //    col 0     col 1       col 2     col  3
    // rebuild pad geom in similar way (numbering -1)
  //*****************************************************************
  TCanvas* canvas8Panel;
  TPad* pad8Panel[8];
  Double_t topRCornerX[8];
  Double_t topRCornerY[8];
  Int_t textSizePixel = 30;
  Double_t relSize8P[8];
  CreateCanvasAndPadsFor8PannelTBPlot(canvas8Panel, pad8Panel,  topRCornerX, topRCornerY, relSize8P, textSizePixel);

  for (Int_t l = 0; l < setup->GetNMaxLayer()+1; l++){    
    PlotNoiseWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                hSpectra, setup, true, 0, 275, 1.2, l, 0,
                                Form("%s/Noise_HG_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
    PlotNoiseWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                hSpectra, setup, false, 0, 275, 1.2, l, 0,
                                Form("%s/Noise_LG_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
  }

  
  return true;
}

// ****************************************************************************
// extract pedestral from dedicated data run, no other data in run 
// ****************************************************************************
bool Analyses::CorrectPedestal(void){
  std::cout<<"Correct Pedestal"<<std::endl;
  int evts=TdataIn->GetEntries();
  for(int i=0; i<evts; i++){
    if (i%5000 == 0&& i > 0 && debug > 0) std::cout << "Reading " <<  i << "/" << evts << " events"<< std::endl;
    TdataIn->GetEntry(i);
    RootOutput->cd();
    TdataOut->Fill();
  }
  //RootOutput->cd();
  TdataOut->Write();
  TsetupIn->CloneTree()->Write();
  TcalibIn->GetEntry(0);
  //std::cout<<"What is the value? "<<calib.GetPedestalMeanH(0)<<std::endl;
  TcalibOut->Fill();
  TcalibOut->Write();
  RootOutput->Close();
  RootInput->Close();
  return true;
}


//***********************************************************************************************
//*********************** intial scaling calculation function *********************************
//***********************************************************************************************
bool Analyses::GetScaling(void){
  std::cout<<"GetScaling"<<std::endl;
  
  std::map<int,RunInfo> ri=readRunInfosFromFile(RunListInputName.Data(),debug,0);
  
  std::map<int,TileSpectra> hSpectra;
  std::map<int,TileSpectra> hSpectraTrigg;
  std::map<int, TileSpectra>::iterator ithSpectra;
  std::map<int, TileSpectra>::iterator ithSpectraTrigg;
  
  std::cout << "Additional Output with histos being created: " << RootOutputNameHist.Data() << std::endl;
  if(Overwrite){
    std::cout << "recreating file with hists" << std::endl;
    RootOutputHist = new TFile(RootOutputNameHist.Data(),"RECREATE");
  } else{
    std::cout << "newly creating file with hists" << std::endl;
    RootOutputHist = new TFile(RootOutputNameHist.Data(),"CREATE");
  }
    
  //***********************************************************************************************
  //************************* first pass over tree to extract spectra *****************************
  //***********************************************************************************************
  // entering histoOutput file
  RootOutputHist->mkdir("IndividualCells");
  RootOutputHist->cd("IndividualCells");

  TcalibIn->GetEntry(0);
  int evts=TdataIn->GetEntries();
  int runNr = -1;
  for(int i=0; i<evts; i++){
    TdataIn->GetEntry(i);
    if (i == 0)runNr = event.GetRunNumber();
    if (i%5000 == 0 && i > 0 && debug > 0) std::cout << "Reading " <<  i << " / " << evts << " events" << std::endl;
    if (i == 0 && debug > 2) std::cout << "decoding cell IDs" << std::endl ;
    for(int j=0; j<event.GetNTiles(); j++){
      Caen* aTile=(Caen*)event.GetTile(j);
      if (i == 0 && debug > 2) std::cout << ((TString)setup->DecodeCellID(aTile->GetCellID())).Data() << std::endl;
      
      ithSpectra=hSpectra.find(aTile->GetCellID());
      double hgCorr = aTile->GetADCHigh()-calib.GetPedestalMeanH(aTile->GetCellID());
      double lgCorr = aTile->GetADCLow()-calib.GetPedestalMeanL(aTile->GetCellID());
            
      if(ithSpectra!=hSpectra.end()){
        ithSpectra->second.FillSpectra(lgCorr,hgCorr);
        if (hgCorr > 3*calib.GetPedestalSigH(aTile->GetCellID()) && lgCorr > 3*calib.GetPedestalSigL(aTile->GetCellID()) && hgCorr < 3900 )
          ithSpectra->second.FillCorr(lgCorr,hgCorr);
      } else {
        RootOutputHist->cd("IndividualCells");
        hSpectra[aTile->GetCellID()]=TileSpectra("mip1st",aTile->GetCellID(),calib.GetTileCalib(aTile->GetCellID()),debug);
        hSpectra[aTile->GetCellID()].FillSpectra(lgCorr,hgCorr);;
        if (hgCorr > 3*calib.GetPedestalSigH(aTile->GetCellID()) && lgCorr > 3*calib.GetPedestalSigL(aTile->GetCellID() && hgCorr < 3900) )
          hSpectra[aTile->GetCellID()].FillCorr(lgCorr,hgCorr);;
        RootOutput->cd();
      }
    }
    RootOutput->cd();
  }
  // TdataOut->Write();
  TsetupIn->CloneTree()->Write();
  
  RootOutputHist->cd();
 
  //***********************************************************************************************
  //***** Monitoring histos for calib parameters & fits results of 1st iteration ******************
  //***********************************************************************************************
  int maxChannelPerLayer        = (setup->GetNMaxColumn()+1)*(setup->GetNMaxRow()+1);
  // monitoring applied pedestals
  TH1D* hMeanPedHGvsCellID      = new TH1D( "hMeanPedHG_vsCellID","mean Ped High Gain vs CellID ; cell ID; #mu_{noise, HG} (arb. units) ",
                                            setup->GetMaxCellID()+1, -0.5, setup->GetMaxCellID()+1-0.5);
  hMeanPedHGvsCellID->SetDirectory(0);
  TH1D* hMeanPedHG              = new TH1D( "hMeanPedHG","mean Ped High Gain ; #mu_{noise, HG} (arb. units); counts ",
                                            500, -0.5, 500-0.5);
  hMeanPedHG->SetDirectory(0);
  TH2D* hspectraHGMeanVsLayer   = new TH2D( "hspectraHGMeanVsLayer","Mean Ped High Gain; layer; brd channel; #mu_{Ped HG} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGMeanVsLayer->SetDirectory(0);
  TH2D* hspectraHGSigmaVsLayer  = new TH2D( "hspectraHGSigmaVsLayer","Sigma Ped High Gain; layer; brd channel; #sigma_{Ped HG} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGSigmaVsLayer->SetDirectory(0);
  TH1D* hMeanPedLGvsCellID      = new TH1D( "hMeanPedLG_vsCellID","mean Ped Low Gain vs CellID ; cell ID; #mu_{noise, LG} (arb. units) ",
                                            setup->GetMaxCellID()+1, -0.5, setup->GetMaxCellID()+1-0.5);
  hMeanPedLGvsCellID->SetDirectory(0);
  TH1D* hMeanPedLG             = new TH1D( "hMeanPedLG","mean Ped Low Gain ; #mu_{noise, LG} (arb. units); counts ",
                                            500, -0.5, 500-0.5);
  hMeanPedLG->SetDirectory(0);
  TH2D* hspectraLGMeanVsLayer   = new TH2D( "hspectraLGMeanVsLayer","Mean Ped Low Gain; layer; brd channel; #mu_{PED LG} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGMeanVsLayer->SetDirectory(0);
  TH2D* hspectraLGSigmaVsLayer  = new TH2D( "hspectraLGSigmaVsLayer","Sigma Ped Low Gain; layer; brd channel; #sigma_{Ped LG} (arb. units)",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGSigmaVsLayer->SetDirectory(0);
  // monitoring 1st iteration mip fits
  TH2D* hspectraHGMaxVsLayer1st   = new TH2D( "hspectraHGMaxVsLayer_1st","Max High Gain; layer; brd channel; Max_{HG, 1^{st}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGMaxVsLayer1st->SetDirectory(0);
  TH2D* hspectraHGFWHMVsLayer1st   = new TH2D( "hspectraHGFWHMVsLayer_1st","FWHM High Gain; layer; brd channel; FWHM_{HG, 1^{st}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGFWHMVsLayer1st->SetDirectory(0);
  TH2D* hspectraHGLMPVVsLayer1st   = new TH2D( "hspectraHGLMPVVsLayer_1st","MPV High Gain; layer; brd channel; MPV_{HG, 1^{st}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGLMPVVsLayer1st->SetDirectory(0);
  TH2D* hspectraHGLSigmaVsLayer1st = new TH2D( "hspectraHGLSigmaVsLayer_1st","Sigma Landau High Gain; layer; brd channel; #sigma_{L,HG, 1^{st}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGLSigmaVsLayer1st->SetDirectory(0);
  TH2D* hspectraHGGSigmaVsLayer1st = new TH2D( "hspectraHGGSigmaVsLayer_1st","Sigma Gauss High Gain; layer; brd channel; #sigma_{G,HG, 1^{st}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGGSigmaVsLayer1st->SetDirectory(0);
  TH2D* hLGHGCorrVsLayer = new TH2D( "hLGHGCorrVsLayer","LG-HG corr; layer; brd channel; a_{LG-HG} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hLGHGCorrVsLayer->SetDirectory(0);
  TH2D* hHGLGCorrVsLayer = new TH2D( "hHGLGCorrVsLayer","HG-LG corr; layer; brd channel; a_{HG-LG} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hHGLGCorrVsLayer->SetDirectory(0);
  
  TH1D* hMaxHG1st             = new TH1D( "hMaxHG1st","Max High Gain ;Max_{HG, 1^{st}} (arb. units) ; counts ",
                                            2000, -0.5, 2000-0.5);
  hMaxHG1st->SetDirectory(0);
  TH1D* hLGHGCorr             = new TH1D( "hLGHGCorr","LG-HG corr ; a_{LG-HG} (arb. units) ; counts ",
                                            400, -20, 20);
  hLGHGCorr->SetDirectory(0);
  TH1D* hHGLGCorr             = new TH1D( "hHGLGCorr","LG-HG corr ; a_{HG-LG} (arb. units) ; counts ",
                                            400, -1., 1.);
  hHGLGCorr->SetDirectory(0);

  
  // fit pedestal
  double* parameters    = new double[6];
  double* parErrAndRes  = new double[6];
  bool isGood;
  int currCells = 0;
  for(ithSpectra=hSpectra.begin(); ithSpectra!=hSpectra.end(); ++ithSpectra){
    if (currCells%20 == 0 && currCells > 0 && debug > 0)
      std::cout << "============================== cell " <<  currCells << " / " << hSpectra.size() << " cells" << std::endl;
    currCells++;
    for (int p = 0; p < 6; p++){
      parameters[p]   = 0;
      parErrAndRes[p] = 0;
    }
    isGood        = ithSpectra->second.FitMipHG(parameters, parErrAndRes, debug, yearData, false, 1);
    long cellID   = ithSpectra->second.GetCellID();
    int layer     = setup->GetLayer(cellID);
    int chInLayer = setup->GetChannelInLayer(cellID);
    
    // fill cross-check histos
    hMeanPedHGvsCellID->SetBinContent(hMeanPedHGvsCellID->FindBin(cellID), calib.GetPedestalMeanH(cellID));
    hMeanPedLGvsCellID->SetBinContent(hMeanPedLGvsCellID->FindBin(cellID), calib.GetPedestalMeanL(cellID));
    hMeanPedHG->Fill(calib.GetPedestalMeanH(cellID));
    hMeanPedLG->Fill(calib.GetPedestalMeanL(cellID));
    
    int bin2D     = hspectraHGMeanVsLayer->FindBin(layer,chInLayer);
    hspectraHGMeanVsLayer->SetBinContent(bin2D, calib.GetPedestalMeanH(cellID));
    hspectraHGSigmaVsLayer->SetBinContent(bin2D, calib.GetPedestalSigH(cellID));
    hspectraLGMeanVsLayer->SetBinContent(bin2D, calib.GetPedestalMeanL(cellID));
    hspectraLGSigmaVsLayer->SetBinContent(bin2D, calib.GetPedestalSigL(cellID));

    if (isGood){
      hspectraHGMaxVsLayer1st->SetBinContent(bin2D, parameters[4]);
      hspectraHGFWHMVsLayer1st->SetBinContent(bin2D, parameters[5]);
      hspectraHGLMPVVsLayer1st->SetBinContent(bin2D, parameters[1]);
      hspectraHGLMPVVsLayer1st->SetBinError(bin2D, parErrAndRes[1]);
      hspectraHGLSigmaVsLayer1st->SetBinContent(bin2D, parameters[0]);
      hspectraHGLSigmaVsLayer1st->SetBinError(bin2D, parErrAndRes[0]);
      hspectraHGGSigmaVsLayer1st->SetBinContent(bin2D, parameters[3]);
      hspectraHGGSigmaVsLayer1st->SetBinError(bin2D, parErrAndRes[3]);
      
      hMaxHG1st->Fill(parameters[4]);
    }
    
    isGood=ithSpectra->second.FitCorr(debug);
    hLGHGCorrVsLayer->SetBinContent(bin2D,ithSpectra->second.GetCorrModel(0)->GetParameter(1));
    hLGHGCorrVsLayer->SetBinError(bin2D,ithSpectra->second.GetCorrModel(0)->GetParError(1));
    hHGLGCorrVsLayer->SetBinContent(bin2D,ithSpectra->second.GetCorrModel(1)->GetParameter(1));
    hHGLGCorrVsLayer->SetBinError(bin2D,ithSpectra->second.GetCorrModel(1)->GetParError(1));    
    hLGHGCorr->Fill(ithSpectra->second.GetCorrModel(0)->GetParameter(1));
    hHGLGCorr->Fill(ithSpectra->second.GetCorrModel(1)->GetParameter(1));
  }

  TH1D* hHGTileSum[20];
  for (int c = 0; c < maxChannelPerLayer; c++ ){
    hHGTileSum[c] = new TH1D( Form("hHGTileSum_absChB%d",c),"av ADC surrounding cells ; ADC (arb. units); counts ",
                              4000, -0.5, 4000-0.5);
    hHGTileSum[c]->SetDirectory(0);
  }
  
  // setup trigger sel
  TRandom3* rand    = new TRandom3();
  Int_t localTriggerTiles = 4;
  double factorMinTrigg   = 0.9;
  double factorMaxTrigg   = 3.;
  if (yearData == 2023){
    localTriggerTiles = 6;
    factorMaxTrigg    = 2.;
  }
  RootOutputHist->mkdir("IndividualCellsTrigg");
  RootOutputHist->cd("IndividualCellsTrigg");  
  //***********************************************************************************************
  //************************* first pass over tree to extract spectra *****************************
  //***********************************************************************************************  
  double averageScale = calib.GetAverageScaleHigh();
  std::cout << "average HG mip: " << averageScale << std::endl;
  for(int i=0; i<evts; i++){
    TdataIn->GetEntry(i);
    if (i%5000 == 0 && i > 0 && debug > 0) std::cout << "Reading " <<  i << " / " << evts << " events" << std::endl;
    for(int j=0; j<event.GetNTiles(); j++){
      Caen* aTile=(Caen*)event.GetTile(j);
      if (i == 0 && debug > 2) std::cout << ((TString)setup->DecodeCellID(aTile->GetCellID())).Data() << std::endl;
      long currCellID = aTile->GetCellID();
      
      // read current tile
      ithSpectraTrigg=hSpectraTrigg.find(aTile->GetCellID());
      double hgCorr = aTile->GetADCHigh()-calib.GetPedestalMeanH(aTile->GetCellID());
      double lgCorr = aTile->GetADCLow()-calib.GetPedestalMeanL(aTile->GetCellID());

      aTile->SetLocalTriggerPrimitive(event.CalculateLocalMuonTrigg(setup, calib, rand, currCellID, localTriggerTiles));
      // estimate local muon trigger
      bool localMuonTrigg = event.InspectIfLocalMuonTrigg(currCellID, averageScale, factorMinTrigg, factorMaxTrigg);
      int chInLayer = setup->GetChannelInLayer(currCellID);    
      hHGTileSum[chInLayer]->Fill(aTile->GetLocalTriggerPrimitive());
      
      if(ithSpectraTrigg!=hSpectraTrigg.end()){
        ithSpectraTrigg->second.FillTrigger(aTile->GetLocalTriggerPrimitive());
      } else {
        RootOutputHist->cd("IndividualCellsTrigg");
        hSpectraTrigg[currCellID]=TileSpectra("mipTrigg",currCellID,calib.GetTileCalib(currCellID),debug);
        hSpectraTrigg[currCellID].FillTrigger(aTile->GetLocalTriggerPrimitive());;
        RootOutput->cd();
      }
     
      // only fill tile spectra if 4 surrounding tiles on average are compatible with muon response
      if (localMuonTrigg){
        ithSpectraTrigg=hSpectraTrigg.find(aTile->GetCellID());
        ithSpectraTrigg->second.FillSpectra(lgCorr,hgCorr);
        if (hgCorr > 3*calib.GetPedestalSigH(currCellID) && lgCorr > 3*calib.GetPedestalSigL(currCellID) && hgCorr < 3900 )
          ithSpectraTrigg->second.FillCorr(lgCorr,hgCorr);
      }
    }
    RootOutput->cd();
    TdataOut->Fill();
  }
  TdataOut->Write();
  
  //***********************************************************************************************
  //***** Monitoring histos for fits results of 2nd iteration ******************
  //***********************************************************************************************
  RootOutputHist->cd();
  
  // monitoring trigger 
  TH2D* hmipTriggers              = new TH2D( "hmipTriggers","muon triggers; layer; brd channel; counts ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hmipTriggers->SetDirectory(0);
  TH2D* hSuppresionNoise          = new TH2D( "hSuppresionNoise","S/B noise region; layer; brd channel; S/B noise region",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hSuppresionNoise->SetDirectory(0);
  TH2D* hSuppresionSignal         = new TH2D( "hSuppresionSignal","S/B signal region; layer; brd channel; S/B signal region",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hSuppresionSignal->SetDirectory(0);

  // monitoring 2nd iteration mip fits
  TH2D* hspectraHGMaxVsLayer2nd   = new TH2D( "hspectraHGMaxVsLayer_2nd","Max High Gain; layer; brd channel; Max_{HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGMaxVsLayer2nd->SetDirectory(0);
  TH2D* hspectraHGFWHMVsLayer2nd   = new TH2D( "hspectraHGFWHMVsLayer_2nd","FWHM High Gain; layer; brd channel; FWHM_{HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGFWHMVsLayer2nd->SetDirectory(0);
  TH2D* hspectraHGLMPVVsLayer2nd   = new TH2D( "hspectraHGLMPVVsLayer_2nd","MPV High Gain; layer; brd channel; MPV_{HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGLMPVVsLayer2nd->SetDirectory(0);
  TH2D* hspectraHGLSigmaVsLayer2nd = new TH2D( "hspectraHGLSigmaVsLayer_2nd","Sigma Landau High Gain; layer; brd channel; #sigma_{L,HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGLSigmaVsLayer2nd->SetDirectory(0);
  TH2D* hspectraHGGSigmaVsLayer2nd = new TH2D( "hspectraHGGSigmaVsLayer_2nd","Sigma Gauss High Gain; layer; brd channel; #sigma_{G,HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGGSigmaVsLayer2nd->SetDirectory(0);
  TH2D* hspectraLGMaxVsLayer2nd   = new TH2D( "hspectraLGMaxVsLayer_2nd","Max High Gain; layer; brd channel; Max_{LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGMaxVsLayer2nd->SetDirectory(0);
  TH2D* hspectraLGFWHMVsLayer2nd   = new TH2D( "hspectraLGFWHMVsLayer_2nd","FWHM High Gain; layer; brd channel; FWHM_{LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGFWHMVsLayer2nd->SetDirectory(0);
  TH2D* hspectraLGLMPVVsLayer2nd   = new TH2D( "hspectraLGLMPVVsLayer_2nd","MPV High Gain; layer; brd channel; MPV_{LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGLMPVVsLayer2nd->SetDirectory(0);
  TH2D* hspectraLGLSigmaVsLayer2nd = new TH2D( "hspectraLGLSigmaVsLayer_2nd","Sigma Landau High Gain; layer; brd channel; #sigma_{L,LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGLSigmaVsLayer2nd->SetDirectory(0);
  TH2D* hspectraLGGSigmaVsLayer2nd = new TH2D( "hspectraLGGSigmaVsLayer_2nd","Sigma Gauss High Gain; layer; brd channel; #sigma_{G,LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGGSigmaVsLayer2nd->SetDirectory(0);

  TH1D* hMaxHG2nd             = new TH1D( "hMaxHG2nd","Max High Gain ;Max_{HG, 2^{nd}} (arb. units) ; counts ",
                                            2000, -0.5, 2000-0.5);
  hMaxHG2nd->SetDirectory(0);
  TH1D* hMaxLG2nd             = new TH1D( "hMaxLG2nd","Max High Gain ;Max_{LG, 2^{nd}} (arb. units) ; counts ",
                                            400, -0.5, 400-0.5);
  hMaxLG2nd->SetDirectory(0);


  currCells = 0;
  for(ithSpectraTrigg=hSpectraTrigg.begin(); ithSpectraTrigg!=hSpectraTrigg.end(); ++ithSpectraTrigg){
    if (currCells%20 == 0 && currCells > 0 && debug > 0)
      std::cout << "============================== cell " <<  currCells << " / " << hSpectraTrigg.size() << " cells" << std::endl;
    currCells++;
    for (int p = 0; p < 6; p++){
      parameters[p]   = 0;
      parErrAndRes[p] = 0;
    }
    isGood=ithSpectraTrigg->second.FitMipHG(parameters, parErrAndRes, debug, yearData, false, averageScale);
    
    long cellID     = ithSpectraTrigg->second.GetCellID();
    int layer       = setup->GetLayer(cellID);
    int chInLayer   = setup->GetChannelInLayer(cellID);    
    int bin2D       = hspectraHGMeanVsLayer->FindBin(layer,chInLayer);

    Int_t binNLow   = ithSpectraTrigg->second.GetHG()->FindBin(-1*calib.GetPedestalSigH(cellID));
    Int_t binNHigh  = ithSpectraTrigg->second.GetHG()->FindBin(3*calib.GetPedestalSigH(cellID));
    Int_t binSHigh  = ithSpectraTrigg->second.GetHG()->FindBin(3800);
    
    double S_NoiseR = ithSpectraTrigg->second.GetHG()->Integral(binNLow, binNHigh);
    double S_SigR   = ithSpectraTrigg->second.GetHG()->Integral(binNHigh, binSHigh);
    
    ithSpectra      = hSpectra.find(cellID);
    double B_NoiseR = ithSpectra->second.GetHG()->Integral(binNLow , binNHigh);
    double B_SigR   = ithSpectra->second.GetHG()->Integral(binNHigh, binSHigh);
    
    double SB_NoiseR  = (B_NoiseR != 0.) ? S_NoiseR/B_NoiseR : 0;
    double SB_SigR    = (B_SigR != 0.) ? S_SigR/B_SigR : 0;
    
    hmipTriggers->SetBinContent(bin2D, ithSpectraTrigg->second.GetHG()->GetEntries());
    hSuppresionNoise->SetBinContent(bin2D, SB_NoiseR);
    hSuppresionSignal->SetBinContent(bin2D, SB_SigR);
    if (isGood){
      hspectraHGMaxVsLayer2nd->SetBinContent(bin2D, parameters[4]);
      hspectraHGFWHMVsLayer2nd->SetBinContent(bin2D, parameters[5]);
      hspectraHGLMPVVsLayer2nd->SetBinContent(bin2D, parameters[1]);
      hspectraHGLMPVVsLayer2nd->SetBinError(bin2D, parErrAndRes[1]);
      hspectraHGLSigmaVsLayer2nd->SetBinContent(bin2D, parameters[0]);
      hspectraHGLSigmaVsLayer2nd->SetBinError(bin2D, parErrAndRes[0]);
      hspectraHGGSigmaVsLayer2nd->SetBinContent(bin2D, parameters[3]);
      hspectraHGGSigmaVsLayer2nd->SetBinError(bin2D, parErrAndRes[3]);
      hMaxHG2nd->Fill(parameters[4]);
    }
    for (int p = 0; p < 6; p++){
      parameters[p]   = 0;
      parErrAndRes[p] = 0;
    }
    isGood=ithSpectraTrigg->second.FitMipLG(parameters, parErrAndRes, debug, yearData, false, 1);
    if (isGood){
      hspectraLGMaxVsLayer2nd->SetBinContent(bin2D, parameters[4]);
      hspectraLGFWHMVsLayer2nd->SetBinContent(bin2D, parameters[5]);
      hspectraLGLMPVVsLayer2nd->SetBinContent(bin2D, parameters[1]);
      hspectraLGLMPVVsLayer2nd->SetBinError(bin2D, parErrAndRes[1]);
      hspectraLGLSigmaVsLayer2nd->SetBinContent(bin2D, parameters[0]);
      hspectraLGLSigmaVsLayer2nd->SetBinError(bin2D, parErrAndRes[0]);
      hspectraLGGSigmaVsLayer2nd->SetBinContent(bin2D, parameters[3]);
      hspectraLGGSigmaVsLayer2nd->SetBinError(bin2D, parErrAndRes[3]);
      hMaxLG2nd->Fill(parameters[4]);
    }
  }
  
  RootOutput->cd();
  TcalibOut->Fill();
  TcalibOut->Write();
  RootOutput->Close();


  RootOutputHist->cd("IndividualCells");
    for(ithSpectra=hSpectra.begin(); ithSpectra!=hSpectra.end(); ++ithSpectra){
      ithSpectra->second.Write(true);
    }
  RootOutputHist->cd("IndividualCellsTrigg");
    for(ithSpectra=hSpectraTrigg.begin(); ithSpectra!=hSpectraTrigg.end(); ++ithSpectra){
      ithSpectra->second.Write(true);
    }
  RootOutputHist->cd();
    hMeanPedHGvsCellID->Write();
    hMeanPedHG->Write();
    hMeanPedLGvsCellID->Write();
    hMeanPedLG->Write();
    
    hspectraHGMeanVsLayer->Write();
    hspectraLGMeanVsLayer->Write();
    hspectraHGSigmaVsLayer->Write();
    hspectraLGSigmaVsLayer->Write();
    hspectraHGMaxVsLayer1st->Write();
    hspectraHGFWHMVsLayer1st->Write();
    hspectraHGLMPVVsLayer1st->Write();
    hspectraHGLSigmaVsLayer1st->Write();
    hspectraHGGSigmaVsLayer1st->Write();
    hMaxHG1st->Write();
    
    hLGHGCorrVsLayer->Write();
    hHGLGCorrVsLayer->Write();
    hLGHGCorr->Write();
    hHGLGCorr->Write();
    
    hspectraHGMaxVsLayer2nd->Write();
    hspectraHGFWHMVsLayer2nd->Write();
    hspectraHGLMPVVsLayer2nd->Write();
    hspectraHGLSigmaVsLayer2nd->Write();
    hspectraHGGSigmaVsLayer2nd->Write();
    hMaxHG2nd->Write();
    
    hspectraLGMaxVsLayer2nd->Write();
    hspectraLGFWHMVsLayer2nd->Write();
    hspectraLGLMPVVsLayer2nd->Write();
    hspectraLGLSigmaVsLayer2nd->Write();
    hspectraLGGSigmaVsLayer2nd->Write();
    hMaxLG2nd->Write();
    
    for (int c = 0; c< maxChannelPerLayer; c++)
      hHGTileSum[c]->Write();
    hmipTriggers->Write();
    hSuppresionNoise->Write();
    hSuppresionSignal->Write();
  // fill calib tree & write it
  // close open root files
  RootOutputHist->Write();
  RootOutputHist->Close();

  RootInput->Close();

  // Get run info object
  std::map<int,RunInfo>::iterator it=ri.find(runNr);
  
  // create directory for plot output
  TString outputDirPlots = GetPlotOutputDir();
  gSystem->Exec("mkdir -p "+outputDirPlots);
  
  //**********************************************************************
  // Create canvases for channel overview plotting
  //**********************************************************************
  Double_t textSizeRel = 0.035;
  StyleSettingsBasics("pdf");
  SetPlotStyle();

  TCanvas* canvas2DCorr = new TCanvas("canvasCorrPlots","",0,0,1450,1200);  // gives the page size
  DefaultCancasSettings( canvas2DCorr, 0.08, 0.13, 0.02, 0.07);

  canvas2DCorr->SetLogz(0);
  PlotSimple2D( canvas2DCorr, hspectraHGMeanVsLayer, -10000, -10000, textSizeRel, Form("%s/HG_NoiseMean.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGSigmaVsLayer,-10000, -10000, textSizeRel, Form("%s/HG_NoiseSigma.pdf", outputDirPlots.Data()), it->second, 1,  kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGMaxVsLayer1st, -10000, -10000, textSizeRel, Form("%s/HG_MaxMip_1st.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGFWHMVsLayer1st, -10000, -10000, textSizeRel, Form("%s/HG_FWHMMip_1st.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGLMPVVsLayer1st, -10000, -10000, textSizeRel, Form("%s/HG_LandMPVMip_1st.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGLSigmaVsLayer1st, -10000, -10000, textSizeRel, Form("%s/HG_LandSigMip_1st.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGGSigmaVsLayer1st, -10000, -10000, textSizeRel, Form("%s/HG_GaussSigMip_1st.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGMaxVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/HG_MaxMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGFWHMVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/HG_FWHMMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGLMPVVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/HG_LandMPVMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGLSigmaVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/HG_LandSigMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGGSigmaVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/HG_GaussSigMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  canvas2DCorr->SetLogz(1);
  TString drawOpt = "colz";
  if (yearData == 2023)
    drawOpt = "colz,text";
  PlotSimple2D( canvas2DCorr, hmipTriggers, -10000, -10000, textSizeRel, Form("%s/MuonTriggers.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, drawOpt);
  PlotSimple2D( canvas2DCorr, hSuppresionNoise, -10000, -10000, textSizeRel, Form("%s/SuppressionNoise.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, drawOpt);
  PlotSimple2D( canvas2DCorr, hSuppresionSignal, -10000, -10000, textSizeRel, Form("%s/SuppressionSignal.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, drawOpt);
  
  canvas2DCorr->SetLogz(0);
  PlotSimple2D( canvas2DCorr, hspectraLGMeanVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_NoiseMean.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGSigmaVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_NoiseSigma.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGMaxVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/LG_MaxMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGFWHMVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/LG_FWHMMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGLMPVVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/LG_LandMPVMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGLSigmaVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/LG_LandSigMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGGSigmaVsLayer2nd, -10000, -10000, textSizeRel, Form("%s/LG_GaussSigMip_2nd.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");

  PlotSimple2D( canvas2DCorr, hLGHGCorrVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_HG_Corr.pdf", outputDirPlots.Data()), it->second, 1, kTRUE, "colz");
  PlotSimple2D( canvas2DCorr, hHGLGCorrVsLayer, -10000, -10000, textSizeRel, Form("%s/HG_LG_Corr.pdf", outputDirPlots.Data()), it->second, 1, kTRUE, "colz");
  //***********************************************************************************************************
  //********************************* 8 Panel overview plot  **************************************************
  //***********************************************************************************************************
  //*****************************************************************
    // Test beam geometry (beam coming from viewer)
    //===========================================================
    //||    8 (4)    ||    7 (5)   ||    6 (6)   ||    5 (7)   ||  row 0
    //===========================================================
    //||    1 (0)    ||    2 (1)   ||    3 (2)   ||    4 (3)   ||  row 1
    //===========================================================
    //    col 0     col 1       col 2     col  3
    // rebuild pad geom in similar way (numbering -1)
  //*****************************************************************
  TCanvas* canvas8Panel;
  TPad* pad8Panel[8];
  Double_t topRCornerX[8];
  Double_t topRCornerY[8];
  Int_t textSizePixel = 30;
  Double_t relSize8P[8];
  CreateCanvasAndPadsFor8PannelTBPlot(canvas8Panel, pad8Panel,  topRCornerX, topRCornerY, relSize8P, textSizePixel);

  TCanvas* canvas8PanelProf;
  TPad* pad8PanelProf[8];
  Double_t topRCornerXProf[8];
  Double_t topRCornerYProf[8];
  Double_t relSize8PProf[8];
  CreateCanvasAndPadsFor8PannelTBPlot(canvas8PanelProf, pad8PanelProf,  topRCornerXProf, topRCornerYProf, relSize8PProf, textSizePixel, 0.045, "Prof", false);

 
  for (Int_t l = 0; l < setup->GetNMaxLayer()+1; l++){    
    PlotMipWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                              hSpectra, hSpectraTrigg, setup, true, -100, 1500, 1.2, l, 0,
                              Form("%s/MIP_HG_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
    PlotMipWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                              hSpectra, hSpectraTrigg, setup, false, -100, 500, 1.2, l, 0,
                              Form("%s/MIP_LG_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
    PlotTriggerPrimWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                      hSpectraTrigg, setup, averageScale, factorMinTrigg, factorMaxTrigg,
                                      0, 4000, 1.2, l, 0, Form("%s/TriggPrimitive_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
    PlotCorrWithFitsFullLayer(canvas8PanelProf,pad8PanelProf, topRCornerXProf, topRCornerYProf, relSize8PProf, textSizePixel, 
                              hSpectra, setup, false, -100, 800, 1.2, l, 0,
                              Form("%s/LGHG_Corr_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
    PlotCorrWithFitsFullLayer(canvas8PanelProf,pad8PanelProf, topRCornerXProf, topRCornerYProf, relSize8PProf, textSizePixel, 
                              hSpectra, setup, true, -100, 4000, 1.2, l, 0,
                              Form("%s/HGLG_Corr_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
  }

  
  return true;
}

//***********************************************************************************************
//*********************** improved scaling calculation function *********************************
//***********************************************************************************************
bool Analyses::GetImprovedScaling(void){
  std::cout<<"GetImprovedScaling"<<std::endl;
  
  std::map<int,RunInfo> ri=readRunInfosFromFile(RunListInputName.Data(),debug,0);
  
  std::map<int,TileSpectra> hSpectra;
  std::map<int,TileSpectra> hSpectraTrigg;
  std::map<int, TileSpectra>::iterator ithSpectra;
  std::map<int, TileSpectra>::iterator ithSpectraTrigg;
  
  std::cout << "Additional Output with histos being created: " << RootOutputNameHist.Data() << std::endl;
  if(Overwrite){
    std::cout << "recreating file with hists" << std::endl;
    RootOutputHist = new TFile(RootOutputNameHist.Data(),"RECREATE");
  } else{
    std::cout << "newly creating file with hists" << std::endl;
    RootOutputHist = new TFile(RootOutputNameHist.Data(),"CREATE");
  }
    
  // setup trigger sel
  double factorMinTrigg   = 1.1;
  double factorMaxTrigg   = 2.5;
  if (yearData == 2023){
    factorMinTrigg    = 0.9;
    factorMaxTrigg    = 2.;
  }
  
  RootOutputHist->mkdir("IndividualCells");
  RootOutputHist->mkdir("IndividualCellsTrigg");
  RootOutputHist->cd("IndividualCellsTrigg");  
  //***********************************************************************************************
  //************************* first pass over tree to extract spectra *****************************
  //***********************************************************************************************  
  TcalibIn->GetEntry(0);
  int evts=TdataIn->GetEntries();
  int runNr = -1;
  double averageScale     = calib.GetAverageScaleHigh();
  double averageScaleLow  = calib.GetAverageScaleLow();
  std::cout << "average HG mip: " << averageScale << " LG mip: "<< averageScaleLow << std::endl;
  for(int i=0; i<evts; i++){
    TdataIn->GetEntry(i);
    if (i == 0)runNr = event.GetRunNumber();
    TdataIn->GetEntry(i);
    if (i%5000 == 0 && i > 0 && debug > 0) std::cout << "Reading " <<  i << " / " << evts << " events" << std::endl;
    for(int j=0; j<event.GetNTiles(); j++){
      Caen* aTile=(Caen*)event.GetTile(j);
      if (i == 0 && debug > 2) std::cout << ((TString)setup->DecodeCellID(aTile->GetCellID())).Data() << std::endl;
      long currCellID = aTile->GetCellID();
      
      // read current tile
      ithSpectraTrigg=hSpectraTrigg.find(aTile->GetCellID());
      double hgCorr = aTile->GetADCHigh()-calib.GetPedestalMeanH(aTile->GetCellID());
      double lgCorr = aTile->GetADCLow()-calib.GetPedestalMeanL(aTile->GetCellID());

      // estimate local muon trigger
      bool localMuonTrigg = event.InspectIfLocalMuonTrigg(currCellID, averageScale, factorMinTrigg, factorMaxTrigg);
      
      if(ithSpectraTrigg!=hSpectraTrigg.end()){
        ithSpectraTrigg->second.FillTrigger(aTile->GetLocalTriggerPrimitive());
      } else {
        RootOutputHist->cd("IndividualCellsTrigg");
        hSpectraTrigg[currCellID]=TileSpectra("mipTrigg",currCellID,calib.GetTileCalib(currCellID),debug);
        hSpectraTrigg[currCellID].FillTrigger(aTile->GetLocalTriggerPrimitive());;
        RootOutput->cd();
      }
      
      ithSpectra=hSpectra.find(aTile->GetCellID());
      if (ithSpectra!=hSpectra.end()){
        ithSpectra->second.FillSpectra(lgCorr,hgCorr);
        if (hgCorr > 3*calib.GetPedestalSigH(currCellID) && lgCorr > 3*calib.GetPedestalSigL(currCellID) && hgCorr < 3900 )
          ithSpectra->second.FillCorr(lgCorr,hgCorr);
      } else {
        RootOutputHist->cd("IndividualCells");
        hSpectra[currCellID]=TileSpectra("mip1st",currCellID,calib.GetTileCalib(currCellID),debug);
        hSpectra[aTile->GetCellID()].FillSpectra(lgCorr,hgCorr);;
        if (hgCorr > 3*calib.GetPedestalSigH(aTile->GetCellID()) && lgCorr > 3*calib.GetPedestalSigL(aTile->GetCellID() && hgCorr < 3900) )
          hSpectra[aTile->GetCellID()].FillCorr(lgCorr,hgCorr);;

        RootOutput->cd();
      }
      
     
      // only fill tile spectra if 4 surrounding tiles on average are compatible with muon response
      if (localMuonTrigg){
        ithSpectraTrigg=hSpectraTrigg.find(aTile->GetCellID());
        ithSpectraTrigg->second.FillSpectra(lgCorr,hgCorr);
        if (hgCorr > 3*calib.GetPedestalSigH(currCellID) && lgCorr > 3*calib.GetPedestalSigL(currCellID) && hgCorr < 3900 )
          ithSpectraTrigg->second.FillCorr(lgCorr,hgCorr);
      }
    }
    RootOutput->cd();
    TdataOut->Fill();
  }
  TdataOut->Write();
  TsetupIn->CloneTree()->Write();
  
  //***********************************************************************************************
  //***** Monitoring histos for fits results of 2nd iteration ******************
  //***********************************************************************************************
  RootOutputHist->cd();
  int maxChannelPerLayer        = (setup->GetNMaxColumn()+1)*(setup->GetNMaxRow()+1);
  // monitoring trigger 
  TH2D* hmipTriggers              = new TH2D( "hmipTriggers","muon triggers; layer; brd channel; counts ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hmipTriggers->SetDirectory(0);
  TH2D* hSuppresionNoise          = new TH2D( "hSuppresionNoise","S/B noise region; layer; brd channel; S/B noise region",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hSuppresionNoise->SetDirectory(0);
  TH2D* hSuppresionSignal         = new TH2D( "hSuppresionSignal","S/B signal region; layer; brd channel; S/B signal region",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hSuppresionSignal->SetDirectory(0);

  // monitoring 2nd iteration mip fits
  TH2D* hspectraHGMaxVsLayer   = new TH2D( "hspectraHGMaxVsLayer","Max High Gain; layer; brd channel; Max_{HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGMaxVsLayer->SetDirectory(0);
  TH2D* hspectraHGFWHMVsLayer   = new TH2D( "hspectraHGFWHMVsLayer","FWHM High Gain; layer; brd channel; FWHM_{HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGFWHMVsLayer->SetDirectory(0);
  TH2D* hspectraHGLMPVVsLayer   = new TH2D( "hspectraHGLMPVVsLayer","MPV High Gain; layer; brd channel; MPV_{HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGLMPVVsLayer->SetDirectory(0);
  TH2D* hspectraHGLSigmaVsLayer = new TH2D( "hspectraHGLSigmaVsLayer","Sigma Landau High Gain; layer; brd channel; #sigma_{L,HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGLSigmaVsLayer->SetDirectory(0);
  TH2D* hspectraHGGSigmaVsLayer = new TH2D( "hspectraHGGSigmaVsLayer","Sigma Gauss High Gain; layer; brd channel; #sigma_{G,HG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraHGGSigmaVsLayer->SetDirectory(0);
  TH2D* hspectraLGMaxVsLayer   = new TH2D( "hspectraLGMaxVsLayer","Max High Gain; layer; brd channel; Max_{LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGMaxVsLayer->SetDirectory(0);
  TH2D* hspectraLGFWHMVsLayer   = new TH2D( "hspectraLGFWHMVsLayer","FWHM High Gain; layer; brd channel; FWHM_{LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGFWHMVsLayer->SetDirectory(0);
  TH2D* hspectraLGLMPVVsLayer   = new TH2D( "hspectraLGLMPVVsLayer","MPV High Gain; layer; brd channel; MPV_{LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGLMPVVsLayer->SetDirectory(0);
  TH2D* hspectraLGLSigmaVsLayer = new TH2D( "hspectraLGLSigmaVsLayer","Sigma Landau High Gain; layer; brd channel; #sigma_{L,LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGLSigmaVsLayer->SetDirectory(0);
  TH2D* hspectraLGGSigmaVsLayer = new TH2D( "hspectraLGGSigmaVsLayer","Sigma Gauss High Gain; layer; brd channel; #sigma_{G,LG, 2^{nd}} (arb. units) ",
                                            setup->GetNMaxLayer()+1, -0.5, setup->GetNMaxLayer()+1-0.5, maxChannelPerLayer, -0.5, maxChannelPerLayer-0.5);
  hspectraLGGSigmaVsLayer->SetDirectory(0);

  TH1D* hMaxHG             = new TH1D( "hMaxHG","Max High Gain ;Max_{HG} (arb. units) ; counts ",
                                            2000, -0.5, 2000-0.5);
  hMaxHG->SetDirectory(0);
  TH1D* hMaxLG             = new TH1D( "hMaxLG","Max Low Gain ;Max_{LG} (arb. units) ; counts ",
                                            400, -0.5, 400-0.5);
  hMaxLG->SetDirectory(0);


  int currCells = 0;
  double* parameters    = new double[6];
  double* parErrAndRes  = new double[6];
  bool isGood;
  for(ithSpectraTrigg=hSpectraTrigg.begin(); ithSpectraTrigg!=hSpectraTrigg.end(); ++ithSpectraTrigg){
    if (currCells%20 == 0 && currCells > 0 && debug > 0)
      std::cout << "============================== cell " <<  currCells << " / " << hSpectraTrigg.size() << " cells" << std::endl;
    currCells++;
    for (int p = 0; p < 6; p++){
      parameters[p]   = 0;
      parErrAndRes[p] = 0;
    }
    isGood=ithSpectraTrigg->second.FitMipHG(parameters, parErrAndRes, debug, yearData, true, averageScale);
    
    long cellID     = ithSpectraTrigg->second.GetCellID();
    int layer       = setup->GetLayer(cellID);
    int chInLayer   = setup->GetChannelInLayer(cellID);    
    int bin2D       = hspectraHGMaxVsLayer->FindBin(layer,chInLayer);

    Int_t binNLow   = ithSpectraTrigg->second.GetHG()->FindBin(-1*calib.GetPedestalSigH(cellID));
    Int_t binNHigh  = ithSpectraTrigg->second.GetHG()->FindBin(3*calib.GetPedestalSigH(cellID));
    Int_t binSHigh  = ithSpectraTrigg->second.GetHG()->FindBin(3800);
    
    double S_NoiseR = ithSpectraTrigg->second.GetHG()->Integral(binNLow, binNHigh);
    double S_SigR   = ithSpectraTrigg->second.GetHG()->Integral(binNHigh, binSHigh);
    
    ithSpectra      = hSpectra.find(cellID);
    double B_NoiseR = ithSpectra->second.GetHG()->Integral(binNLow , binNHigh);
    double B_SigR   = ithSpectra->second.GetHG()->Integral(binNHigh, binSHigh);
    
    double SB_NoiseR  = (B_NoiseR != 0.) ? S_NoiseR/B_NoiseR : 0;
    double SB_SigR    = (B_SigR != 0.) ? S_SigR/B_SigR : 0;
    
    hmipTriggers->SetBinContent(bin2D, ithSpectraTrigg->second.GetHG()->GetEntries());
    hSuppresionNoise->SetBinContent(bin2D, SB_NoiseR);
    hSuppresionSignal->SetBinContent(bin2D, SB_SigR);
    if (isGood){
      hspectraHGMaxVsLayer->SetBinContent(bin2D, parameters[4]);
      hspectraHGFWHMVsLayer->SetBinContent(bin2D, parameters[5]);
      hspectraHGLMPVVsLayer->SetBinContent(bin2D, parameters[1]);
      hspectraHGLMPVVsLayer->SetBinError(bin2D, parErrAndRes[1]);
      hspectraHGLSigmaVsLayer->SetBinContent(bin2D, parameters[0]);
      hspectraHGLSigmaVsLayer->SetBinError(bin2D, parErrAndRes[0]);
      hspectraHGGSigmaVsLayer->SetBinContent(bin2D, parameters[3]);
      hspectraHGGSigmaVsLayer->SetBinError(bin2D, parErrAndRes[3]);
      hMaxHG->Fill(parameters[4]);
    }
    for (int p = 0; p < 6; p++){
      parameters[p]   = 0;
      parErrAndRes[p] = 0;
    }
    isGood=ithSpectraTrigg->second.FitMipLG(parameters, parErrAndRes, debug, yearData, true, averageScaleLow);
    if (isGood){
      hspectraLGMaxVsLayer->SetBinContent(bin2D, parameters[4]);
      hspectraLGFWHMVsLayer->SetBinContent(bin2D, parameters[5]);
      hspectraLGLMPVVsLayer->SetBinContent(bin2D, parameters[1]);
      hspectraLGLMPVVsLayer->SetBinError(bin2D, parErrAndRes[1]);
      hspectraLGLSigmaVsLayer->SetBinContent(bin2D, parameters[0]);
      hspectraLGLSigmaVsLayer->SetBinError(bin2D, parErrAndRes[0]);
      hspectraLGGSigmaVsLayer->SetBinContent(bin2D, parameters[3]);
      hspectraLGGSigmaVsLayer->SetBinError(bin2D, parErrAndRes[3]);
      hMaxLG->Fill(parameters[4]);
    }
  }
  
  RootOutput->cd();
  TcalibOut->Fill();
  TcalibOut->Write();
  
  double averageScaleUpdated     = calib.GetAverageScaleHigh();
  double averageScaleUpdatedLow  = calib.GetAverageScaleLow();
  std::cout << "average input HG mip: " << averageScale << " LG mip: "<< averageScaleLow << std::endl;
  std::cout << "average updated HG mip: " << averageScaleUpdated << " LG mip: "<< averageScaleUpdatedLow << std::endl;

  RootOutput->Close();


  RootOutputHist->cd("IndividualCellsTrigg");
    for(ithSpectra=hSpectraTrigg.begin(); ithSpectra!=hSpectraTrigg.end(); ++ithSpectra){
      ithSpectra->second.Write(true);
    }
  RootOutputHist->cd();
    
    hspectraHGMaxVsLayer->Write();
    hspectraHGFWHMVsLayer->Write();
    hspectraHGLMPVVsLayer->Write();
    hspectraHGLSigmaVsLayer->Write();
    hspectraHGGSigmaVsLayer->Write();
    hMaxHG->Write();
    
    hspectraLGMaxVsLayer->Write();
    hspectraLGFWHMVsLayer->Write();
    hspectraLGLMPVVsLayer->Write();
    hspectraLGLSigmaVsLayer->Write();
    hspectraLGGSigmaVsLayer->Write();
    hMaxLG->Write();
    
    hmipTriggers->Write();
    hSuppresionNoise->Write();
    hSuppresionSignal->Write();
  // fill calib tree & write it
  // close open root files
  RootOutputHist->Write();
  RootOutputHist->Close();

  RootInput->Close();

  // Get run info object
  std::map<int,RunInfo>::iterator it=ri.find(runNr);
  
  // create directory for plot output
  TString outputDirPlots = GetPlotOutputDir();
  gSystem->Exec("mkdir -p "+outputDirPlots);
  
  //**********************************************************************
  // Create canvases for channel overview plotting
  //**********************************************************************
  Double_t textSizeRel = 0.035;
  StyleSettingsBasics("pdf");
  SetPlotStyle();

  TCanvas* canvas2DCorr = new TCanvas("canvasCorrPlots","",0,0,1450,1200);  // gives the page size
  DefaultCancasSettings( canvas2DCorr, 0.08, 0.13, 0.02, 0.07);

  canvas2DCorr->SetLogz(0);
  PlotSimple2D( canvas2DCorr, hspectraHGMaxVsLayer, -10000, -10000, textSizeRel, Form("%s/HG_MaxMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGFWHMVsLayer, -10000, -10000, textSizeRel, Form("%s/HG_FWHMMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGLMPVVsLayer, -10000, -10000, textSizeRel, Form("%s/HG_LandMPVMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGLSigmaVsLayer, -10000, -10000, textSizeRel, Form("%s/HG_LandSigMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraHGGSigmaVsLayer, -10000, -10000, textSizeRel, Form("%s/HG_GaussSigMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  canvas2DCorr->SetLogz(1);
  TString drawOpt = "colz";
  if (yearData == 2023)
    drawOpt = "colz,text";
  PlotSimple2D( canvas2DCorr, hmipTriggers, -10000, -10000, textSizeRel, Form("%s/MuonTriggers.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, drawOpt);
  PlotSimple2D( canvas2DCorr, hSuppresionNoise, -10000, -10000, textSizeRel, Form("%s/SuppressionNoise.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, drawOpt);
  PlotSimple2D( canvas2DCorr, hSuppresionSignal, -10000, -10000, textSizeRel, Form("%s/SuppressionSignal.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, drawOpt);
  
  canvas2DCorr->SetLogz(0);
  PlotSimple2D( canvas2DCorr, hspectraLGMaxVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_MaxMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGFWHMVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_FWHMMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGLMPVVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_LandMPVMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGLSigmaVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_LandSigMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");
  PlotSimple2D( canvas2DCorr, hspectraLGGSigmaVsLayer, -10000, -10000, textSizeRel, Form("%s/LG_GaussSigMip.pdf", outputDirPlots.Data()), it->second, 1, kFALSE, "colz");

  //***********************************************************************************************************
  //********************************* 8 Panel overview plot  **************************************************
  //***********************************************************************************************************
  //*****************************************************************
    // Test beam geometry (beam coming from viewer)
    //===========================================================
    //||    8 (4)    ||    7 (5)   ||    6 (6)   ||    5 (7)   ||  row 0
    //===========================================================
    //||    1 (0)    ||    2 (1)   ||    3 (2)   ||    4 (3)   ||  row 1
    //===========================================================
    //    col 0     col 1       col 2     col  3
    // rebuild pad geom in similar way (numbering -1)
  //*****************************************************************
  TCanvas* canvas8Panel;
  TPad* pad8Panel[8];
  Double_t topRCornerX[8];
  Double_t topRCornerY[8];
  Int_t textSizePixel = 30;
  Double_t relSize8P[8];
  CreateCanvasAndPadsFor8PannelTBPlot(canvas8Panel, pad8Panel,  topRCornerX, topRCornerY, relSize8P, textSizePixel);
 
  for (Int_t l = 0; l < setup->GetNMaxLayer()+1; l++){    
    PlotMipWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                              hSpectra, hSpectraTrigg, setup, true, -100, 1500, 1.2, l, 0,
                              Form("%s/MIP_HG_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
    PlotMipWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                              hSpectra, hSpectraTrigg, setup, false, -100, 500, 1.2, l, 0,
                              Form("%s/MIP_LG_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
    PlotTriggerPrimWithFitsFullLayer (canvas8Panel,pad8Panel, topRCornerX, topRCornerY, relSize8P, textSizePixel, 
                                      hSpectraTrigg, setup, averageScale, factorMinTrigg, factorMaxTrigg,
                                      0, 4000, 1.2, l, 0, Form("%s/TriggPrimitive_Layer%02d.pdf" ,outputDirPlots.Data(), l), it->second);
  }

  
  return true;
}

//***********************************************************************************************
//*********************** Calibration routine ***************************************************
//***********************************************************************************************
bool Analyses::Calibrate(void){
  std::cout<<"Calibrate"<<std::endl;
  TcalibIn->GetEntry(0);
  int evts=TdataIn->GetEntries();
  for(int i=0; i<evts; i++){
    TdataIn->GetEntry(i);
    for(int j=0; j<event.GetNTiles(); j++){
      Caen* aTile=(Caen*)event.GetTile(j);
      double energy=0;
      if(aTile->GetADCHigh()<3800){
        energy=(aTile->GetADCHigh()-calib.GetPedestalMeanH(aTile->GetCellID()))/calib.GetScaleHigh(aTile->GetCellID());
      }
      else{
        energy=(aTile->GetADCLow()-calib.GetPedestalMeanL(aTile->GetCellID()))/calib.GetScaleLow(aTile->GetCellID());
      }
      aTile->SetE(energy);
    }
    RootOutput->cd();
    TdataOut->Fill();
  }
  TdataOut->Write();
  TsetupIn->CloneTree()->Write();
  TcalibOut->Fill();
  TcalibOut->Write();
  RootOutput->Close();
  RootInput->Close();      
  
  return true;
}


//***********************************************************************************************
//*********************** Create output files ***************************************************
//***********************************************************************************************
bool Analyses::CreateOutputRootFile(void){
  if(Overwrite){
    RootOutput=new TFile(RootOutputName.Data(),"RECREATE");
  } else{
    RootOutput = new TFile(RootOutputName.Data(),"CREATE");
  }
  if(RootOutput->IsZombie()){
    std::cout<<"Error opening '"<<RootOutput<<"'no reachable path? Exist without force mode to overwrite?..."<<std::endl;
    return false;
  }
  return true;
}


