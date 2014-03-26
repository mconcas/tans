#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Direzione.h"
#include <TMath.h>
#include <TObject.h>
#include <TString.h>
#endif

ClassImp(Direzione)

//____________{Con,Des}tructors____________
Direzione::Direzione(): TObject(),
   identity(0),
   theta(0.),
   phi(0.),
   dc1(0.),
   dc2(0.),
   dc3(0.),
   isrotated(kFALSE) { };

Direzione::Direzione( Double_t fTheta, Double_t fPhi, 
                      Int_t fIdentity): TObject(),
      identity(fIdentity),
      theta(fTheta),
      phi(fPhi),
      isrotated(kFALSE) {
         dc1=TMath::Sin(fTheta)*TMath::Cos(fPhi);
         dc2=TMath::Sin(fTheta)*TMath::Sin(fPhi);
         dc3=TMath::Cos(fTheta);
      };

Direzione::~Direzione() { 
   if(gDebug) Printf( "*** direzione object destroyed ***" ); 
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
   
   ///////////////////////////////////////////////////////////////////
   // Since D[ATan(x)] = [-Pi,+Pi] -> One have to discuss the cosines
   // directors values in order to find the true value of Phi.
   ///////////////////////////////////////////////////////////////////
   theta=TMath::ACos(dc3);
   if(dc1>0) {
      if(dc2>=0) phi=TMath::ATan(dc2/dc1);
      else phi=2*TMath::Pi()+TMath::ATan(dc2/dc1);
   }
   else {
      if(dc1!=0) { // dc1<0
         if(dc2>=0) phi=TMath::Pi()+TMath::ATan(dc2/dc1);
         else phi=TMath::Pi()+TMath::ATan(dc2/dc1);
      }
      else { // dc1=0
         if(dc2>=0) phi=0.5*TMath::Pi();
         else phi=1.5*TMath::Pi();
      }
   }   
}
void Direzione::UpdateDirCos() {
   dc1=TMath::Sin(theta)*TMath::Cos(phi);
   dc2=TMath::Sin(theta)*TMath::Sin(phi);
   dc3=TMath::Cos(theta);
}

void Direzione::FlipBit() {
   if(isrotated) isrotated = kFALSE;
   else isrotated = kTRUE;
}

void Direzione::Rotate(Double_t fTheta, Double_t fPhi) {

      // Define 2D Rotation Matrix
      Double_t mr[3][3]; 
      mr[0][0] = -TMath::Sin(phi); 
      mr[1][0] = TMath::Cos(phi); 
      mr[2][0] = 0.; 
      mr[0][1] = -TMath::Cos(phi)*TMath::Cos(theta);
      mr[1][1] = -TMath::Cos(theta)*TMath::Sin(phi);
      mr[2][1] = TMath::Sin(theta);
      mr[0][2] = TMath::Sin(theta)*TMath::Cos(phi);
      mr[1][2] = TMath::Sin(theta)*TMath::Sin(phi);
      mr[2][2] = TMath::Cos(theta);

      // Extract coords form object and put them into an array.
      Double_t cdp[3];
      cdp[0]=TMath::Sin(fTheta)*TMath::Cos(fPhi);
      cdp[1]=TMath::Sin(fTheta)*TMath::Sin(fPhi);
      cdp[2]=TMath::Cos(fTheta);
      
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
      this->FlipBit();

      // Update Theta and Phi
      this->UpdateAng();
}
