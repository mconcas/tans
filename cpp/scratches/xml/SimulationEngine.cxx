#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TXMLEngine.h>
#include <TObject.h>
#include <SimulationEngine.h>
#endif

ClassImp(SimulationEngine)

//____________{Con,Des}tructors____________
SimulationEngine::SimulationEngine() :
   TObject(),
   fXMLEngine() { fPardatafile=""; }

SimulationEngine::SimulationEngine(TString Pardatafile) :
    TObject(),
    fPardatafile(Pardatafile),
    fXMLEngine() {
}


void SimulationEngine::SetParamFile(TString Filename) {
   fPardatafile=Filename;
}
