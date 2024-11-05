#include "Analyses.h"
#include <vector>
#include "TF1.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

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
    TString tempNameOut = RootOutputName.ReplaceAll(".root","_Hists.root");
    if(Overwrite){
      RootOutput=new TFile(RootOutputName.Data(),"RECREATE");
      if (!Convert){
        std::cout << "creating additional histo file: " << tempNameOut.Data()<< std::endl;
        RootOutputHist=new TFile(tempNameOut.Data(),"RECREATE");
      }
    } else{
      RootOutput = new TFile(RootOutputName.Data(),"CREATE");
      if (!Convert){
         std::cout << "creating additional histo file: " << tempNameOut.Data()<< std::endl;
         RootOutputHist = new TFile(tempNameOut.Data(),"CREATE");
      }
    }
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
    //matchingbranch=TcalibIn->SetBranchAddress("calib",&calibextra);
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
  std::map<int,RunInfo> ri=readRunInfosFromFile("./DataTakingDB.csv",0,0);
  TObjArray* tok=ASCIIinputName.Tokenize("/");
  TString file=((TObjString*)tok->At(tok->GetEntries()-1))->String();
  delete tok;
  tok=file.Tokenize("_");
  TString header=((TObjString*)tok->At(0))->String();
  delete tok;
  TString RunString=header(3,header.Sizeof());
  std::map<int,RunInfo>::iterator it=ri.find(RunString.Atoi());
  //std::cout<<RunString.Atoi()<<"\t"<<it->second.species<<std::endl;
  event.SetRunNumber(RunString.Atoi());
  event.SetROtype(2);
  event.SetBeamName(it->second.species);
  event.SetBeamID(it->second.pdg);
  event.SetBeamEnergy(it->second.energy);
  TString aline;
  TObjArray* tokens;
  std::map<int,std::vector<Caen> > tmpEvent;
  std::map<int,double> tmpTime;
  std::map<int,std::vector<Caen> >::iterator itevent;
  long tempEvtCounter = 0;
  while(!ASCIIinput.eof()){
    aline.ReadLine(ASCIIinput);
    if(!ASCIIinput.good()) break;
    if(aline[0]=='/')continue;
    tokens=aline.Tokenize(" \t");
    tokens->SetOwner(true);
    int Nfields=tokens->GetEntries();
    if(((TObjString*)tokens->At(0))->String()=="Brd") {
      tokens->Clear();
      delete tokens;
      continue;
    }
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
      itevent->second.push_back(aTile);
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
          itevent->second.push_back(aTile);
          tokens->Clear();
          delete tokens;
      }
      if((int)itevent->second.size()==setup->GetTotalNbChannels()/*8*64*/){
        //Fill the tree the event is complete and erase from the map
        event.SetTimeStamp(tmpTime[TriggerID]/8);
        event.SetEventID(itevent->first);
        for(std::vector<Caen>::iterator itv=itevent->second.begin(); itv!=itevent->second.end(); ++itv){
          event.AddTile(new Caen(*itv));
        }
        TdataOut->Fill();
        tmpEvent.erase(itevent);
        tmpTime.erase(TriggerID);
      }
    }
    else{//This is a new event;
      tempEvtCounter++;
      if (tempEvtCounter%5000 == 0 && debug > 0) std::cout << "Converted " <<  tempEvtCounter << " events" << std::endl;
      std::vector<Caen> vCaen;
      vCaen.push_back(aTile);
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
        vCaen.push_back(aTile);
        tokens->Clear();
        delete tokens;
      }
      tmpTime[TriggerID]=Time;
      tmpEvent[TriggerID]=vCaen;
    }
  }
  if (debug > 0) std::cout << "Converted a total of " <<  tempEvtCounter << " events" << std::endl;
  
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

// ****************************************************************************
// extract pedestral from dedicated data run, no other data in run 
// ****************************************************************************
bool Analyses::GetPedestal(void){
  std::cout<<"GetPedestal"<<std::endl;
  
  // create HG and LG histo's per channel
  TH1D** hspectraHG         = new TH1D*[setup->GetTotalNbChannels()];
  TH1D** hspectraLG         = new TH1D*[setup->GetTotalNbChannels()];
  
  TF1** pedHG               = new TF1*[setup->GetTotalNbChannels()];
  TF1** pedLG               = new TF1*[setup->GetTotalNbChannels()];
  TH2D* hspectraHGvsCellID  = new TH2D( "hspectraHG_vsCellID","ADC spectrum High Gain vs CellID ",
                                        setup->GetTotalNbChannels(), -0.5, setup->GetTotalNbChannels()-0.5, 4000,0,4000);
  TH2D* hspectraLGvsCellID  = new TH2D( "hspectraLG_vsCellID","ADC spectrum Low Gain vs CellID ",
                                        setup->GetTotalNbChannels(), -0.5, setup->GetTotalNbChannels()-0.5, 4000,0,4000);
  TH1D* hMeanPedHGvsCellID  = new TH1D( "hMeanPedHG_vsCellID","mean Ped High Gain vs CellID ",
                                        setup->GetTotalNbChannels(), -0.5, setup->GetTotalNbChannels()-0.5);
  TH1D* hMeanPedLGvsCellID  = new TH1D( "hMeanPedLG_vsCellID","mean Ped Low Gain vs CellID ",
                                        setup->GetTotalNbChannels(), -0.5, setup->GetTotalNbChannels()-0.5);
  for(int ich=0; ich<setup->GetTotalNbChannels(); ich++){
    hspectraHG[ich]=new TH1D(Form("hspectraHGCellID%d",ich),Form("ADC spectrum High Gain CellID %d",ich),4000,0,4000);
    hspectraLG[ich]=new TH1D(Form("hspectraLGCellID%d",ich),Form("ADC spectrum Low  Gain CellID %d",ich),4000,0,4000);
  }
  
  std::cout << "Have " << 64 << " layers/module" << std::endl; 
  // Event loop to fill histograms & output tree
  int evts=TdataIn->GetEntries();
  for(int i=0; i<evts; i++){
    TdataIn->GetEntry(i);
    if (i%5000 == 0&& i > 0 && debug > 0) std::cout << "Reading " <<  i << "/" << evts << " events"<< std::endl;
    for(int j=0; j<event.GetNTiles(); j++){
      Caen* aTile=(Caen*)event.GetTile(j);
      if (i == 0 && debug > 2) std::cout << ((TString)setup->DecodeCellID(aTile->GetCellID())).Data() << std::endl;
      hspectraHG[aTile->GetCellID()]->Fill(aTile->GetADCHigh());
      hspectraLG[aTile->GetCellID()]->Fill(aTile->GetADCLow());
      hspectraHGvsCellID->Fill(aTile->GetCellID(), aTile->GetADCHigh());
      hspectraLGvsCellID->Fill(aTile->GetCellID(), aTile->GetADCLow());
    }
    RootOutput->cd();
    TdataOut->Fill();
  }
  // write output tree (copy of raw data)
  TdataOut->Write();
  // write setup tree (copy of raw data)
  TsetupIn->CloneTree()->Write();
  
  
  RootOutputHist->cd();
  // fit pedestal   
  TFitResultPtr result;
  for(int ich=0; ich<setup->GetTotalNbChannels(); ich++){
    // estimate HG pedestal per channel
    if (debug > 1) std::cout << "fitting channel: " << ich << std::endl;
    pedHG[ich] = new TF1(Form("fpedHGCellID%d",ich),"gaus",0,400);
    pedHG[ich]->SetNpx(400);
    pedHG[ich]->SetParLimits(1,0,hspectraHG[ich]->GetMean()+100);     // might need to make these values settable
    pedHG[ich]->SetParLimits(2,0,100);     // might need to make these values settable    
    pedHG[ich]->SetParLimits(0,0,hspectraHG[ich]->GetEntries());
    pedHG[ich]->SetParameter(0,hspectraHG[ich]->GetEntries()/5);
    pedHG[ich]->SetParameter(1,hspectraHG[ich]->GetMean());
    pedHG[ich]->SetParameter(2,10);
    result=hspectraHG[ich]->Fit(pedHG[ich],"QRMNE0S");      // initial fit
    double minHGFit = result->Parameter(1)-2*result->Parameter(2);
    double maxHGFit = result->Parameter(1)+2*result->Parameter(2);
    if (debug > 1) std::cout << minHGFit << "\t" << maxHGFit << std::endl;
    result=hspectraHG[ich]->Fit(pedHG[ich],"QRMNE0S","",minHGFit, maxHGFit);  // limit to 2sigma range of previous fit
    hMeanPedHGvsCellID->SetBinContent(ich, result->Parameter(1));
    hMeanPedHGvsCellID->SetBinError(ich, result->Parameter(2));
    
    // write fitted values to calib object
    calib.SetPedestalMeanH(result->Parameter(1),ich);
    calib.SetPedestalSigH(result->Parameter(2),ich);
    // estimate LG pedestal per channel
    pedLG[ich] = new TF1(Form("fpedLGCellID%d",ich),"gaus",0,400);
    pedLG[ich]->SetNpx(400);
    pedLG[ich]->SetParLimits(1,0,hspectraLG[ich]->GetMean()+100);     // might need to make these values settable
    pedLG[ich]->SetParLimits(2,0,100);     // might need to make these values settable    
    pedLG[ich]->SetParLimits(0,0,hspectraLG[ich]->GetEntries());
    pedLG[ich]->SetParameter(0,hspectraLG[ich]->GetEntries()/5);
    pedLG[ich]->SetParameter(1,hspectraLG[ich]->GetMean());
    pedLG[ich]->SetParameter(2,10);
    result=hspectraLG[ich]->Fit(pedLG[ich],"QRMNE0S"); // initial fit
    double minLGFit = result->Parameter(1)-2*result->Parameter(2);
    double maxLGFit = result->Parameter(1)+2*result->Parameter(2);
    if (debug > 1) std::cout << minLGFit << "\t" << maxLGFit << "\t" << hspectraLG[ich]->GetEntries() << "\t" << hspectraLG[ich]->GetMean()<< std::endl;
    result=hspectraLG[ich]->Fit(pedLG[ich],"QRMNE0S","", minLGFit, maxLGFit);  // limit to 2sigma
    // write fitted values to calib object
    calib.SetPedestalMeanL(result->Parameter(1),ich);
    calib.SetPedestalSigL(result->Parameter(2),ich);
    hMeanPedLGvsCellID->SetBinContent(ich, result->Parameter(1));
    hMeanPedLGvsCellID->SetBinError(ich, result->Parameter(2));

    // write Histo and fit to output
    // hspectraLG[ich]->Write();
  }  
  TcalibOut->Fill();
  TcalibOut->Write();

  // entering histoOutput file
  RootOutputHist->cd();
  for(int ich=0; ich<setup->GetTotalNbChannels(); ich++){
    // write Histo and fit to output
    if(hspectraHG[ich]) hspectraHG[ich]->Write();
    if(pedHG[ich]) pedHG[ich]->Write();
    if(hspectraLG[ich]) hspectraLG[ich]->Write();
    if(pedLG[ich]) pedLG[ich]->Write();
  }
  hspectraHGvsCellID->Write();
  hMeanPedHGvsCellID->Write();
  hspectraLGvsCellID->Write();
  hMeanPedLGvsCellID->Write();
  
  // fill calib tree & write it
  // close open root files
  RootOutputHist->Close();
  RootOutput->Close();
  RootInput->Close();
  return true;
}

bool Analyses::CorrectPedestal(void){
  std::cout<<"Correct Pedestal"<<std::endl;
  int evts=TdataIn->GetEntries();
  for(int i=0; i<evts; i++){
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
bool Analyses::GetScaling(void){
  std::cout<<"GetScaling"<<std::endl;
  TH1D** hspectraHG   = new TH1D*[setup->GetTotalNbChannels()];
  TProfile** hspectraLGHG = new TProfile*[setup->GetTotalNbChannels()];
  for(int ich=0; ich<setup->GetTotalNbChannels(); ich++){
    hspectraHG[ich]  =new TH1D(Form("hMIPspectraHGCellID%d",ich),Form("ADC spectrum High Gain CellID %d",ich),4000,0,4000);
    hspectraLGHG[ich]=new TProfile(Form("hCoorspectraLGHGCellID%d",ich),Form("ADC Low  Gain/High Gain correlation CellID %d",ich),800,0,800);
  }
  TcalibIn->GetEntry(0);
  int evts=TdataIn->GetEntries();
  for(int i=0; i<evts; i++){
    TdataIn->GetEntry(i);
    if (i%5000 == 0 && i > 0 && debug > 0) std::cout << "Reading " <<  i << " events/ " << evts << std::endl;
    if (i == 0 && debug > 2) std::cout << "decoding cell IDs" << std::endl ;
    for(int j=0; j<event.GetNTiles(); j++){
      Caen* aTile=(Caen*)event.GetTile(j);
      if (i == 0 && debug > 2) std::cout << ((TString)setup->DecodeCellID(aTile->GetCellID())).Data() << std::endl;
      if(aTile->GetADCHigh()<3900) hspectraHG[aTile->GetCellID()]->Fill(aTile->GetADCHigh()-calib.GetPedestalMeanH(aTile->GetCellID())-7*calib.GetPedestalSigH(aTile->GetCellID()));
      if((aTile->GetADCLow() -calib.GetPedestalMeanL(aTile->GetCellID())-3*calib.GetPedestalSigL(aTile->GetCellID())>0)&&//Above Low gain noise
        (aTile->GetADCHigh()-calib.GetPedestalMeanH(aTile->GetCellID())-3*calib.GetPedestalSigH(aTile->GetCellID())>0)&&//Above High gain noise
        (aTile->GetADCHigh()<3900))//below saturation
        hspectraLGHG[aTile->GetCellID()]->Fill(aTile->GetADCLow()-calib.GetPedestalMeanL(aTile->GetCellID()),aTile->GetADCHigh()-calib.GetPedestalMeanH(aTile->GetCellID()));
    }
    RootOutput->cd();
    TdataOut->Fill();
  }
  TdataOut->Write();
  TsetupIn->CloneTree()->Write();
  TF1* corr = new TF1("corr","[0]+[1]*x",0,4000);
  TF1* sign = new TF1("sign","[0]*TMath::Landau(x,[1],[2],false)+[3]*exp(-[4]*x)+[5]*exp(-(x-[6])*(x-[6])/2/[7]/[7])",0,4000);
  sign->SetParLimits(0,0,1e7);
  sign->SetParLimits(1,0,600);
  sign->SetParLimits(2,0,600);
  sign->SetParLimits(3,0,1e6);
  sign->SetParLimits(4,0,10);
  sign->SetParLimits(6,-1000,0);
  sign->SetParLimits(7,0,500);
  TFitResultPtr result;
  corr->SetParLimits(0,-50,200);
  corr->SetParLimits(1,0, 50);
  for(int ich=0; ich<setup->GetTotalNbChannels(); ich++){
    sign->SetParameter(0,hspectraHG[ich]->GetMaximum());
    sign->SetParameter(1,hspectraHG[ich]->GetMaximumBin()+15);
    sign->SetParameter(2,50.);
    sign->SetParameter(3,10);
    sign->SetParameter(4,0.01);
    sign->SetParameter(5,hspectraHG[ich]->GetMaximum());
    sign->FixParameter(6,-7*calib.GetPedestalSigH(ich));
    sign->FixParameter(7,calib.GetPedestalSigH(ich));
    result=hspectraHG[ich]->Fit(sign,"SR");
    hspectraHG[ich]->Write();
    //std::cout<<ich<<"\t"<<hspectraLGHG[ich]->GetEntries()<<" "<<result <<std::endl;
    if(result!=-1) calib.SetScaleHigh(result->Parameter(1)+7*calib.GetPedestalSigH(ich),ich);
    else calib.SetScaleHigh(0.,ich);
    corr->SetParameter(0,0.);
    corr->SetParameter(1,10.);
    result=hspectraLGHG[ich]->Fit(corr,"SR");
    hspectraLGHG[ich]->Write();
    if(result!=-1)calib.SetScaleLow(calib.GetScaleHigh(ich)/result->Parameter(1),ich);
    else calib.SetScaleLow(0,ich);
  }
  TcalibOut->Fill();
  TcalibOut->Write();
  RootOutput->Close();
  RootInput->Close();      
  return true;
}
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
