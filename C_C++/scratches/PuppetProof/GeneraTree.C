#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "Punto.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TString.h"
#define nofiles 10
#define nevents 100000

void GeneraTree(Bool_t moreRandom = kFALSE, const char *outfn = NULL)
{
  // Apertura del file di output
  TFile hfile(outfn?outfn:"htree.root","RECREATE");
  // dichiarazione del TTree
  TTree *tree = new TTree("T","TTree con 2 branches");

  if (moreRandom) gRandom->SetSeed( time(NULL) );

  // Dichiarazione di un TClonesArray
  TClonesArray *ptrhits = new TClonesArray("Punto",nevents);
  TClonesArray &hits = *ptrhits;

  // Definizione di una struct
  typedef struct {
    Double_t X,Y,Z;
    Int_t mult;} POINT;
  static POINT point;

  // Dichiarazione dei 2 branch del TTree
  tree->Branch("VertMult",&point.X,"X/D:Y:Z:mult/I");
  tree->Branch("Hits",&ptrhits);
      
  for(Int_t i=0; i<nevents;i++){ // loop sugli eventi
      
      // Genero una molteplicità e un vertice
      Int_t numpart=80;  
      // while(numpart<=0){
          // numpart=(Int_t)(0.5+gRandom->Gaus(50.,20.));
      // }
      point.mult=numpart;
      point.X=gRandom->Gaus(0.,0.01);
      point.Y=gRandom->Gaus(0.,0.01);
      point.Z=gRandom->Gaus(0.,5.3);
      for (Int_t j=0; j<numpart; j++){
          // Genero un hit in modo del tutto random (dummy)
          new(hits[j]) Punto(-5.+gRandom->Rndm()*10.,
            5.+gRandom->Rndm()*10,15.+gRandom->Rndm()*10.);
      }
      tree->Fill();
      ptrhits->Clear();  
  }
  // Save all objects in this file
  tree->Write();
  // Close the file. 
  hfile.Close();
}

void mio_GeneraTree(Int_t n=nofiles) {
  TString buf;
  for (Int_t i=0;i<n;++i){
    buf.Form("htree/htree_%03d.root", i);
    GeneraTree(kTRUE, buf.Data());
  }
}

