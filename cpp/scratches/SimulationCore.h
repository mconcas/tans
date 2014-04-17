#ifndef SIMULATIONCORE_H
#define SIMULATIONCORE_H

#include <TObject.h>
#include <TXMLEngine.h>
#include <TSystem.h>



class SimulationCore : public TObject {

   public:

      //____________{Con,Des}tructors____________
      SimulationCore();
      SimulationCore(TString Pardatafile);
      virtual ~SimulationCore() { };

      //____________Setters______________________
      void SetParamFile(TString Filename);

      //____________Inline_Getters_______________
      TString             GetParamFile() { return fParDataFileName; }
      XMLDocPointer_t     GetXMLDocPointer() { return fXmldocfileptr; }

   private:
      TString             fParDataFileName;    // XML filename.
      TXMLEngine          *fXMLEngine;         // XMLEngine ptr for xml.
      XMLDocPointer_t     fXmldocfileptr;      // Pointer to the XML content.

      // Parameters
      Int_t    fDebug;
      Int_t    fNuVertices;
      Bool_t   fDryRun;
      Bool_t   fMontecarloTruth;
      Int_t    fNoiseLevel;
      // Double_t fZetaLen;
      // Double_t fBPipeRad;
      // Double_t fFirstRad;
      // Double_t fSecondRad;
      TString  fOutFileName;
      Int_t    fCustomMult;
      Bool_t   fMultipleScat;
      TString  fInputData;
      TString  fMultDist;
      TString  fEtaDist;


   ClassDef(SimulationCore,1)
};
#endif