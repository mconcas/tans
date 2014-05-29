#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TSystem.h>
#include "SimulationCore.h"
#endif


void SimulationSteer(TString ConfigFile="data/Noise_X_Multscatt_disabled_events_1M/xml/runconfig_0.xml")
{  
   SimulationCore *core=new SimulationCore(ConfigFile);
   core->Initialize();
   core->Status();
   core->Run();
   core->SimulationCore::~SimulationCore();
}

void ChainedSimulation() {
   for(Int_t i=0;i<=5;++i) {
      TString Buffer;
      Buffer.Form("data/Noise_X_Multscatt_disabled_events_100K/xml/runconfig_%d.xml",i);
      SimulationSteer(Buffer);
   }
}
