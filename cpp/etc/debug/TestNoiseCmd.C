#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Direzione.h"
#include "Hit.h"
#include "Vertice.h"
#include <TClonesArray.h>
#include <TFile.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TMaterial.h>
#include <TMath.h>
#include <TSystem.h>
#include <TTree.h>
#include <TH3F.h>
#endif


void TestNoiseCmd(Int_t fNoiseLevelSec=1000) {
   TH3F *graphptr=new TH3F("Debug Noise","noisegraph",fNoiseLevelSec,
      -30,30,fNoiseLevelSec,-30,30,fNoiseLevelSec, -164.6/2, 
      164.6/2);

   for( Int_t m=0; m<fNoiseLevelSec; ++m ) {
         Hit *tNoiseHitPtrS=Hit::EleNoiseOnCyl( 20., -100, 
            100 );
      graphptr->Fill( tNoiseHitPtrS->GetPuntoX(),
                      tNoiseHitPtrS->GetPuntoY(),
                      tNoiseHitPtrS->GetPuntoZ() );
      delete tNoiseHitPtrS;
   }
   graphptr->Draw();
}