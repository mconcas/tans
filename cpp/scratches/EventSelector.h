//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 12 11:03:59 2014 by ROOT version 5.34/11
// from TTree Events Tree/This contains the true      events.
// found on file: events.root
//////////////////////////////////////////////////////////

#ifndef EventSelector_h
#define EventSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

// Header file for the classes stored in the TTree if any.
#include <TObject.h>
#include <TClonesArray.h>

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxBeampipe = 29;
const Int_t kMaxFirstlayer = 26;
const Int_t kMaxSecondlayer = 19;

class EventSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
 //Vertice         *Vertices;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Double_t        X;
   Double_t        Y;
   Double_t        Z;
   Double_t        Theta;
   Double_t        Phi;
   Double_t        CRadius;
   Double_t        SRadius;
   Int_t           multiplicity;
   Int_t           idnumber;
   Double_t        noiselevel;
   Int_t           Beampipe_;
   UInt_t          Beampipe_fUniqueID[kMaxBeampipe];   //[Beampipe_]
   UInt_t          Beampipe_fBits[kMaxBeampipe];   //[Beampipe_]
   Double_t        Beampipe_X[kMaxBeampipe];   //[Beampipe_]
   Double_t        Beampipe_Y[kMaxBeampipe];   //[Beampipe_]
   Double_t        Beampipe_Z[kMaxBeampipe];   //[Beampipe_]
   Double_t        Beampipe_Theta[kMaxBeampipe];   //[Beampipe_]
   Double_t        Beampipe_Phi[kMaxBeampipe];   //[Beampipe_]
   Double_t        Beampipe_CRadius[kMaxBeampipe];   //[Beampipe_]
   Double_t        Beampipe_SRadius[kMaxBeampipe];   //[Beampipe_]
   Int_t           Beampipe_layernum[kMaxBeampipe];   //[Beampipe_]
   Int_t           Beampipe_idnumber[kMaxBeampipe];   //[Beampipe_]
   Bool_t          Beampipe_realhit[kMaxBeampipe];   //[Beampipe_]
   Int_t           Firstlayer_;
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
   Int_t           Secondlayer_;
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
   TBranch        *b_Vertices_fUniqueID;   //!
   TBranch        *b_Vertices_fBits;   //!
   TBranch        *b_Vertices_X;   //!
   TBranch        *b_Vertices_Y;   //!
   TBranch        *b_Vertices_Z;   //!
   TBranch        *b_Vertices_Theta;   //!
   TBranch        *b_Vertices_Phi;   //!
   TBranch        *b_Vertices_CRadius;   //!
   TBranch        *b_Vertices_SRadius;   //!
   TBranch        *b_Vertices_multiplicity;   //!
   TBranch        *b_Vertices_idnumber;   //!
   TBranch        *b_Vertices_noiselevel;   //!
   TBranch        *b_Beampipe_;   //!
   TBranch        *b_Beampipe_fUniqueID;   //!
   TBranch        *b_Beampipe_fBits;   //!
   TBranch        *b_Beampipe_X;   //!
   TBranch        *b_Beampipe_Y;   //!
   TBranch        *b_Beampipe_Z;   //!
   TBranch        *b_Beampipe_Theta;   //!
   TBranch        *b_Beampipe_Phi;   //!
   TBranch        *b_Beampipe_CRadius;   //!
   TBranch        *b_Beampipe_SRadius;   //!
   TBranch        *b_Beampipe_layernum;   //!
   TBranch        *b_Beampipe_idnumber;   //!
   TBranch        *b_Beampipe_realhit;   //!
   TBranch        *b_Firstlayer_;   //!
   TBranch        *b_Firstlayer_fUniqueID;   //!
   TBranch        *b_Firstlayer_fBits;   //!
   TBranch        *b_Firstlayer_X;   //!
   TBranch        *b_Firstlayer_Y;   //!
   TBranch        *b_Firstlayer_Z;   //!
   TBranch        *b_Firstlayer_Theta;   //!
   TBranch        *b_Firstlayer_Phi;   //!
   TBranch        *b_Firstlayer_CRadius;   //!
   TBranch        *b_Firstlayer_SRadius;   //!
   TBranch        *b_Firstlayer_layernum;   //!
   TBranch        *b_Firstlayer_idnumber;   //!
   TBranch        *b_Firstlayer_realhit;   //!
   TBranch        *b_Secondlayer_;   //!
   TBranch        *b_Secondlayer_fUniqueID;   //!
   TBranch        *b_Secondlayer_fBits;   //!
   TBranch        *b_Secondlayer_X;   //!
   TBranch        *b_Secondlayer_Y;   //!
   TBranch        *b_Secondlayer_Z;   //!
   TBranch        *b_Secondlayer_Theta;   //!
   TBranch        *b_Secondlayer_Phi;   //!
   TBranch        *b_Secondlayer_CRadius;   //!
   TBranch        *b_Secondlayer_SRadius;   //!
   TBranch        *b_Secondlayer_layernum;   //!
   TBranch        *b_Secondlayer_idnumber;   //!
   TBranch        *b_Secondlayer_realhit;   //!

   EventSelector(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~EventSelector() { }
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

   ClassDef(EventSelector,0);
};

#endif

#ifdef EventSelector_cxx
void EventSelector::Init(TTree *tree)
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
   fChain->SetBranchAddress("X", &X, &b_Vertices_X);
   fChain->SetBranchAddress("Y", &Y, &b_Vertices_Y);
   fChain->SetBranchAddress("Z", &Z, &b_Vertices_Z);
   fChain->SetBranchAddress("Theta", &Theta, &b_Vertices_Theta);
   fChain->SetBranchAddress("Phi", &Phi, &b_Vertices_Phi);
   fChain->SetBranchAddress("CRadius", &CRadius, &b_Vertices_CRadius);
   fChain->SetBranchAddress("SRadius", &SRadius, &b_Vertices_SRadius);
   fChain->SetBranchAddress("multiplicity", &multiplicity, &b_Vertices_multiplicity);
   fChain->SetBranchAddress("idnumber", &idnumber, &b_Vertices_idnumber);
   fChain->SetBranchAddress("noiselevel", &noiselevel, &b_Vertices_noiselevel);
   fChain->SetBranchAddress("Beampipe", &Beampipe_, &b_Beampipe_);
   fChain->SetBranchAddress("Beampipe.fUniqueID", Beampipe_fUniqueID, &b_Beampipe_fUniqueID);
   fChain->SetBranchAddress("Beampipe.fBits", Beampipe_fBits, &b_Beampipe_fBits);
   fChain->SetBranchAddress("Beampipe.X", Beampipe_X, &b_Beampipe_X);
   fChain->SetBranchAddress("Beampipe.Y", Beampipe_Y, &b_Beampipe_Y);
   fChain->SetBranchAddress("Beampipe.Z", Beampipe_Z, &b_Beampipe_Z);
   fChain->SetBranchAddress("Beampipe.Theta", Beampipe_Theta, &b_Beampipe_Theta);
   fChain->SetBranchAddress("Beampipe.Phi", Beampipe_Phi, &b_Beampipe_Phi);
   fChain->SetBranchAddress("Beampipe.CRadius", Beampipe_CRadius, &b_Beampipe_CRadius);
   fChain->SetBranchAddress("Beampipe.SRadius", Beampipe_SRadius, &b_Beampipe_SRadius);
   fChain->SetBranchAddress("Beampipe.layernum", Beampipe_layernum, &b_Beampipe_layernum);
   fChain->SetBranchAddress("Beampipe.idnumber", Beampipe_idnumber, &b_Beampipe_idnumber);
   fChain->SetBranchAddress("Beampipe.realhit", Beampipe_realhit, &b_Beampipe_realhit);
   fChain->SetBranchAddress("Firstlayer", &Firstlayer_, &b_Firstlayer_);
   fChain->SetBranchAddress("Firstlayer.fUniqueID", Firstlayer_fUniqueID, &b_Firstlayer_fUniqueID);
   fChain->SetBranchAddress("Firstlayer.fBits", Firstlayer_fBits, &b_Firstlayer_fBits);
   fChain->SetBranchAddress("Firstlayer.X", Firstlayer_X, &b_Firstlayer_X);
   fChain->SetBranchAddress("Firstlayer.Y", Firstlayer_Y, &b_Firstlayer_Y);
   fChain->SetBranchAddress("Firstlayer.Z", Firstlayer_Z, &b_Firstlayer_Z);
   fChain->SetBranchAddress("Firstlayer.Theta", Firstlayer_Theta, &b_Firstlayer_Theta);
   fChain->SetBranchAddress("Firstlayer.Phi", Firstlayer_Phi, &b_Firstlayer_Phi);
   fChain->SetBranchAddress("Firstlayer.CRadius", Firstlayer_CRadius, &b_Firstlayer_CRadius);
   fChain->SetBranchAddress("Firstlayer.SRadius", Firstlayer_SRadius, &b_Firstlayer_SRadius);
   fChain->SetBranchAddress("Firstlayer.layernum", Firstlayer_layernum, &b_Firstlayer_layernum);
   fChain->SetBranchAddress("Firstlayer.idnumber", Firstlayer_idnumber, &b_Firstlayer_idnumber);
   fChain->SetBranchAddress("Firstlayer.realhit", Firstlayer_realhit, &b_Firstlayer_realhit);
   fChain->SetBranchAddress("Secondlayer", &Secondlayer_, &b_Secondlayer_);
   fChain->SetBranchAddress("Secondlayer.fUniqueID", Secondlayer_fUniqueID, &b_Secondlayer_fUniqueID);
   fChain->SetBranchAddress("Secondlayer.fBits", Secondlayer_fBits, &b_Secondlayer_fBits);
   fChain->SetBranchAddress("Secondlayer.X", Secondlayer_X, &b_Secondlayer_X);
   fChain->SetBranchAddress("Secondlayer.Y", Secondlayer_Y, &b_Secondlayer_Y);
   fChain->SetBranchAddress("Secondlayer.Z", Secondlayer_Z, &b_Secondlayer_Z);
   fChain->SetBranchAddress("Secondlayer.Theta", Secondlayer_Theta, &b_Secondlayer_Theta);
   fChain->SetBranchAddress("Secondlayer.Phi", Secondlayer_Phi, &b_Secondlayer_Phi);
   fChain->SetBranchAddress("Secondlayer.CRadius", Secondlayer_CRadius, &b_Secondlayer_CRadius);
   fChain->SetBranchAddress("Secondlayer.SRadius", Secondlayer_SRadius, &b_Secondlayer_SRadius);
   fChain->SetBranchAddress("Secondlayer.layernum", Secondlayer_layernum, &b_Secondlayer_layernum);
   fChain->SetBranchAddress("Secondlayer.idnumber", Secondlayer_idnumber, &b_Secondlayer_idnumber);
   fChain->SetBranchAddress("Secondlayer.realhit", Secondlayer_realhit, &b_Secondlayer_realhit);
}

Bool_t EventSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef EventSelector_cxx
