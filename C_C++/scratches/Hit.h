//////////////////////////////////////////////////////////////////////
//class HitCylinder: This class represents the hit generated by 
//                   particles impacting against detectors. 
//                   Hits objects are generally stored in a file, they
//                   are fundamental data for analysis and 
//                   reconstruction.
//Written by Matteo Concas: mett1990@gmail.com 
//Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////

#ifndef Hit_H
#define Hit_H
#include "Punto.h"
#include "Vertice.h"
#include <TString.h>
#include <TMaterial.h>

class Hit : public Punto {
   public:
      Hit();
      Hit(TString fId, TString fDetShape, Int_t fLayer,
         TString fVtxName, Double_t fX, Double_t fY, Double_t fZ);
      //virtual ~Hit();

      // ____________Inline_Getters_______________
      TString GetVertNameFromHit() const {return vtxname;}
      TString GetDetShape() const {return vtxname;}
      Int_t GetLayerNumber() const {return layernum;}

      //_____________Generators___________________

      //Given three coordinates, generate a Hit starting from a 
      //vertex.
      static Hit *HitOnCylFromVertex(Vertice &fOrigin,Double_t fTheta,
         Double_t fPhi,Double_t fRadius, Int_t fLayno);

      //Given three coordinates, generate a Hit starting from a Hit.
      static Hit *HitOnCylFromHit(Hit &fOrigin, Double_t fTheta,
         Double_t fPhi, Double_t fRadius, Int_t fLayno);

      //Given three coordinates, generate a Hit starting from a Hit, 
      //including the possibility of multiple scattering with a 
      //TMaterial along the trajectory.
      static Hit *HitOnCylWithMScat(Hit &fOrigin, Double_t fRadius, 
         Int_t fLayno, TMaterial &fMaterial, Double_t fWidth, 
         Double_t fP=750, Int_t fZ=1, Double_t fBeta=1);

   protected:
      Int_t layernum;       // It keeps track of the layer hit
      TString detectorshape;// It keeps memory of the detector's shape
      TString vtxname;      // Vertex Name

   ClassDef(Hit,1)
};
#endif