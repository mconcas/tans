#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Hit.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "TH3F.h"
#include "TMath.h"
// #include "Vertice.h"
// #include "Hit.h"
#define nevent 300
#define nhits 25
#endif

void RandomizeHit(Int_t seed=4326) {
   gRandom->SetSeed( seed );
   /*Do a span of theta and phi in their dominions.*/
   TH3F* histptr=new TH3F("Hist_ptr","Histogram",nevent, -50., 50., 
      nevent, -50., 50., nevent, -1640., 1640.);
   // Generate a random Vertex
   Vertice v("Vertex_0", gRandom->Gaus(0,0.1),gRandom->Gaus(0,0.1),
            gRandom->Gaus(0,530),0); // Mult is set to zero due to the
                                     // different way to iteration.
   if (gDebug) Printf("Vertex generated.");
   Int_t array_index=0;
   Double_t threesome[3]; 
   /*Loop on "events"*/
   for(Int_t i=0;i<100;++i) { 
      if (gDebug) Printf("Entered first loop."); // Loop on Theta
      for(Int_t j=0;j<8;++j) {  // Loop on Phi
         if (gDebug) Printf("Entered second loop.");
         Hit *hitptr=Hit::HitOnCylFromVertex(v,
            gRandom->Rndm()*TMath::Pi(), 
            2*TMath::Pi()*j/8, 30., 1);
         if (gDebug) Printf("Created Hit n°: %d ",array_index);
         threesome[0]=hitptr->GetPuntoX();                  
         threesome[1]=hitptr->GetPuntoY();         
         threesome[2]=hitptr->GetPuntoZ();
         if(gDebug) {
            Printf("X Coordinate for Punto n°%d is: %lf",
               array_index, threesome[0]);
            Printf("Y Coordinate for Punto n°%d is: %lf",
               array_index, threesome[1]);
            Printf("Z Coordinate for Punto n°%d is: %lf",
               array_index, threesome[2]);
         }
         /*Detectors length is 16.40cm. */
         if (TMath::Abs(threesome[2])<=820) 
            histptr->Fill(threesome[0], threesome[1],threesome[2]);
         delete hitptr;
         array_index++;
      }
   }
   histptr->Draw();
} 