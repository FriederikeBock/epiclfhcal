#include "iostream"
#include "fstream"
#include "vector"
#include "map"
#include "utility"
#include "TString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1D.h"
#include "TObjArray.h"
#include "TObjString.h"

using namespace std;

struct MyBoard {
  int boardID;
  int LG[64];
  int HG[64];
  float time;
  float x[64];
  float y[64];
  float z[64];
};
struct LRC {
  int layer;
  int row;
  int col;
};

map<pair<int, int>, LRC> LoadMapping(TString);

int main(int argc, char* argv[]){

  TF1* ped=new TF1("ped","gaus",0,600);
  TString InputName=Form("%s",argv[1]);
  TString OutputName=Form("%s.root",TString(InputName(0,InputName.Length()-4)).Data());
  cout<<OutputName.Data()<<endl;
  TString MappingName=Form("%s",argv[2]);
  cout<<MappingName<<endl;
  TString CalibName;
  int ReadWrite=0;
  fstream CalibFile;
  if(argc>3){
    if(TString(Form("%s",argv[4])).Atoi()==1) ReadWrite=1;
    cout<<"Taking calibration from "<<argv[3]<<endl;
    CalibName=Form("%s",argv[3]);
    if(ReadWrite) CalibFile.open(CalibName.Data(),ios::out);
    else CalibFile.open(CalibName.Data(),ios::in);
  }
  //  return 0;
  TFile* output=new TFile(OutputName.Data(),"RECREATE");
  int Trg;
  float time[8];
  int HGcaen[8][64];
  int LGcaen[8][64];
  float xcaen[8][64];
  float ycaen[8][64];
  float zcaen[8][64];
  //TString asscaen[8][64];
  int HGlrc[64][2][4];
  int LGlrc[64][2][4];
  float xlrc[64][2][4];
  float ylrc[64][2][4];
  float zlrc[64][2][4];
  //TString asslrc[64][2][4];
  TTree* tree = new TTree("tree","tree");
  tree->Branch("TriggerID",&Trg,"TriggerID/I");
  tree->Branch("time",time,"time[8]/F");
  tree->Branch("HGcaen",HGcaen,"HGcaen[8][64]/I");
  tree->Branch("LGcaen",LGcaen,"LGcaen[8][64]/I");
  tree->Branch("xcaen",xcaen,"xcaen[8][64]/F");
  tree->Branch("ycaen",ycaen,"ycaen[8][64]/F");
  tree->Branch("zcaen",zcaen,"zcaen[8][64]/F");
  //tree->Branch("asscaen",asscaen)
  tree->Branch("HGlrc",HGlrc,"HGlrc[64][2][4]/I");
  tree->Branch("LGlrc",LGlrc,"LGlrc[64][2][4]/I");
  tree->Branch("xlrc",xlrc,"xlrc[64][2][4]/F");
  tree->Branch("ylrc",ylrc,"ylrc[64][2][4]/F");
  tree->Branch("zlrc",zlrc,"zlrc[64][2][4]/F");
  fstream ASCIIinput;
  //ASCIIinput.open("/home/vandrieu/Documents/ePIC/LFHCAL_BeamTest2024/CAEN_RO/Run410_list.txt",ios::in);
  ASCIIinput.open(Form("%s",InputName.Data()),ios::in);
  TString aline;
  TObjArray* tokens;
  map<int, vector<MyBoard> > mEvents;
  map<int, vector<MyBoard> >::iterator itevent;
  map<pair<int,int>, LRC> mappingConversion=LoadMapping(Form("%s",MappingName.Data()));
  TH1D* hcalibHG[8][64];
  TH1D* hcalibLG[8][64];
  for(int iboard=0; iboard<8; iboard++){
    for(int ichannel=0; ichannel<64; ichannel++){
      hcalibHG[iboard][ichannel]=new TH1D(Form("hcalibHG%d_%d",iboard,ichannel),Form("HG Board %d Channel %d spectrum; ADC [counts];",iboard,ichannel),4000,0,4000);
      hcalibLG[iboard][ichannel]=new TH1D(Form("hcalibLG%d_%d",iboard,ichannel),Form("LG Board %d Channel %d spectrum; ADC [counts];",iboard,ichannel),4000,0,4000);
    }
  }
  while(!ASCIIinput.eof()){
    //getline(ASCIIinput,aline);
    aline.ReadLine(ASCIIinput);
    if(!ASCIIinput.good()) break;
    if(aline[0]=='/') continue;
    tokens=aline.Tokenize(" \t");
    tokens->SetOwner(true);
    int Nfields=tokens->GetEntries();
    if(((TObjString*)tokens->At(0))->String()=="Brd") {
      tokens->Clear();
      delete tokens;
      continue;
    }
    if(Nfields!=7){
      cout<<"Unexpected number of fields"<<endl;
      cout<<"Expected 7, got: "<<Nfields<<", exit"<<endl;
      for(int j=0; j<Nfields;j++) cout<<j<<"  "<<((TObjString*)tokens->At(j))->String()<<endl;
      tokens->Clear();
      delete tokens;
      return -1;
    }
    int TriggerID=((TObjString*)tokens->At(5))->String().Atoi();
    MyBoard aBoard;
    int NHits=((TObjString*)tokens->At(6))->String().Atoi();
    aBoard.time=((TObjString*)tokens->At(4))->String().Atof();
    aBoard.boardID=((TObjString*)tokens->At(0))->String().Atoi();
    int channel=((TObjString*)tokens->At(1))->String().Atoi();
    aBoard.HG[channel]=((TObjString*)tokens->At(3))->String().Atoi();
    aBoard.LG[channel]=((TObjString*)tokens->At(2))->String().Atoi();
    for(int ich=1; ich<NHits;ich++){
          aline.ReadLine(ASCIIinput);
	  tokens=aline.Tokenize(" ");
	  tokens->SetOwner(true);
	  Nfields=tokens->GetEntries();
	  if(Nfields!=4){
	    cout<<"Expecting 4 fields but read "<<Nfields<<endl;
	    return -1;
	  }
	  channel=((TObjString*)tokens->At(1))->String().Atoi();
	  aBoard.HG[channel]=((TObjString*)tokens->At(3))->String().Atoi();
	  aBoard.LG[channel]=((TObjString*)tokens->At(2))->String().Atoi();
    }
    itevent=mEvents.find(TriggerID);
    if(itevent!=mEvents.end()){
      itevent->second.push_back(aBoard);
      if(itevent->second.size()==8){
	//Fill the tree the event is complete and erase from the map
	Trg=itevent->first;
	for(int iboard=0; iboard<8; iboard++){
	  int brd=itevent->second[iboard].boardID;
	  time[brd]=itevent->second[iboard].time;
	  for(int ich=0; ich<64; ich++){
	    HGcaen[brd][ich]=itevent->second[iboard].HG[ich];
	    LGcaen[brd][ich]=itevent->second[iboard].LG[ich];
	    hcalibHG[brd][ich]->Fill(HGcaen[brd][ich]);
	    hcalibLG[brd][ich]->Fill(LGcaen[brd][ich]);
	    HGlrc[mappingConversion[make_pair(brd,ich)].layer][mappingConversion[make_pair(brd,ich)].row][mappingConversion[make_pair(brd,ich)].col]=HGcaen[brd][ich];
	    LGlrc[mappingConversion[make_pair(brd,ich)].layer][mappingConversion[make_pair(brd,ich)].row][mappingConversion[make_pair(brd,ich)].col]=LGcaen[brd][ich];
	    if(ich%8>3){
	      xcaen[brd][ich]=-7.5+5*(3-(ich%8)%4);
	      ycaen[brd][ich]=-2.5;
	      //HGlrc[7-ich/8+8*brd][1][(ich%8)%4]=itevent->second[brd].HG[ich];
	      //LGlrc[7-ich/8+8*brd][1][(ich%8)%4]=itevent->second[brd].LG[ich];
	    }
	    else{
	      xcaen[brd][ich]=7.5-5*(3-(ich%8)%4);
	      ycaen[brd][ich]=2.5;
	      //HGlrc[7-ich/8+8*brd][0][3-(ich%8)%4]=itevent->second[brd].HG[ich];
	      //LGlrc[7-ich/8+8*brd][0][3-(ich%8)%4]=itevent->second[brd].LG[ich];
	    }
	    zcaen[brd][ich]=((63-ich)/8)*(2.)+16*brd;
	    xlrc[mappingConversion[make_pair(brd,ich)].layer][mappingConversion[make_pair(brd,ich)].row][mappingConversion[make_pair(brd,ich)].col]=xcaen[brd][ich];
	    ylrc[mappingConversion[make_pair(brd,ich)].layer][mappingConversion[make_pair(brd,ich)].row][mappingConversion[make_pair(brd,ich)].col]=ycaen[brd][ich];
	    zlrc[mappingConversion[make_pair(brd,ich)].layer][mappingConversion[make_pair(brd,ich)].row][mappingConversion[make_pair(brd,ich)].col]=zcaen[brd][ich];
	  }
	}
	tree->Fill();
	mEvents.erase(itevent);
      }
    }
    else{//This is a new triggerID, ie new event
      vector<MyBoard> vboard;
      vboard.push_back(aBoard);
      mEvents[TriggerID]=vboard;  
    }
  }
  TCanvas* c = new TCanvas("c","c",800,400);
  c->Divide(2,1);
  c->cd(1)->SetLogy();
  c->cd(2)->SetLogy();
  if(CalibFile.is_open()&&(ReadWrite==1)){
    c->Print("TestDistributions.pdf[");
    for(int iboard=0; iboard<8; iboard++){
      for(int ichannel=0; ichannel<64; ichannel++){
	ped->SetParameters(1000.,70.,15.);
	ped->SetParLimits(1,1,500);
	ped->SetParLimits(2,1,60);
	c->cd(1);
	hcalibHG[iboard][ichannel]->GetXaxis()->SetRangeUser(0,600);
	hcalibHG[iboard][ichannel]->Draw();
	hcalibHG[iboard][ichannel]->Fit(ped);
	CalibFile<<iboard<<"\t"<<ichannel<<"\t"<<ped->GetParameter(1)<<"\t"<<ped->GetParameter(2);
	c->cd(2);
	hcalibLG[iboard][ichannel]->GetXaxis()->SetRangeUser(0,600);
	hcalibLG[iboard][ichannel]->Draw();
	hcalibLG[iboard][ichannel]->Fit(ped);
	CalibFile<<ped->GetParameter(1)<<"\t"<<ped->GetParameter(2)<<endl;
	c->Print("TestDistributions.pdf");
      }
    }
    c->Print("TestDistributions.pdf]");
    CalibFile.close();
  }

  cout<<"Number of partial events: "<<mEvents.size()<<endl;
  tree->Write();
  output->Close();
  return 0;
}


map<pair<int, int>, LRC> LoadMapping(TString name){
  cout<<"Loading the map from "<<name.Data()<<endl;
  map<pair<int, int>, LRC> m;
  fstream config;
  config.open(name.Data(),ios::in);
  if(!config.is_open()){
    cout<<"could not open the mapping file input"<<endl;
  }
  int board, channel, layer, chInLayer, row, column;
  string assemblyLayer;
  pair<int,int> mykey;
  LRC myvalue;
  while(!config.eof()){
    config>>board>>channel>>layer>>assemblyLayer>>chInLayer>>row>>column;
    if(!config.good()){
      break;
    }
    mykey.first=board;
    mykey.second=channel;
    myvalue.layer=layer;
    myvalue.row=row;
    myvalue.col=column;
    m[mykey]=myvalue;
  }
  return m;
}
