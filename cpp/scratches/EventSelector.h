//////////////////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 12 11:03:59 2014 by ROOT version 5.34/11
// from TTree Events Tree/This contains the true events.
// found on file: events.root
//////////////////////////////////////////////////////////////////////

#ifndef EVENTSELECTOR_H
#define EVENTSELECTOR_H

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include <TH3F.h>
// #include <TObject.h>
#include <Vertice.h>
#include <Hit.h>
#include <Direzione.h>
#include <TClonesArray.h>


class EventSelector : public TSelector {
public :
   TTree          *fChain;
   Vertice        *fVertex;
   TClonesArray   *fHitsFirstLayer;
   TClonesArray   *fHitsSecondLayer;
   Hit            *fAnaHitScnd;
   Hit            *fAnaHitFrst;
   // TH3F           *fPuppetHistIO;
   TH1F           *fHistPhi;
   Int_t          fNumberOfEvents;

   EventSelector(TTree* =0) : fChain(0), fHistPhi(0) {}
   virtual ~EventSelector() {}
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) {
      return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;}
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(EventSelector,0);
};

#endif
