//////////////////////////////////////////////////////////
//
// TSlector Template
//
//////////////////////////////////////////////////////////

#ifndef ProofFirst_h
#define ProofFirst_h

#include <TSelector.h>

class TH1F;
class TRandom;
class ProofFirst : public TSelector{
public:

    //Define members here    
    TH1F       *fH1F;             //! Output histogram    
    TRandom    *fRandom;          //! Random number generator
    
    ProofFirst();
    virtual ~ProofFirst();
    virtual Int_t     Version() const { return 2; }
    virtual void      Begin(TTree *tree);
    virtual void      SlaveBegin(TTree *tree);
    virtual Bool_t    Process(Long64_t entry);
    virtual void      SetOption(const char *option) { fOption = option; }
    virtual void      SetObject(TObject *obj) { fObject = obj;}
    virtual void      SetInputList(TList *input) { fInput = input; }
    virtual TList    *GetOutputList() const { return fOutput; }
    virtual void      SlaveTerminate();
    virtual void      Terminate();


    ClassDef(ProofFitst,2);
};

#endif