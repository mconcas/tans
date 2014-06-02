////////////////////////////////////////////////////////////////////////////////
//class Vertice: inherits from Punto, adding some features, one over
//               all is "multiplicity", the number of particles
//               generated in each collision.
//               In this «trivial» case this «Vertice class» may
//               result pointless.
//               The idea is to derive a set of more «physics classes»
//               from the geometrical and abstracts ones in order to
//               easily describe experiment and add customizations
//               without rewrite the whole code.
//Written by Matteo Concas: mett1990@gmail.com Sep 2013
//Exam: «Tecniche di analisi numerica e simulazione»
////////////////////////////////////////////////////////////////////////////////

#ifndef VERTICE_H
#define VERTICE_H
#include "Punto.h"
#include "TString.h"
class Vertice : public Punto {
   public:
      Vertice();
      Vertice(Double_t X, Double_t Y, Double_t Z,
         Int_t Mult, Int_t IDnumber, Double_t Noiselvl=0);
      virtual ~Vertice();

      //____________Setters______________________
      void SetVerticeMult(Int_t Mult);
      void SetVerticeID(Int_t IDno);
      void SetVerticeNL(Double_t Noiselvl);
      void SetVerticeGoodness(Bool_t Goodness);

      //____________Inline_Getters_______________
      Int_t GetVerticeMult() {return fMultiplicity;}
      Int_t GetVerticeID() {return fIdnumber;}
      Double_t GetVerticeNL() {return fNoiselevel;}
      Bool_t GetVerticeGoodness() {return fIsAGoodGuy;}

   protected:
      Int_t fMultiplicity;   // Number of generated tracks.
      Int_t fIdnumber;       // Tag useful in simulation.
      Double_t fNoiselevel;  // Set noise level.
      Bool_t fIsAGoodGuy;    // Post simulation flag, useful for eff. analysis.

   ClassDef(Vertice,1)
};

#endif
