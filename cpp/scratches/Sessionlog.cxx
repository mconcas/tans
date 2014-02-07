#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TObject.h>
#include "Sessionlog.h"
#include <TStopwatch.h>
#include <TSystem.h>
#include <TFile.h>
#endif

ClassImp(Sessionlog)

//___________{De,Con}structors_______________
Sessionlog::Sessionlog() : TObject(),
   genvertices(0) {}

Sessionlog::Sessionlog(Int_t fGenvertices) : TObject(),
   genvertices(fGenvertices) {
      if(!gSystem) gSystem = new TSystem();
      timer.Start(kTRUE);
      gSystem->GetSysInfo(&systeminfo);
      hostname = gSystem->HostName();
}

Double_t Sessionlog::GetRealTime() { return timer.RealTime(); }

Double_t Sessionlog::GetCpuTime() { return timer.CpuTime(); }

TFile Sessionlog::GetSessionLogFile(TString fFilename) {
   TFile outfile(fFilename.Data(),"RECREATE");
   if (outfile.IsZombie())
      Printf("A problem occured creating %s file", fFilename.Data()); 
   return outfile;
}
void Sessionlog::ResetSysInfo() {
   gSystem->GetSysInfo(&systeminfo);
}

