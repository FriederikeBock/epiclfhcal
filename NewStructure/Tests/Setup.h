#ifndef SETUP_H
#define SETUP_H

#include <cstddef>
#include <TObject.h>

class Setup{
 public:
  Setup(const Setup &)=delete;
  Setup& operator=(const Setup&) = delete;
  
  static Setup* getInstance(){
    if(instance==NULL) instance=new Setup();
    return instance;
  }

  void set(double q, double w){
    a=q;
    b=w;
  }
  void setPed(double p){
    c=p;
  }
  double get1();
  double get2();
  double getP();
  
 private:
  static Setup* instance;
  double a;
  double b;
  double c;
  Setup(){}
  ~Setup() {}
  ClassDef(Setup,1)
};




#endif
