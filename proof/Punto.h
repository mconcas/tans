#ifndef PUNTO_H
#define PUNTO_H

#include "TObject.h"

class Punto : public TObject
{

public:

  Punto();
  Punto(Double_t X, Double_t Y, Double_t Z, Int_t Mult);
 
  virtual ~Punto();
  
  // Setters
  void Set_Punto_X(const Double_t nX);
  void Set_Punto_Y(const Double_t nY);
  void Set_Punto_Z(const Double_t nZ);
  void Set_Punto_M(const Int_t nMult);
  // Getters
  Double_t GetX() const {return fX;} 
  Double_t GetY() const {return fY;}
  Double_t GetZ() const {return fZ;}
  Int_t    GetMult() const {return fMult;}

private:

  Double_t fX;
  Double_t fY;
  Double_t fZ;
  Int_t fMult;

ClassDef(Punto,1)
};


#endif 


