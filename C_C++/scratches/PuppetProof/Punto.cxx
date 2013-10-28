#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TMath.h>
#include "Punto.h"
#include <TObject.h>
#include <TString.h>
#include <TSystem.h>
#endif

ClassImp(Punto)

//___________{De,Con}structors_______________
Punto::Punto(): TObject(),
   fX(0.),
   fY(0.),    
   fZ(0.) { }

Punto::Punto(Double_t X, Double_t Y, Double_t Z): 
   TObject(),
   fX(X),
   fY(Y),
   fZ(Z) { }

Punto::~Punto() { }


