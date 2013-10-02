#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Direzione.h"
#include <TMath.h>
#include <TString.h>
#include <TObject.h>
#endif

ClassImp(Direzione)

//____________{Con,Des}tructors____________
Direzione::Direzione(): TObject(), 
   directname(""),
   theta(0.),
   phi(0.),
   dc1(0.),
   dc2(0.),
   dc3(0.),
   isrotated(kFALSE) { };

Direzione::Direzione(TString fDirectname, Double_t fTheta,
   Double_t fPhi): TObject(),
      directname(fDirectname),
      theta(fTheta),
      phi(fPhi),
      isrotated(kFALSE) { };

Direzione::~Direzione() { 
   if (gDebug) Printf("*** \"%s\" Direzione object was destroyed ***",
   directname.Data()); 
}

//_________Setters___________________________
void Direzione::SetDirectTheta(const Double_t fTheta) {
   theta=fTheta;
}

void Direzione::SetDirectPhi(const Double_t fPhi) {
   phi=fPhi;
}

void Direzione::SetCosDir1( const Double_t fC1 ){
   dc1=fC1;  
}

void Direzione::SetCosDir2( const Double_t fC2 ){
   dc2=fC2;
}

void Direzione::SetCosDir3( const Double_t fC3 ){
   dc3=fC3;
}

void Direzione::SetAllCos( const Double_t fC1, const Double_t fC2, 
   const Double_t fC3 ) {
   dc1=fC1;
   dc2=fC2;
   dc3=fC3;
}

void Direzione::UpdateAng() {
   theta=TMath::ACos(dc3);
   if (theta!=0) phi=TMath::ASin(dc2/theta);
   else phi=0;
}

void Direzione::FlipBit() {
   if (isrotated) isrotated = kFALSE;
   else isrotated = kTRUE;
   if (gDebug) Printf("Direction %s has been rotated",
      directname.Data());
}

void Direzione::Rotate(Double_t fTheta, Double_t fPhi,
   TString fMoreinfo) {

      // Info will be put into the name.
      directname+=fMoreinfo;
      
      // Define 2D Rotation Matrix
      Double_t mr[3][3]; 
      mr[0][0] = -TMath::Sin(fPhi); 
      mr[1][0] = TMath::Cos(fPhi); 
      mr[2][0] = 0.; 
      mr[0][1] = -TMath::Cos(fPhi)*TMath::Sin(fTheta);
      mr[1][1] = -TMath::Cos(fTheta)*TMath::Sin(fPhi);
      mr[2][1] = TMath::Sin(fTheta);
      mr[0][2] = TMath::Sin(fTheta)*TMath::Cos(fPhi);
      mr[1][2] = TMath::Sin(fTheta)*TMath::Sin(fPhi);
      mr[2][2] = TMath::Cos(fTheta);

      // Extract coords form object and put them into an array.
      Double_t cdp[3];
      cdp[0]=dc1;
      cdp[1]=dc2;
      cdp[2]=dc3;
      
      // Define the result array.
      Double_t fCd[3];
      
      // Compute the product
      for(Int_t i=0; i<3; ++i) {
         fCd[i]=0;
         for (Int_t j=0; j<3; ++j) fCd[i]+=mr[i][j]*cdp[j];
      }
      dc1=fCd[0];
      dc2=fCd[1];
      dc3=fCd[2];
}
