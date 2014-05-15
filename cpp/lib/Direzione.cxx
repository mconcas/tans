#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Direzione.h"
#include <TMath.h>
#include <TObject.h>
#include <TString.h>
#endif

ClassImp(Direzione)

//____________{Con,Des}tructors____________
Direzione::Direzione():
   TObject(),
   fIdnumber(0),
   fTheta(0.),
   fPhi(0.),
   fDc1(0.),
   fDc2(0.),
   fDc3(0.),
   fIsrotated(kFALSE) { };

Direzione::Direzione( Double_t Theta, Double_t Phi,
   Int_t Idnumber):
      TObject(),
      fIdnumber(Idnumber),
      fTheta(Theta),
      fPhi(Phi),
      fIsrotated(kFALSE)
{
         fDc1=TMath::Sin(Theta)*TMath::Cos(Phi);
         fDc2=TMath::Sin(Theta)*TMath::Sin(Phi);
         fDc3=TMath::Cos(Theta);
};

Direzione::~Direzione() {
   if(gDebug) Printf( "*** direzione object destroyed ***" );
}

//_________Setters___________________________
void Direzione::SetDirectTheta(const Double_t Theta)
{
   fTheta=Theta;
}

void Direzione::SetDirectPhi(const Double_t Phi)
{
   fPhi=Phi;
}

void Direzione::SetCosDir1(const Double_t C1)
{
   fDc1=C1;
}

void Direzione::SetCosDir2(const Double_t C2)
{
   fDc2=C2;
}

void Direzione::SetCosDir3(const Double_t C3)
{
   fDc3=C3;
}

void Direzione::SetAllCos(const Double_t C1, const Double_t C2,
   const Double_t C3 )
{
   fDc1=C1;
   fDc2=C2;
   fDc3=C3;
   UpdateAng();
}

void Direzione::SetAllAngles(const Double_t Theta, const Double_t Phi)
{
   fTheta=Theta;
   fPhi=Phi;
   UpdateDirCos();
}

void Direzione::SetDirectID(const Double_t id)
{
   fIdnumber=id;
}

void Direzione::UpdateAng()
{

   ///////////////////////////////////////////////////////////////////
   // Since D[ATan(x)] = [-Pi,+Pi] -> One have to discuss the cosines
   // directors values in order to find the true value of Phi.
   ///////////////////////////////////////////////////////////////////
   fTheta=TMath::ACos(fDc3);
   if(fDc1>0) {
      if(fDc2>=0) fPhi=TMath::ATan(fDc2/fDc1);
      else fPhi=2*TMath::Pi()+TMath::ATan(fDc2/fDc1);
   } else {
      if(fDc1!=0) { // dc1<0
         if(fDc2>=0) fPhi=TMath::Pi()+TMath::ATan(fDc2/fDc1);
         else fPhi=TMath::Pi()+TMath::ATan(fDc2/fDc1);
      } else { // dc1=0
         if(fDc2>=0) fPhi=0.5*TMath::Pi();
         else fPhi=1.5*TMath::Pi();
      }
   }
}

void Direzione::UpdateDirCos()
{
   fDc1=TMath::Sin(fTheta)*TMath::Cos(fPhi);
   fDc2=TMath::Sin(fTheta)*TMath::Sin(fPhi);
   fDc3=TMath::Cos(fTheta);
}

void Direzione::FlipBit()
{
   if(fIsrotated) fIsrotated = kFALSE;
   else fIsrotated = kTRUE;
}

void Direzione::Rotate(Double_t Theta, Double_t Phi)
{

      // Define 2D Rotation Matrix
      Double_t mr[3][3];
      mr[0][0] = -TMath::Sin(fPhi);
      mr[1][0] = TMath::Cos(fPhi);
      mr[2][0] = 0.;
      mr[0][1] = -TMath::Cos(fPhi)*TMath::Cos(fTheta);
      mr[1][1] = -TMath::Cos(fTheta)*TMath::Sin(fPhi);
      mr[2][1] = TMath::Sin(fTheta);
      mr[0][2] = TMath::Sin(fTheta)*TMath::Cos(fPhi);
      mr[1][2] = TMath::Sin(fTheta)*TMath::Sin(fPhi);
      mr[2][2] = TMath::Cos(fTheta);

      // Extract coords form object and put them into an array.
      Double_t cdp[3];
      cdp[0]=TMath::Sin(Theta)*TMath::Cos(Phi);
      cdp[1]=TMath::Sin(Theta)*TMath::Sin(Phi);
      cdp[2]=TMath::Cos(Theta);

      // Define the result array.
      Double_t Cd[3];

      // Compute the product
      for(Int_t i=0; i<3; ++i) {
         Cd[i]=0;
         for (Int_t j=0; j<3; ++j) Cd[i]+=mr[i][j]*cdp[j];
      }
      fDc1=Cd[0];
      fDc2=Cd[1];
      fDc3=Cd[2];
      this->FlipBit();

      // Update Theta and Phi
      this->UpdateAng();
}
