//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
// This macro generates collision vertices and events propagation.
// Written by Matteo Concas: mett1990@gmail.com
// Exam: «Tecniche di analisi numerica e simulazione»
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Direzione.h"
#include "Hit.h"
#include "Vertice.h"
#include <TClonesArray.h>
#include <TFile.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TMaterial.h>
#include <TMath.h>
#include <TSystem.h>
#include <TTree.h>
#endif

Int_t EventGenerator( const Int_t    debug=0,
                      const Int_t    nVertices=500,
                      const Bool_t   dryRun=kFALSE,
                      const Bool_t   fFlagMontecarloTruth=kFALSE,
                      const Int_t    fNoiseLevel=5,
                      const Double_t fZetLen=164.6,
                      const Double_t fBPipeRad=30.,
                      const Double_t fFirstRad=40.,
                      const Double_t fSecondRad=70.,
                      const TString  fOutFileName="events.root",
                      const Int_t    fCustomMult=0,
                      const Bool_t   fMultiScat=kTRUE,
                      const TString  fInData="kinem.root",
                      const TString  fMulDist="hmul",
                      const TString  fEtadist="heta"
                    /*const Bool_t   needforlogfile=kTRUE,*/ ) {

   ///////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////
   // Setup the environment.
   //
   // ~ Create output file.
   // ~ Reset gRandom global pointer.
   // ~ Extract infos form input file.
   // ~ Define and create the containers.
   // ~ Define problems specifications, e.g. TMaterials.
   //
   ///////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////

   gDebug=debug;
   TFile outfile(fOutFileName.Data(),"RECREATE");
   if (outfile.IsZombie()) {
      Printf("A problem occured creating %s file",
              fOutFileName.Data());
      return 1;
   }
   if ( gRandom ) delete gRandom;
   gRandom=new TRandom3( time( NULL ) );
   TFile infile(fInData.Data(),"READ");
   TH1F *histEtaptr=static_cast<TH1F*>(infile.Get(fEtadist.Data()));

   // If it's not a custom (or debug) run -> fCustomMult=0.
   // Read input data and store them in a new histogram pointer.
   TH1F *hisMulptr=static_cast<TH1F*>(infile.Get(fMulDist.Data()));
   outfile.cd();

   // Trees
   TTree *MainTree=new TTree("Montecarlo Truth","Each event is\
      composed by a vertex and a #multiplicity of hits.");

   TTree *EventsTree=new TTree("Events Tree","This contains the true\
      events.");

   // TMaterials can stock useful data source for multiple scattering.
   TMaterial Beryllium("Berillio","Be",8,4,1.85,35.28,0);
   TMaterial Silicon("Silicio","Si",28,14,2.33,9.37,0);

   // Containers for MainTree
   TClonesArray* hitsbpipeptr = new TClonesArray("Hit",100);
   TClonesArray &hitsbpipe = *hitsbpipeptr;
   TClonesArray* hitsfirstptr = new TClonesArray("Hit",100);
   TClonesArray &hitsfirst = *hitsfirstptr;
   TClonesArray* hitssecondptr= new TClonesArray("Hit",100);
   TClonesArray &hitssecond = *hitssecondptr;
   // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // TClonesArray* direction1ptr = new TClonesArray("Direzione",100);
   // TClonesArray &direction1 = *direction1ptr;
   // TClonesArray* direction2ptr = new TClonesArray("Direzione",100);
   // TClonesArray &direction2 = *direction2ptr;
   // TClonesArray* direction3ptr = new TClonesArray("Direzione",100);
   // TClonesArray &direction3 = *direction3ptr;
   // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   // Containers for EventsTree
   // TClonesArray* rhitsbpipeptr = new TClonesArray("Hit",100);
   // TClonesArray &rhitsbpipe = *rhitsbpipeptr;
   TClonesArray* rhitsfirstptr = new TClonesArray("Hit",100);
   TClonesArray &rhitsfirst = *rhitsfirstptr;
   TClonesArray* rhitssecondptr = new TClonesArray("Hit",100);
   TClonesArray &rhitssecond = *rhitssecondptr;
   // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // TClonesArray* rdirection1ptr =new TClonesArray("Direzione",100);
   // TClonesArray &rdirection1 =*rdirection1ptr;
   // TClonesArray* rdirection2ptr =new TClonesArray("Direzione",100);
   // TClonesArray &rdirection2 =*rdirection2ptr;
   // TClonesArray* rdirection3ptr= new TClonesArray("Direzione",100);
   // TClonesArray &rdirection3 =*rdirection3ptr;
   // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Vertice* vertex=new Vertice();

   // MainTree branches.
   MainTree->Branch("Vertices",    vertex);
   MainTree->Branch("Beampipe",    &hitsbpipeptr);
   MainTree->Branch("Firstlayer",  &hitsfirstptr);
   MainTree->Branch("Secondlayer", &hitssecondptr);
   // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // MainTree->Branch("Direction1",  &direction1ptr);
   // MainTree->Branch("Direction2",  &direction2ptr);
   // MainTree->Branch("Direction3",  &direction3ptr);
   // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   // EventTree branches.
   // EventsTree->Branch("Vertices",    vertex);
   // EventsTree->Branch("Beampipe",    &rhitsbpipeptr);
   EventsTree->Branch("Firstlayer",  &rhitsfirstptr);
   EventsTree->Branch("Secondlayer", &rhitssecondptr);
   // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   // EventsTree->Branch("Direction1",  &rdirection1ptr);
   // EventsTree->Branch("Direction2",  &rdirection2ptr);
   // EventsTree->Branch("Direction3",  &rdirection3ptr);
   // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Int_t percentage=0.;
   // Debug welcome and summary
   Printf("\n\t++++++++++++++++++++++++++++++++++++++++++++++");
   Printf("\t+                                            +");
   Printf("\t+        Events simulation is running!       +");
   Printf("\t+                                            +");
   Printf("\t++++++++++++++++++++++++++++++++++++++++++++++\n");
   Printf("\nSummary:");
   Printf("Output file:              \t\t%s", fOutFileName.Data());
   Printf("Number of events:         \t\t%d", nVertices);
   if( fCustomMult==0 )
      Printf("Multiplicity picked from: \t\t%s", fInData.Data());
   else {
      if(fCustomMult==-1)
         Printf("Multiplicity uniformely generated from 1 to 52");
      else
         Printf("Multiplicity manually set at: %d", fCustomMult);
   }
   Printf("Multiple Scattering:      \t\t%s", fMultiScat ?
      "\x1B[32menabled\x1B[0m" : "\x1B[31mdisabled\x1B[0m");
   Printf("Noise Level:              \t\t%d", fNoiseLevel);



   ///////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////
   //
   // Loop for events generation.
   //
   ///////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////

   for(Int_t i=0;i<nVertices;++i) {
      // Show progress percentage "live".
      printf("Progress status:     \t\t\t%d%% \r", percentage);
      
      Int_t k=0;
      Int_t u=0;
      Int_t v=0;

      switch ( fCustomMult ) {
         case -1:

            //////////////////////////////////////////////////////////
            // Pick an uniformly distributed multiplicity and set it
            // for the vertex instance.
            //////////////////////////////////////////////////////////
            vertex->SetVerticeMult( static_cast<Int_t>
               (gRandom->Rndm()*52));
            break;

         case 0:

            //////////////////////////////////////////////////////////
            // Pick athe multiplicity from a given distribution and
            // set it in the vertex instance.
            //////////////////////////////////////////////////////////
            if(gDebug) Printf("[debug] multiplicity picked from %s ",
                fInData.Data());

            // mulRaw = hisMulptr->GetRandom();
            vertex->SetVerticeMult( static_cast<Int_t>( hisMulptr->
               GetRandom()+0.5) ); // static_cast<Int_t>() returns
                                   // rounded down values.
            break;

         default:
            Printf("[debug]: entered in the default option.");

            //////////////////////////////////////////////////////////
            // Multiplicity is manually set by the function invoker.
            //////////////////////////////////////////////////////////
            if( fCustomMult>0 ) vertex->SetVerticeMult( fCustomMult );
            else return 1;
            break;
      }
      ////////////////////////////////////////////////////////////////
      // Set vertex Noiselevel
      vertex->SetVerticeNL(fNoiseLevel);

      ////////////////////////////////////////////////////////////////
      // Generate random vertex position.
      vertex->SetPuntoX( gRandom->Gaus(0,0.1) );
      vertex->SetPuntoY( gRandom->Gaus(0,0.1) );
      vertex->SetPuntoZ( gRandom->Gaus(0,53) );

      ////////////////////////////////////////////////////////////////
      // Hits generation loop.
      Int_t lim=vertex->GetVerticeMult();
      for(Int_t j=0; j<lim; ++j) {

         Double_t temp=2*gRandom->Rndm()*TMath::Pi();
         Direzione* fDirect=new Direzione(2*TMath::ATan(TMath::Exp(
            -histEtaptr->GetRandom())),temp,j);

         // Fill TClonesArrays with a std CConstructor.
         // new(direction1[j]) Direzione(*fDirect);
         // new(rdirection1[j]) Direzione(*fDirect);

         /////////////////////////////////////////////////////////////
         // Propagate from vertex and add it to the TClonesArray.
         Hit* tHitBPptr=Hit::HitOnCylFromVertex( *vertex,
                              *fDirect,fBPipeRad,j );

         if(TMath::Abs(tHitBPptr->GetPuntoZ())<=fZetLen/2) {
         // new(rdirection2[k]) Direzione(*fDirect);
         // new(rhitsbpipe[k]) Hit(*tHitBPptr);
         // k+=1;
         // }
         
         // new(direction2[j]) Direzione(*fDirect);
         new(hitsbpipe[k]) Hit(*tHitBPptr);
         k+=1;
         }

         /////////////////////////////////////////////////////////////
         // Propagate to 1st detector and add it to the proper
         // TClonesArray.
         Hit* tHitFLptr=tHitBPptr->GetHitOnCyl( *fDirect,fFirstRad,
            Beryllium,0.8,j,fMultiScat,1 );

         fDirect->FlipBit(); // Reset Rotation bit.

         if(TMath::Abs(tHitFLptr->GetPuntoZ())<=fZetLen/2) {
         // new(rdirection3[u]) Direzione(*fDirect);
            new(rhitsfirst[u]) Hit(*tHitFLptr);
            u+=1;
         }

         // new(direction3[j]) Direzione(*fDirect);
         new(hitsfirst[j]) Hit(*tHitFLptr);

         /////////////////////////////////////////////////////////////
         // Propagate to 2nd cylinder and add it to the TClonesArray
         Hit *tHitSLptr=tHitFLptr->GetHitOnCyl( *fDirect,fSecondRad,
            Silicon,0.2,j,fMultiScat,2 );

         if(TMath::Abs(tHitSLptr->GetPuntoZ())<=fZetLen/2) {
            // new(rdirection3[v]) Direzione(*fDirect);
            new(rhitssecond[v]) Hit(*tHitSLptr);
            v+=1;
         }

         // new(direction3[j]) Direzione(*fDirect);
         new(hitssecond[j]) Hit(*tHitSLptr);

         /////////////////////////////////////////////////////////////
         // Noise algorithm.
         // We have to consider the electronic noise generated on the
         // two layers. We also assume that it's uniformely
         // distributed on layers surfaces.
         // Noise sources:
         //
         // ~ Electronic noise.
         // ~ Untracked particles.


         const Int_t fNoiseLevelSec=(Int_t)
            (vertex->GetVerticeNL()*(fSecondRad/fFirstRad));

         // On the first layer
         for( Int_t n=0; n<fNoiseLevel; ++n ) {
            Hit *tNoiseHitPtrF=Hit::EleNoiseOnCyl( fFirstRad,
               -fZetLen/2,fZetLen/2);
            new(rhitsfirst[u+n]) Hit( *tNoiseHitPtrF );
            delete tNoiseHitPtrF;
         }

         // On the second layer.
         for( Int_t m=0; m<fNoiseLevelSec; ++m ) {
            Hit * tNoiseHitPtrS=Hit::EleNoiseOnCyl( fSecondRad,
               -fZetLen/2,fZetLen/2);
            new(rhitssecond[v+m]) Hit( *tNoiseHitPtrS );
            delete tNoiseHitPtrS;
         }




         // Clean up pointers
         delete tHitSLptr;
         delete tHitBPptr;
         delete tHitFLptr;
         delete fDirect;
      }

      ////////////////////////////////////////////////////////////////
      // Fill trees.
      // Clear TClonesArrays.
      ////////////////////////////////////////////////////////////////
      if(!(dryRun)) {
         // MainTree->Fill();
         EventsTree->Fill();
      }

         hitsbpipeptr->Clear();
         hitsfirstptr->Clear();
         hitssecondptr->Clear();
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // direction1ptr->Clear();
      // direction2ptr->Clear();
      // direction3ptr->Clear();
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
         // rhitsbpipeptr->Clear();
         rhitsfirstptr->Clear();
         rhitssecondptr->Clear();
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // rdirection1ptr->Clear();
      // rdirection2ptr->Clear();
      // rdirection3ptr->Clear();
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      percentage=static_cast<Int_t>(i*100/nVertices);
   }

   ///////////////////////////////////////////////////////////////////
   // Finalyze the simulation.
   ///////////////////////////////////////////////////////////////////
   if( !fFlagMontecarloTruth ) MainTree->TTree::~TTree();
   if(!dryRun) outfile.Write();
   Printf("Progress status:     \t\t\t100%%");
   Printf("Simulation process ended.");
   histEtaptr->TH1F::~TH1F();
   hisMulptr->TH1F::~TH1F();

   // MainTree->TTree::~TTree();
   vertex->Vertice::~Vertice();
   outfile.Close();
   infile.Close();

   return 0;
}
