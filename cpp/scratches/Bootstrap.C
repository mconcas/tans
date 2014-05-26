#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TSystem.h>
#include <TString.h>
#endif

void Bootstrap(const TString Macro="SimulationSteer.C",
   const TString Option="force") 
{  
   if(Macro=="SimulationSteer.C") {
      // (Re)Compile classes, macros, etc.
      TString option;
      if(Option.Contains("force")) option="kfg";
      else option="kg";
      gSystem->CompileMacro("Punto.cxx",option);
      gSystem->CompileMacro("Direzione.cxx",option);
      gSystem->CompileMacro("Vertice.cxx",option);
      gSystem->CompileMacro("Hit.cxx",option);
      gSystem->CompileMacro("SimulationCore.cxx",option);
      gSystem->CompileMacro(Macro.Data());
   } else {
      gSystem->CompileMacro(Macro.Data());
   }
}