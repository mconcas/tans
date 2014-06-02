#ifndef SIMULATIONCORE_H
#define SIMULATIONCORE_H

#include <TObject.h>
#include <TXMLEngine.h>
#include <TMaterial.h>
#include <TSystem.h>

struct pipe_t {
   Int_t     fLayer;
   TMaterial fMaterial;
   Double_t  fZetaLen;
   Double_t  fThickness;
   Double_t  fPipeRad;
};


class SimulationCore : public TObject {

   public:

      //____________{Con,Des}tructors____________
      SimulationCore();
      SimulationCore(TString Pardatafile,TString SimName="Default");
      virtual ~SimulationCore() { };

      //____________Setters______________________
      void SetParamFile(TString Filename);
      Bool_t Initialize();

      //____________Inline_Getters_______________
      TString             GetParamFile() {return fParDataFileName;}
      XMLDocPointer_t     GetXMLDocPointer() {return fXmlDocFileptr;}

      //____________Run_Simulation_______________
      Bool_t Status();
      Bool_t Run();

   private:

      // XML Specifications
      TString             fParDataFileName;    // XML filename.
      TXMLEngine          *fXMLEngine;         // XMLEngine ptr for xml.
      XMLDocPointer_t     fXmlDocFileptr;      // Pointer to the XML content.
      XMLNodePointer_t    fIndexNodeptr;       // Index running on XML element.

      // Parameters
      Int_t               kDebug;              // Debug flag.
      Bool_t              kMultipleScat;       // Enable/Disable multiple scatt.
      Bool_t              kDryRun;             // Dry run flag.
      Bool_t              kSmearing;           // Enable/Disable gaus smearing.
      TString             fSimulationName;     // Item Name.
      TString             fInputData;          // Input file.
      Int_t               fFixedMult;          // Fixed multiplicity, if exists.
      TString             fMultDist;           // Multiplicity histogram name.
      TString             fEtaDist;            // η histogram name.
      Int_t               fNumVertices;        // Number of events to generate.
      Int_t               fNoiseLevel;         // Noise level.
      pipe_t              fBeampipe;           // Beampipe struct.
      pipe_t              fFirstLayer;         // First detector struct.
      pipe_t              fSecondLayer;        // Second detector struct.
      
      TString             fOutFileName;        // Output file name.
      Double_t            fRmsX;               // Rms of X coordinate.
      Double_t            fRmsY;               // Rms of Y coordinate.
      Double_t            fRmsZ;               // Rms of Z coordinate.


   ClassDef(SimulationCore,1)
};
#endif
