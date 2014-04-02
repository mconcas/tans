#define NewSelector_cxx
// The class definition in NewSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("NewSelector.C")
// Root > T->Process("NewSelector.C","some options")
// Root > T->Process("NewSelector.C+")
//

#include "NewSelector.h"
#include "Riostream.h"
#include <TH2.h>
#include <TStyle.h>
#include <TSystem.h>
#include "Punto.h"

Bool_t NewSelector::Notify()
{
   // The Notify() function is called when a new file is opened. 
   return kTRUE;
}

void NewSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   
   if (!tree) return;
   fChain = tree;

   //fChain->SetMakeClass(1);
   fHits = new TClonesArray("Punto");
   fChain->SetBranchAddress("VertMult", (Double_t *)&fVertMult);
   fChain->SetBranchAddress("Hits", &fHits);

}

void NewSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
  
   //TString option = GetOption();
   
   //////////////////////////////////////////////////////////////////
   //Personal code and source of errors
   //////////////////////////////////////////////////////////////////

   Printf("Ciao da %s", gSystem->HostName());

}

void NewSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   //TString option = GetOption();

   //////////////////////////////////////////////////////////////////
   //Personal code and source of errors
   //////////////////////////////////////////////////////////////////

   // fHits = new TClonesArray("Punto");

   fHistX = new TH1I("fHistX", "X Histogram", 100, -10, 10);
   fOutput->Add( fHistX );

}

Bool_t NewSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. 

   //////////////////////////////////////////////////////////////////
   //Personal code and source of errors
   //////////////////////////////////////////////////////////////////
   
   // Printf("Processing Entry number %lld\n", entry);
   ++fNumberOfEvents;

   fChain->GetEvent(entry);

   Int_t num = fHits->GetEntries();

   // Printf("Ev=%lld Mult=%d(%d) X=%lf Y=%lf Z=%lf",
      // entry,
      // fVertMult.mult, num, fVertMult.X, fVertMult.Y, fVertMult.Z);

   Punto *tst;
   for (Int_t j=0; j<num; j++){
      tst=(Punto*)fHits->At(j);
      // Printf("  #%d: X=%lf Y=%lf Z=%lf",
        // j, tst->GetX(), tst->GetY(), tst->GetZ());
      // fHistX->Fill( tst->GetX() );
   }

   return kTRUE;
}

void NewSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

   delete fHits;
   printf("\nTotal Number of Events: %d\n", fNumberOfEvents);

}

void NewSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   //////////////////////////////////////////////////////////////////
   //Personal code and source of errors
   //////////////////////////////////////////////////////////////////

   fHistX = dynamic_cast<TH1I *>( fOutput->FindObject("fHistX") );
   if (fHistX) fHistX->Draw();

}
