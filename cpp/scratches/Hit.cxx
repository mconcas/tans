#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Punto.h"
#include "Vertice.h"
#include "Hit.h"
#include "Direzione.h"
#include <TMath.h>
#include <TMaterial.h>
#include <TRandom3.h>
#include <TSystem.h>
#endif

ClassImp (Hit)

Hit::Hit(): Punto(),
   fLayernum(0),
   fIdnumber(0),
   fRealhit(kFALSE) { }

Hit::Hit( Double_t X, Double_t Y, Double_t Z, Int_t Layerno,
   Int_t Idnumber ):
      Punto( X,Y,Z ),
      fLayernum( Layerno ),
      fIdnumber( Idnumber ),
      fRealhit( kFALSE ) { }

Hit::~Hit()
{ 
   if (gDebug) Printf( "*** hit object destroyed ***" ); 
}

void Hit::NowRealHit() { fRealhit=kTRUE; }
void Hit::SetHitID( Int_t Idnumber ) { fIdnumber=Idnumber; }
void Hit::SetHitLayno( Int_t Layernum ) { fLayernum=Layernum; }

Double_t Hit::ComputeT( Double_t Theta, Double_t Phi, Double_t XO,
   Double_t YO, Double_t Radius ) 
{
      const Double_t sintheta=TMath::Sin(Theta);
      const Double_t sinphi=TMath::Sin(Phi);
      const Double_t cosphi=TMath::Cos(Phi);
      const Double_t t=(-(XO*cosphi+YO*sinphi)+TMath::Sqrt((XO*cosphi
         +YO*sinphi)*(XO*cosphi+YO*sinphi)-XO*XO-YO*YO+Radius*Radius))/sintheta;

      return t;
}

Hit *Hit::HitOnCylFromVertex( Vertice& Origin, Direzione& Direct,
   Double_t Radius, Int_t Ide, Int_t Layerno ) 
{

      // Extract theta and phi from "Direzione".
      const Double_t theta=Direct.GetDirectTheta();
      const Double_t phi=Direct.GetDirectPhi();

      ////////////////////////////////////////////////////////////////
      // Manage the ϑ=0 exception
      if( theta==0 || theta==TMath::Pi()) {
         Hit *OnCyl=new Hit();
         if (gDebug) Printf("ϑ=0 exception -> no scattering.");
         return OnCyl;
      } else {

      //////////////////////////////////////////////////////////////////////////
      // Get origin coordinates: xO, yO, zO from "Vertice"
      const Double_t xO=Origin.GetPuntoX();
      const Double_t yO=Origin.GetPuntoY();
      const Double_t zO=Origin.GetPuntoZ();

      //////////////////////////////////////////////////////////////////////////
      // Compute the "t" value.
      const Double_t t=ComputeT(theta,phi,xO,yO,Radius);

      // Build item in return.
      Hit *OnCyl=new Hit(xO+t*Direct.GetDirCos1(),yO+t*Direct.GetDirCos2(),
         zO+t*Direct.GetDirCos3(),Layerno,Ide);

      return OnCyl;
   }
}

Hit *Hit::GetHitOnCyl(Direzione& Direct,Double_t Radius,TMaterial& Material,
   Double_t Width,Bool_t Multiscat,Int_t Layno,Double_t P,Int_t Z,Double_t Beta) 
{
   if(Multiscat) {

      //////////////////////////////////////////////////////////////////////////
      // θo represents the rms of a gaussian distribution centred
      // in 0.
      // Formula used for the MScattering θo evaluation:
      // Dimensionalities:
      // [x]=cm; [X_0]=cm; [c]=3x10**8; [b]=#; [fP]=MeV/c
      // θo = (13.6 MeV/β*c*p)*Z*(√(x/X_0))[1+0.038*Ln(x/X_0)]
      //////////////////////////////////////////////////////////////////////////
      // Get the Radiation Length: X_0
      const Double_t X_0 = Material.GetRadLength();

      //////////////////////////////////////////////////////////////////////////
      // WARNING!
      // Since fP should be passed with a dimensionality of [fP]= MeV/c,
      // it's obvious that "c" wouldn't appear in this operation.
      const Double_t ThetaZero=(13.6/(Beta*P))*Z*TMath::Sqrt(Width/X_0)*(1+
         0.038*TMath::Log(Width/X_0));

      //////////////////////////////////////////////////////////////////////////
      // Generate θ form Gaussian distribution with mean 0 and
      // rms θo. Phi is uniformely distributed between 0<=φ<2Pi
      const Double_t thetalocal=gRandom->Gaus(0.,ThetaZero);
      const Double_t philocal=gRandom->Rndm()*2*TMath::Pi();

      //////////////////////////////////////////////////////////////////////////
      // Rotate direction in order to refer it to the Laboratory
      // Reference System.
      Direct.Direzione::Rotate(thetalocal,philocal);

   }
   
   // Extract theta and phi from "Direzione".
   const Double_t theta=Direct.GetDirectTheta();
   const Double_t phi=Direct.GetDirectPhi();

   /////////////////////////////////////////////////////////////////////////////
   // Manage the ϑ=0 exception
   if(theta==0||TMath::Abs(theta)==TMath::Pi()) {
      Hit *OnCyl=new Hit();
      if (gDebug) Printf("ϑ=0 exception -> no scattering.");

      return OnCyl;
   } else {

      //////////////////////////////////////////////////////////////////////////
      // Compute "t" value.
      const Double_t t=ComputeT(theta,phi,fX,fY,Radius);
      
      // Returned item.
      Hit *OnCyl=new Hit(fX+t*Direct.GetDirCos1(),fY+t*Direct.GetDirCos2(),
                         fZ+t*Direct.GetDirCos3(),Layno,fIdnumber);

         return OnCyl;
   }
}

////////////////////////////////////////////////////////////////////////////////
// One can find the theta RMS dividing RPhi by the detector radius.
void Hit::GausSmearing(Double_t DRadius,Double_t RMSZeta,Double_t RMSRTheta) 
{
      this->SetPuntoZ(this->GetPuntoZ()+gRandom->Gaus(0,RMSZeta));
      this->SetPuntoPhi(this->GetPuntoPhi()+gRandom->Gaus(0,RMSRTheta/DRadius));
}


Hit* Hit::NoiseOnCyl(Double_t CRadius,Double_t ZetaMin,Double_t ZetaMax) 
{
   Hit* Noise=new Hit();
   Noise->SetPuntoCRadius(CRadius);
   Noise->SetPuntoZ(ZetaMin+(ZetaMax-ZetaMin)*gRandom->Rndm());
   Noise->SetPuntoPhi(2*TMath::Pi()*gRandom->Rndm());
   Noise->SetHitID(-1);
   return Noise;
}
