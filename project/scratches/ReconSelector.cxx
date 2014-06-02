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

// Δϕ=0.001866[deg]
// kTRUE=1
// kFALSE=0
// Utility functions prototypes.
Double_t ZEvaluation(Hit &OnFirstlayer, Hit &OnSecondlayer);
void XtrapolateZeta(TH1F &Rough,TH1F &Fine,ZReal_t &ZetaFound);
void CustomizeGraph(TGraph &Graph);

// Analysis functions.
TGraphErrors ResidualVsNoise(TNtuple* Ntuple, Int_t ArrDim);
TGraphAsymmErrors EfficiencyVsNoise(TNtuple* Ntuple, Int_t ArrDim);
TGraphErrors ResidualVsCoordZ(TNtuple* Ntuple, Int_t ArrDim);
TGraphAsymmErrors EfficiencyVsCoordZ(TNtuple* Ntuple, Int_t Bins);
TGraphErrors ResidualVsMultiplicity(TNtuple* Ntuple, Int_t ArrDim);
TGraphAsymmErrors EfficiencyVsMultiplicity(TNtuple* Ntuple, Int_t Bins);

ReconSelector::ReconSelector(TTree *) :
   fChain(0),
   fRecZetaHistptr(0),
   fResidualZetaptr(0),
   fResultsNtuple(0),
   fDebugZetaGenerated(0),
   fReconVertices(0),
   // Precision settings.
   fDeltaPhi(/*0.001866*/0.005),
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
   Printf("\x1B[31m\n\t++ +++ ++ ++ ++++ +++ ++ +++ +++ ++\x1B[0m");
   Printf("\x1B[31m\t+     Reconstruction Selector     +\x1B[0m");
   Printf("\x1B[31m\t+ ++ +++ + + ++ ++ +++ + ++ ++ ++ +\x1B[0m\n\n");
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
   // TString String;
   // String.Form("RoughandFine_%lld.root",entry);
   // TFile Filetto(String.Data(),"RECREATE");
   // FineHist.Write();
   // RoughHist.Write();
   // Filetto.Close();
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
   TGraphErrors ResidualVsNoiseGraph=ResidualVsNoise(fResultsNtuple,6);
 
   // Efficiency vs Noise.
   TGraphAsymmErrors EfficiencyVsNoiseGraph=EfficiencyVsNoise(fResultsNtuple,6); 
   
   // Residual vs Z position.
   TGraphErrors ResidualVsCoordZGraph=ResidualVsCoordZ(fResultsNtuple,7);

   // Efficiency vs Z position.
   TGraphAsymmErrors EfficiencyVsCoordZGraph=EfficiencyVsCoordZ(fResultsNtuple,
      9);
    
   // Residual vs Multiplicity.
   TGraphErrors ResidualVsMultiplicityGraph=ResidualVsMultiplicity(
      fResultsNtuple,51);
  
   // Efficiency vs Multiplicity.
   TGraphAsymmErrors EfficiencyVsMultiplicityGraph=EfficiencyVsMultiplicity(
      fResultsNtuple,51);

   // Save data.
   TString Prefix="data/Noise_X_Multscatt_disabled_events_1M/analysis/";
   TFile outfile(Prefix+"Analysis_1M_No_MS_No_Noise.root","RECREATE");
   if(outfile.IsZombie()) {
      Printf("A problem occured creating file");
   }

   ResidualVsNoiseGraph.Write();
   EfficiencyVsNoiseGraph.Write();
   ResidualVsCoordZGraph.Write();
   EfficiencyVsCoordZGraph.Write();
   ResidualVsMultiplicityGraph.Write();
   EfficiencyVsMultiplicityGraph.Write();
   
   Printf("Maximum Value: %f",fRecZetaHistptr->GetMaximum());
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

void CustomizeGraph(TGraph &Graph) {
   Graph.GetXaxis()->SetLabelSize(0.03);
   Graph.GetYaxis()->SetLabelSize(0.03);
   Graph.GetYaxis()->SetTitleOffset(1.2);
   Graph.SetMarkerColor(kGreen);
   Graph.SetMarkerStyle(kCircle);
}

//------------------------------------------------------------------------------
//                      RESIDUAL VS NOISE
//------------------------------------------------------------------------------
TGraphErrors ResidualVsNoise(TNtuple *Ntuple, Int_t ArrDim)
{
   Double_t Noise[ArrDim];
   Double_t ErrNoise[ArrDim];
   for(Int_t i=0;i<ArrDim;++i) ErrNoise[i]=0.;
   Double_t Residual[ArrDim];
   Double_t ErrResidual[ArrDim];
   TF1 *FitGaus=new TF1();
   for(Int_t i=0;i<ArrDim;++i) {
      Noise[i]=i*ArrDim;
      TH1F ResidualHist("residual","Residual Z",3000,-1.5,1.5);
      TString Formula;
      Formula.Form("ReconGood==1&&Noise==%d&&Multiplicity>15",i*ArrDim);
      TCut Cut(Formula.Data());
      Ntuple->Draw("(ZResidual)>>residual",Cut);
      ResidualHist.Fit("gaus","Q");
      FitGaus=(TF1*) ResidualHist.GetFunction("gaus");
      Residual[i]=FitGaus->GetParameter(2);
      ErrResidual[i]=FitGaus->GetParError(2);      
   }
   TGraphErrors ResidualVsNoise(ArrDim,Noise,Residual,ErrNoise,ErrResidual);
   ResidualVsNoise.SetNameTitle("ResidualsVsNoise","Residuals vs Noise");
   ResidualVsNoise.GetXaxis()->SetTitle("Noise lvl");
   ResidualVsNoise.GetYaxis()->SetTitle("Residuals (mm)");
   CustomizeGraph(ResidualVsNoise);

   return ResidualVsNoise;
}

//------------------------------------------------------------------------------
//                      EFFICIENCY VS NOISE
//------------------------------------------------------------------------------
TGraphAsymmErrors EfficiencyVsNoise(TNtuple *Ntuple, Int_t Bins)
{
   TH1I verticesimulated("verticesimulated","vertices",Bins,0,31);
   TH1I verticesreconstructed("verticesreconstructed","vertices",Bins,0,31);
   Ntuple->Draw("(Noise)>>verticesimulated");
   Ntuple->Draw("(Noise)>>verticesreconstructed","ReconGood==1");
   TGraphAsymmErrors EfficiencyVsNoise(&verticesreconstructed,
      &verticesimulated);
   EfficiencyVsNoise.SetNameTitle("EfficiencyVsNoise",
      "Efficiency vs Noise");
   EfficiencyVsNoise.GetXaxis()->SetTitle("Noise level");
   EfficiencyVsNoise.GetYaxis()->SetTitle("Efficiency");
   CustomizeGraph(EfficiencyVsNoise);

   return EfficiencyVsNoise;
}

//------------------------------------------------------------------------------
//                      RESIDUAL VS COORDINATE Z
//------------------------------------------------------------------------------
TGraphErrors ResidualVsCoordZ(TNtuple *Ntuple, Int_t ArrDim) {
   Double_t ZCoord[7]={-69.9,-46,-23,0,23,46,69.9};
   Double_t ErrZCoord[ArrDim];
   for(Int_t i=0;i<ArrDim;++i) ErrZCoord[i]=0.;
   Double_t RangeLimits[8]={-82.3,-57.5,-34.5,-11.5,11.5,34.5,57.5,82.3};
   Double_t Residual[ArrDim];
   Double_t ErrResidual[ArrDim];
   TF1 *FitGaus=new TF1();
   for(Int_t j=0;j<ArrDim;++j) {
      TH1F ResidualHist("residual","Residual Z",3000,-0.5,0.5);
      TString Formula;
      Formula.Form("(ZRecon>%f&&ZTruth>%f)&&(ZRecon<=%f&&ZTruth<=%f)",
         RangeLimits[j],RangeLimits[j],RangeLimits[j+1],RangeLimits[j+1]);
      TCut Cut(Formula.Data());
      Ntuple->Draw("(ZResidual)>>residual",Cut);
      if(ResidualHist.GetMaximum()!=0) {
         ResidualHist.Fit("gaus","Q");
         FitGaus=(TF1*)ResidualHist.GetFunction("gaus");
         Residual[j]=FitGaus->GetParameter(2);
         ErrResidual[j]=FitGaus->GetParError(2);
      } else {
         Residual[j]=0.;
         ErrResidual[j]=0;
      }
   }

   TGraphErrors ResidualVsCoordZ(ArrDim,ZCoord,Residual,ErrResidual);
   ResidualVsCoordZ.SetNameTitle("ResidualsVsZ","Residuals vs Z coordinate");
   ResidualVsCoordZ.GetXaxis()->SetTitle("Z coord (mm)");
   ResidualVsCoordZ.GetYaxis()->SetTitle("Residuals (mm)");
   CustomizeGraph(ResidualVsCoordZ);

   return ResidualVsCoordZ;
}

//------------------------------------------------------------------------------
//                      EFFICIENCY VS COORDINATE Z
//------------------------------------------------------------------------------
TGraphAsymmErrors EfficiencyVsCoordZ(TNtuple* Ntuple, Int_t Bins) {
   TH1D verticesimulated("verticesimulated","vertices",Bins,-82.3,
      82.3);
   TH1D verticesreconstructed("verticesreconstructed","vertices",Bins,
      -82.3,82.3);
   Ntuple->Draw("ZTruth>>verticesimulated");
   Ntuple->Draw("ZRecon>>verticesreconstructed",
      "ReconGood==1");
   TGraphAsymmErrors EfficiencyVsCoordZ(&verticesreconstructed,
      &verticesimulated);
   EfficiencyVsCoordZ.SetNameTitle("EfficiencyVsZ","Efficiency vs Z coord");
   EfficiencyVsCoordZ.GetXaxis()->SetTitle("Z coord (mm)");
   EfficiencyVsCoordZ.GetYaxis()->SetTitle("Efficiency");
   CustomizeGraph(EfficiencyVsCoordZ);
   
   return EfficiencyVsCoordZ;
}

//------------------------------------------------------------------------------
//                      RESIDUAL VS MULTIPLICITY
//------------------------------------------------------------------------------
TGraphErrors ResidualVsMultiplicity(TNtuple *Ntuple, Int_t ArrDim) {
   Double_t Multiplicity[ArrDim];
   Double_t ErrMultiplicity[ArrDim];
   for(Int_t i=0;i<ArrDim;++i) {
      ErrMultiplicity[i]=0.;
      Multiplicity[i]=i+2;
   }
   Double_t Residual[ArrDim];
   Double_t ErrResidual[ArrDim];
   TF1 *FitGaus=new TF1();
   for(Int_t j=2;j<ArrDim+2;++j) {
      TH1F ResidualHist("residual","Residual Z",300,-1.5,1.5);
      TString Formula;
      Formula.Form("(ReconGood==1)&&(Multiplicity==%d)",j);
      TCut Cut(Formula.Data());
      Ntuple->Draw("(ZResidual)>>residual",Cut);
      Printf("------ Fit Number %d ------",j);
      ResidualHist.Fit("gaus");
      FitGaus=(TF1*)ResidualHist.GetFunction("gaus");
      Residual[j-2]=FitGaus->GetParameter(2);
      ErrResidual[j-2]=FitGaus->GetParError(2);
   }
   TGraphErrors ResidualVsMultiplicity(ArrDim,Multiplicity,Residual,
      ErrMultiplicity,ErrResidual);
   ResidualVsMultiplicity.SetNameTitle("ResidualVsMultiplicity",
      "Residuals vs Multiplicity");
   ResidualVsMultiplicity.GetXaxis()->SetTitle("Multiplicity");
   ResidualVsMultiplicity.GetYaxis()->SetTitle("Residuals (mm)");
   CustomizeGraph(ResidualVsMultiplicity);
   return ResidualVsMultiplicity;
} 

//------------------------------------------------------------------------------
//                      EFFICIENCY VS MULTIPLICITY
//------------------------------------------------------------------------------
TGraphAsymmErrors EfficiencyVsMultiplicity(TNtuple *Ntuple, Int_t Bins) {
   TH1I verticesimulated("simulated","vertices",Bins,0,60);
   TH1I verticesreconstructed("reconstructed","vertices",Bins,0,60);
   Ntuple->Draw("(Multiplicity)>>simulated");
   Ntuple->Draw("(Multiplicity)>>reconstructed","ReconGood==1");
   TGraphAsymmErrors EfficiencyVsMultiplicity(&verticesreconstructed,
      &verticesimulated);
   EfficiencyVsMultiplicity.SetNameTitle("EfficiencyVsMultiplicity",
      "Efficiency vs Multiplicity");
   EfficiencyVsMultiplicity.GetXaxis()->SetTitle("Multiplicity");
   EfficiencyVsMultiplicity.GetYaxis()->SetTitle("Efficiency");
   CustomizeGraph(EfficiencyVsMultiplicity);
   
   return EfficiencyVsMultiplicity;
}