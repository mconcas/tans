#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "Punto.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TString.h"
#define nofiles 50
#define nevents 100000
// #define molt 5


void TigeneroiTree(Bool_t moreRandom = kFALSE, const char *outfn = NULL)
{

  TFile hfile(outfn?outfn:"htree.root","RECREATE");
  TTree *tree = new TTree("T","TTree con 2 branches");
  if (moreRandom) gRandom->SetSeed( time(NULL) );

  TClonesArray *ptrhits = new TClonesArray("Punto",nevents);
  TClonesArray &hits = *ptrhits;

  Punto vertmult;

  tree->Branch("Hits",&ptrhits);
  tree->Branch("VertMult", &vertmult);
      
  for(Int_t i=0; i<nevents;i++){ // loop sugli eventi
      
      Int_t molt = gRandom->Integer(100)+1;

      vertmult.Set_Punto_X( gRandom->Gaus(0.,0.01) );
      vertmult.Set_Punto_Y( gRandom->Gaus(0.,0.01) );
      vertmult.Set_Punto_Z( gRandom->Gaus(0.,5.3)  );
      vertmult.Set_Punto_M(molt);

      for (Int_t j=0; j<molt; j++){
          // Genero un hit in modo del tutto random (dummy)
          new(hits[j]) Punto(-5.+gRandom->Rndm()*10.,5.+gRandom->Rndm()*10,15.+gRandom->Rndm()*10.,0);
      }

      //#ifeq 0
      // // Debug
      //Printf("Evento %d - moltepl: %d",i,molt);
      //Punto *point=(Punto*)ptrvertmult->At(i);
      // Printf("x= %f ; y= %f; z= %f ",point->GetX(),point->GetY(),point->GetZ());
      //point->~Punto();
      //Printf("Entries nel TClonesArray \"ptrhits\": %d",ptrhits->GetEntries());
      // Printf("Entries nel TClonesArray \"ptrvertmult\": %d\n",ptrvertmult->GetEntries());
      // for (Int_t j=0; j<molt; j++){
	        // Punto *tst=(Punto*)ptrhits->At(j);
	        // cout<<"Punto "<<j<<") x, y, z = "<<tst->GetX()<<"; "<<tst->GetY()<<"; "<<tst->GetZ()<<endl;
          // tst->~Punto();
      // }
      // Printf("");
      // fine del debug
      //#endif

      tree->Fill();
      ptrhits->Clear();
  }
  
  hfile.Write();
  hfile.Close();

}

void GeneraTree(Int_t n=nofiles) {
  TString buf;
  for (Int_t i=0;i<n;++i){
    buf.Form("htree/htree_%03d.root", i);
    TigeneroiTree(kTRUE, buf.Data());
  }
}

