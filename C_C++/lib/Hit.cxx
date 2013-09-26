#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Punto.h"
#include "Vertice.h"
#include "Hit.h"
#include "TSystem.h"
#include "TMath.h"
#include <TMaterial.h>
#include <TRandom3.h>
#endif

ClassImp (Hit)

Hit::Hit(): Punto(),
   layernum(0) {
      detectorshape="";
      vtxname="";
}

Hit::Hit(TString fId, TString fDetShape, Int_t fLayer,
   TString fVtxName, Double_t fX, Double_t fY, 
   Double_t fZ): Punto(fId, fX, fY, fZ),
   layernum(fLayer) { 
      detectorshape=fDetShape;
      vtxname=fVtxName;
}

Hit *Hit::HitOnCylFromVertex(Vertice &fOrigin, Double_t fTheta,
   Double_t fPhi, Double_t fRadius, Int_t fLayno) {
   /*Please note: the reference system always referred to the Z axis.
   
   Manage the ϑ=0 exception*/
   if(fTheta==0 || fTheta==TMath::Pi()) { 
      Hit *OnCyl=new Hit();
      if (gDebug) Printf("ϑ=0 exception, it will be no scattering.");
      OnCyl->SetPuntoName("ϑ=0 exception, it will be no scattering.");
      return OnCyl;
   }   
   else {
      /*Get origin coordinates: xO and yO */
      const TString fShape="Cylindrical";
      const Double_t xO=fOrigin.GetPuntoX();
      const Double_t yO=fOrigin.GetPuntoY();
      const Double_t zO=fOrigin.GetPuntoZ();
      const Double_t sintheta=TMath::Sin(fTheta);
      const Double_t sinphi=TMath::Sin(fPhi);
      const Double_t costheta=TMath::Cos(fTheta);
      const Double_t cosphi=TMath::Cos(fPhi);
      const TString vertname=fOrigin.GetPuntoName();
      /*Compute the "t" value from vertex coordinates and angles.*/
      const Double_t t=(-(xO*cosphi+yO*sinphi)+TMath::Sqrt((xO*cosphi
         +yO*sinphi)*(xO*cosphi+yO*sinphi)-xO*xO-yO*yO
      +fRadius*fRadius))/TMath::Abs(sintheta); 
         /*Abs is due the two possible results.*/
      Double_t coord[3];
      coord[0]=xO+t*sintheta*cosphi; 
      coord[1]=yO+t*sintheta*sinphi;
      coord[2]=zO+t*costheta;
      TString hitname;
      hitname.Form("From_%s_on_layno_%d",vertname.Data(),fLayno);
      /*Build item in return...*/
      Hit *OnCyl=new Hit(hitname, fShape, fLayno, vertname, coord[0], 
         coord[1], coord[2]);
      return OnCyl;
   }  
}

Hit *Hit::HitOnCylFromHit(Hit &fOrigin, Double_t fTheta,
   Double_t fPhi, Double_t fRadius, Int_t fLayno) {
   // Please note: the reference system always referred to the Z axis.

   // Manage the ϑ=0 exception
   if(fTheta==0 || fTheta==TMath::Pi()) { 
      Hit *OnCyl=new Hit();
      if (gDebug) Printf("ϑ=0 exception, it will be no scattering.");
      OnCyl->SetPuntoName("ϑ=0 exception, it will be no scattering.");
      return OnCyl;
   }   
   else {
      // Get origin coordinates: xO and yO 
      const TString fShape="Cylindrical";
      const Double_t xO=fOrigin.GetPuntoX();
      const Double_t yO=fOrigin.GetPuntoY();
      const Double_t zO=fOrigin.GetPuntoZ();
      const Double_t sintheta=TMath::Sin(fTheta);
      const Double_t sinphi=TMath::Sin(fPhi);
      const Double_t costheta=TMath::Cos(fTheta);
      const Double_t cosphi=TMath::Cos(fPhi);
      const TString origname=fOrigin.GetPuntoName();
      // Compute the "t" value from vertex coordinates and angles.
      const Double_t t=(-(xO*cosphi+yO*sinphi)+TMath::Sqrt((xO*cosphi
         +yO*sinphi)*(xO*cosphi+yO*sinphi)-xO*xO-yO*yO
      +fRadius*fRadius))/TMath::Abs(sintheta); 
         // Abs is due the two possible results.
      Double_t coord[3];
      coord[0]=xO+t*sintheta*cosphi; 
      coord[1]=yO+t*sintheta*sinphi;
      coord[2]=zO+t*costheta;
      TString hitname;
      hitname.Form("From_%s_on_layno_%d", origname.Data(), fLayno);
      Hit *OnCyl=new Hit(hitname, fShape, fLayno, origname, coord[0], 
         coord[1], coord[2]);
      return OnCyl;
   }
}

Hit *Hit::HitOnCylWithMScat(Hit &fOrigin, Double_t fRadius, 
   Int_t fLayno, TMaterial &fMaterial, Double_t fWidth, Double_t fP,
   Int_t fZ, Double_t fBeta) {

   ///////////////////////////////////////////////////////////////////
   // θo represents the rms of a gaussian distribution centred in 0.
   // Formula used for the MScattering θo evaluation:
   // θo = (13.6 MeV/β*c*p)*Z*(√(x/X_0))[1+0.038*Ln(x/X_0)]
   // Get the Radiation Length: X_0
   ///////////////////////////////////////////////////////////////////
   Double_t X_0 = fMaterial.GetRadLength();

   // WARNING!
   // Since fP should be passed with a dimensionality of [fP] = MeV/c
   // it's obvious that "c" wouldn't appear in this calculation.
   Double_t fThetaZero=(13.6/(fBeta*fP))*fZ*TMath::Sqrt(fWidth/X_0)*\
      (1+0.038*TMath::Log(fWidth/X_0));

   // If any, reset gRandom
   if (gRandom) gRandom->Delete();
   gRandom=new TRandom3( time(NULL) );

   // Generate θ form Gaussian distr.
   Double_t thetahit=gRandom->Gaus(0.,fThetaZero);

   // A uniformly-distributed φ angle. 
   Double_t phihit=gRandom->Rndm()*TMath::Pi()*2;

   // Return a pointer to Hit.
   Hit *reshit=Hit::HitOnCylFromHit(fOrigin, thetahit,
      phihit, fRadius, fLayno);

   return reshit;
}