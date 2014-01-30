//////////////////////////////////////////////////////////////////////
//This macro simulates p-p collisions.
//Written by Matteo Concas: mett1990@gmail.com 
//Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Hit.h"
#include "Vertice.h"
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>-
#include <TMath.h>
#include <TProof.h>
#include <TRandom3.h>
#include <TSystem.h>
#include <TTree.h>
#include <TNtuple.h> 
#endif

void Simulation(Int_t debug=0, const Int_t nVertices=10000, 
               const TString fInData="kinem.root",
               const TString fMulDist="hmul",
               const TString fEtadist="heta",
               const TString fVerOutName="vertices.root",
               const TString fHitsOutName="simulationhits.root") {
   // Set gDebug level
   gDebug=debug; 

   ///////////////////////////////////////////////////////////////////
   // Create output vertices file. 
   TFile voutfile(fVerOutName.Data(),"RECREATE");
   if (voutfile.IsZombie()) Printf("There was a problem creating %s \
                                 file.", fVerOutName.Data());
   ///////////////////////////////////////////////////////////////////
   // Create output hits file. 
   TFile houtfile(fHitsOutName.Data(),"RECREATE");
   if (houtfile.IsZombie()) Printf("There was a problem creating %s \
                                 file.", fHitsOutName.Data());
   
   // Refresh gRandom.
   if (gRandom) delete gRandom;
   gRandom=new TRandom3( time( NULL ) );
   
      ///////////////////////////////////////////////////////////////////
      // Read input data and store them in new histograms.
      TFile infile(fInData.Data(),"READ");
      TH1F *hisMultptr=static_cast<TH1F*>(infile.Get(fMulDist.Data()));
   voutfile.cd();

   // Define containters.
   TTree *EventTree=new TTree("Events", "TTree with generated \
      events");

   // Vertex instance.
   Vertice vertex;

   // Create a Branch refererring it to the vertex instance.
   EventTree->Branch("Vertices", &vertex);
   
   // Vertex indexing.
   TString vName;

   // Loop on the events
   for(Int_t i=0;i<nVertices;++i) {
      // Baptise vertex.
      vName.Form("Vertice n°%d",i);
      vertex.SetPuntoName(vName.Data());
      
      // Generate random position and multiplicity.
      vertex.SetPuntoX(gRandom->Gaus(0,0.1));
      vertex.SetPuntoY(gRandom->Gaus(0,0.1));
      vertex.SetPuntoZ(gRandom->Gaus(0,53));
      vertex.SetVerticeMult( static_cast<Int_t>(
         hisMultptr->GetRandom()) );

      if (gDebug) {
         Printf("Vertex Name: %s", vertex.GetPuntoName().Data());
         Printf("Vertex Mult: %d", vertex.GetVerticeMult());
         Printf("Vertex X: %f", vertex.GetPuntoX());
         Printf("Vertex Y: %f", vertex.GetPuntoY());
         Printf("Vertex Z: %f", vertex.GetPuntoZ());         
      }

      // Put data into an array.
      Float_t values[3];
      values[0]=vertex.GetPuntoX();
      values[1]=vertex.GetPuntoY();
      values[2]=vertex.GetPuntoZ();
      Int_t multirray=vertex.GetVerticeMult();
      // values[3]=vertex.GetVerticeMult();

      // Fill containers.
      EventTree->Fill();
      VerticesTuple->Fill(values);
      VerticesTuple->Fill(multirray);
   }
   if (gDebug) { 
      // EventTree->Print();
      VerticesTuple->Print();
   }
   Finalize event file.
   EventTree->Write();
   voutfile.Closhe();

}