//////////////////////////////////////////////////////////////////////
//This macro simulates p-p collisions.
//Written by Matteo Concas: mett1990@gmail.com 
//Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Hit.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "TTree.h"
#include "Vertice.h"
#endif

void Vertex_generator(const Int_t nVertices=10000, 
               const TString fInData="kinem.root",
               const TString fMulDist="hmul",
               /*const TString fEtaDist="heta",*/ 
               const TString fOutData="vertices.root") {
   TFile outfile(fOutData.Data(),"RECREATE");
   if (outfile.IsZombie()) Printf("There was a problem creating %s \
                                 file.", fOutData.Data());  
   // Refresh gRandom
   if (gRandom) delete gRandom;
   gRandom=new TRandom3( time( NULL ) );

   // Read input data, then store them in new histograms.
   TFile infile(fInData.Data(),"READ");
   outfile.cd();
   TH1F *hisMultptr=static_cast<TH1F*>(infile.Get(fMulDist.Data()));
   // Containters.
   TTree *tree=new TTree("Vertices","Tree with generated vertices.");
   Vertice vertex;
   tree->Branch("Vertices", &vertex);
   TString vName;
   // loop on events
   for(Int_t i=0;i<nVertices;++i) {
      vName.Form("Vertice n°%d",i);
      vertex.SetPuntoName(vName);
      vertex.SetPuntoX(gRandom->Gaus(0,0.1));
      vertex.SetPuntoY(gRandom->Gaus(0,0.1));
      vertex.SetPuntoZ(gRandom->Gaus(0,53));
      vertex.SetVerticeMult( (Int_t)(hisMultptr->GetRandom()) );
      if (gDebug) {
         Printf("Vertex Name: %s", vertex.GetPuntoName().Data());
         Printf("Vertex Mult: %d", vertex.GetVerticeMult());
         Printf("Vertex X: %f", vertex.GetPuntoX());
         Printf("Vertex Y: %f", vertex.GetPuntoY());
         Printf("Vertex Z: %f", vertex.GetPuntoZ());         
      }
     tree->Fill();
   }
   tree->Print();
   // Finalize
   tree->Write();
   outfile.Close();
}
