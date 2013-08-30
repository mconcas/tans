#include <Riostream.h>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "Punto.h"
#include "TMath.h"
#include "TClonesArray.h"

void leggitree() {

  // definizione struct
  typedef struct {
    Double_t X,Y,Z;
    Int_t mult;
  } POINT;

  POINT point;
  memset(&point, 0, sizeof(point));

  // Dichiarazione TClonesArray
  TClonesArray *hits = new TClonesArray("Punto",100);

  //Apertura file di input
  TFile hfile("htree/htree_001.root");

  //Lettura TTree  e branch
  TTree *tree = (TTree*)hfile.Get("T");
  //TBranch *b1=tree->GetBranch("VertMult");
  //TBranch *b2=tree->GetBranch("Hits");

  // Definizione degli indirizzi per la lettura dei dati su ttree
  //b1->SetAddress(&point.X);
  //b2->SetAddress(&hits);

  tree->SetBranchAddress("VertMult", &point);
  tree->SetBranchAddress("Hits", &hits);

  // loop sugli ingressi nel TTree
  for(Int_t ev=0; ev<tree->GetEntries(); ev++) {

    tree->GetEvent(ev);
    Int_t num = hits->GetEntries();

    Printf("Ev=%d Mult=%d(%d) X=%lf Y=%lf Z=%lf",
      ev, point.mult, num, point.X, point.Y, point.Z);

    Punto *tst;
    for (Int_t j=0; j<num; j++){
      tst=(Punto*)hits->At(j);
      Printf("  #%d: X=%lf Y=%lf Z=%lf",
        j, tst->GetX(), tst->GetY(), tst->GetZ());
    }

  }
 
}
