#define NewSelector_cxx
#include "NewSelector.h"
#include "Riostream.h"
#include <TH2.h>
#include "TCanvas.h"
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
   // The Init() function is called when the selector needs to 
   // initialize
   // a new tree or chain. Typically here the branch addresses 
   // and branch
   // pointers of the tree will be set.
   
   if (!tree) return;
   fChain = tree;

   Printf("Changed file!");

   //fChain->SetMakeClass(1);

   fChain->SetBranchAddress("VertMult", /*(Double_t *)*/&fVertMult);
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
   // When running with PROOF SlaveBegin() is called on each slave 
   // server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   //TString option = GetOption();

   //////////////////////////////////////////////////////////////////
   //Personal code and source of errors
   //////////////////////////////////////////////////////////////////

   fHits = new TClonesArray("Punto");
   fVertMult = new Punto();

   fHistX=new TH1I("fHistX", "X Hits Histogram", 100, -10, 10);
   fHistVerZ=new TH1I("fHistVerZ", "Z Vert Histogram", 100, -10, 10);
   fOutput->Add( fHistX );
   fOutput->Add( fHistVerZ );

}

Bool_t NewSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree 
   // (or possibly
   // keyed object in the case of PROOF) to be processed. 

   //////////////////////////////////////////////////////////////////
   //Personal code and source of errors
   //////////////////////////////////////////////////////////////////
   
   if (gDebug)
      Printf("Processing Entry number %lld [%s]", entry, 
         fChain->ClassName());

   ++fNumberOfEvents;

   fChain->GetTree()->GetEvent(entry);

   Int_t num = fHits->GetEntries();
   Int_t num2 = fVertMult->GetMult();

   if (gDebug)
      Printf("Ev=%lld Mult=%d(%d) X=%lf Y=%lf Z=%lf",
         entry,
         num, num2,
         fVertMult->GetX(), fVertMult->GetY(), fVertMult->GetZ());

   Punto *tst;
   for (Int_t j=0; j<num; j++){
      tst=(Punto*)fHits->At(j);
      if (gDebug)
         Printf("  #%d: X=%lf Y=%lf Z=%lf",
         j, tst->GetX(), tst->GetY(), tst->GetZ());
         fHistX->Fill( tst->GetX() );
         fHistVerZ->Fill( fVertMult->GetZ() ); 
   }

   return kTRUE;
}

void NewSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or 
   //objects
   // have been processed. When running with PROOF SlaveTerminate() 
   //is called
   // on each slave server.

   delete fHits;
   printf("\nTotal Number of Events: %d\n", fNumberOfEvents);

}

void NewSelector::Terminate()
{
   // The Terminate() function is the last function to be called 
   // during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   //////////////////////////////////////////////////////////////////
   //Personal code and source of errors
   //////////////////////////////////////////////////////////////////
   TCanvas *c1 = new TCanvas("c1","Canvas Supercool",700,500);
   c1->Divide(2);
   c1->cd(1);
   fHistX=dynamic_cast<TH1I *>( fOutput->FindObject("fHistX") );
   if (fHistX) fHistX->Draw();
   c1->cd(2);
   fHistVerZ=dynamic_cast<TH1I *>( fOutput->FindObject("fHistVerZ") );
   if (fHistVerZ) fHistVerZ->Draw();

}
