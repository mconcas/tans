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
      Direzione(Double_t Theta, Double_t Phi, Int_t Idnumber);
      virtual ~Direzione();

      //____________Setters______________________
      void SetDirectTheta(const Double_t Theta);
      void SetDirectPhi(const Double_t Phi);
      void SetCosDir1(const Double_t C1);
      void SetCosDir2(const Double_t C2);
      void SetCosDir3(const Double_t C3);
      void SetAllCos(const Double_t C1, const Double_t C2,
                      const Double_t C3 );
      void SetAllAngles(const Double_t Theta, const Double_t Phi);
      void SetDirectID(const Double_t id);
      void FlipBit();
      void Rotate(Double_t Theta, Double_t Phi);

      //____________Getters______________________
      Int_t    GetDirectID()     const {return fIdnumber;}
      Double_t GetDirectTheta()  const {return fTheta;}
      Double_t GetDirectPhi()    const {return fPhi;}
      Double_t GetDirCos1()      const {return fDc1;}
      Double_t GetDirCos2()      const {return fDc2;}
      Double_t GetDirCos3()      const {return fDc3;}
      Bool_t   GetRotStatus()    const {return fIsrotated;}

   private:

      void UpdateAng();
      void UpdateDirCos();

   protected:

      //____________Datamembers__________________
      Int_t    fIdnumber;    // Identification number.
      Double_t fTheta;       // Theta angle.
      Double_t fPhi;         // Phi angle.
      Double_t fDc1;         // Director cosine x.
      Double_t fDc2;         // Director cosine y.
      Double_t fDc3;         // Director cosine z.
      Bool_t   fIsrotated;   // Debug flag.

   ClassDef(Direzione, 1)

};

#endif
