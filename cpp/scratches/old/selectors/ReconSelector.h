//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Apr  1 10:20:51 2014 by ROOT version 5.34/11
// from TTree Events Tree/This contains the true      events.
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
#include <TObject.h>
#include "Punto.h"
#include "Hit.h"
#include "Vertice.h"

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxFirstlayer = 29;
const Int_t kMaxSecondlayer = 27;

class ReconSelector : public TSelector {
public :
   TTree          *fChain;   // pointer to the analyzed TTree/TChain
   // Vertice        *fAnalyzedVertex;
   TClonesArray   *fHitClonesArray_FL;      // 1st layer analyzed arr.
   Hit            *fAnalyzedHit_FL;         // 1st layer analyzed hit.
   TClonesArray   *fHitClonesArray_SL;      // 2nd layer analyzed arr.
   Hit            *fAnalyzedHit_SL;         // 2nd layer analyzed hit.
   Int_t          fCounter;
   TH1F           *testHist;

   // Declaration of leaf types
   Int_t           Firstlayer;
   UInt_t          Firstlayer_fUniqueID[kMaxFirstlayer];   //[Firstlayer_]
   UInt_t          Firstlayer_fBits[kMaxFirstlayer];   //[Firstlayer_]
   Double_t        Firstlayer_X[kMaxFirstlayer];   //[Firstlayer_]
   Double_t        Firstlayer_Y[kMaxFirstlayer];   //[Firstlayer_]
   Double_t        Firstlayer_Z[kMaxFirstlayer];   //[Firstlayer_]
   Double_t        Firstlayer_Theta[kMaxFirstlayer];   //[Firstlayer_]
   Double_t        Firstlayer_Phi[kMaxFirstlayer];   //[Firstlayer_]
   Double_t        Firstlayer_CRadius[kMaxFirstlayer];   //[Firstlayer_]
   Double_t        Firstlayer_SRadius[kMaxFirstlayer];   //[Firstlayer_]
   Int_t           Firstlayer_layernum[kMaxFirstlayer];   //[Firstlayer_]
   Int_t           Firstlayer_idnumber[kMaxFirstlayer];   //[Firstlayer_]
   Bool_t          Firstlayer_realhit[kMaxFirstlayer];   //[Firstlayer_]
   Int_t           Secondlayer;
   UInt_t          Secondlayer_fUniqueID[kMaxSecondlayer];   //[Secondlayer_]
   UInt_t          Secondlayer_fBits[kMaxSecondlayer];   //[Secondlayer_]
   Double_t        Secondlayer_X[kMaxSecondlayer];   //[Secondlayer_]
   Double_t        Secondlayer_Y[kMaxSecondlayer];   //[Secondlayer_]
   Double_t        Secondlayer_Z[kMaxSecondlayer];   //[Secondlayer_]
   Double_t        Secondlayer_Theta[kMaxSecondlayer];   //[Secondlayer_]
   Double_t        Secondlayer_Phi[kMaxSecondlayer];   //[Secondlayer_]
   Double_t        Secondlayer_CRadius[kMaxSecondlayer];   //[Secondlayer_]
   Double_t        Secondlayer_SRadius[kMaxSecondlayer];   //[Secondlayer_]
   Int_t           Secondlayer_layernum[kMaxSecondlayer];   //[Secondlayer_]
   Int_t           Secondlayer_idnumber[kMaxSecondlayer];   //[Secondlayer_]
   Bool_t          Secondlayer_realhit[kMaxSecondlayer];   //[Secondlayer_]

   // List of branches
   // TBranch        *b_Firstlayer;   //!
   // TBranch        *b_Firstlayer_fUniqueID;   //!
   // TBranch        *b_Firstlayer_fBits;   //!
   // TBranch        *b_Firstlayer_X;   //!
   // TBranch        *b_Firstlayer_Y;   //!
   // TBranch        *b_Firstlayer_Z;   //!
   // TBranch        *b_Firstlayer_Theta;   //!
   // TBranch        *b_Firstlayer_Phi;   //!
   // TBranch        *b_Firstlayer_CRadius;   //!
   // TBranch        *b_Firstlayer_SRadius;   //!
   // TBranch        *b_Firstlayer_layernum;   //!
   // TBranch        *b_Firstlayer_idnumber;   //!
   // TBranch        *b_Firstlayer_realhit;   //!
   // TBranch        *b_Secondlayer;   //!
   // TBranch        *b_Secondlayer_fUniqueID;   //!
   // TBranch        *b_Secondlayer_fBits;   //!
   // TBranch        *b_Secondlayer_X;   //!
   // TBranch        *b_Secondlayer_Y;   //!
   // TBranch        *b_Secondlayer_Z;   //!
   // TBranch        *b_Secondlayer_Theta;   //!
   // TBranch        *b_Secondlayer_Phi;   //!
   // TBranch        *b_Secondlayer_CRadius;   //!
   // TBranch        *b_Secondlayer_SRadius;   //!
   // TBranch        *b_Secondlayer_layernum;   //!
   // TBranch        *b_Secondlayer_idnumber;   //!
   // TBranch        *b_Secondlayer_realhit;   //!

   ReconSelector(TTree* =0) : 
      fChain(0),
      fHitClonesArray_FL(0), 
      fHitClonesArray_SL(0),
      testHist(0) { }

   virtual ~ReconSelector() { }

   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process( Long64_t entry );
   virtual Int_t   GetEntry( Long64_t entry, Int_t getall=0 ) 
      {return fChain ? fChain->GetTree()->GetEntry(entry,getall) : 0;}
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(ReconSelector,0);
};

#endif
