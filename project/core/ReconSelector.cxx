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
void CenterHistRange(TH1 &Hist, Double_t Extremals[2], 
   Double_t Center, Double_t Enlargement=0.f);
void XtrapolateZeta(TH1F &Rough,TH1F &Fine,ZReal_t &ZetaFound, 
   Bool_t RecursionFlag=kFALSE);


ReconSelector::ReconSelector(TTree *) :
   fChain(0),
   fRecZetaHistptr(0),
   fResidualZetaptr(0),
   fResultsNtuple(0),
   fDebugZetaGenerated(0),
   fReconVertices(0),
   fDeltaPhi(0.005),                  // [rad]
   fBinSizeRoughHist(0.2),            // cm
   fBinSizeFineHist(0.0005)           // cm
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
   fNBinsRoughHist=(Int_t)(16.46/fBinSizeRoughHist);
   fNBinsFineHist=(Int_t)(16.46/fBinSizeFineHist);
   fRecZetaHistptr=new TH1F("Reconstructed","Zreconstructed",50000,-8.23,8.23);
   fResidualZetaptr=new TH1F("Residual Zeta","Z Residuals",50000,-8.23,8.23);
   fResultsNtuple=new TNtuple("ResNtuple","Results",
      "ZTruth:ZRecon:ReconGood:ZResidual:Noise:Multiplicity");
   fDebugZetaGenerated=new TH1F("Generated","ZGenerated",50000,-8.23,8.23);
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
   TH1F RoughHist=TH1F("Rough HistZ","Z",fNBinsRoughHist,-8.23,8.23);
   TH1F FineHist=TH1F("Fine HistZ","Z",fNBinsFineHist,-8.23,8.23);

   // Loop over the TClonesArrays and fill the Histograms.
   for(Int_t v=0;v<fEntriesLTwo;v++) {
      fAnaHitScnd=(Hit*)fHitClonesArray_SL->At(v);
      for(Int_t j=0;j<fEntriesLOne;j++) {
         fAnaHitFrst=(Hit*)fHitClonesArray_FL->At(j);
         // Tolerance fixed to an integer multiple of Δϕ.
         if(Punto::GetDeltaPhi(*fAnaHitFrst,*fAnaHitScnd)<=fDeltaPhi) {
            Double_t ZetaRecon=ZEvaluation(*fAnaHitFrst,*fAnaHitScnd);
            if(TMath::Abs(ZetaRecon)<=8.23) {
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

   gSystem->Exec("mkdir -p ./results");
   TFile OutNtuple("./results/ResultsNtuple.root","RECREATE");
   if(OutNtuple.IsZombie()) {
      Printf("A problem occured creating file");
   }
   fResultsNtuple->Write();
   OutNtuple.Close();  

   // Save Debug Histos. >>! REMOVE IN FINAL VERSION!<<
   TFile outfile("./results/Debug.root","RECREATE");
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

void CenterHistRange(TH1 &Hist, Double_t Extremals[2], Double_t Center, 
   Double_t Enlargement)
{
   // swap/order array.
   if(Extremals[0]>Extremals[1]) {
      Double_t temp=Extremals[0];
      Extremals[0]=Extremals[1];
      Extremals[1]=temp;
   }
   Double_t RangeWidth=TMath::Abs(Extremals[1]-Extremals[0])+
      Enlargement*TMath::Abs(Extremals[1]-Extremals[0]);
   Extremals[0]=Center-RangeWidth/2;
   Extremals[1]=Center+RangeWidth/2;
   Hist.SetAxisRange(Extremals[0],Extremals[1]);
}

void XtrapolateZeta(TH1F &Rough,TH1F &Fine,ZReal_t &ZetaFound, 
   Bool_t RecursionFlag)
{
   ZetaFound.fCoord=0.;
   ZetaFound.fError=0.;
   ZetaFound.fGood=kFALSE;

   // Find 1st Maximum (bin) in Rough, starting from the left margin.
   Double_t FirstMaxRough=Rough.GetMaximum();
   if(FirstMaxRough==0) {return;}
   Int_t NBinsRough=Rough.GetNbinsX();
   Double_t BinSizeRoughHist=Rough.GetBinWidth(2);
   Int_t FirstMaxRoughBin=Rough.GetMaximumBin();

   // Find 2nd Maximum (bin) in Rough, starting from the right margin.
   Double_t SecondMaxRough=0;
   Int_t SecondMaxRoughBin=0;
   Int_t Iterator=NBinsRough;
   do {
      SecondMaxRough=Rough.GetBinContent(Iterator);
      SecondMaxRoughBin=Iterator;
      Iterator--;
   } while(SecondMaxRough<FirstMaxRough);

   /////////////////////////////////////////////////////////////////////////////
   // From the comparison between two found maximum positions one cane take
   // some choices:
   // In this particulare case we ask that the two maximums can be the same or,
   // at most, be adjacent.
   
   Double_t ArrLimits[2]={Rough.GetBinLowEdge(FirstMaxRoughBin),
      Rough.GetBinLowEdge(SecondMaxRoughBin)+BinSizeRoughHist};
   TH1F* Roughptr=&Rough;
   Double_t Center=0.f;
   switch(SecondMaxRoughBin-FirstMaxRoughBin) {
      case 0:
         // Printf("case: %d",SecondMaxRoughBin-FirstMaxRoughBin);
         do {
            Fine.SetAxisRange(ArrLimits[0],ArrLimits[1]);
            ZetaFound.fCoord=Fine.GetMean(1);
            ZetaFound.fError=Fine.GetMeanError(1);
            ZetaFound.fGood=kTRUE;
            Center=ArrLimits[0]+(ArrLimits[1]-ArrLimits[0])/2;
            // Printf("<looped, fCoord=%f, Center=%f>",ZetaFound.fCoord,Center);
            CenterHistRange(Fine,ArrLimits,ZetaFound.fCoord,0.2);
         } while(TMath::Abs(ZetaFound.fCoord-Center)>=0.0001/*cm*/);
         // Printf("Out from while() loop: z=%f c=%f",ZetaFound.fCoord,Center);           
         break;
      case 1:
         // Printf("case: %d",SecondMaxRoughBin-FirstMaxRoughBin);
         do {
            Fine.SetAxisRange(ArrLimits[0],ArrLimits[1]);
            ZetaFound.fCoord=Fine.GetMean(1);
            ZetaFound.fError=Fine.GetMeanError(1);
            ZetaFound.fGood=kTRUE;
            Center=ArrLimits[0]+(ArrLimits[1]-ArrLimits[0])/2;
            // Printf("<looped, fCoord=%f, Center=%f>",ZetaFound.fCoord,Center);
            CenterHistRange(Fine,ArrLimits,ZetaFound.fCoord,0.2);
         } while(TMath::Abs(ZetaFound.fCoord-Center)>=0.0001/*cm*/);
         // Printf("Out from while() loop: z=%f c=%f",ZetaFound.fCoord,Center); 
         break;
      case 2:
         // Printf("case: %d",SecondMaxRoughBin-FirstMaxRoughBin);
         if(RecursionFlag) return;
         // Double BinSize.
         Roughptr=(TH1F*)Rough.Rebin(2);
         XtrapolateZeta(*Roughptr,Fine,ZetaFound,kTRUE);
         break;
      default:
         // Printf("case: %d",SecondMaxRoughBin-FirstMaxRoughBin);
         return;
   }
}
