#define DeltaPhiSelector_cxx
#include "DeltaPhiSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>


void DeltaPhiSelector::Init(TTree *tree) 
{   
   if (!tree) return;
   fChain=tree;
   if(gDebug) Printf("[debug > init(): \"%s\" tree loaded.",tree->GetName());
   fChain->SetBranchAddress("Firstlayer",&fHitClonesArray_FL);
   fChain->SetBranchAddress("Secondlayer",&fHitClonesArray_SL);
}
   

Bool_t DeltaPhiSelector::Notify() 
{
   // Notify function.
   if(gDebug) Printf("[debug > Notify() function called.");
   return kTRUE;
}

void DeltaPhiSelector::Begin(TTree *) 
{
   Printf("\n\t++ +++ ++ ++ ++++ +++ ++ +++ +++ ++");
   Printf("\t+      Selector to find Δϕ        +");
   Printf("\t+ ++ +++ + + ++ ++ +++ + ++ ++ ++ +\n\n");

   fCounter=0;
}

void DeltaPhiSelector::SlaveBegin(TTree *) 
{   
   fPhiHistogram=new TH1F("DeltaPhi","DeltaPhi distribution",1000,-0.015,0.015);
   if( fPhiHistogram ) Printf("Histogram created");
   fOutput->Add(fPhiHistogram);
}

Bool_t DeltaPhiSelector::Process(Long64_t entry) 
{
   
   if(entry!=0) {
      fHitClonesArray_FL->Clear();
      fHitClonesArray_SL->Clear();
   }

   /////////////////////////////////////////////////////////////////////////////
   // Get the tree entry. It means that all pointers are shifted to the proper 
   // items those belong to the event analyzed. (e.g. TClonesArray pointers)
   fChain->GetTree()->GetEntry(entry);

   // Obtain the number of entries.
   Int_t fEntriesLOne=fHitClonesArray_FL->GetEntries();
   Int_t fEntriesLTwo=fHitClonesArray_SL->GetEntries();

   // Loop over the TClonesArrays.
   for (Int_t v=0;v<fEntriesLTwo;v++) {
      fAnaHitScnd = (Hit*)fHitClonesArray_SL->At(v);
      for (Int_t j=0;j<fEntriesLOne;j++) {
         fCounter++;
         fAnaHitFrst=(Hit*)fHitClonesArray_FL->At(j);
         if(fAnaHitFrst->GetHitID()==fAnaHitScnd->GetHitID()) {
            if(gDebug) Printf("%f",TMath::Abs(fAnaHitFrst->GetPuntoPhi())-
               TMath::Abs(fAnaHitScnd->GetPuntoPhi()));

            // Control over the period of angles.
            if(TMath::Abs(fAnaHitFrst->GetPuntoPhi())-
               TMath::Abs(fAnaHitScnd->GetPuntoPhi())>=6.) {
               
               if(fAnaHitFrst->GetPuntoPhi()>fAnaHitScnd->GetPuntoPhi()) {
                  fPhiHistogram->Fill(TMath::Abs(fAnaHitFrst->GetPuntoPhi())+
                     TMath::Abs(fAnaHitScnd->GetPuntoPhi())-2*TMath::Pi());
               } else {
                  fPhiHistogram->Fill(-TMath::Abs(fAnaHitFrst->GetPuntoPhi())+
                     TMath::Abs(fAnaHitScnd->GetPuntoPhi())-2*TMath::Pi());
               }
            } else {
            fPhiHistogram->Fill(TMath::Abs(fAnaHitFrst->GetPuntoPhi())-
               TMath::Abs(fAnaHitScnd->GetPuntoPhi()));
            }
         }
      }
   }

   return kTRUE;
}

void DeltaPhiSelector::SlaveTerminate() 
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   Printf("SlaveTerminate() function called.");
   Printf("Counter value: %d",fCounter);
}

void DeltaPhiSelector::Terminate() 
{
   if(fOutput->IsEmpty()) Printf("[debug >[WARNING!] TList is empty!");
   Printf("[debug > Entries in output TList: %d", fOutput->GetEntries());
   // fPhiHistogram=static_cast<TH1F*>(fOutput->FindObject("DeltaPhi"));
   fPhiHistogram=static_cast<TH1F*>(fOutput->FindObject(fPhiHistogram));
   TFile outfile("histo.root","RECREATE");
   if (outfile.IsZombie()) {
      Printf("A problem occured creating file");
   }
   if (fPhiHistogram) fPhiHistogram->Write(); 
}
