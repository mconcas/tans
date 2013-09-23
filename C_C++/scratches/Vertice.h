//////////////////////////////////////////////////////////////////////
//class Vertice: inherits from Punto, adding some features, one over  
//               all is "multiplicity", the number of particles 
//               generated in each collision.
//               In this «trivial» case this «Vertice class» may 
//               result pointless.
//               The idea is to derive a set of more «physics classes»  
//               from the geometrical and abstracts ones in order to 
//               easily describe experiment and add customizations 
//               without rewrite the whole source code.
//Written by Matteo Concas: mett1990@gmail.com Sep 2013
//Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////

#ifndef VERTICE_H
#define VERTICE_H
#include "Punto.h"
#include "TString.h"
class Vertice : public Punto {
   public: 
      Vertice();
      Vertice(TString fName, Double_t fX, Double_t fY, Double_t fZ, 
         Int_t fMult);
      virtual ~Vertice();
      //____________Setters______________________
      void SetVerticeMult(Int_t fMult);
      //____________Inline_Getters_______________
      Int_t GetVerticeMult() const {return multiplicity;}

   protected:
      Int_t multiplicity; // Number of generated tracks. 
   ClassDef(Vertice,1)
};

#endif