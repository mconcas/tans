//////////////////////////////////////////////////////////////////////
// class Sessionlog: utility class for session debugging, prifiling, 
// etc.
// Written by Matteo Concas: mett1990@gmail.com 
// Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////

#ifndef SESSIONLOG_H
#define SESSIONLOG_H
#include <TObject.h>
#include <TStopwatch.h>
#include <TSystem.h>
#include <TFile.h>

class Sessionlog : public TObject {

   public:
      Sessionlog();
      Sessionlog(Int_t fGenvertices);
      virtual ~Sessionlog() {};

      //____________Inline_Getters_______________
      TString  GetHostname() const { return hostname; }
      TString  GetCpuType()  const { return systeminfo.fCpuType; }
      TString  GetModel()    const { return systeminfo.fModel; }
      Int_t    GetCpuSpeed() const { return systeminfo.fCpuSpeed; }

      //____________Getters______________________
      Double_t GetRealTime(); 
      Double_t GetCpuTime();        
      TFile    GetSessionLogFile(TString fFilename);

      //____________Setters______________________
      void ResetSysInfo();
      

   protected: 
      Int_t        genvertices; // nuber of generated vertices.
      TString      hostname;    // hostname
      SysInfo_t    systeminfo;  // structt containing useful infos.
      TStopwatch   timer;       // timer object.
      
      ClassDef(Sessionlog,1)
};
#endif