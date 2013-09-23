//////////////////////////////////////////////////////////////////////
// Puppettiamo.
//////////////////////////////////////////////////////////////////////


#if !defined (__CINT__) || defined (__MAKECINT__)
// #include "Vertice.h"
#include "TH1F.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "TMath.h"
#endif

Double_t GetRandomFromDistFile(const TString fName="../kinem.root",
         const TString fKeyname="heta") {
   TFile *fileptr=new TFile(fName.Data());
   TH1F *histptr=static_cast<TH1F*>(fileptr->Get(fKeyname.Data()));
   Double_t result=histptr->GetRandom();
   fileptr->Close();
   return result;
}

Double_t InvertedFunction(Double_t fValue) {
   Double_t part=2*TMath::ATan(TMath::Exp(-fValue));
   return part;
};