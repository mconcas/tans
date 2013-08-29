#include "ProofFirst.h"
#include "TH1F.h"
#include "TRandom3.h"

//______________________________
ProofFirst::ProofFirst() {
	// Constructor
	fH1F = 0;
	fRandom = 0;
}

//______________________________
ProofFirst::~ProofFirst() {
	// Destructor
	if (fRandom) delete fRandom;
}


//______________________________
void ProofFirst::SlaveBegin(TTree * /*tree*/) {
	
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
 
   // TString option = GetOption();
 
   // Histogram
   fH1F = new TH1F("FirstH1F"), "First TH1F in PROOF", 100, -10., 10.);
   fOutput->Add(fH1F);
}