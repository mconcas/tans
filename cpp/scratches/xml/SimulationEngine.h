#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include <TObject.h>
#include <TXMLEngine.h>
#include <TSystem.h>

class SimulationEngine : public TObject {

   public:
      //____________{Con,Des}tructors____________
      SimulationEngine();
      SimulationEngine(TString Pardatafile);
      virtual ~SimulationEngine() { };

      //____________Setters______________________
      void SetParamFile(TString Filename);

      //____________Inline_Getters_______________
      TString             GetParamFile() { return fPardatafile; }
      XMLDocPointer_t     GetXMLDocPointer() { return fXmldocfileptr; }

   private:
      TString             fPardatafile;   // XML filename with input parameters.
      TXMLEngine          fXMLEngine;     // XMLEngine for xml encoding mngmnt.
      XMLDocPointer_t	  fXmldocfileptr; // Pointer to the parsed XML file.


   ClassDef(SimulationEngine,1)
};
#endif
