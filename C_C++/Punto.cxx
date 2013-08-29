//#if !defined (__CINT__) || defined (__MAKECINT__)
#include "TMath.h"
#include "Riostream.h"
#include "Punto.h"
#include "TObject.h"
#include "TString.h"
//#endif

ClassImp(Punto)

//___________Standard_Constructor_______________
Punto::Punto(): TObject(),
  X(0.),
  Y(0.),    
  Z(0.),
  Theta(0.),
  Phi(0.),
  CRadius(0.),
  SRadius(0.){}





//___________Constructor________________________
Punto::Punto(TString GName, Double_t GX, Double_t GY, Double_t GZ): TObject(),
  Name(GName),
  X(GX),
  Y(GY),
  Z(GZ){
    //____Automatically_generate_cylindrical/spherical_datamembers
  SRadius = TMath::Sqrt(X*X+Y*Y+Z*Z);
  CRadius = TMath::Sqrt(X*X+Y*Y);
  if(X==0.){
    if(X==0. && Y==0. && Z==0.){
      //cout<<"[Debug]: It is the origin. \n";
    Phi=0;
    Theta=0;    //Set Theta and Phi at zero, by default if point is the axes' origin.  
    }
    else Phi = TMath::Pi()*0.5;  //_____lays on YZ plan
  }
  else{
    if(Y!=0){ //.
      if((X>0)&&(Y>0)) Phi= TMath::ATan(Y/X);
      if((X<0)&&(Y>0)) Phi= -TMath::ATan(Y/X)+0.5*TMath::Pi();
      if((X<0)&&(Y<0)) Phi= TMath::ATan(Y/X)+TMath::Pi();
      if((X>0)&&(Y<0)) Phi= -TMath::ATan(Y/X)+(1.5)*TMath::Pi();
    }
    Theta = TMath::ACos(Z/SRadius);
  }
}
  

Punto::~Punto(){
  //Standard Destructor
  cout<<"!!! \""<<Name<<"\" PUNTO OBJECT DESTROYED !!!\n";
}
//_________Setters________________________________
void Punto::Set_Punto_X(const Double_t nX){
  X=nX;
    if(X==0.){     
    if(X==0. && Y==0. && Z==0.){
      cout << "***     È l'origine!    ***" << endl;
    Phi=0;
    Theta=0;
    }
    else Phi = TMath::Pi()*0.5;
  }
  else{ 
   if(Y!=0){ //Mi assicuro di non essere nei casi precedenti.
      if((X>0)&&(Y>0)) Phi= TMath::ATan(Y/X);
      if((X<0)&&(Y>0)) Phi= -TMath::ATan(Y/X)+0.5*TMath::Pi();
      if((X<0)&&(Y<0)) Phi= TMath::ATan(Y/X)+TMath::Pi();
      if((X>0)&&(Y<0)) Phi= -TMath::ATan(Y/X)+(1.5)*TMath::Pi();
    }  
    Theta = TMath::ACos(Z/SRadius);
  }
  SRadius = TMath::Sqrt(X*X+Y*Y+Z*Z);
  CRadius = TMath::Sqrt(X*X+Y*Y);
  
}
void Punto::Set_Punto_Y(const Double_t nY){
  Y=nY;
    if(X==0.){     
    if(X==0. && Y==0. && Z==0.){
      cout << "***     È l'origine!    ***" << endl;
    Phi=0;
    Theta=0;
    }
    else Phi = TMath::Pi()*0.5;
  }
  else{ 
    if(Y!=0){ //Mi assicuro di non essere nei casi precedenti.
      if((X>0)&&(Y>0)) Phi= TMath::ATan(Y/X);
      if((X<0)&&(Y>0)) Phi= -TMath::ATan(Y/X)+0.5*TMath::Pi();
      if((X<0)&&(Y<0)) Phi= TMath::ATan(Y/X)+TMath::Pi();
      if((X>0)&&(Y<0)) Phi= -TMath::ATan(Y/X)+(1.5)*TMath::Pi();
    }  
    Theta = TMath::ACos(Z/SRadius);
  }
  SRadius = TMath::Sqrt(X*X+Y*Y+Z*Z);
  CRadius = TMath::Sqrt(X*X+Y*Y);
  
}
void Punto::Set_Punto_Z(const Double_t nZ){
  Z=nZ;
    if(X==0.){     
    if(X==0. && Y==0. && Z==0.){
      cout << "***     È l'origine!    ***" << endl;
    Phi=0;
    Theta=0;
    }
    else Phi = TMath::Pi()*0.5;
  }
  else{ 
    if(Y!=0){ //Mi assicuro di non essere nei casi precedenti.
      if((X>0)&&(Y>0)) Phi= TMath::ATan(Y/X);
      if((X<0)&&(Y>0)) Phi= -TMath::ATan(Y/X)+0.5*TMath::Pi();
      if((X<0)&&(Y<0)) Phi= TMath::ATan(Y/X)+TMath::Pi();
      if((X>0)&&(Y<0)) Phi= -TMath::ATan(Y/X)+(1.5)*TMath::Pi();
    }  
    Theta = TMath::ACos(Z/SRadius);
  }
  SRadius = TMath::Sqrt(X*X+Y*Y+Z*Z);
  CRadius = TMath::Sqrt(X*X+Y*Y);
}
void Punto::Set_Punto_Theta(const Double_t nTheta){
  Theta=nTheta;
  if(Theta==0.) cout << "***     Angolo theta non d'interesse (0)!!!!    ***" << endl;
  else{
  X = CRadius*TMath::Cos(Phi);
  Y = CRadius*TMath::Sin(Phi);
  Z = CRadius*(1/(TMath::Tan(Theta)));
  }
}
void Punto::Set_Punto_Phi(const Double_t nPhi){
  Phi=nPhi;
  if(Theta==0.) cout << "***     Angolo theta non d'interesse (0)!!!!    ***" << endl;
  else{
  X = CRadius*TMath::Cos(Phi);
  Y = CRadius*TMath::Sin(Phi);
  Z = CRadius*(1/(TMath::Tan(Theta)));
  }
}
void Punto::Set_Punto_CRadius(const Double_t nCRadius){
  CRadius=nCRadius;
}
void Punto::Set_Punto_Name(const TString nName){
  Name=nName;
}
