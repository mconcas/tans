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
   layernum(0),
   detectorshape(""),
   vtxname("") { }

Hit::Hit(TString fId, TString fDetShape, Int_t fLayer, 
   TString fVtxName, Double_t fX, Double_t fY, Double_t fZ): 
      Punto(fId, fX, fY, fZ),
      layernum(fLayer),
      detectorshape(fDetShape),
      vtxname(fVtxName) { }

Hit::~Hit() {
   if (gDebug) Printf("*** \"%s\" Hit object was destroyed *** ", 
      this->GetPuntoName().Data());
}

Double_t Hit::ComputeT(Double_t fTheta, Double_t fPhi,
   Double_t fXO, Double_t fYO, Double_t fRadius) {
      const Double_t sintheta=TMath::Sin(fTheta);
      const Double_t sinphi=TMath::Sin(fPhi);
      const Double_t cosphi=TMath::Cos(fPhi);
      const Double_t t=(-(fXO*cosphi+fYO*sinphi)
            +TMath::Sqrt((fXO*cosphi+fYO*sinphi)*(fXO*cosphi
            +fYO*sinphi)-fXO*fXO-fYO*fYO+fRadius*fRadius))/sintheta;
   return t;
}

Hit *Hit::HitOnCylFromVertex(Vertice &fOrigin, Direzione &fDirect,
   Double_t fRadius, Int_t fLayno) {

      // Extract theta and phi from "Direzione".
      const Double_t fThe=fDirect.GetDirectTheta();
      const Double_t fPhi=fDirect.GetDirectPhi();
      
      ////////////////////////////////////////////////////////////////
      // Manage the ϑ=0 exception
      ////////////////////////////////////////////////////////////////
      if(fThe==0 || fThe==TMath::Pi()) { 
         Hit *OnCyl=new Hit();
         if (gDebug) Printf("ϑ=0 exception, \
            it will be no scattering.");
         OnCyl->SetPuntoName("ϑ=0 exception, \
            it didn't have any scattering.");
         
         return OnCyl;
      }   
      else {

      ////////////////////////////////////////////////////////////////
      // Get origin coordinates: xO and yO from "Vertice"
      ////////////////////////////////////////////////////////////////
      const TString fShape="Cylindrical";
      const Double_t xO=fOrigin.GetPuntoX();
      const Double_t yO=fOrigin.GetPuntoY();
      const Double_t zO=fOrigin.GetPuntoZ();
      const TString vertname=fOrigin.GetPuntoName();
      
      ////////////////////////////////////////////////////////////////
      // Compute the "t" value.
      ////////////////////////////////////////////////////////////////
      const Double_t t=ComputeT(fThe,fPhi,xO,yO,fRadius);

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
         const Double_t X_0 = fMaterial.GetRadLength();
         
         /////////////////////////////////////////////////////////////  
         // WARNING!
         // Since fP should be passed with a dimensionality of [fP] 
         // [fP]= MeV/c it's obvious that "c" wouldn't appear in this 
         // computation.
         /////////////////////////////////////////////////////////////
         const Double_t fThetaZero=(13.6/(fBeta*fP))*fZ*\
         TMath::Sqrt(fWidth/X_0)*(1+0.038*TMath::Log(fWidth/X_0));
         
         /////////////////////////////////////////////////////////////
         // Generate θ form Gaussian distribution with mean 0 and 
         // rms θo. Phi is uniformely distributed between 0<=φ<2Pi
         /////////////////////////////////////////////////////////////
         if (gRandom) gRandom->Delete();
         gRandom=new TRandom3( time(NULL) );
         const Double_t thetalocal=gRandom->Gaus(0.,fThetaZero); 
         const Double_t philocal=gRandom->Rndm()*2*TMath::Pi();
         if (gDebug) Printf("ϑ= %f, φ= %f", thetalocal, philocal);

         /////////////////////////////////////////////////////////////
         // Rotate direction in order to refer it to the Laboratory 
         // Reference System.
         /////////////////////////////////////////////////////////////
         fDirect.Direzione::Rotate(thetalocal, philocal, 
            "Multiple Scattering.");
         if (gDebug) Printf("ϑ= %f, φ= %f", fDirect.GetDirectPhi(),
            fDirect.GetDirectTheta());
      }  

      // Extract theta and phi from "Direzione".
      const Double_t fThe=fDirect.GetDirectTheta();
      const Double_t fPhi=fDirect.GetDirectPhi();
      
      ////////////////////////////////////////////////////////////////
      // Manage the ϑ=0 exception
      ////////////////////////////////////////////////////////////////
      if(fThe==0 || TMath::Abs(fThe)==TMath::Pi()) { 
         Hit *OnCyl=new Hit();
         if (gDebug) Printf("ϑ=0 exception, \
            it will be no scattering.");
         OnCyl->SetPuntoName("ϑ=0 exception, \
            it didn't have any scattering.");

         return OnCyl;
      } 

      ////////////////////////////////////////////////////////////////
      // If ϑ!=0  
      ////////////////////////////////////////////////////////////////
      else {

         // Set data
         const TString fShape="Cylindrical";
         const TString origname=Name;
         TString hitname;
         hitname.Form("From_%s_on_layno_%d", origname.Data(), 
            fLayno);

         /////////////////////////////////////////////////////////////
         // Compute the "t" value.
         /////////////////////////////////////////////////////////////
         const Double_t t=ComputeT(fThe, fPhi, X, Y, fRadius);
         
         // Returned item.
         Hit *OnCyl=new Hit(hitname, fShape, fLayno, origname, 
               X+t*fDirect.GetDirCos1(), 
               Y+t*fDirect.GetDirCos2(),
               Z+t*fDirect.GetDirCos3());
               
            return OnCyl;
      }
}    
