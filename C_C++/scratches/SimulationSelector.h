//////////////////////////////////////////////////////////////////////
//class SimulationSelector: Analysis class that implements a 
//«Process()» method also useful for parallel computation.
// 
//Written by Matteo Concas: mett1990@gmail.com 
//Exam: «Tecniche di analisi numerica e simulazione» 
//////////////////////////////////////////////////////////////////////

#ifndef SimulationSelector_h
#define SimulationSelector_h

#include <TChain.h>
#include <TFile.h>
#include <TObject.h>
#include <TROOT.h>
#include <TSelector.h>
#include <TH1F.h>
#include "Vertice.h"

#include "Punto.h"

class SimulationSelector : public TSelector {

public :
   TTree *fTree;           // Pointer to the analyzed TTree or TChain.
   TH1F *etahist;          // Pointer to the pseudorapidity histogram.
   TH1F *fHistX;           // I/O debugger. 
   TH1F *fHistY;           // I/O debugger.
   TH1F *fHistZ;           // I/O debugger.
   TH1I *fHistMult;        // I/O debugger.          
   Vertice fAnalyzedVert;  // Declaration of leaf types.
   TBranch *b_vertices;    // Branch to analyze.
   Int_t fNumberOfEvents;  // Events counter.
   TFile *kinem;           // Kinematic file pointer.

   SimulationSelector(TTree* /*tree*/=0) : fTree(0), etahist(0),
      fNumberOfEvents(0), kinem(0) { }
   virtual ~SimulationSelector() { };
   virtual Int_t   Version() const { return 1; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall=0) {

                     // If any, return entry. 
                     return fTree ? fTree->GetTree()
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