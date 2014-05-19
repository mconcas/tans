#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TXMLEngine.h>
#include <TObject.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TMaterial.h>
#include <TMath.h>
#include <TSystem.h>
#include <TTree.h>
#include "SimulationCore.h"
#include "Direzione.h"
#include "Hit.h"
#include "Vertice.h"
#endif

// Utility functions prototypes.
//
// Wrap some commands in order to make a better code readability.
XMLNodePointer_t FromChildToNextParent(TXMLEngine* engine,
   XMLNodePointer_t index);

// Wrap some commands in order to make code readability lighter.
XMLNodePointer_t DrainDetectorData(pipe_t &pipe,TXMLEngine* engine,
   XMLNodePointer_t index);

// Returns theta angle from a distribution Histogram.
Double_t ThetaFromEta(TH1F* etahist);

// Class.
ClassImp(SimulationCore)

SimulationCore::SimulationCore() :
   TObject(),
   fXMLEngine(),
   fSimulationName(0)
{
   fParDataFileName="";
}

SimulationCore::SimulationCore(TString Pardatafile, TString SimName) :
   TObject(),
   fParDataFileName(Pardatafile),
   fXMLEngine(),
   fSimulationName(SimName) { }

void SimulationCore::SetParamFile(TString Filename)
{
   fParDataFileName=Filename;
}

Bool_t SimulationCore::Initialize()
{
   // Read and store Simulation parameters.
   fXMLEngine=new TXMLEngine();
   fXmlDocFileptr=fXMLEngine->ParseFile(fParDataFileName.Data());

   // Set index on root element. THe functions GetChild() and GetNext() allow to
   // browse the XMLdocument.
   fIndexNodeptr=fXMLEngine->DocGetRootElement(fXmlDocFileptr);
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);

   // Read and store «infile» name.
   fInputData=fXMLEngine->GetAttrValue(fXMLEngine->GetFirstAttr(fIndexNodeptr));
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);

   if((TString)fXMLEngine->GetAttrValue(fXMLEngine->GetFirstAttr(fIndexNodeptr))
      =="distribution") {
      fMultDist=(TString)fXMLEngine->GetNodeContent(fIndexNodeptr);
      fFixedMult=0;
   } else {
      Printf("Multiplicity histogram not specified, setting fixed value: %d",
         atoi(fXMLEngine->GetNodeContent(fIndexNodeptr)));
      fFixedMult=atoi(fXMLEngine->GetNodeContent(fIndexNodeptr));

   }
   fIndexNodeptr=fXMLEngine->GetNext(fIndexNodeptr);
   if((TString)fXMLEngine->GetAttrValue(fXMLEngine->GetFirstAttr(fIndexNodeptr))
      =="distribution") {
      fEtaDist=fXMLEngine->GetNodeContent(fIndexNodeptr);
   } else {
      Printf("Error: η histogram not specified.");
      return kFALSE;
   }

   fIndexNodeptr=FromChildToNextParent(fXMLEngine,fIndexNodeptr);

   // Get vertices parameters.
   fDebug=fXMLEngine->GetIntAttr(fIndexNodeptr,"debug");
   fNoiseLevel=fXMLEngine->GetIntAttr(fIndexNodeptr,"noise");
   fNumVertices=fXMLEngine->GetIntAttr(fIndexNodeptr,"num");
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);
   fIndexNodeptr=fXMLEngine->GetNext(fIndexNodeptr);
   if((TString)fXMLEngine->GetAttrValue(fXMLEngine->GetFirstAttr(fIndexNodeptr))
      =="gaussian") fRmsX=atof(fXMLEngine->GetNodeContent(fIndexNodeptr));
   else fRmsX=0.;
   fIndexNodeptr=fXMLEngine->GetNext(fIndexNodeptr);
   if((TString)fXMLEngine->GetAttrValue(fXMLEngine->GetFirstAttr(fIndexNodeptr))
      =="gaussian") fRmsY=atof(fXMLEngine->GetNodeContent(fIndexNodeptr));
   else fRmsY=0.;
   fIndexNodeptr=fXMLEngine->GetNext(fIndexNodeptr);
   if((TString)fXMLEngine->GetAttrValue(fXMLEngine->GetFirstAttr(fIndexNodeptr))
      =="gaussian") fRmsZ=atof(fXMLEngine->GetNodeContent(fIndexNodeptr));
   else fRmsZ=0.;
   fIndexNodeptr=FromChildToNextParent(fXMLEngine,fIndexNodeptr);

   // Check if Multiple scattering is enabled.
   if((TString)fXMLEngine->GetAttrValue(fXMLEngine->GetFirstAttr(fIndexNodeptr))
      =="true") fMultipleScat=kTRUE;
   else fMultipleScat=kFALSE;

   // Configure cylindrical detectors.
   fIndexNodeptr=fXMLEngine->GetNext(fIndexNodeptr);
   fBeampipe.fLayer=fXMLEngine->GetIntAttr(fIndexNodeptr,"layernum");
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);
   fIndexNodeptr=DrainDetectorData(fBeampipe,fXMLEngine,fIndexNodeptr);
   fIndexNodeptr=FromChildToNextParent(fXMLEngine,fIndexNodeptr);
   fFirstLayer.fLayer=fXMLEngine->GetIntAttr(fIndexNodeptr,"layernum");
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);
   fIndexNodeptr=DrainDetectorData(fFirstLayer,fXMLEngine,fIndexNodeptr);
   fIndexNodeptr=FromChildToNextParent(fXMLEngine,fIndexNodeptr);
   fSecondLayer.fLayer=fXMLEngine->GetIntAttr(fIndexNodeptr,"layernum");
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);
   fIndexNodeptr=DrainDetectorData(fSecondLayer,fXMLEngine,fIndexNodeptr);
   fIndexNodeptr=FromChildToNextParent(fXMLEngine,fIndexNodeptr);

   // Other parameters.
   if((TString)fXMLEngine->GetAttr(fIndexNodeptr,"dryrun")=="kFALSE")
      fDryRun=kFALSE;
   else fDryRun=kTRUE;

   if((TString)fXMLEngine->GetAttr(fIndexNodeptr,"montecarlo")=="kFALSE")
      fMontecarloTruth=kFALSE;
      else fMontecarloTruth=kTRUE;
   fOutFileName=(TString)fXMLEngine->GetNodeContent(fIndexNodeptr);

   // If kinematic file is not retrieved (it isn't provided from the
   // github repository and it won't in the future) try to download it directly
   // from this URL:
   // http://personalpages.to.infn.it/~masera/tans/tans2013/miscellanea/...
   TFile infile(fInputData.Data(),"READ");
   if(infile.IsZombie()) {
      TString Prefix="http://personalpages.to.infn.it/~masera/tans/tans2013/";
      TString Url=Form("miscellanea/%s",fInputData.Data());
      TString Curl=Form("curl -u tans09:Bayes2009 -o %s ",fInputData.Data());
      TString Cmd=Curl+Prefix+Url;
      Printf("\x1B[31mWARNING: %s file not found! "
         "Trying to download it directly from:"
         "http://personalpages.to.infn.it/~masera/tans/tans2013/miscellanea/"
         "\x1B[0m",
         fInputData.Data());
      gSystem->Exec(Cmd.Data());
   }
   infile.Close();

   return kTRUE;
}

Bool_t SimulationCore::Status()
{
   Printf("\x1B[31m\nSimulation %s status:\x1B[0m", fSimulationName.Data());
   Printf("Fixed multiplicity value: %d",fFixedMult);
   Printf("Number of vertices: %d",fNumVertices);
   if(fMultipleScat) Printf("Multiple scattering: enabled.");
   else Printf("Multiple scattering: disabled.");
   Printf("Noise level: %d",fNoiseLevel);
   Printf("Beampipe data:\n\t\
      Layer:      %d \n\t\
      Length:     %f \n\t\
      Thickness:  %f \n\t\
      Pipe Rad:   %f \n\t",
      fBeampipe.fLayer,fBeampipe.fZetaLen,fBeampipe.fThickness,
      fBeampipe.fPipeRad);
   Printf("Firstlayer data: \n\t\
      Layer:      %d \n\t\
      Length:     %f \n\t\
      Thickness:  %f \n\t\
      Pipe Rad:   %f \n\t",
      fFirstLayer.fLayer,fFirstLayer.fZetaLen,fFirstLayer.fThickness,
      fFirstLayer.fPipeRad);
   Printf("Secondlayer data: \n\t\
      Layer:      %d \n\t\
      Length:     %f \n\t\
      Thickness:  %f \n\t\
      Pipe Rad:   %f \n\t",
      fSecondLayer.fLayer,fSecondLayer.fZetaLen,fSecondLayer.fThickness,
      fSecondLayer.fPipeRad);
   Printf("Rms X: %f",fRmsX);
   Printf("Rms Y: %f",fRmsY);
   Printf("Rms Z: %f",fRmsZ);

   return kTRUE;
}

Bool_t SimulationCore::Run()
{
   gDebug=fDebug;
   TFile outfile(fOutFileName.Data(),"RECREATE");
   if (outfile.IsZombie()) {
      Printf("A problem occured creating %s file",
              fOutFileName.Data());
      return kFALSE;
   }
   if(gRandom) delete gRandom;
   gRandom=new TRandom3(time(NULL));
   TFile infile(fInputData.Data(),"READ");
   TH1F *histEtaptr=static_cast<TH1F*>(infile.Get(fEtaDist.Data()));
   TH1F *hisMulptr=static_cast<TH1F*>(infile.Get(fMultDist.Data()));
   outfile.cd();

   // Trees: MainTree contains the Montecarlo truth. EventsTree just contains
   // the hits measured by detectors.
   TTree *MainTree=new TTree("Montecarlo Truth","Each event is composed by a \
      vertex and a #multiplicity of hits.");
   TTree *EventsTree=new TTree("Events Tree","This contains the true events.");

   // Containers for Montecarlo truth Tree. Hits on beampipe will be saved too.
   TClonesArray* hitsbpipeptr=new TClonesArray("Hit",100);
   TClonesArray &hitsbpipe=*hitsbpipeptr;
   TClonesArray* hitsfirstptr=new TClonesArray("Hit",100);
   TClonesArray &hitsfirst=*hitsfirstptr;
   TClonesArray* hitssecondptr=new TClonesArray("Hit",100);
   TClonesArray &hitssecond=*hitssecondptr;

   // Containers for EventsTree.
   TClonesArray* rhitsfirstptr=new TClonesArray("Hit",100);
   TClonesArray &rhitsfirst=*rhitsfirstptr;
   TClonesArray* rhitssecondptr=new TClonesArray("Hit",100);
   TClonesArray &rhitssecond=*rhitssecondptr;

   // Vertex.
   Vertice vertex=Vertice();
   Direzione direction=Direzione();

   // Montecarlo Tree branches.
   MainTree->Branch("Vertices",&vertex);
   MainTree->Branch("Beampipe",&hitsbpipeptr);
   MainTree->Branch("Firstlayer",&hitsfirstptr);
   MainTree->Branch("Secondlayer",&hitssecondptr);

   // EventTree branches.
   EventsTree->Branch("Vertices",&vertex);
   EventsTree->Branch("Firstlayer",&rhitsfirstptr);
   EventsTree->Branch("Secondlayer",&rhitssecondptr);

   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   Int_t percentage=0.;
   // Print a debug welcome and summary
   Printf("\n\t++++++++++++++++++++++++++++++++++++++++++++++");
   Printf("\t+                                            +");
   Printf("\t+        Events simulation is running!       +");
   Printf("\t+                                            +");
   Printf("\t++++++++++++++++++++++++++++++++++++++++++++++\n");
   Printf("\nSummary:");
   Printf("Output file:              \t\t%s", fOutFileName.Data());
   Printf("Number of events:         \t\t%d", fNumVertices);
   if( fFixedMult==0 )
      Printf("Multiplicity picked from: \t\t%s", fInputData.Data());
   else Printf("Multiplicity manually set to: \t\t%d", fFixedMult);

   Printf("Multiple Scattering:      \t\t%s", fMultipleScat ?
      "\x1B[32menabled\x1B[0m" : "\x1B[31mdisabled\x1B[0m");
   Printf("Save Montecarlo truth:     \t\t%s",
      fMontecarloTruth ? "\x1B[32myes\x1B[0m" :
      "\x1B[31mno\x1B[0m");

   Printf("Noise Level:              \t\t%d", fNoiseLevel);

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////
   //
   // Loop for events generation.
   //
   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////
   Int_t i=0;
   do {
   // for(Int_t i=0;i<fNumVertices;++i) {

      // Show progress percentage.
      printf("Progress status:     \t\t\t%d%% \r", percentage);
      Int_t u=0;
      Int_t v=0;

      if(fFixedMult==0)
         // static_cast<Int_t>() returns rounded-down values.
         vertex.SetVerticeMult(static_cast<Int_t>(hisMulptr->GetRandom()+0.5));
      else vertex.SetVerticeMult(fFixedMult);

      //////////////////////////////////////////////////////////////////////////
      // Set vertex Noiselevel
      vertex.SetVerticeNL(fNoiseLevel);

      //////////////////////////////////////////////////////////////////////////
      // Generate random vertex position.
      vertex.SetPuntoX(gRandom->Gaus(0,fRmsX));
      vertex.SetPuntoY(gRandom->Gaus(0,fRmsY));
      vertex.SetPuntoZ(gRandom->Gaus(0,fRmsZ));

      //////////////////////////////////////////////////////////////////////////
      // Hits generation loop.
      const Int_t multiplicity=vertex.GetVerticeMult();

      // Transport code.
      Bool_t FirstFlag=kFALSE;
      Bool_t SecndFlag=kFALSE;

      for(Int_t j=0;j<multiplicity;++j) {
         direction.SetAllAngles(ThetaFromEta(histEtaptr),2*gRandom->Rndm()
            *TMath::Pi());
         direction.SetDirectID(j);

         ///////////////////////////////////////////////////////////////////////
         // Propagate from vertex and add it to the TClonesArray.
         Hit* tHitBPptr=Hit::HitOnCylFromVertex(vertex,direction,
            fBeampipe.fPipeRad,j);

         ///////////////////////////////////////////////////////////////////////
         // Propagate to 1st detector and add it to the proper
         // TClonesArray.
         //
         // fBeampipe.fMaterial, fBeampipe.fThickness are used to determine the
         // deviation due the multiple scattering effect.

         Hit* tHitFLptr=tHitBPptr->GetHitOnCyl(direction,fFirstLayer.fPipeRad,
              fBeampipe.fMaterial,fBeampipe.fThickness,fMultipleScat,1);

         // Reset Rotation bit.
         if(fMultipleScat) direction.FlipBit();

         ///////////////////////////////////////////////////////////////////////
         // Propagate to 2nd cylinder and add it to the TClonesArray
         Hit *tHitSLptr=tHitFLptr->GetHitOnCyl(direction,fSecondLayer.fPipeRad,
            fFirstLayer.fMaterial,fFirstLayer.fThickness,fMultipleScat,2);

         ///////////////////////////////////////////////////////////////////////
         // Register data.
         if(TMath::Abs(tHitFLptr->GetPuntoZ())<=fFirstLayer.fZetaLen/2) {
            new(rhitsfirst[u]) Hit(*tHitFLptr);
            u+=1;
            FirstFlag=kTRUE;
         }
         if(TMath::Abs(tHitSLptr->GetPuntoZ())<=fSecondLayer.fZetaLen/2) {
            new(rhitssecond[v]) Hit(*tHitSLptr);
            v+=1;
            SecndFlag=kTRUE;
         }

         new(hitsbpipe[j]) Hit(*tHitBPptr);
         new(hitsfirst[j]) Hit(*tHitFLptr);
         new(hitssecond[j]) Hit(*tHitSLptr);

         ///////////////////////////////////////////////////////////////////////
         // Noise algorithm.
         for(Int_t n=0;n<fNoiseLevel;++n) {
            Hit *tNoiseHitPtrF=Hit::NoiseOnCyl(fFirstLayer.fPipeRad,
               -fFirstLayer.fZetaLen/2,fFirstLayer.fZetaLen/2);
            new(rhitsfirst[u+n]) Hit(*tNoiseHitPtrF);

            Hit *tNoiseHitPtrS=Hit::NoiseOnCyl(fSecondLayer.fPipeRad,
               -fSecondLayer.fZetaLen/2,fSecondLayer.fZetaLen/2);
            new(rhitssecond[v+n]) Hit(*tNoiseHitPtrS);
            tNoiseHitPtrS->Hit::~Hit();
            tNoiseHitPtrF->Hit::~Hit();
         }

         // Delete pointers
         tHitSLptr->Hit::~Hit();
         tHitBPptr->Hit::~Hit();
         tHitFLptr->Hit::~Hit();
      }

      //////////////////////////////////////////////////////////////////////////
      // Fill trees.
      // Clear TClonesArrays.
      if(!(fDryRun)) {
         if(fMontecarloTruth) MainTree->Fill();
         EventsTree->Fill();
      }
      outfile.FlushWriteCache();
      hitsbpipeptr->Delete();
      hitsfirstptr->Delete();
      hitssecondptr->Delete();
      rhitsfirstptr->Delete();
      rhitssecondptr->Delete();

      percentage=static_cast<Int_t>(i*100/fNumVertices);

      // Increase counter if at least one track hits both the first and the
      // second layer.
      if(FirstFlag && SecndFlag) i+=1;
   } while(i<fNumVertices);

   /////////////////////////////////////////////////////////////////////////////
   // Finalyze the simulation.
   if(!fMontecarloTruth) MainTree->TTree::~TTree();
   if(!fDryRun) outfile.Write();
   Printf("Progress status:     \t\t\t100%%");
   Printf("Simulation process ended.");
   histEtaptr->TH1F::~TH1F();
   if(fFixedMult==0) hisMulptr->TH1F::~TH1F();
   vertex.Vertice::~Vertice();
   direction.Direzione::~Direzione();
   outfile.Close("R");
   infile.Close();

   return kTRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Functions definition
XMLNodePointer_t FromChildToNextParent(TXMLEngine* engine,
   XMLNodePointer_t index)
{
   index=engine->GetParent(index);
   index=engine->GetNext(index);

   return index;
}

XMLNodePointer_t DrainDetectorData(pipe_t &pipe,TXMLEngine* engine,
   XMLNodePointer_t index)
{
   pipe.fZetaLen=atof(engine->GetNodeContent(index));
   index=engine->GetNext(index);
   pipe.fPipeRad=atof(engine->GetNodeContent(index));
   index=engine->GetNext(index);
   TString materialname=engine->GetAttr(index,"type");
   TString materialtype=engine->GetAttr(index,"title");
   pipe.fMaterial=TMaterial(materialname.Data(),materialtype.Data(),
      engine->GetIntAttr(index,"A"),
      engine->GetIntAttr(index,"Z"),
      atof(engine->GetAttr(index,"density")),
      atof(engine->GetAttr(index,"radl")),
      atof(engine->GetAttr(index,"inter")));
   pipe.fThickness=atof(engine->GetNodeContent(index));

   return index;
}

Double_t ThetaFromEta(TH1F* etahist)
{
   return 2*TMath::ATan(TMath::Exp(-etahist->GetRandom()));
}
