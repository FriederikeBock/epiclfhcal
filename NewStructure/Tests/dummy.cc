#include <iostream>
#include <Rtypes.h>
#include <TFile.h>
#include <TTree.h>
#include "./Setup.h"

Setup* Setup::instance=nullptr;

int main(int argc,char* argv[]){
  TFile* f = new TFile("Dummy.root","RECREATE");
  TTree* t= new TTree("t","t");
  Setup* s;
  s=Setup::getInstance();
  t->Branch("setup",&s);
  s->set(33.,2.);
  t->Fill();
  t->Write();
  std::cout<<s->get1()<<"\t"<<s->get2()<<std::endl;
  f->Close();
  return 0;
}
