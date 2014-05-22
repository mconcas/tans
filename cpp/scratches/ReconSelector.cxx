#define ReconSelector_cxx
#include "ReconSelector.h"
#include <TGraphErrors.h>
#include <TCut.h>
#include <TF1.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TMath.h>
#include <TNtuple.h>
#include <TSystem.h>
// #include <TCanvas.h>

// Δϕ=0.001866[deg]
// kTRUE=1
// kFALSE=0
// Utility functions prototypes.
//

Double_t ZEvaluation(Hit &OnFirstlayer, Hit &OnSecondlayer);
void XtrapolateZeta(TH1F &Rough,TH1F &Fine,ZReal_t &ZetaFound);
Double_t ErrEfficiency(Double_t Eff,Int_t Nz);

ReconSelector::ReconSelector(TTree *) :
   fChain(0),
   fRecZetaHistptr(0),
   fResidualZetaptr(0),
   fReconVertices(0),
   // Precision settings.
   fDeltaPhi(0.001866),
   fBinSizeRoughHist(2.),             // mm
   fBinSizeFineHist(0.5)            // mm
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
   Printf("\x1B[31m\n\t++ +++ ++ ++ ++++ +++ ++ +++ +++ ++\x1B[0m");
   Printf("\x1B[31m\t+     Reconstruction Selector     +\x1B[0m");
   Printf("\x1B[31m\t+ ++ +++ + + ++ ++ +++ + ++ ++ ++ +\x1B[0m\n\n");

   fCounter=0;
   fReconVCounter=0;
}

void ReconSelector::SlaveBegin(TTree *)
{
   fNBinsRoughHist=(Int_t)(164.6/fBinSizeRoughHist+1);
   fNBinsFineHist=(Int_t)(164.6/fBinSizeFineHist+1);
   fRecZetaHistptr=new TH1F("Reconstructed Zeta","Z Recons",
      fNBinsFineHist,-82.3,82.3);
   fResidualZetaptr=new TH1F("Z Residuals","Residuals",500,-5.5,5.5);
   fResultsNtuple=new TNtuple("ResNtuple","Results",
      "ZTruth:TruthGoodness:ZRecon:ReconGoodness:ZResidual:Noise");
   // Add to Output list.
   fOutput->Add(fResidualZetaptr);
   fOutput->Add(fRecZetaHistptr);
   fOutput->Add(fResultsNtuple);
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

   // Obtain total number of entries. These are the loop limits.
   Int_t fEntriesLOne=fHitClonesArray_FL->GetEntries();
   Int_t fEntriesLTwo=fHitClonesArray_SL->GetEntries();
   fCounter++;
   if(fEntriesLTwo==0||fEntriesLOne==0) fCounter--; // count iterations.

   // This temporary histograms store all the Zetas deriving from tracklets.
   // The «rough» one has larger bins than the «fine» one.
   // We find the cluster in the first histogram. Once localized the area of
   // interest, one can continue with the Z reconstruction, refining data.
   TH1F RoughHist=TH1F("Rough HistZ","Z",fNBinsRoughHist,-82.3,82.3);
   TH1F FineHist=TH1F("Fine HistZ","Z",fNBinsFineHist,-82.3,82.3);

   // Loop over the TClonesArrays and fill the Histograms.
   for(Int_t v=0;v<fEntriesLTwo;v++) {
      fAnaHitScnd=(Hit*)fHitClonesArray_SL->At(v);
      // Smearing is applied in reading phase.
      fAnaHitScnd->GausSmearing(70,0.12,0.03);
      for(Int_t j=0;j<fEntriesLOne;j++) {
         fAnaHitFrst=(Hit*)fHitClonesArray_FL->At(j);
         // Smearing is applied in reading phase.
         fAnaHitFrst->GausSmearing(40,0.12,0.03);
         // Tolerance fixed to an integer multiple of Δϕ.
         if(Punto::GetDeltaPhi(*fAnaHitFrst,*fAnaHitScnd)<=3*fDeltaPhi) {
            Double_t ZetaRecon=ZEvaluation(*fAnaHitFrst,*fAnaHitScnd);
            if(TMath::Abs(ZetaRecon)<=82.3) {
               RoughHist.Fill(ZetaRecon);
               FineHist.Fill(ZetaRecon);
            }
         }
      }
   }

   // If any problem edit XtrapolateZeta definition.
   XtrapolateZeta(RoughHist,FineHist,fZetaFound);

   // Performance analysis can start here.
   // Fill Ntuple.
   fResultsNtuple->Fill(fAnaVertex->GetPuntoZ(), // Z Montecarlo.
      (Int_t)fAnaVertex->GetVerticeGoodness(),  // Is a good event.
      fZetaFound.fCoord,                         // Z Reconstructed.
      (Int_t)fZetaFound.fGood,                   // Is a good reconstruction.
      fAnaVertex->GetPuntoZ()-fZetaFound.fCoord, // Zm-Zr -> Residual.
      fAnaVertex->GetVerticeNL());               // Noise level.

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
   Printf("Counter value: %d",fCounter);
   Printf("Recon counter value: %d",fReconVCounter);

}

void ReconSelector::Terminate()
{
   if(fOutput->IsEmpty()) Printf("[debug >[WARNING!] TList is empty!");
   // Otherwise retrieve Ntuple.
   fResultsNtuple=static_cast<TNtuple*>(fOutput->FindObject(fResultsNtuple));
   // Analysis
   Double_t Xarray[6];
   Double_t ErrXarray[6]={0,0,0,0,0,0};
   Double_t ResArray[6];
   Double_t ErrResArray[6];
   Double_t EfficiencyArray[6];
   Double_t ErrEfficiencyArray[6];
   for(Int_t i=0;i<6;++i) {
      TH1F ResidualHist("residual","Residual Z",100,-1.5,1.5);
      TH1I HistNVertWithNL("vertices","vertices",2,0,2);
      TH1I HistNVertWithNLAndGood("goodvertices","vertices",2,0,2);
      TString CutResFormula;
      TString CutEffFormula1;
      TString CutEffFormula2;
      CutResFormula.Form("TruthGoodness==1&&ReconGoodness==1&&Noise==%d",i*6);
      CutEffFormula1.Form("TruthGoodness==1&&Noise==%d",i*6);
      CutEffFormula2.Form("TruthGoodness==1&&ReconGoodness==1&&Noise==%d",i*6);
      TCut Cut(CutResFormula.Data());
      TCut Cut1(CutEffFormula1.Data());
      TCut Cut2(CutEffFormula2.Data());
      // Fill histogram with cut data.
      fResultsNtuple->Draw("(ZTruth-ZRecon)>>residual",Cut);
      fResultsNtuple->Draw("(TruthGoodness)>>vertices",Cut1);
      fResultsNtuple->Draw("(ReconGoodness)>>goodvertices",Cut2);
      // Fit
      ResidualHist.Fit("gaus");
      TF1 *FittedGaus=(TF1*)ResidualHist.GetFunction("gaus");
      Xarray[i]=i*6;
      ResArray[i]=FittedGaus->GetParameter(2);
      ErrResArray[i]=FittedGaus->GetParError(2);
      EfficiencyArray[i]=(Float_t)HistNVertWithNLAndGood.GetMaximum()/
         HistNVertWithNL.GetMaximum();
      ErrEfficiencyArray[i]=ErrEfficiency(EfficiencyArray[i],
         HistNVertWithNL.GetMaximum());
   }
   TGraphErrors ResidualsGraphErrors(6,Xarray,ResArray,ErrXarray,ErrResArray);
   TGraphErrors EfficiencyGraphErrors(6,Xarray,EfficiencyArray,ErrXarray,
      ErrEfficiencyArray);

   TFile outfile("Analysis.root","RECREATE");
      if(outfile.IsZombie()) {
      Printf("A problem occured creating file");
   }
   ResidualsGraphErrors.Write();
   EfficiencyGraphErrors.Write();
}

/******************************************************************************/
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
   // At the moment I'm not sure if it's useful that ZetaFound struct is a
   // datamaber.
   ZetaFound.fCoord=0.;
   ZetaFound.fError=0.;
   ZetaFound.fGood=kFALSE;
   Int_t NBinsRough=Rough.GetNbinsX();
   Int_t NBinsFine=Fine.GetNbinsX();
   Double_t BinSizeRoughHist=Rough.GetBinWidth(2);
   Double_t BinSizeFineHist=Fine.GetBinWidth(2);

   // Find 1st Maximum (bin) in Rough, starting from the left margin.
   Double_t FirstMaxRough=Rough.GetMaximum();
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
      Double_t LowLimit=Rough.GetBinLowEdge(FirstMaxRoughBin)-0.4;
      Double_t UpLimit=Rough.GetBinLowEdge(SecondMaxRoughBin)
         +BinSizeRoughHist+0.4;
      Fine.SetAxisRange(LowLimit,UpLimit);
      // Henceforth the procedure is similar to the one adopted on raw data,
      // but with a different tolerance range.
      Double_t FirstMaxFine=Fine.GetMaximum();
      Int_t FirstMaxFineBin=Fine.GetMaximumBin();
      Double_t SecondMaxFine=0;
      Int_t SecondMaxFineBin=0;
      Int_t Iterator2=NBinsFine;
      // Find 2nd Maximum (bin) in Fine, starting from the right.
      do {
         SecondMaxFine=Fine.GetBinContent(Iterator2);
         SecondMaxFineBin=Iterator2;
         Iterator2--;
      } while(SecondMaxFine<FirstMaxFine ||
         Fine.GetBinLowEdge(SecondMaxFineBin)>=UpLimit);

      // It ensures that the maximum is found in the defined range of interest
      // arbitrarily defined and hardcoded.
      if(SecondMaxFineBin-FirstMaxFineBin<=10) {
         ZetaFound.fCoord=Fine.GetBinLowEdge(FirstMaxFineBin)
            +(Fine.GetBinLowEdge(SecondMaxFineBin)+BinSizeFineHist-
               Fine.GetBinLowEdge(FirstMaxFineBin))/2;
         ZetaFound.fError=(Fine.GetBinLowEdge(SecondMaxFineBin)+
            BinSizeFineHist-Fine.GetBinLowEdge(FirstMaxFineBin))/2;
         ZetaFound.fGood=kTRUE;
      }
   }
}

Double_t ErrEfficiency(Double_t Eff,Int_t Nz)
{
   return TMath::Sqrt(Eff*(1-Eff)/Nz);
};