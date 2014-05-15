#define ReconSelector_cxx
#include "ReconSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>
#include <TSystem.h>
#include <TCanvas.h>

// Δϕ=0.001866[deg]
struct ZTable_t {
   Double_t PeakValue[3];
   Int_t fZBin[3];
};

Double_t ZResidual(Vertice &Vtx, Double_t &ZetaRecons);

Double_t ZEvaluation(Hit &OnFirstlayer, Hit &OnSecondlayer);

ReconSelector::ReconSelector(TTree *) :
   fChain(0),
   fRecZetaHistptr(0),
   fReconVertices(0),
   // Precision settings.
   fDeltaPhi(0.001866),
   fBinSizeRoughHist(2.),             // mm
   fBinSizeFineHist(0.05)            // mm
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
   fNBinsRoughHist=(Int_t)(164.6/fBinSizeRoughHist+1);
   fNBinsFineHist=(Int_t)(164.6/fBinSizeFineHist+1);
   fRecZetaHistptr=new TH1F("Reconstructed Zeta","Z Recons",
      fNBinsFineHist,-82.3,82.3);
   fOutput->Add(fRecZetaHistptr);
}

Bool_t ReconSelector::Process(Long64_t entry)
{  
   Bool_t Flag=kFALSE;
   // Printf("Processing entry #%lld.",entry);
   // Reset TCloneArrays if it isn't the first iteration.
   if(entry!=0) {
      fHitClonesArray_FL->Delete(); // Clear -> Delete!
      fHitClonesArray_SL->Delete(); // Clear -> Delete!
   }

   fZetaFound.fCoord=0.;
   fZetaFound.fError=0.;
   fZetaFound.fGood=kFALSE; 
   
   /////////////////////////////////////////////////////////////////////////////
   // Get the tree entry. It means that all pointers are shifted to the proper
   // items those belong to the event analyzed. (e.g. TClonesArray pointers)
   fChain->GetTree()->GetEntry(entry);

   // Obtain the number of entries. These are the loop limits.
   Int_t fEntriesLOne=fHitClonesArray_FL->GetEntries();
   Int_t fEntriesLTwo=fHitClonesArray_SL->GetEntries();

   // This temporary histograms store all the Zetas deriving from tracklets.
   // The «rough» one has larger bins than the «fine» one.
   // We find the cluster in the first histogram. Once localized the area of 
   // interest, one can continue with the Z reconstruction, refining data. 
   
   TH1F RoughHist=TH1F("Rough HistZ","Z",fNBinsRoughHist,-82.3,82.3);
   TH1F FineHist=TH1F("Fine HistZ","Z",fNBinsFineHist,-82.3,82.3);
   
   // Loop over the TClonesArrays -> Fill the Histograms.
   for(Int_t v=0;v<fEntriesLTwo;v++) {
      fAnaHitScnd=(Hit*)fHitClonesArray_SL->At(v);
      fAnaHitScnd->GausSmearing(70,0.12,0.03);
      for(Int_t j=0;j<fEntriesLOne;j++) {
         fCounter++; // Keep count of the number of iteration.
         fAnaHitFrst=(Hit*)fHitClonesArray_FL->At(j);
         fAnaHitFrst->GausSmearing(40,0.12,0.03);

         ///////////////////////////////////////////////////////////////////////
         // If δϕ<Δϕ, where δϕ is the difference between the angle of one hit
         // on the second layer and angle of each hit on first layer, evaluate Z
         // value and compare it with the true value of vertex object.
         // By default is considered a tolerance of 3σ of the Δϕ histogram.
         if(Punto::GetDeltaPhi(*fAnaHitFrst,*fAnaHitScnd)<=3*fDeltaPhi) {
            Double_t ZetaRecon=ZEvaluation(*fAnaHitFrst,*fAnaHitScnd);
            if(TMath::Abs(ZetaRecon)<=82.3) {
               RoughHist.Fill(ZetaRecon);
               FineHist.Fill(ZetaRecon);
               //fRecZetaHistptr->Fill(ZetaRecon);
               // Printf("----->%f",ZetaRecon);
            }        
         }
      }
   }
   
   //---------------------------------------------------------------------------
   // Find 1st Maximum (bin) in RoughHist, starting from the left.
   Double_t FirstMaxRough=RoughHist.GetMaximum();
   Int_t FirstMaxRoughBin=RoughHist.GetMaximumBin();
   Double_t SecondMaxRough=0;
   Int_t SecondMaxRoughBin=0;
   Int_t Iterator=fNBinsRoughHist;
   // Find 2nd Maximum (bin) in RoughHist, starting from the right. 
   do {
      SecondMaxRough=RoughHist.GetBinContent(Iterator);
      SecondMaxRoughBin=Iterator;
      // Printf("Iterator=%d",Iterator);
      Iterator--;
   } while(SecondMaxRough<FirstMaxRough);
   if(kFALSE) Printf(" Debug: FirstMaxRough  |> %1.1f <| FirstMaxRoughBin  %d\n"
         "\tSecondMaxRough |> %1.1f <| SecondMaxRoughBin %d\n",FirstMaxRough,
         FirstMaxRoughBin,SecondMaxRough,SecondMaxRoughBin);
   /////////////////////////////////////////////////////////////////////////////
   // From the comparison between two found maximum positions one cane make 
   // some choices:
   // In this particulare case we ask that the two maximums can be the same or, 
   // at most, be adjacent.
   if(SecondMaxRoughBin-FirstMaxRoughBin<2) {
      // Shrink the refined histogram range of interest.
      Double_t LowLimit=RoughHist.GetBinLowEdge(FirstMaxRoughBin)-0.4;
      Double_t UpLimit=RoughHist.GetBinLowEdge(SecondMaxRoughBin)
         +fBinSizeRoughHist+0.4;
      FineHist.SetAxisRange(LowLimit,UpLimit);
      // Henceforth the procedure is similar to the one adopted before, with 
      // a different tolerance range.
      Double_t FirstMaxFine=FineHist.GetMaximum();
      Int_t FirstMaxFineBin=FineHist.GetMaximumBin();
      Double_t SecondMaxFine=0;
      Int_t SecondMaxFineBin=0;
      Int_t Iterator2=fNBinsFineHist;
      // Find 2nd Maximum (bin) in FineHist, starting from the right. 
      do {
         SecondMaxFine=FineHist.GetBinContent(Iterator2);
         SecondMaxFineBin=Iterator2;
         // Printf("Iterator2=%d",Iterator2);
         Iterator2--;
      } while(SecondMaxFine<FirstMaxFine || 
         FineHist.GetBinLowEdge(SecondMaxFineBin)>=UpLimit);
      // It ensures that the maximum is found in the defined range of interest.
      
      if(kFALSE) Printf(" Debug: FirstMaxFine  |> %1.1f <| FirstMaxFineBin  %d\n"
            "\tSecondMaxFine |> %1.1f <| SecondMaxFineBin %d\n",FirstMaxFine,
            FirstMaxFineBin,SecondMaxFine,SecondMaxFineBin);
      if(SecondMaxFineBin-FirstMaxFineBin<=10) {
         fZetaFound.fCoord=FineHist.GetBinLowEdge(FirstMaxFineBin)
            +(FineHist.GetBinLowEdge(SecondMaxFineBin)+fBinSizeFineHist-
               FineHist.GetBinLowEdge(FirstMaxFineBin))/2;
         fZetaFound.fError=(FineHist.GetBinLowEdge(SecondMaxFineBin)+
            fBinSizeFineHist-FineHist.GetBinLowEdge(FirstMaxFineBin))/2;
         fZetaFound.fGood=kTRUE;
      }
   }

   //---------------------------------------------------------------------------
   if(fZetaFound.fGood) fRecZetaHistptr->Fill(fZetaFound.fCoord);
   return kTRUE;
}

void ReconSelector::SlaveTerminate()
{
   /////////////////////////////////////////////////////////////////////////////
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate()  is called
   // on each slave server.
   Printf("SlaveTerminate() function called.");
   Printf("fReconVertices value: %d", fReconVertices);
   Printf("Counter value: %d",fCounter);
}

void ReconSelector::Terminate()
{
   if(fOutput->IsEmpty()) Printf("[debug >[WARNING!] TList is empty!");
   // Finalize 
   fRecZetaHistptr=static_cast<TH1F*>(fOutput->FindObject(fRecZetaHistptr));
   TFile outfile("ZReconstructed.root","RECREATE");
   if(outfile.IsZombie()) {
      Printf("A problem occured creating file");
   }
   if(fRecZetaHistptr) fRecZetaHistptr->Write();
}

Double_t ZEvaluation(Hit &OnFirstlayer, Hit &OnSecondlayer)
{
   Double_t result=(OnFirstlayer.GetPuntoZ()
      *OnSecondlayer.GetPuntoCRadius()-OnSecondlayer.GetPuntoZ()*
      OnFirstlayer.GetPuntoCRadius())/(OnSecondlayer.GetPuntoCRadius()
      -OnFirstlayer.GetPuntoCRadius());
   if(TMath::Abs(CorrMachinePrecision(result))!=0)
      Printf("\x1B[31mZ CALCOLATO %1.15lf\x1B[0m",result);
   return result;
}

Double_t ZResidual(Vertice &Vtx, Double_t &ZetaRecons)
{
   return Vtx.GetPuntoZ()-ZetaRecons;
}