
#ifndef RETTA_H
#define RETTA_H
#include "TObject.h"
#include "Punto.h"
#include "TString.h"

class Retta : public TObject {
 public:
  Retta(Double_t gm1, Double_t gm2, Double_t gm3);
  Retta(TString gRname, const Punto &gorigin, Double_t gm1, Double_t gm2, Double_t gm3);
  virtual ~Retta();

  //_____________Setters__________________

  void Set_Retta_m1(const Double_t nm1);
  void Set_Retta_m2(const Double_t nm2);
  void Set_Retta_m3(const Double_t nm3);
  void Set_Retta_Origin(const Punto &norigin);

  //_____________Inline_Getters___________

  Double_t Get_Retta_m1() const {return m1;}
  Double_t Get_Retta_m2() const {return m2;}
  Double_t Get_Retta_m3() const {return m3;} 
  Double_t Get_Retta_rTheta() const {return rTheta;}
  Double_t Get_Retta_rPhi() const {return rPhi;}
  TString Get_Retta_Name() const {return Rname;} 
  Punto* Get_Retta_Origin()  {return &origin;}
  //Punto Get_Retta_Origin() const {return origin;}


  //_____________Getters__________________
  Punto* Get_Retta_Evaluated(TString pName, Double_t x);   
  //Punto Get_Retta_Evaluated(TString pName, Double_t x);
  Punto* Intersecate_Cylinder(Double_t radius);



 protected:
  TString Rname;
  Double_t m1;
  Double_t m2;
  Double_t m3;
  Double_t rTheta;
  Double_t rPhi;
  Punto origin;

  ClassDef(Retta,1)
};


#endif
