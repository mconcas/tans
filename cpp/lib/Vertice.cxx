#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Vertice.h"
#include "Punto.h"
#include "TMath.h"
#include "TString.h"
#include "TSystem.h"
#endif

ClassImp(Vertice)


//___________{De,Con}structors_______________
Vertice::Vertice(): Punto(),
   multiplicity(0),
   idnumber(0),
   noiselevel(0) { }

Vertice::Vertice(Double_t fX, Double_t fY, Double_t fZ, Int_t fMult,
   Int_t fIDnumber, Double_t fNoiselvl): 
   Punto(fX,fY,fZ), 
   multiplicity(fMult),
   idnumber(fIDnumber),
   noiselevel(fNoiselvl) { }

Vertice::~Vertice() {
   if (gDebug) Printf("*** vertex object destroyed ***");
}

//____________Multiplicity_Setter____________
void Vertice::SetVerticeMult(Int_t fMult) { multiplicity=fMult; }
void Vertice::SetVerticeID(Int_t fIDno) { idnumber=fIDno; }
void Vertice::SetVerticeNL(Double_t fNoiselvl) { 
   noiselevel=fNoiselvl; };
