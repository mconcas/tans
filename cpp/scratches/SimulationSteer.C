#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TSystem.h>
#include "SimulationCore.h"
#endif


void SimulationSteer()
{
   SimulationCore *core=new SimulationCore("xml/runconfig.xml");
   core->Initialize();
   core->Status();
   core->Run();
   core->SimulationCore::~SimulationCore();
}
