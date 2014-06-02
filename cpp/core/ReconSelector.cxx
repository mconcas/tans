#define ReconSelector_cxx
#include "ReconSelector.h"
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TCut.h>
#include <TF1.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TMath.h>
#include <TNtuple.h>
#include <TSystem.h>

// MEMO:
// Δϕ=0.001866[deg]
// kTRUE=1
// kFALSE=0


// Utility functions references.
Double_t ZEvaluation(Hit &OnFirstlayer, Hit &OnSecondlayer);
void XtrapolateZeta(TH1F &Rough,TH1F &Fine,ZReal_t &ZetaFound);

ReconSelector::ReconSelector(TTree *) :
   fChain(0),
   fRecZetaHistptr(0),
   fResidualZetaptr(0),
   fResultsNtuple(0),
   fDebugZetaGenerated(0),
   fReconVertices(0),
   fDeltaPhi(0.005),                  // [rad]
   fBinSizeRoughHist(1.),             // mm
   fBinSizeFineHist(0.005)            // mm
   {
      fAnaVertex=new Vertice();
      fHitClonesArray_FL=new TClonesArray("Hit",kMaxFirstlayer);
      fHitClonesArray_SL=new TClonesArray("Hit",kMaxSecondlayer);
   }

void ReconSelector::Init(TTree *tree)
{
   if(!tree) return;
   fChain=tree;
   Printf("Init(): \"%s\" tree loaded.",tree->GetName());
   fChain->SetBranchAddress("Vertices",&fAnaVertex);
   fChain->SetBranchAddress("Firstlayer",&fHitClonesArray_FL);
   fChain->SetBranchAddress("Secondlayer",&fHitClonesArray_SL);
}

Bool_t ReconSelector::Notify()
{
   // Notify function.
   Printf("Notify() function called.");
   return kTRUE;
}

void ReconSelector::Begin(TTree *)
{
   Printf("\x1B[32m\n\t++ +++ ++ ++ ++++ +++ ++ +++ +++ ++\x1B[0m");
   Printf("\x1B[32m\t+     Reconstruction Selector     +\x1B[0m");
   Printf("\x1B[32m\t+ ++ +++ + + ++ ++ +++ + ++ ++ ++ +\x1B[0m\n\n");
}

void ReconSelector::SlaveBegin(TTree *)
{
   fNBinsRoughHist=(Int_t)(164.6/fBinSizeRoughHist);
   fNBinsFineHist=(Int_t)(164.6/fBinSizeFineHist);
   fRecZetaHistptr=new TH1F("Reconstructed","Zreconstructed",50000,-82.3,82.3);
   fResidualZetaptr=new TH1F("Residual Zeta","Z Residuals",50000,-82.3,82.3);
   fResultsNtuple=new TNtuple("ResNtuple","Results",
      "ZTruth:ZRecon:ReconGood:ZResidual:Noise:Multiplicity");
   fDebugZetaGenerated=new TH1F("Generated","ZGenerated",50000,-82.3,82.3);
   // Add to Output list.
   fOutput->Add(fResidualZetaptr);
   fOutput->Add(fRecZetaHistptr);
   fOutput->Add(fResultsNtuple);
   fOutput->Add(fDebugZetaGenerated);
}

Bool_t ReconSelector::Process(Long64_t entry)
{
   // Reset TCloneArrays if it isn't the first iteration.
   if(entry!=0) {
      fHitClonesArray_FL->Delete(); // Clear() changed to: Delete().
      fHitClonesArray_SL->Delete(); // Clear() changed to: Delete().
   }

   /////////////////////////////////////////////////////////////////////////////
   // Get the tree entry. It means that all pointers are shifted to the proper
   // items belonging to the analyzed event. (e.g. TClonesArray pointers)
   fChain->GetTree()->GetEntry(entry);

   // Obtain total number of entries. These are the loops limit.
   Int_t fEntriesLOne=fHitClonesArray_FL->GetEntries();
   Int_t fEntriesLTwo=fHitClonesArray_SL->GetEntries();

   // This temporary histograms store all the Zetas deriving from tracklets.
   // The «rough» one has larger bins than the «fine» one.
   // We find the cluster in the first histogram. Once localized the area of
   // interest, one can continue with the Z reconstruction, refining the 
   // analysis.
   TH1F RoughHist=TH1F("Rough HistZ","Z",fNBinsRoughHist,-82.3,82.3);
   TH1F FineHist=TH1F("Fine HistZ","Z",fNBinsFineHist,-82.3,82.3);

   // Loop over the TClonesArrays and fill the Histograms.
   for(Int_t v=0;v<fEntriesLTwo;v++) {
      fAnaHitScnd=(Hit*)fHitClonesArray_SL->At(v);
      for(Int_t j=0;j<fEntriesLOne;j++) {
         fAnaHitFrst=(Hit*)fHitClonesArray_FL->At(j);
         // Tolerance fixed to an integer multiple of Δϕ.
         if(Punto::GetDeltaPhi(*fAnaHitFrst,*fAnaHitScnd)<=fDeltaPhi) {
            Double_t ZetaRecon=ZEvaluation(*fAnaHitFrst,*fAnaHitScnd);
            if(TMath::Abs(ZetaRecon)<=82.3) {
               RoughHist.Fill(ZetaRecon);
               FineHist.Fill(ZetaRecon);
            }
         }
      }
   }
   fDebugZetaGenerated->Fill(fAnaVertex->GetPuntoZ());
   // If any problem, edit XtrapolateZeta definition.
   XtrapolateZeta(RoughHist,FineHist,fZetaFound);
   fResidualZetaptr->Fill(fZetaFound.fCoord-fAnaVertex->GetPuntoZ());
   if(fZetaFound.fGood) fRecZetaHistptr->Fill(fZetaFound.fCoord);
   // Performance analysis can start here.
   // Fill Ntuple.
   Double_t Truthflag=-1;
   if(fZetaFound.fGood) Truthflag=1.;
   fResultsNtuple->Fill(fAnaVertex->GetPuntoZ(),    // Z Montecarlo.
      fZetaFound.fCoord,                            // Z Reconstructed.
      Truthflag,                                    // Is a good reconstruction.
      fAnaVertex->GetPuntoZ()-fZetaFound.fCoord,    // Zm-Zr -> Residual.
      fAnaVertex->GetVerticeNL(),                   // Noise level.
      fAnaVertex->GetVerticeMult());                // Multiplicity.

   return kTRUE;
}

void ReconSelector::SlaveTerminate()
{
   /////////////////////////////////////////////////////////////////////////////
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate()  is called
   // on each slave server.
   Printf("SlaveTerminate() function called.");
   Printf("fReconVertices value: %d",fReconVertices);
   Printf("Recon counter value: %d",fReconVCounter);
}

void ReconSelector::Terminate()
{
   if(fOutput->IsEmpty()) Printf("[debug >[WARNING!] TList is empty!");
   // Otherwise retrieve Ntuple.
   // fResultsNtuple=static_cast<TNtuple*>(fOutput->FindObject(fResultsNtuple));
   // Analysis
   // Residuals vs Noise.
   TFile OutNtuple("Ntupla.root","RECREATE");
   if(OutNtuple.IsZombie()) {
      Printf("A problem occured creating file");
   }
   fResultsNtuple->Write();
   OutNtuple.Close();  

   // Save Debug Histos. >>! REMOVE !<<
   
   TFile outfile("Debug.root","RECREATE");
   if(outfile.IsZombie()) {
      Printf("A problem occured creating file");
   }
   fRecZetaHistptr->Write();
   fResidualZetaptr->Write();
   fDebugZetaGenerated->Write();
   outfile.Close();
}

/******************************************************************************/
/*                           Functions definition                             */
/******************************************************************************/

// Retrieve track(let)s intersections with Z axis.
Double_t ZEvaluation(Hit &OnFirstlayer, Hit &OnSecondlayer)
{
   Double_t result=(OnFirstlayer.GetPuntoZ()
      *OnSecondlayer.GetPuntoCRadius()-OnSecondlayer.GetPuntoZ()*
      OnFirstlayer.GetPuntoCRadius())/(OnSecondlayer.GetPuntoCRadius()
      -OnFirstlayer.GetPuntoCRadius());

   return result;
}

void XtrapolateZeta(TH1F &Rough,TH1F &Fine,ZReal_t &ZetaFound)
{
   // ATM I'm not sure if it's useful that ZetaFound struct is a
   // datamaber.
   ZetaFound.fCoord=0.;
   ZetaFound.fError=0.;
   ZetaFound.fGood=kFALSE;

   // Find 1st Maximum (bin) in Rough, starting from the left margin.
   Double_t FirstMaxRough=Rough.GetMaximum();
   
   // In case of empty histograms return.
   if(FirstMaxRough==0) {return;}

   Int_t NBinsRough=Rough.GetNbinsX();
   Int_t NBinsFine=Fine.GetNbinsX();
   Double_t BinSizeRoughHist=Rough.GetBinWidth(2);
   Double_t BinSizeFineHist=Fine.GetBinWidth(2);


   Int_t FirstMaxRoughBin=Rough.GetMaximumBin();
   Double_t SecondMaxRough=0;
   Int_t SecondMaxRoughBin=0;
   Int_t Iterator=NBinsRough;

   // Find 2nd Maximum (bin) in Rough, starting from the right margin.
   do {
      SecondMaxRough=Rough.GetBinContent(Iterator);
      SecondMaxRoughBin=Iterator;
      Iterator--;
   } while(SecondMaxRough<FirstMaxRough);

   /////////////////////////////////////////////////////////////////////////////
   // From the comparison between two found maximum positions one cane make
   // some choices:
   // In this particulare case we ask that the two maximums can be the same or,
   // at most, be adjacent.
   
   if(SecondMaxRoughBin-FirstMaxRoughBin<2) {
      // Shrink the refined histogram range of interest. Add arbitrarily large
      // margins.
      Int_t OffsetLeft=0;
      Int_t OffsetRight=0;
      if(Rough.GetBinContent(FirstMaxRoughBin-1)!=0) OffsetLeft=-1;
      if(Rough.GetBinContent(SecondMaxRoughBin+1)!=0) OffsetRight=1;
      Double_t LowLimit=Rough.GetBinLowEdge(FirstMaxRoughBin+OffsetLeft);
      Double_t UpLimit=Rough.GetBinLowEdge(SecondMaxRoughBin+OffsetRight)
         +BinSizeRoughHist;
        
      Fine.SetAxisRange(LowLimit,UpLimit);
      // Henceforth the procedure is similar to the one adopted on raw data,
      // but with a different tolerance range.
      Double_t FirstMaxFine=Fine.GetMaximum();
      if(FirstMaxFine==0) {return;}
      Int_t FirstMaxFineBin=Fine.GetMaximumBin();
      Double_t SecondMaxFine=0;
      Int_t SecondMaxFineBin=0;
      Int_t Iterator2=NBinsFine;
      // Find 2nd Maximum (bin) in Fine, starting from the right.
      do {
         SecondMaxFine=Fine.GetBinContent(Iterator2);
         SecondMaxFineBin=Iterator2;
         Iterator2--;
      } while(SecondMaxFine<FirstMaxFine||
         Fine.GetBinLowEdge(SecondMaxFineBin)>UpLimit);
      Fine.SetAxisRange(Fine.GetBinLowEdge(FirstMaxFineBin),
      Fine.GetBinLowEdge(SecondMaxFineBin)+BinSizeFineHist);
      ZetaFound.fCoord=Fine.GetMean(1);
      ZetaFound.fError=Fine.GetMeanError(1);
      ZetaFound.fGood=kTRUE;
   }
}
