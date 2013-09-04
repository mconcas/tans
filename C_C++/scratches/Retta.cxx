#if !defined (__CINT__) || defined (__MAKECINT__)
#include "TMath.h"
#include "Riostream.h"
#include "Punto.h"
#include "Retta.h"
#include "TObject.h"
#include "TString.h"
#endif

ClassImp(Retta)

Retta::Retta(Double_t gm1, Double_t gm2, Double_t gm3): TObject(),
  Rname("Default"),
  m1(gm1),
  m2(gm2),       //Standard Constructor: origin is setted (0,0,0)
  m3(gm3),
  origin("Default",0.,0.,0.)
  {
    rTheta = TMath::ACos(gm3);
    if(gm3==0) rPhi = 2*(TMath::ASin(gm2/TMath::Sin(rTheta)) + TMath::Pi()/2);
    else rPhi = 2*(TMath::ACos(gm1/TMath::Sin(rTheta)) + TMath::Pi()/2);
  }
Retta::Retta(TString gRname, const Punto &gorigin, Double_t gm1, Double_t gm2, Double_t gm3):   
  TObject(),
  Rname(gRname),
  m1(gm1),
  m2(gm2),       //m1,m2,m3 sono i coseni direttori, quindi compresi tra -1 e 1
  m3(gm3),
  origin(gorigin){
    rTheta = TMath::ACos(gm3);
    if(gm3==0) rPhi = 2*(TMath::ASin(gm2/TMath::Sin(rTheta)) + TMath::Pi()/2);
    else rPhi = 2*(TMath::ACos(gm1/TMath::Sin(rTheta)) + TMath::Pi()/2);
  }
Retta::~Retta(){
  cout<<"*** \" "<<Rname<<"\" OBJECT DESTROYED !!!  ***";
}

//____________Setters_________________

void Retta::Set_Retta_m1(const Double_t nm1){
  m1 = nm1;
  rTheta = TMath::ACos(m3);
    if(m3==0) rPhi = 2*(TMath::ASin(m2/TMath::Sin(rTheta)) + TMath::Pi()/2);
    else rPhi = 2*(TMath::ACos(m1/TMath::Sin(rTheta)) + TMath::Pi()/2);
}
void Retta::Set_Retta_m2(const Double_t nm2){
  m2 = nm2;
  rTheta = TMath::ACos(m3);
    if(m3==0) rPhi = 2*(TMath::ASin(m2/TMath::Sin(rTheta)) + TMath::Pi()/2);
    else rPhi = 2*(TMath::ACos(m1/TMath::Sin(rTheta)) + TMath::Pi()/2);
}
void Retta::Set_Retta_m3(const Double_t nm3){
  m3 = nm3;
  rTheta = TMath::ACos(m3);
    if(m3==0) rPhi = TMath::ASin(m2/TMath::Sin(rTheta));
    else rPhi = TMath::ACos(m1/TMath::Sin(rTheta));
}

void Retta::Set_Retta_Origin(const Punto &norigin){
  origin = norigin;
}

//____________Getters________________

Punto* Retta::Get_Retta_Evaluated(TString pName, Double_t x){
  Punto *pPoint = new Punto();
  pPoint->Set_Punto_Name(pName);
  pPoint->Set_Punto_X(origin.Get_Punto_X()+m1*x);
  pPoint->Set_Punto_Y(origin.Get_Punto_Y()+m2*x);
  pPoint->Set_Punto_Z(origin.Get_Punto_Z()+m3*x);
  return pPoint;
}

Punto* Retta::Intersecate_Cylinder(Double_t radius){
  Double_t thetaa = this->Get_Retta_rTheta();
  Double_t phii = this->Get_Retta_rPhi();
  Punto* origine = this->Get_Retta_Origin();
  Double_t Xo = origine->Get_Punto_X();
  Double_t Yo = origine->Get_Punto_Y();
  Double_t T = (-(Xo*TMath::Cos(phii)+Yo*TMath::Sin(phii))+/*CAMBIOSEGNOOOOO*/ \
  TMath::Sqrt(radius*radius-(Xo*TMath::Sin(phii)-\
  Yo*TMath::Cos(phii))*(Xo*TMath::Sin(phii)-Yo*TMath::Cos(phii))))\
  /TMath::Sin(thetaa);
  Punto* RePunto = this->Get_Retta_Evaluated("Hit",T);
  return RePunto;
}

/*Punto Retta::Get_Retta_Evaluated(TString pName, Double_t x){
  Punto pPoint = Punto();
  pPoint.Set_Punto_Name(pName);
  pPoint.Set_Punto_X(origin.Get_Punto_X()+m1*x);
  pPoint.Set_Punto_Y(origin.Get_Punto_Y()+m2*x);
  pPoint.Set_Punto_Z(origin.Get_Punto_Z()+m3*x);
  return pPoint;
  }*/


