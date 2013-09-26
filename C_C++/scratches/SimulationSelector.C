#define SimulationSelector_cxx
#include "SimulationSelector.h"
#include <TH1F.h>
#include <TH2.h>
#include <TStyle.h>

void SimulationSelector::Init(TTree *tree)
{
   if (!tree) return;
   fChain = tree;
   fChain->SetBranchAddress("Vertices", &fAnalyzedVert, &b_vertices);
}

Bool_t SimulationSelector::Notify()
{
   if (gDebug) Printf("New File/TTree was opened.");
   return kTRUE;
}

void SimulationSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // With PROOF Begin() os only called on the client.
   Printf("Session started from client %s", gSystem->HostName());
   TFile *kinem=new TFile("kinem.root", "READ");
   // Debugger
   if (gDebug && kinem->IsZombie()) Printf("WARNING: there was some \
      problem opening \"kinem.root\" file. ");

   etahist=static_cast<TH1F*>(kinem->Get("heta"));
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
   return kTRUE;
}

void SimulationSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or 
   // objects have been processed. When running with PROOF 
   // SlaveTerminate() is calledon each slave server.

}

void SimulationSelector::Terminate()
{
   // The Terminate() function is the last function to be called 
   // during a query. It always runs on the client, it can be used to 
   // present the results graphically or save the results to file.

}
