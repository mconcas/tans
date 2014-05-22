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
   fMultiplicity(0),
   fIdnumber(0),
   fNoiselevel(0),
   fIsAGoodGuy(kFALSE) { }

Vertice::Vertice(Double_t X,Double_t Y,Double_t Z,Int_t Mult,Int_t IDnumber,
   Double_t Noiselvl):
   Punto(X,Y,Z),
   fMultiplicity(Mult),
   fIdnumber(IDnumber),
   fNoiselevel(Noiselvl),
   fIsAGoodGuy(kFALSE) { }

Vertice::~Vertice() {if(gDebug) Printf("*** vertex object destroyed ***");}

//____________Multiplicity_Setter____________
void Vertice::SetVerticeMult(Int_t Mult) {fMultiplicity=Mult;}
void Vertice::SetVerticeID(Int_t IDno) {fIdnumber=IDno;}
void Vertice::SetVerticeNL(Double_t Noiselvl) {fNoiselevel=Noiselvl;}
void Vertice::SetVerticeGoodness(Bool_t Goodness) {fIsAGoodGuy=Goodness;}
