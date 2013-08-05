#ifndef PUNTO_H
#define PUNTO_H
#include "TObject.h"
#include "TString.h"

class Punto : public TObject {
 public:
  Punto();
  Punto(TString gName, Double_t gX, Double_t gY, Double_t gZ);
  virtual ~Punto();
    
    
//____________Setters______________________  
  void Set_Punto_X(const Double_t nX);
  void Set_Punto_Y(const Double_t nY);
  void Set_Punto_Z(const Double_t nZ);
  void Set_Punto_Theta(const Double_t nTheta);
  void Set_Punto_Phi(const Double_t nPhi);
  void Set_Punto_CRadius(const Double_t nCRadius);
  void Set_Punto_Name(const TString nName);
  
    
//____________Inline_Getters_______________
  Double_t Get_Punto_X() const {return X;}
  Double_t Get_Punto_Y() const {return Y;}
  Double_t Get_Punto_Z() const {return Z;}
  Double_t Get_Punto_Theta() const {return Theta;}
  Double_t Get_Punto_Phi() const {return Phi;}
  Double_t Get_Punto_SRadius() const {return SRadius;}
  Double_t Get_Punto_CRadius() const {return CRadius;}
  TString Get_Punto_Name() const {return Name;}

 protected:
  TString Name;
  Double_t X;
  Double_t Y;
  Double_t Z;
  Double_t Theta;
  Double_t Phi;
  Double_t CRadius;
  Double_t SRadius;
  ClassDef(Punto,1)
    
};


#endif
