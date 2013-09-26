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
   multiplicity(0.) { }

Vertice::Vertice(TString fName, Double_t fX, Double_t fY, Double_t fZ,
   Int_t fMult): Punto(fName,fX,fY,fZ), multiplicity(fMult) { }

Vertice::~Vertice() {
   if (gDebug) Printf("*** \"%s\" Vertex object was destroyed *** ", 
      this->GetPuntoName().Data());
}

//____________Multiplicity_Setter____________
void Vertice::SetVerticeMult(Int_t fMult) { multiplicity=fMult; }