//////////////////////////////////////////////////////////
// 
// 
// 
// 
//////////////////////////////////////////////////////////

#ifndef SimulationSelector_h
#define SimulationSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include "Vertice.h"
#include <TObject.h>
#include "Punto.h"

class SimulationSelector : public TSelector {
public :

   TTree *fChain;   //pointer to the analyzed TTree or TChain
   TH1F *etahist;
   // Declaration of leaf types
   Vertice         *fAnalyzedVert;

   // List of branches
   TBranch        *b_vertices;

   // Member Functions
   SimulationSelector(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~SimulationSelector() {};
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { 
                     return fChain ? fChain->GetTree()
                     ->GetEntry(entry, getall) : 0; 
                   }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(SimulationSelector,0);
};

#endif