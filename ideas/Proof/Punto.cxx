#include "TObject.h"
#include "TMath.h"
#include "Punto.h"



ClassImp(Punto)

//________________________________________________________________________
Punto::Punto():TObject(),
 fX(0.),
 fY(0.),
 fZ(0.),
 fMult(0) {
   // default constructor
 }


//___________________________________________________________________________
Punto::Punto(Double_t X, Double_t Y, Double_t Z, const Int_t Mult=0):TObject(),
 fX(X),
 fY(Y),
 fZ(Z),
 fMult(Mult) {
	//standard constructor 
}	

//Setters
void Punto::Set_Punto_X(const Double_t nX) { fX=nX; };
void Punto::Set_Punto_Y(const Double_t nY) { fY=nY; };
void Punto::Set_Punto_Z(const Double_t nZ) { fZ=nZ; };
void Punto::Set_Punto_M(const Int_t nMult) { fMult = nMult; };
  
//___________________________________________________________________________
Punto::~Punto()	 {
  // destructor
}

