#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TSystem.h>
#include "SimulationCore.h"
#endif
  

void SimulationSteer(TString ConfigFile="withms_1M/xml/runconfig_0.xml")
{  
   SimulationCore *core=new SimulationCore(ConfigFile);
   core->Initialize();
   core->Status();
   core->Run();
   core->SimulationCore::~SimulationCore();
}

void ChainedSimulation(TString XmlDir="xml/",Int_t Start=0,Int_t Finish=5) {
   for(Int_t i=Start;i<=Finish;++i) {
      TString Buffer;
      Buffer.Form(XmlDir+"runconfig_%d.xml",i);
      SimulationSteer(Buffer);
   }
}
