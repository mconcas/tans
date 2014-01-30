//////////////////////////////////////////////////////////////////////
//class Direzione: basic geometry class.
//
//Written by Matteo Concas: mett1990@gmail.com 
//Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////
#ifndef DIREZIONE_H
#define DIREZIONE_H

#include <TObject.h>
#include <TString.h>

class Direzione : public TObject {

   public:
      //____________{Con,Des}tructors____________
      Direzione();
      Direzione(Double_t fTheta, Double_t fPhi, Int_t fIdentity);
      virtual ~Direzione();

      //____________Setters______________________
      void SetDirectTheta( const Double_t fTheta);
      void SetDirectPhi( const Double_t fPhi);
      void SetCosDir1( const Double_t fC1 ); 
      void SetCosDir2( const Double_t fC2 ); 
      void SetCosDir3( const Double_t fC3 );
      void SetAllCos( const Double_t fC1, const Double_t fC2, 
                     const Double_t fC3 );
      void FlipBit();
      void Rotate(Double_t fTheta, Double_t fPhi);

      //____________Getters______________________
      Int_t    GetDirectID()     const { return identity; }
      Double_t GetDirectTheta()  const { return theta; }
      Double_t GetDirectPhi()    const { return phi ;}
      Double_t GetDirCos1()      const { return dc1; }
      Double_t GetDirCos2()      const { return dc2; }
      Double_t GetDirCos3()      const { return dc3; }
      Bool_t   GetRotStatus()    const { return isrotated; }
      
   private:

      void UpdateAng();
      void UpdateDirCos();

   protected:

      //____________Datamembers__________________
      Int_t    identity;    // Identification number.
      Double_t theta;       // Theta angle.
      Double_t phi;         // Phi angle.
      Double_t dc1;         // Director cosine x.
      Double_t dc2;         // Director cosine y.
      Double_t dc3;         // Director cosine z.
      Bool_t   isrotated;   // Debug flag.

   ClassDef(Direzione, 1)

};

#endif