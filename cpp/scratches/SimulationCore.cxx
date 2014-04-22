#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TXMLEngine.h>
#include <TObject.h>
#include "SimulationCore.h"
#endif

XMLNodePointer_t FromChildToNextParent(TXMLEngine* engine, 
   XMLNodePointer_t index) 
{
   index=engine->GetParent(index);
   index=engine->GetNext(index);

   return index;
}

XMLNodePointer_t DrainDetectorData(pipe_t &pipe, TXMLEngine* engine,
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

ClassImp(SimulationCore)

//____________{Con,Des}tructors____________
SimulationCore::SimulationCore() :
   TObject(),
   fXMLEngine() { fParDataFileName=""; }

SimulationCore::SimulationCore(TString Pardatafile) :
   TObject(),
   fParDataFileName(Pardatafile),
   fXMLEngine() { }


void SimulationCore::SetParamFile(TString Filename) 
{
   fParDataFileName=Filename;
}

Int_t SimulationCore::Initialize()
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
      fMultDist=fXMLEngine->GetNodeContent(fIndexNodeptr);
   } else {
      Printf("Error: multiplicity histogram not specified.");
      return -1;
   }
   fIndexNodeptr=fXMLEngine->GetNext(fIndexNodeptr);
   if((TString)fXMLEngine->GetAttrValue(fXMLEngine->GetFirstAttr(fIndexNodeptr))
      =="distribution") {
      fEtaDist=fXMLEngine->GetNodeContent(fIndexNodeptr);
   } else {
      Printf("Error: η histogram not specified.");
      return -1;
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
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);
   fIndexNodeptr=DrainDetectorData(fBeampipe,fXMLEngine,fIndexNodeptr);
   fIndexNodeptr=FromChildToNextParent(fXMLEngine,fIndexNodeptr);
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);
   fIndexNodeptr=DrainDetectorData(fFirstLayer,fXMLEngine,fIndexNodeptr);
   fIndexNodeptr=FromChildToNextParent(fXMLEngine,fIndexNodeptr);
   fIndexNodeptr=fXMLEngine->GetChild(fIndexNodeptr);
   fIndexNodeptr=DrainDetectorData(fSecondLayer,fXMLEngine,fIndexNodeptr);
   fIndexNodeptr=FromChildToNextParent(fXMLEngine,fIndexNodeptr);
   if((TString)fXMLEngine->GetAttr(fIndexNodeptr,"dryrun")=="kFALSE")
      fDryRun=kFALSE;
   else fDryRun=kTRUE;
   
   if((TString)fXMLEngine->GetAttr(fIndexNodeptr,"montecarlo")=="kFALSE") 
      fMontecarloTruth=kFALSE;
      else fMontecarloTruth=kTRUE;
   fOutFileName=(TString)fXMLEngine->GetNodeContent(fIndexNodeptr);
   
   return 0; 
}