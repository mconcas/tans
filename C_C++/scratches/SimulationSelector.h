//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep 23 19:20:41 2013 by ROOT version 5.34/09
// from TTree Vertices/Tree with generated vertices.
// found on file: vertices.root
//////////////////////////////////////////////////////////

#ifndef SimulationSelector.C_h
#define SimulationSelector.C_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.
#include "./Vertice.h"
#include <TObject.h>
#include "./Punto.h"

// Fixed size dimensions of array or collections stored in the TTree if any.

class SimulationSelector.C : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
 //Vertice         *Vertices;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   TString         Name;
   Double_t        X;
   Double_t        Y;
   Double_t        Z;
   Double_t        Theta;
   Double_t        Phi;
   Double_t        CRadius;
   Double_t        SRadius;
   Int_t           multiplicity;

   // List of branches
   TBranch        *b_Vertices_fUniqueID;   //!
   TBranch        *b_Vertices_fBits;   //!
   TBranch        *b_Vertices_Name;   //!
   TBranch        *b_Vertices_X;   //!
   TBranch        *b_Vertices_Y;   //!
   TBranch        *b_Vertices_Z;   //!
   TBranch        *b_Vertices_Theta;   //!
   TBranch        *b_Vertices_Phi;   //!
   TBranch        *b_Vertices_CRadius;   //!
   TBranch        *b_Vertices_SRadius;   //!
   TBranch        *b_Vertices_multiplicity;   //!

   SimulationSelector.C(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~SimulationSelector.C() { }
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

   ClassDef(SimulationSelector.C,0);
};

#endif

#ifdef SimulationSelector.C_cxx
void SimulationSelector.C::Init(TTree *tree)
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

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_Vertices_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_Vertices_fBits);
   fChain->SetBranchAddress("Name", &Name, &b_Vertices_Name);
   fChain->SetBranchAddress("X", &X, &b_Vertices_X);
   fChain->SetBranchAddress("Y", &Y, &b_Vertices_Y);
   fChain->SetBranchAddress("Z", &Z, &b_Vertices_Z);
   fChain->SetBranchAddress("Theta", &Theta, &b_Vertices_Theta);
   fChain->SetBranchAddress("Phi", &Phi, &b_Vertices_Phi);
   fChain->SetBranchAddress("CRadius", &CRadius, &b_Vertices_CRadius);
   fChain->SetBranchAddress("SRadius", &SRadius, &b_Vertices_SRadius);
   fChain->SetBranchAddress("multiplicity", &multiplicity, &b_Vertices_multiplicity);
}

Bool_t SimulationSelector.C::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef SimulationSelector.C_cxx
