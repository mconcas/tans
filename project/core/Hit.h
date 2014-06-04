////////////////////////////////////////////////////////////////////////////////
//
// Class Hit: This class represents the hit generated by
//            particles impacting against detectors.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef Hit_H
#define Hit_H
#include "Punto.h"
#include "Vertice.h"
#include "Direzione.h"
#include <TString.h>
#include <TMaterial.h>

class Hit : public Punto {
   public:
      Hit();
      Hit(Double_t X,Double_t Y,Double_t Z,Int_t Layerno,Int_t Idnumber=0);
      virtual ~Hit();

      // ____________Inline_Getters_______________
      Int_t  GetLayerNumber() {return fLayernum;}
      Bool_t IsReal()         {return fIdnumber;}
      Int_t  GetHitID()       {return fRealhit;}

      //_____________Setter_______________________
      void NowRealHit();
      void SetHitID(Int_t Idnumber);
      void SetHitLayno(Int_t Layernum);

      //////////////////////////////////////////////////////////////////////////
      // Compute the cylinder-line intersection and return the "t"
      // parameter.
      static Double_t ComputeT(Double_t Theta,Double_t Phi,Double_t XO,
         Double_t YO,Double_t Radius);

      //////////////////////////////////////////////////////////////////////////
      // It generates a Hit starting from a vertex.
      static Hit HitOnCylFromVertex(Vertice& Origin,Direzione& Direct,
         Double_t Radius,Int_t Idnumber,Int_t Layerno=0);

      //////////////////////////////////////////////////////////////////////////
      // It generates a Hit starting from a Hit, including the
      // possibility of multiple scattering with a TMaterial that
      // cross the trajectory.
      Hit GetHitOnCyl(Direzione& Direct,Double_t Radius,TMaterial& Material,
         Double_t Width, Bool_t Multscat=kTRUE,Int_t Layerno=0,Double_t P=750,
         Int_t Z=1,Double_t Beta=1);


      //_____________Smearing_Generator______
      //////////////////////////////////////////////////////////////////////////
      // Since the finite resolution of detectors the coordinates are
      // slightly modified by a Gaussian algorythm.
      void GausSmearing(Double_t DRadius,Double_t RMSZeta,Double_t RMSRPhi);

      //_____________Noise_Generator_________
      //////////////////////////////////////////////////////////////////////////
      // The first generator creates uniformly distributed hits on
      // a cylindrical surface (e.g. a Detector).
      static Hit NoiseOnCyl(Double_t CRadius,Double_t Zetamin,
         Double_t Zetamax);

   protected:
      Int_t  fLayernum;       // It keeps track of the layer hit
      Int_t  fIdnumber;       // Useful flag for Montecarlo purposes
      Bool_t fRealhit;        // Is it a real hit?

   ClassDef(Hit,1)
};
#endif
