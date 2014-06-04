////////////////////////////////////////////////////////////////////////////////
//
// Class Vertice            
//               
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
      Int_t GetVerticeMult() const {return fMultiplicity;}
      Int_t GetVerticeID() const {return fIdnumber;}
      Double_t GetVerticeNL() const {return fNoiselevel;}
      Bool_t GetVerticeGoodness() const {return fIsAGoodGuy;}

   protected:
      Int_t fMultiplicity;   // Number of generated tracks.
      Int_t fIdnumber;       // Tag useful in simulation.
      Double_t fNoiselevel;  // Set noise level.
      Bool_t fIsAGoodGuy;    // Post simulation flag, useful for eff. analysis.

   ClassDef(Vertice,1)
};
#endif
