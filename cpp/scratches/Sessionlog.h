//////////////////////////////////////////////////////////////////////
//class Sessionlog: utility class for create a logfile.
//
//Written by Matteo Concas: mett1990@gmail.com 
//Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////

#ifndef SESSIONLOG_H
#define SESSIONLOG_H
#include <TObject.h>

class Sessionlog : public TObject {

   public:
      Sessionlog();
      Sessionlog();
      virtual ~Sessionlog() {};
      TString GetHostname() const { return hostname; }
      

   protected: 
      TString hostname;


}