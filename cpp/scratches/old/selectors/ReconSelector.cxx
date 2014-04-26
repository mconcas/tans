#define ReconSelector_cxx
// The class definition in ReconSelector.h has been generated automatically
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
// Root > T->Process("ReconSelector.C")
// Root > T->Process("ReconSelector.C","some options")
// Root > T->Process("ReconSelector.C+")
//

#include "ReconSelector.h"
#include <TH2.h>
#include "TRandom3.h"
#include "TH1F.h"
#include <TStyle.h>


void ReconSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   

      
   // TString option = GetOption();

}

void ReconSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   testHist = new TH1F("testHist","Histest",100,-1,1);
   fOutput->Add( testHist );

}

void ReconSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   // fChain->SetMakeClass(1);
   fChain->SetBranchAddress("Firstlayer", &fHitClonesArray_FL);
   fChain->SetBranchAddress("Secondlayer", &fHitClonesArray_SL);
   // fChain->SetBranchAddress("Firstlayer", &Firstlayer, &b_Firstlayer);
   // fChain->SetBranchAddress("Firstlayer.fUniqueID", Firstlayer_fUniqueID, &b_Firstlayer_fUniqueID);
   // fChain->SetBranchAddress("Firstlayer.fBits", Firstlayer_fBits, &b_Firstlayer_fBits);
   // fChain->SetBranchAddress("Firstlayer.X", Firstlayer_X, &b_Firstlayer_X);
   // fChain->SetBranchAddress("Firstlayer.Y", Firstlayer_Y, &b_Firstlayer_Y);
   // fChain->SetBranchAddress("Firstlayer.Z", Firstlayer_Z, &b_Firstlayer_Z);
   // fChain->SetBranchAddress("Firstlayer.Theta", Firstlayer_Theta, &b_Firstlayer_Theta);
   // fChain->SetBranchAddress("Firstlayer.Phi", Firstlayer_Phi, &b_Firstlayer_Phi);
   // fChain->SetBranchAddress("Firstlayer.CRadius", Firstlayer_CRadius, &b_Firstlayer_CRadius);
   // fChain->SetBranchAddress("Firstlayer.SRadius", Firstlayer_SRadius, &b_Firstlayer_SRadius);
   // fChain->SetBranchAddress("Firstlayer.layernum", Firstlayer_layernum, &b_Firstlayer_layernum);
   // fChain->SetBranchAddress("Firstlayer.idnumber", Firstlayer_idnumber, &b_Firstlayer_idnumber);
   // fChain->SetBranchAddress("Firstlayer.realhit", Firstlayer_realhit, &b_Firstlayer_realhit);
   // fChain->SetBranchAddress("Secondlayer", &Secondlayer, &b_Secondlayer);
   // fChain->SetBranchAddress("Secondlayer.fUniqueID", Secondlayer_fUniqueID, &b_Secondlayer_fUniqueID);
   // fChain->SetBranchAddress("Secondlayer.fBits", Secondlayer_fBits, &b_Secondlayer_fBits);
   // fChain->SetBranchAddress("Secondlayer.X", Secondlayer_X, &b_Secondlayer_X);
   // fChain->SetBranchAddress("Secondlayer.Y", Secondlayer_Y, &b_Secondlayer_Y);
   // fChain->SetBranchAddress("Secondlayer.Z", Secondlayer_Z, &b_Secondlayer_Z);
   // fChain->SetBranchAddress("Secondlayer.Theta", Secondlayer_Theta, &b_Secondlayer_Theta);
   // fChain->SetBranchAddress("Secondlayer.Phi", Secondlayer_Phi, &b_Secondlayer_Phi);
   // fChain->SetBranchAddress("Secondlayer.CRadius", Secondlayer_CRadius, &b_Secondlayer_CRadius);
   // fChain->SetBranchAddress("Secondlayer.SRadius", Secondlayer_SRadius, &b_Secondlayer_SRadius);
   // fChain->SetBranchAddress("Secondlayer.layernum", Secondlayer_layernum, &b_Secondlayer_layernum);
   // fChain->SetBranchAddress("Secondlayer.idnumber", Secondlayer_idnumber, &b_Secondlayer_idnumber);
   // fChain->SetBranchAddress("Secondlayer.realhit", Secondlayer_realhit, &b_Secondlayer_realhit);
}

Bool_t ReconSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   Printf("[debug > Notify() function called.");
   return kTRUE;
}

Bool_t ReconSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either ReconSelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   Int_t numero=entry*100000;
   for(Int_t i=0; i<numero; i++) {
      testHist->Fill(gRandom->Gaus(0,1));
      ++fCounter;
   }
   
   return kTRUE;
}

void ReconSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   Printf("\nTotal Number of loops on slave: %d", fCounter);
}

void ReconSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   testHist=dynamic_cast<TH1F*>(fOutput
      ->FindObject("testHist"));
      testHist->Dump();
   if (testHist) testHist->Draw();

}
