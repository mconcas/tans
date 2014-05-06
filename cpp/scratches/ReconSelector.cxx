#define ReconSelector_cxx
#include "ReconSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>

// Δϕ=0.001866[deg] 


Double_t ZEvaluation(Hit &OnFirstlayer, Hit &OnSecondlayer)
{
   Double_t result=(OnFirstlayer.GetPuntoZ()*OnSecondlayer.GetPuntoCRadius()
      -OnSecondlayer.GetPuntoZ()*OnFirstlayer.GetPuntoCRadius())/
      (OnSecondlayer.GetPuntoCRadius()-OnFirstlayer.GetPuntoCRadius());

   return result;
}

Double_t ZResidual(Vertice &Vtx, Double_t &ZetaRecons)
{
   return Vtx.GetPuntoZ()-ZetaRecons;
}


ReconSelector::ReconSelector(TTree *) : 
   fChain(0),
   fResZetaHistptr(0),
   fDeltaPhi(0.001866) 
{
   fAnaVertex=new Vertice();
   fHitClonesArray_FL=new TClonesArray("Hit",kMaxFirstlayer);
   fHitClonesArray_SL=new TClonesArray("Hit",kMaxSecondlayer);
} 

void ReconSelector::Init(TTree *tree) 
{   
   if(!tree) return;
   fChain=tree;
   Printf("[debug > init(): \"%s\" tree loaded.",tree->GetName());
   fChain->SetBranchAddress("Vertices",&fAnaVertex);
   fChain->SetBranchAddress("Firstlayer",&fHitClonesArray_FL);
   fChain->SetBranchAddress("Secondlayer",&fHitClonesArray_SL);
}
   

Bool_t ReconSelector::Notify() 
{
   // Notify function.
   Printf("[debug > Notify() function called.");
   return kTRUE;
}

void ReconSelector::Begin(TTree *) 
{  
   Printf("\x1B[31m\n\t++ +++ ++ ++ ++++ +++ ++ +++ +++ ++\x1B[0m");
   Printf("\x1B[31m\t+     Reconstruction Selector     +\x1B[0m");
   Printf("\x1B[31m\t+ ++ +++ + + ++ ++ +++ + ++ ++ ++ +\x1B[0m\n\n");

   fCounter=0;
}

void ReconSelector::SlaveBegin(TTree *) 
{   
   // Store the Z Residual.  
   fResZetaHistptr=new TH1F("Reconstructed Zeta","Reconstructed Zeta",
      1000,-2. /*mm*/,2.3 /*mm*/);
   fOutput->Add(fResZetaHistptr);
}

Bool_t ReconSelector::Process(Long64_t entry) 
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
      fAnaHitScnd=(Hit*)fHitClonesArray_SL->At(v);
      for (Int_t j=0;j<fEntriesLOne;j++) {
         fCounter++;
         fAnaHitFrst=(Hit*)fHitClonesArray_FL->At(j);

         ///////////////////////////////////////////////////////////////////////
         // If δϕ<Δϕ, where δϕ is the difference between the phi from one hit 
         // on the second layer and phi of each hit on first layer, compute Z 
         // value and compare with the true value from vertex object.
         // By default a tolerance of 3σ is considered.
         if(Hit::GetDeltaPhi(*fAnaHitFrst,*fAnaHitScnd)<=3*fDeltaPhi) {
            Double_t ZetaRecon=ZEvaluation(*fAnaHitFrst,*fAnaHitScnd);
            fResZetaHistptr->Fill(ZResidual(*fAnaVertex,ZetaRecon));
            ++fReconVertices;
            ++fCounter;
         } else {
            ++fCounter;
         }
      }
   }

   return kTRUE;
}

void ReconSelector::SlaveTerminate() 
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   Printf("SlaveTerminate() function called.");
   Printf("fReconVertices value: %d", fReconVertices);
   Printf("Counter value: %d",fCounter);
}

void ReconSelector::Terminate() 
{
   if(fOutput->IsEmpty()) Printf("[debug >[WARNING!] TList is empty!");
   /* Finalize */
   fResZetaHistptr=static_cast<TH1F*>(fOutput->FindObject(fResZetaHistptr));
   TFile outfile("ZResiduals.root","RECREATE");
   if(outfile.IsZombie()) {
      Printf("A problem occured creating file");
   }
   if(fResZetaHistptr) fResZetaHistptr->Write(); 
}
