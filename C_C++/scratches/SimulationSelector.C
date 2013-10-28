#define SimulationSelector_cxx
#include "SimulationSelector.h"
#include <TCanvas.h>
#include <TH1F.h>
#include <TH1I.h>
#include <TH2.h>
#include <TSystem.h>
#include <TStyle.h>

void SimulationSelector::Init(TTree *tree) {
   if (!tree) {
      if (gDebug) Printf("** WARNING: no valid tree provided. **");
      return;
   }

   // TTree and TBranches setup.
   fTree=tree;
   Printf("Sto per dumpare");
   fTree->Dump();
   fTree->SetBranchAddress("Vertices", &fAnalyzedVert);
}

Bool_t SimulationSelector::Notify() {
   if (gDebug) Printf("New File/TTree was opened.");
   return kTRUE;
}

void SimulationSelector::Begin(TTree * /*tree*/) {

   // The Begin() function is called at the start of the query.
   // With PROOF Begin() os only called on the client.
   Printf("Session started from client %s", gSystem->HostName());
   
   // Read kinematic file.
   kinem=new TFile("kinem.root", "READ");
   if (gDebug && kinem->IsZombie()) Printf("WARNING: there was some \
      problem opening \"kinem.root\" file. ");
   etahist=static_cast<TH1F*>(kinem->Get("heta"));
   
   // Hope these will be available in the following functions.
   fHistX=new TH1F("fHistX", "X Histogram", 10000, -20, 20);
   // fHistY=new TH1F("fHistY", "Y Histogram", 10000, -20, 20);
   // fHistZ=new TH1F("fHistZ", "Z Histogram", 10000, -20, 20);
   // fHistMult=new TH1I("fHistMult", "Multiplicity Histogram", 10000,
   //   0, 100);
   
   fOutput->Add( fHistX );
   // fOutput->Add( fHistY );
   // fOutput->Add( fHistZ );
   // fOutput->Add( fHistMult );
}

void SimulationSelector::SlaveBegin(TTree * /*tree*/)
{
   // SlaveBegin() is called for each entry in the tree/keyed object.
   

}

Bool_t SimulationSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree 
   // (or possibly keyed object in the case of PROOF) to be processed.
   // The processing can be stopped by calling Abort().
   Printf("Processing Entry number %lld\n", entry);
   ++fNumberOfEvents;   

   // Maybe it changes directory? Perhaps it set address on the 
   // #entry object.
   Printf("E' qui la festa?");
   fTree->GetEvent(entry);
   
   // Fill histograms.
   fHistX->Fill(fAnalyzedVert.GetPuntoX());
   // fHistX->Fill(fAnalyzedVert.GetPuntoY()); 
   // fHistX->Fill(fAnalyzedVert.GetPuntoZ());
   // fHistMult->Fill(fAnalyzedVert.GetVerticeMult());
   
   // Stupid debug print.
    // Printf("Ev=%lld Mult=%d X=%lf Y=%lf Z=%lf", entry, 
      // fAnalyzedVert.GetVerticeMult(), fAnalyzedVert.GetPuntoX(),
      // fAnalyzedVert.GetPuntoY(), fAnalyzedVert.GetPuntoZ());
   
   return kTRUE;
}

void SimulationSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or 
   // objects have been processed. When running with PROOF 
   // SlaveTerminate() is called on each slave server.

   Printf("Total Number of Events: %d", fNumberOfEvents);
}

void SimulationSelector::Terminate()
{
   // The Terminate() function is the last function to be called 
   // during a query. It always runs on the client, it can be used to 
   // present the results graphically or save the results to file.

   fHistX = dynamic_cast<TH1F *>( fOutput->FindObject("fHistX") );
   // fHistY = dynamic_cast<TH1F *>( fOutput->FindObject("fHistY") );
   // fHistZ = dynamic_cast<TH1F *>( fOutput->FindObject("fHistZ") );
   // fHistMult=dynamic_cast<TH1I *>( fOutput->FindObject("fHistMult") ); 
   
   if (fHistX) fHistX->Draw();

   kinem->Close();
}
