#include <iostream>
#include <Rtypes.h>
#include <TFile.h>
#include <TTree.h>
#include "./Setup.h"

Setup* Setup::instance=nullptr;

int main(int argc,char* argv[]){
  TFile* f = new TFile("Dummy.root","READ");
  //TFile* f = new TFile("Dummy.root","UPDATE");
  TTree* t= (TTree*)f->Get("t");
  Setup* s;
  s=Setup::getInstance();
  t->SetBranchAddress("setup",&s);
  t->GetEntry(0);
  //s->set(1.,2.);
  //t->Fill();
  //t->Write();
  //s->setPed(42.);
  //t->Refresh();
  std::cout<<s->get1()<<"\t"<<s->get2()<<"\t"<<s->getP()<<std::endl;
  //t->Write("",TObject::kOverwrite);
  f->Close();
  return 0;
}
