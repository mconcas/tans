#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Hit.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TH3F.h"
#include "TMath.h"
// #include "Vertice.h"
// #include "Hit.h"
#define nevent 400
#endif

void RandomizeHit(Int_t seed=4321) {
   gRandom->SetSeed( seed );
   /*Do a span of theta and phi in their dominions.*/
   TH3F* histptr=new TH3F("Hist_ptr","Histogram",nevent, -0.5, 0.5, 
      nevent, -0.5, 0.5, nevent, -10., 10.);
   // Generate a random Vertex
   Vertice v("Vertex_0", gRandom->Gaus(0,0.1),gRandom->Gaus(0,0.1),
            gRandom->Gaus(0,530),0); // Mult is set to zero due to the
                                     // different way to iteration.
   if (gDebug) Printf("Vertex generated.");
   Int_t array_index=0; 
   /*Loop on "events"*/
   for(Int_t i=0;i<20;++i) { 
      if (gDebug) Printf("Entered first loop."); // Loop on Theta
      for(Int_t j=0;j<20;++j) {  // Loop on Phi
         if (gDebug) Printf("Entered second loop.");
         Hit *hitptr=Hit::HitOnCylFromVertex(v, i*TMath::Pi()/20,
            i*TMath::Pi()/10, 30., 1);
         if (gDebug) Printf("Created Hit n°: %d ",array_index);
         histptr->Fill(hitptr->GetPuntoX(),hitptr->GetPuntoY(),
            hitptr->GetPuntoZ());
         delete hitptr;
         array_index++;
      }
   }
   histptr->Draw();
} 