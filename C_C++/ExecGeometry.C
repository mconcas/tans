#if !defined (__CINT__) || defined (__MAKECINT__)
#include "TString.h"
#include "TSystem.h"
#include "TString.h"
#endif
void ExecGeometry(){
gSystem->CompileMacro("Punto.cxx","kfg");
gSystem->CompileMacro("Retta.cxx","kfg");
gSystem->CompileMacro("Simulazione.C","kfg");
}
