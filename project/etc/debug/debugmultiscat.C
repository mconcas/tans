#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Direzione.h"
#include "Hit.h"
#include "Direzione.h"
#include <TMaterial.h>
#include <TMath.h>
#include <TRandom3.h>
#endif

void debugmultiscat(Bool_t bubba=kTRUE) {
   gDebug=3;
   Vertice vertice("Vertice", 0.3452, 1.24234, 2.3523, 55);
   vertice.Dump();
   Direzione direzione("Dal Vertice",gRandom->Rndm()*TMath::Pi(),
      gRandom->Rndm()*TMath::Pi()*2);
   // Direzione.Dump();
   Hit *primohitptr=Hit::HitOnCylFromVertex(vertice,direzione,4.5);
   primohitptr->Dump();
   Hit primohit=*primohitptr;
   TMaterial berillio=TMaterial("Berillio","Be",8,4,1.85,35.28,0);
   Direzione direzione2("Dal Berillio",gRandom->Rndm()*TMath::Pi(),
      gRandom->Rndm()*TMath::Pi()*2);
   direzione2.Dump();
   Hit *secondohitptr=primohit.Hit::GetHitOnCyl(direzione2,7.8,
      berillio,0.8,bubba);
   secondohitptr->Dump();
   direzione2.Dump();
}