#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TNtuple.h>
#include <TCut.h>
#include <TSystem.h>
#include <TF1.h>
#include <TH1I.h>
#include <TH1D.h>
#include <TH1F.h> 
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TFile.h>
#endif

/******************************************************************************/
/******************************************************************************/
/**                          Analysis Macro                                  **/
/******************************************************************************/
/******************************************************************************/

void CustomizeGraph(TGraph &Graph);
TGraphErrors ResidualVsNoise(TNtuple* Ntuple, Int_t ArrDim);
TGraphAsymmErrors EfficiencyVsNoise(TNtuple* Ntuple, Int_t ArrDim);
TGraphErrors ResidualVsCoordZ(TNtuple* Ntuple, Int_t ArrDim);
TGraphAsymmErrors EfficiencyVsCoordZ(TNtuple* Ntuple, Int_t Bins);
TGraphErrors ResidualVsMultiplicity(TNtuple* Ntuple, Int_t MultMax);
TGraphAsymmErrors EfficiencyVsMultiplicity(TNtuple* Ntuple, Int_t Bins);

// This function produce an output file with results 

void Analysis2(const TString InFileName="Ntupla.root", 
   const TString OutFileName="") 
{
   TFile InFile(InFileName.Data(),"READ");
   if(InFile.IsZombie()) {
      Printf("[ERROR!] File %s zombie, returning...",InFileName.Data());
      return;
   }  

   TNtuple *Ntuple=(TNtuple*)InFile.Get("ResNtuple");
   // Residual vs Noise.
   TGraphErrors ResidualVsNoiseGraph=ResidualVsNoise(Ntuple,6);
   // Efficiency vs Noise.
   TGraphAsymmErrors EfficiencyVsNoiseGraph=EfficiencyVsNoise(Ntuple,6); 
   // Residual vs Z position.
   TGraphErrors ResidualVsCoordZGraph=ResidualVsCoordZ(Ntuple,7);
   // Efficiency vs Z position.
   TGraphAsymmErrors EfficiencyVsCoordZGraph=EfficiencyVsCoordZ(Ntuple,9);
   // Residual vs Multiplicity.
   TGraphErrors ResidualVsMultiplicityGraph=ResidualVsMultiplicity(Ntuple,52);
   // Efficiency vs Multiplicity.
   TGraphAsymmErrors EfficiencyVsMultiplicityGraph=EfficiencyVsMultiplicity(
      Ntuple,52);

   // Save data.
   TFile outfile(OutFileName.Data(),"RECREATE");
   if(outfile.IsZombie()) {
      Printf("A problem occured creating file");
   }

   ResidualVsNoiseGraph.Write();
   EfficiencyVsNoiseGraph.Write();
   ResidualVsCoordZGraph.Write();
   EfficiencyVsCoordZGraph.Write();
   ResidualVsMultiplicityGraph.Write();
   EfficiencyVsMultiplicityGraph.Write();
}

void CustomizeGraph(TGraph &Graph) 
{
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
   Printf("\x1B[32m Evaluating Residual vs Noise...\x1B[0m");
   Double_t Noise[ArrDim];
   Double_t ErrNoise[ArrDim];
   for(Int_t i=0;i<ArrDim;++i) ErrNoise[i]=0.;
   Double_t Residual[ArrDim];
   Double_t ErrResidual[ArrDim];
   TF1 *FitGaus=new TF1();
   for(Int_t i=0;i<ArrDim;++i) {
      Noise[i]=i*ArrDim;
      TH1F ResidualHist("residual","Residual Z",3000,-0.15,0.15);
      TString Formula;
      Formula.Form("ReconGood==1&&Noise==%d&&Multiplicity>15",i*ArrDim);
      TCut Cut(Formula.Data());
      Ntuple->Draw("(ZResidual)>>residual",Cut,"goff");
      ResidualHist.Fit("gaus","Q");
      FitGaus=(TF1*) ResidualHist.GetFunction("gaus");
      Residual[i]=FitGaus->GetParameter(2);
      ErrResidual[i]=FitGaus->GetParError(2);      
   }
   TGraphErrors ResidualVsNoise(ArrDim,Noise,Residual,ErrNoise,ErrResidual);
   ResidualVsNoise.SetNameTitle("ResidualsVsNoise","Residuals vs Noise");
   ResidualVsNoise.GetXaxis()->SetTitle("Noise lvl");
   ResidualVsNoise.GetYaxis()->SetTitle("Residuals (cm)");
   CustomizeGraph(ResidualVsNoise);

   return ResidualVsNoise;
}

//------------------------------------------------------------------------------
//                      EFFICIENCY VS NOISE
//------------------------------------------------------------------------------
TGraphAsymmErrors EfficiencyVsNoise(TNtuple *Ntuple, Int_t Bins)
{
   Printf("\x1B[32m Evaluating Efficiency vs Noise...\x1B[0m");
   TH1I verticesimulated("verticesimulated","vertices",Bins,0,31);
   TH1I verticesreconstructed("verticesreconstructed","vertices",Bins,0,31);
   Ntuple->Draw("(Noise)>>verticesimulated","","goff");
   Ntuple->Draw("(Noise)>>verticesreconstructed","ReconGood==1","goff");
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
TGraphErrors ResidualVsCoordZ(TNtuple *Ntuple, Int_t ArrDim) 
{
   Printf("\x1B[32m Evaluating Residual vs Z position...\x1B[0m");
   Double_t ZCoord[7]={-6.99,-4.6,-2.3,0,2.3,4.6,6.99};
   Double_t ErrZCoord[ArrDim];
   for(Int_t i=0;i<ArrDim;++i) ErrZCoord[i]=0.;
   Double_t RangeLimits[8]={-8.23,-5.75,-3.45,-1.15,1.15,3.45,5.75,8.23};
   Double_t Residual[ArrDim];
   Double_t ErrResidual[ArrDim];
   TF1 *FitGaus=new TF1();
   for(Int_t j=0;j<ArrDim;++j) {
      TH1F ResidualHist("residual","Residual Z",3000,-0.05,0.05);
      TString Formula;
      Formula.Form("(ZRecon>%f&&ZTruth>%f)&&(ZRecon<=%f&&ZTruth<=%f)",
         RangeLimits[j],RangeLimits[j],RangeLimits[j+1],RangeLimits[j+1]);
      TCut Cut(Formula.Data());
      Ntuple->Draw("(ZResidual)>>residual",Cut,"goff");
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
   ResidualVsCoordZ.GetXaxis()->SetTitle("Z coord (cm)");
   ResidualVsCoordZ.GetYaxis()->SetTitle("Residuals (cm)");
   CustomizeGraph(ResidualVsCoordZ);

   return ResidualVsCoordZ;
}

//------------------------------------------------------------------------------
//                      EFFICIENCY VS COORDINATE Z
//------------------------------------------------------------------------------
TGraphAsymmErrors EfficiencyVsCoordZ(TNtuple* Ntuple, Int_t Bins) 
{
   Printf("\x1B[32m Evaluating Efficiency vs Z position...\x1B[0m");
   TH1D verticesimulated("verticesimulated","vertices",Bins,-8.23,
      8.23);
   TH1D verticesreconstructed("verticesreconstructed","vertices",Bins,
      -8.23,8.23);
   Ntuple->Draw("ZTruth>>verticesimulated","","goff");
   Ntuple->Draw("ZRecon>>verticesreconstructed",
      "ReconGood==1","goff");
   TGraphAsymmErrors EfficiencyVsCoordZ(&verticesreconstructed,
      &verticesimulated);
   EfficiencyVsCoordZ.SetNameTitle("EfficiencyVsZ","Efficiency vs Z coord");
   EfficiencyVsCoordZ.GetXaxis()->SetTitle("Z coord (cm)");
   EfficiencyVsCoordZ.GetYaxis()->SetTitle("Efficiency");
   CustomizeGraph(EfficiencyVsCoordZ);
   
   return EfficiencyVsCoordZ;
}

//------------------------------------------------------------------------------
//                      RESIDUAL VS MULTIPLICITY
//------------------------------------------------------------------------------
// The last classes are take larger because of the less number of entries.

TGraphErrors ResidualVsMultiplicity(TNtuple *Ntuple, Int_t MultMax) 
{
   Printf("\x1B[32m Evaluating Residual vs Multiplicity...\x1B[0m");
   Int_t ArrDim=MultMax-9;
   Double_t Multiplicity[ArrDim];
   Double_t ErrMultiplicity[ArrDim];
   for(Int_t i=0;i<ArrDim;i++) { 
      Multiplicity[i]=i+2;
      ErrMultiplicity[i]=0.;
   }
   for(Int_t i=0;i<4;++i){
      Multiplicity[ArrDim-i-1]=MultMax-i*3-1;
   }
   Double_t Residual[ArrDim];
   Double_t ErrResidual[ArrDim];
   // TF1 *FitGaus=new TF1();

   // j iterator now loops over multiplicity values.
   for(Int_t j=2;j<=MultMax-12;j++) {
      TH1F ResidualHist("residual","Residual Z",300,-0.15,0.15);
      TString Formula;
      Formula.Form("(ReconGood==1)&&(Multiplicity==%d)",j);
      TCut Cut(Formula.Data());
      Ntuple->Draw("(ZResidual)>>residual",Cut,"goff");
      /*ResidualHist.Fit("gaus","Q");
      FitGaus=(TF1*)ResidualHist.GetFunction("gaus");
      Residual[j-2]=FitGaus->GetParameter(2);
      ErrResidual[j-2]=FitGaus->GetParError(2);*/
      Residual[j-2]=/*FitGaus->GetParameter(2);*/ResidualHist.GetRMS();
      ErrResidual[j-2]=/*FitGaus->GetParError(2);*/ResidualHist.GetRMSError();
   }
   Int_t temp=ArrDim-4;
   for(Int_t j=MultMax-12;j<MultMax;j+=3) {
      TH1F ResidualHist("residual","Residual Z",300,-0.15,0.15);
      TString Formula;
      Formula.Form("(ReconGood==1)&&(Multiplicity>%d)&&(Multiplicity<=%d)",
         j,j+3);
      TCut Cut(Formula.Data());
      Ntuple->Draw("(ZResidual)>>residual",Cut,"goff");
      /*ResidualHist.Fit("gaus","Q");
      FitGaus=(TF1*)ResidualHist.GetFunction("gaus");
      Residual[temp]=FitGaus->GetParameter(2);
      ErrResidual[temp]=FitGaus->GetParError(2);*/
      Residual[temp]=/*FitGaus->GetParameter(2);*/ResidualHist.GetRMS();
      ErrResidual[temp]=/*FitGaus->GetParError(2);*/ResidualHist.GetRMSError();
      temp+=1;

     
   }
   TGraphErrors ResidualVsMultiplicity(ArrDim,Multiplicity,Residual,
      ErrMultiplicity,ErrResidual);
   ResidualVsMultiplicity.SetNameTitle("ResidualVsMultiplicity",
      "Residuals vs Multiplicity");
   ResidualVsMultiplicity.GetXaxis()->SetTitle("Multiplicity");
   ResidualVsMultiplicity.GetYaxis()->SetTitle("Residuals (cm)");
   CustomizeGraph(ResidualVsMultiplicity);
   return ResidualVsMultiplicity;
} 

//------------------------------------------------------------------------------
//                      EFFICIENCY VS MULTIPLICITY
//------------------------------------------------------------------------------
TGraphAsymmErrors EfficiencyVsMultiplicity(TNtuple *Ntuple, Int_t Bins) 
{
   Printf("\x1B[32m Evaluating Efficiency vs Multiplicity...\x1B[0m");
   TH1I verticesimulated("simulated","vertices",Bins,0,60);
   TH1I verticesreconstructed("reconstructed","vertices",Bins,0,60);
   Ntuple->Draw("(Multiplicity)>>simulated","","goff");
   Ntuple->Draw("(Multiplicity)>>reconstructed","ReconGood==1","goff");
   TGraphAsymmErrors EfficiencyVsMultiplicity(&verticesreconstructed,
      &verticesimulated);
   EfficiencyVsMultiplicity.SetNameTitle("EfficiencyVsMultiplicity",
      "Efficiency vs Multiplicity");
   EfficiencyVsMultiplicity.GetXaxis()->SetTitle("Multiplicity");
   EfficiencyVsMultiplicity.GetYaxis()->SetTitle("Efficiency");
   CustomizeGraph(EfficiencyVsMultiplicity);
   
   return EfficiencyVsMultiplicity;
}