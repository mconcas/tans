//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Aug 27 12:08:26 2013 by ROOT version 5.34/09
// from TTree T/TTree con 2 branches
// found on file: htree.root
//////////////////////////////////////////////////////////

#ifndef NewSelector_h
#define NewSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include "./Punto.h"

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxHits = 103;

class NewSelector : public TSelector {
public :
   TTree          *fChain; //!pointer to the analyzed TTree or TChain
   Int_t           fNumberOfEvents;
   // Declaration of leaf types
   Double_t        VertMult_X;
   Double_t        VertMult_Y;
   Double_t        VertMult_Z;
   Int_t           VertMult_mult;
   Int_t           Hits_;
   UInt_t          Hits_fUniqueID[kMaxHits];   //[Hits_]
   UInt_t          Hits_fBits[kMaxHits];   //[Hits_]
   Double_t        Hits_fX[kMaxHits];   //[Hits_]
   Double_t        Hits_fY[kMaxHits];   //[Hits_]
   Double_t        Hits_fZ[kMaxHits];   //[Hits_]

   // List of branches
   TBranch        *b_VertMult;   //!
   TBranch        *b_Hits_;   //!
   TBranch        *b_Hits_fUniqueID;   //!
   TBranch        *b_Hits_fBits;   //!
   TBranch        *b_Hits_fX;   //!
   TBranch        *b_Hits_fY;   //!
   TBranch        *b_Hits_fZ;   //!

   NewSelector(TTree * /*tree*/ =0) : fNumberOfEvents(0) { }
   virtual ~NewSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(NewSelector,0);
};

#endif

#ifdef NewSelector_cxx
void NewSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("VertMult", &VertMult_X, &b_VertMult);
   fChain->SetBranchAddress("Hits", &Hits_, &b_Hits_);
   fChain->SetBranchAddress("Hits.fUniqueID", Hits_fUniqueID, &b_Hits_fUniqueID);
   fChain->SetBranchAddress("Hits.fBits", Hits_fBits, &b_Hits_fBits);
   fChain->SetBranchAddress("Hits.fX", Hits_fX, &b_Hits_fX);
   fChain->SetBranchAddress("Hits.fY", Hits_fY, &b_Hits_fY);
   fChain->SetBranchAddress("Hits.fZ", Hits_fZ, &b_Hits_fZ);
}

Bool_t NewSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef NewSelector_cxx
