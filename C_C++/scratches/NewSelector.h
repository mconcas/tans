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
#include <TH1I.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include "./Punto.h"

// typedef struct {
   // Double_t X,Y,Z;
   // Int_t mult;
// } POINT;

class NewSelector : public TSelector {
public :

   TTree          *fChain; //!pointer to the analyzed TTree or TChain
   Int_t           fNumberOfEvents;

   // Declaration of leaf types
   Punto          fVertMult;
   TClonesArray   *fHits;

   TH1I           *fHistX;

   NewSelector(TTree * /*tree*/ =0) : fNumberOfEvents(0), fHistX(0) { }
   virtual ~NewSelector() {};
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
