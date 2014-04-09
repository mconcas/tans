#define DeltaPhiSelector_cxx
#include "DeltaPhiSelector.h"
#include <TH2.h>
#include <TStyle.h>

void DeltaPhiSelector::Init(TTree *tree) {
   
   if (!tree) return;
   fChain = tree;
   Printf("[debug > init(): \"%s\" tree is loaded.",tree->GetName());
   fChain->SetBranchAddress("Firstlayer", &fHitClonesArray_FL);
   fChain->SetBranchAddress("Secondlayer", &fHitClonesArray_SL);
   // fChain->SetMakeClass(1);
}
   

Bool_t DeltaPhiSelector::Notify() {
   // Notify function.
   Printf("[debug > Notify() function called.");
   return kTRUE;
}

void DeltaPhiSelector::Begin(TTree * /*tree*/) {
   Printf("\n\t++ +++ ++ ++ ++++ +++ ++ +++ +++ ++");
   Printf("\t+       Method to find Δϕ         +");
   Printf("\t+ ++ +++ + + ++ ++ +++ + ++ ++ ++ +\n\n");

   fCounter=0;
   TString option = GetOption();

}

void DeltaPhiSelector::SlaveBegin(TTree * /*tree*/) {
   
   fPhiHistogram = new TH1F("DeltaPhi","DeltaPhi distribution",
      300,-0.5,0.5);
   if( fPhiHistogram ) Printf("Histogram created");
   fOutput->Add( fPhiHistogram );
   // TString option = GetOption();

}

Bool_t DeltaPhiSelector::Process(Long64_t entry) {
   // Get the tree entry.
   fChain->GetTree()->GetEntry( entry );

   // Obtain the number of entries.
   Int_t fEntriesLOne = fHitClonesArray_FL->GetEntries();
   Printf("[debug > entry #: %lld, #L1: %d",1+entry,fEntriesLOne);
   Int_t fEntriesLTwo = fHitClonesArray_SL->GetEntries();
   Printf("[debug > entry #: %lld, #L2: %d",1+entry,fEntriesLTwo);

   // Loop over the TClonesArrays.
   for ( Int_t v=0; v<fEntriesLTwo; v++ ) {
      fCounter++;
      fAnaHitScnd = (Hit*)fHitClonesArray_SL->At(v);
      // fAnaHitScnd->Hit::GausSmearing(70,0.12,0.003);
      for (Int_t j=0; j<fEntriesLOne; j++) {
         // Printf("++ Iteration #%d, #%d",v+1,j+1);
         fAnaHitFrst=(Hit*)fHitClonesArray_FL->At(j);
         // fAnaHitFrst->Hit::GausSmearing(40,0.12,0.003);
         if(fAnaHitFrst->GetHitID()==fAnaHitScnd->GetHitID()) {
            fPhiHistogram->Fill(fAnaHitFrst->GetPuntoPhi()-
               fAnaHitScnd->GetPuntoPhi());
         }
      }
   }
   // delete fAnaHitFrst;
   // delete fAnaHitScnd;
   return kTRUE;
}

void DeltaPhiSelector::SlaveTerminate() {
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   Printf("SlaveTerminate() function called.");
   Printf("Counter value: %d",fCounter);
}

void DeltaPhiSelector::Terminate() {
   fPhiHistogram=dynamic_cast<TH1F*>(fOutput->FindObject("DeltaPhi"));
   if (fPhiHistogram) fPhiHistogram->Draw(); 
}
