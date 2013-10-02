#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Punto.h"
#include "Vertice.h"
#include "Hit.h"
#include "Direzione.h"
#include <TSystem.h>
#include <TMath.h>
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
   TString fVtxName, Double_t fX, Double_t fY, Double_t fZ): 
      Punto(fId, fX, fY, fZ),
      layernum(fLayer) { 
      detectorshape=fDetShape;
      vtxname=fVtxName;
}

Hit::~Hit() {
   if (gDebug) Printf("*** \"%s\" Hit object was destroyed *** ", 
      this->GetPuntoName().Data());
}


Hit *Hit::HitOnCylFromVertex(Vertice &fOrigin, Direzione &fDirect,
   Double_t fRadius, Int_t fLayno) {

      // Extract theta and phi.
      const Double_t fThe=fDirect.GetDirectTheta();
      const Double_t fPhi=fDirect.GetDirectPhi();
   
      // Manage the ϑ=0 exception*/
      if(fThe==0 || fThe==TMath::Pi()) { 
         Hit *OnCyl=new Hit();
         if (gDebug) Printf("ϑ=0 exception, \
            it will be no scattering.");
         OnCyl->SetPuntoName("ϑ=0 exception, \
            it didn't any scattering.");
         
         return OnCyl;
      }   
      else {

      // Get origin coordinates: xO and yO
      const TString fShape="Cylindrical";
      const Double_t xO=fOrigin.GetPuntoX();
      const Double_t yO=fOrigin.GetPuntoY();
      const Double_t zO=fOrigin.GetPuntoZ();
      const Double_t sintheta=TMath::Sin(fThe);
      const Double_t sinphi=TMath::Sin(fPhi);
      // const Double_t costheta=TMath::Cos(fThe);
      const Double_t cosphi=TMath::Cos(fPhi);
      const TString vertname=fOrigin.GetPuntoName();
      
      // Compute the "t" value.
      const Double_t t=(-(xO*cosphi+yO*sinphi)
         +TMath::Sqrt((xO*cosphi+yO*sinphi)*(xO*cosphi
         +yO*sinphi)-xO*xO-yO*yO+fRadius*fRadius))/sintheta;

      // Set Name.
      TString hitname;
      hitname.Form("From_%s_on_layno_%d", vertname.Data(),
      fLayno);

      // Build item in return.
      Hit *OnCyl=new Hit(hitname, fShape, fLayno, vertname, 
         xO+t*fDirect.GetDirCos1(), 
         yO+t*fDirect.GetDirCos2(), 
         zO+t*fDirect.GetDirCos3());

         return OnCyl;
   }  
}

Hit *Hit::GetHitOnCyl(Direzione &fDirect, Double_t fRadius, 
   TMaterial &fMaterial, Double_t fWidth, Bool_t multscat, 
   Int_t fLayno, Double_t fP, Int_t fZ, Double_t fBeta) {
      if (multscat) {

         /////////////////////////////////////////////////////////////
         // θo represents the rms of a gaussian distribution centred 
         // in 0.
         // Formula used for the MScattering θo evaluation:
         // Dimensionalities:
         // [x]=cm ; [X_0]=cm; [c]=3x10**8; [b]=#; [fP]=MeV/c
         // θo = (13.6 MeV/β*c*p)*Z*(√(x/X_0))[1+0.038*Ln(x/X_0)]
         // Get the Radiation Length: X_0
         /////////////////////////////////////////////////////////////
         Double_t X_0 = fMaterial.GetRadLength();
         
         /////////////////////////////////////////////////////////////  
         // WARNING!
         // Since fP should be passed with a dimensionality of [fP] 
         // [fP]= MeV/c it's obvious that "c" wouldn't appear in this 
         // computation.
         /////////////////////////////////////////////////////////////
         Double_t fThetaZero=(13.6/(fBeta*fP))*fZ*\
         TMath::Sqrt(fWidth/X_0)*(1+0.038*TMath::Log(fWidth/X_0));
   
         // Generate θ form Gaussian distr.
         if (gRandom) gRandom->Delete();
         gRandom=new TRandom3( time(NULL) );
         Double_t thetalocal=gRandom->Gaus(0.,fThetaZero); 
         Double_t philocal=gRandom->Rndm()*2*TMath::Pi();

         // Refer to the Laboratory Reference System.
         fDirect.Direzione::Rotate(thetalocal, philocal, 
            "Multiple Scattering.");
      }  

      // Extract theta and phi.
      const Double_t fThe=fDirect.GetDirectTheta();
      const Double_t fPhi=fDirect.GetDirectPhi();
      
      // Manage the ϑ=0 exception
      if(fThe==0 || TMath::Abs(fThe)==TMath::Pi()) { 
         Hit *OnCyl=new Hit();
         if (gDebug) Printf("ϑ=0 exception, \
            it will be no scattering.");
         OnCyl->SetPuntoName("ϑ=0 exception, \
            it didn't any scattering.");

         return OnCyl;
      } 

      // If ϑ!=0  
      else {

         // Get origin coordinates: xO and yO 
         const TString fShape="Cylindrical";
         const Double_t sintheta=TMath::Sin(fThe);
         const Double_t sinphi=TMath::Sin(fPhi);
         // const Double_t costheta=TMath::Cos(fThe);
         const Double_t cosphi=TMath::Cos(fPhi);
         const TString origname=Name;

         // Compute the "t" value.
         const Double_t t=(-(X*cosphi+Y*sinphi)
            +TMath::Sqrt((X*cosphi+Y*sinphi)*(X*cosphi+Y*sinphi)
            -X*X-Y*Y+fRadius*fRadius))/sintheta;
         
         // Set Name.
         TString hitname;
         hitname.Form("From_%s_on_layno_%d", origname.Data(), 
            fLayno);
            
         // Item in return.
         Hit *OnCyl=new Hit(hitname, fShape, fLayno, origname, 
               X+t*fDirect.GetDirCos1(), 
               Y+t*fDirect.GetDirCos2(),
               Z+t*fDirect.GetDirCos3());
               
            return OnCyl;
      }
}    
