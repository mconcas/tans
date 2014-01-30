#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Direzione.h"
#include "Hit.h"
#include "Vertice.h"
#include <TChain.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TH1F.h>
#include <TMaterial.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TSystem.h>
#include <TTree.h>
#endif

void test() {
   TFile infile("kinem.root","READ");
   TH1F* histmult=static_cast<TH1F*>(infile.Get("hmul"));
   TH1F* histeta=static_cast<TH1F*>(infile.Get("heta"));
   if(gRandom) delete gRandom;
   gRandom=new TRandom3(time(NULL));
   for(Int_t i=0; i<10; ++i) {
      Printf("Outer loop.");
      Printf("%d",static_cast<Int_t>(histeta->GetRandom()));
      Printf("%d",static_cast<Int_t>(histeta->GetRandom()));
      Printf("%d",static_cast<Int_t>(histeta->GetRandom()));
      Printf("%d",static_cast<Int_t>(histeta->GetRandom()));
      Printf("%d",static_cast<Int_t>(histeta->GetRandom()));
      for(Int_t j=0; j<5;++j) {
         Printf("Inner loop.");
         Printf("%d",static_cast<Int_t>(histmult->GetRandom()));
         Printf("%d",static_cast<Int_t>(histmult->GetRandom()));
         Printf("%d",static_cast<Int_t>(histmult->GetRandom()));
         Printf("%d",static_cast<Int_t>(histmult->GetRandom()));      
      }
   }
   Printf("Fine dela follia.");
}