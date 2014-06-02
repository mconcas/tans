#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Retta.h"
#include "Riostream.h"
#include "Punto.h"
#include "TH2F.h"
#include "TRandom3.h"
#include "time.h"
#include "TMath.h"
#define R1 30
#define R2 40
#define R3 70
#endif

void cilindriamo(Int_t eventN){
delete gRandom;
 gRandom = new TRandom3(time(NULL));
Int_t schifo=0;
TH2F *debug1 = new TH2F("Debug1","cilindro1",eventN,-80,80,eventN,-80,80);
 TH2F *debug2 = new TH2F("Debug2","cilindro2",eventN,-80,80,eventN,-80,80);
 TH2F *debug3 = new TH2F("Debug3","cilindro3",eventN,-80,80,eventN,-80,80);
 Punto *origine1 = new Punto("origine1",gRandom->Gaus(0,10),gRandom->Gaus(0,10),gRandom->Gaus(0,53));
 Punto *origine2 = new Punto("origine2",gRandom->Gaus(0,10),gRandom->Gaus(0,10),gRandom->Gaus(0,53)); 
 Punto *origine3 = new Punto("origine3",gRandom->Gaus(0,10),gRandom->Gaus(0,10),gRandom->Gaus(0,53));
 debug1->Fill(origine1->Get_Punto_X(),origine1->Get_Punto_Y());
 debug1->Fill(origine2->Get_Punto_X(),origine2->Get_Punto_Y());
 debug1->Fill(origine3->Get_Punto_X(),origine3->Get_Punto_Y());
 for(Int_t i =0;i<eventN;i++){
   Double_t thetaphi[2];
   thetaphi[0]=TMath::Pi()*gRandom->Rndm();
   thetaphi[1]=TMath::Pi()*2*gRandom->Rndm(); 
   Double_t m1 = TMath::Cos(thetaphi[1])*TMath::Sin(thetaphi[0]);
   Double_t m2 = TMath::Sin(thetaphi[0])*TMath::Sin(thetaphi[1]); //Estraggo i coseni direttori
   Double_t m3 = TMath::Cos(thetaphi[0]);
   Retta *retta = new Retta("Retta",*origine1,m1,m2,m3);
   Punto *dot1 = retta->Intersecate_Cylinder(R1);
   
   if(!(dot1->Get_Punto_CRadius()==R1)){
     cout<<"//////////////////////////////////////////////////////////\n";
     cout<<"//          Punto che non matcha (Beam)   "<<i+1<<"             //\n";
     cout<<"//////////////////////////////////////////////////////////\n";
     schifo++;
     //retta->Dump();
     //dot1->Dump();
   }
   else debug1->Fill(dot1->Get_Punto_X(),dot1->Get_Punto_Y());
   Punto *dot2 = retta->Intersecate_Cylinder(R2);
   if(!(dot2->Get_Punto_CRadius()==R2)){
     cout<<"//////////////////////////////////////////////////////////\n";
     cout<<"//          Punto che non matcha (Layer1)   "<<i+1<<"           //\n";
     cout<<"//////////////////////////////////////////////////////////\n";
     schifo++;
     //retta->Dump();
     //dot2->Dump();
   }
   else debug2->Fill(dot2->Get_Punto_X(),dot2->Get_Punto_Y());
   Punto *dot3 = retta->Intersecate_Cylinder(R3);
   if(!(dot3->Get_Punto_CRadius()==R3)){
     cout<<"//////////////////////////////////////////////////////////\n";
     cout<<"//          Punto che non matcha (Layer2)   "<<i+1<<"           //\n";
     cout<<"//////////////////////////////////////////////////////////\n";
     schifo++;
     //retta->Dump();
     //dot3->Dump();
   }
   else debug3->Fill(dot3->Get_Punto_X(),dot3->Get_Punto_Y());
   /*Retta *retta2 = new Retta("Retta",*origine2,m1,m2,m3);
   Punto *dot11 = retta2->Intersecate_Cylinder(R1);
   Punto *dot22 = retta2->Intersecate_Cylinder(R2);
   Punto *dot33 = retta2->Intersecate_Cylinder(R3);
   Retta *retta3 = new Retta("Retta",*origine3,m1,m2,m3);
   Punto *dot111 = retta3->Intersecate_Cylinder(R1);
   Punto *dot222 = retta3->Intersecate_Cylinder(R2);
   Punto *dot333 = retta3->Intersecate_Cylinder(R3);*/
   
   
   
   
   
   /*debug1->Fill(dot11->Get_Punto_X(),dot11->Get_Punto_Y());
   debug2->Fill(dot22->Get_Punto_X(),dot22->Get_Punto_Y());
   debug3->Fill(dot33->Get_Punto_X(),dot33->Get_Punto_Y());
   debug1->Fill(dot111->Get_Punto_X(),dot111->Get_Punto_Y());
   debug2->Fill(dot222->Get_Punto_X(),dot222->Get_Punto_Y());
   debug3->Fill(dot333->Get_Punto_X(),dot333->Get_Punto_Y());*/
   
   delete dot1;
   delete dot2;
   delete dot3;
   /*delete dot11;
   delete dot22;
   delete dot33;
   delete dot111;
   delete dot222;
   delete dot333;*/
 }
 debug1->Draw();
 debug2->Draw("same");
 debug3->Draw("same");
 cout<<"*******************   Ci sono "<<schifo<<" punti che non matchano su "<<eventN*3<<" ************************"<<endl;
}
