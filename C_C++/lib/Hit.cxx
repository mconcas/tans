#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Punto.h"
#include "Vertice.h"
#include "Hit.h"
#include "TSystem.h"
#include "TMath.h"
#endif

ClassImp (Hit)

Hit::Hit(): Punto(),
   layernum(0) {
      detectorshape="";
      vtxname="";
}
Hit::Hit(TString fId, const TString fDetShape, Int_t fLayer,
         TString fVtxName, Double_t fX, Double_t fY, 
         Double_t fZ): Punto(fId, fX, fY, fZ),
         layernum(fLayer) { 
            detectorshape=fDetShape;
            vtxname=fVtxName;
}
Hit *Hit::HitOnCylFromVertex(Vertice fOrigin, Double_t fTheta,
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

Hit *Hit::HitOnCylFromHit(Hit fOrigin, Double_t fTheta,
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
      const TString origname=fOrigin.GetPuntoName();
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
      hitname.Form("From_%s_on_layno_%d", origname.Data(), fLayno);
      /*Build item in return...*/
      Hit *OnCyl=new Hit(hitname, fShape, fLayno, origname, coord[0], 
         coord[1], coord[2]);
      return OnCyl;
   }
}         