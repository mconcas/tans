#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TXMLEngine.h>
#include <TObject.h>
#include <SimulationCore.h>
#endif

ClassImp(SimulationCore)

//____________{Con,Des}tructors____________
SimulationCore::SimulationCore() :
   TObject(),
   fXMLEngine() { fParDataFileName=""; }

SimulationCore::SimulationCore(TString Pardatafile) :
    TObject(),
    fParDataFileName(Pardatafile),
    fXMLEngine() 
{ 

      // Read and store Simulation parameters.
      fXMLEngine=new TXMLEngine();
      fXmldocfileptr=fXMLEngine->ParseFile(fParDataFileName.Data());

}


void SimulationCore::SetParamFile(TString Filename) 
{
   fParDataFileName=Filename;
}
