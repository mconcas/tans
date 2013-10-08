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
// #include "TTree.h"
#include <TNtuple.h> 
#include "Vertice.h"
#endif

void VerticesGenerator(Int_t debug=0, const Int_t nVertices=10000, 
               const TString fInData="kinem.root",
               const TString fMulDist="hmul",
               const TString fOutData="vertices.root") {
   // Set gDebug level
   gDebug=debug; 

   
   // Create output file.
   TFile outfile(fOutData.Data(),"RECREATE");
   if (outfile.IsZombie()) Printf("There was a problem creating %s \
                                 file.", fOutData.Data());  

   // Refresh gRandom.
   if (gRandom) delete gRandom;
   gRandom=new TRandom3( time( NULL ) );

   // Read input data and store them in new histograms.
   TFile infile(fInData.Data(),"READ");
   outfile.cd();
   TH1F *hisMultptr=static_cast<TH1F*>(infile.Get(fMulDist.Data()));
   
   // Define containters.
   TNtuple *VerticesTuple=new TNtuple("Vertices","NTuple with \
      generated vertices.", "x:y:z:mult");

   // Vertex instance.
   Vertice vertex;

   // Create a Branch refererring it to the vertex instance.
   // VerticesTuple->Branch("Vertices", &vertex);
   TString vName;

   // Loop on the events
   for(Int_t i=0;i<nVertices;++i) {
      // Baptise vertex.
      vName.Form("Vertice n°%d",i);
      vertex.SetPuntoName(vName);
      
      // Generate random position and multiplicity.
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
     Float_t values[4];
     values[0]=vertex.GetPuntoX();
     values[1]=vertex.GetPuntoY();
     values[2]=vertex.GetPuntoZ();
     values[3]=vertex.GetVerticeMult();
     VerticesTuple->Fill(values);
   }
   VerticesTuple->Print();
   // Finalize
   VerticesTuple->Write();
   outfile.Close();
}
