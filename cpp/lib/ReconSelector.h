//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Apr  8 10:11:39 2014 by ROOT version 5.34/18
// from TTree Events Tree/This contains the true events.
// found on file: events.root
//////////////////////////////////////////////////////////

#ifndef ReconSelector_h
#define ReconSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include "./Punto.h"
#include "./Vertice.h"
#include "./Hit.h"

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxFirstlayer=50;
const Int_t kMaxSecondlayer=50;


class ReconSelector : public TSelector {
public :
   TTree          *fChain;                  // Pointer to analyzed TTree.
   TClonesArray   *fHitClonesArray_FL;      // 1st layer analyzed arr.
   TClonesArray   *fHitClonesArray_SL;      // 2nd layer analyzed arr.
   TH1F           *fResZetaHistptr;         // Pointer to recons. histogram.
   Vertice        *fAnaVertex;              // Pointer to analyzed vertex.
   Hit            *fAnaHitFrst;             // 1st layer analyzed Hit ptr.
   Hit            *fAnaHitScnd;             // 2nd layer analyzed Hit ptr.
   Int_t           fCounter;                // Debug iteration counter.
   Int_t           fReconVertices;          // Counter for recons. vertices.
   Double_t        fDeltaPhi;               // Delta Phi limit;

   ReconSelector(TTree* =0);
   virtual ~ReconSelector() {}
   virtual Int_t   Version() const {return 2;}
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) 
      {return fChain ? fChain->GetTree()->GetEntry(entry,getall):0;}
   virtual void    SetOption(const char *option) {fOption=option;}
   virtual void    SetObject(TObject *obj) {fObject = obj;}
   virtual void    SetInputList(TList *input) {fInput=input;}
   virtual TList  *GetOutputList() const {return fOutput;}
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(ReconSelector,0);
};

#endif