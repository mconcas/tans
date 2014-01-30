#if !defined (__CINT__) || defined (__MAKECINT__)
#include "Riostream.h"
#include "Punto.h"
#include "Retta.h"
#include "TString.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TPolyLine3D.h"
#define rBP 30
#define rL1 40
#define rL2 70
#define L 164.6
#endif

 //--------------{{Dichiarazione funzioni varie}}------------
Double_t distTheta(Double_t x);
void Scatter(Double_t array[2],TString material);
void Scatter(Double_t array[2],TString material){//non faccio niente e me ne vanto!  
}
Double_t distTheta(Double_t x){ //Pseudorapidità
  Double_t t = 2*TMath::ATan(TMath::Exp(-x));
  return t;
}

  //--------------[Genera Vertice]------------------------
Punto* Genera_Vertex();
Punto* Genera_Vertex(){
  //------------[Coordinate vertice]-----------------------
  Double_t vX = gRandom->Gaus(0,0.1);
  Double_t vY = gRandom->Gaus(0,0.1);
  Double_t vZ = gRandom->Gaus(0,53);
     
  Punto* vertex = new Punto("VERTICE",vX,vY,vZ);
  return vertex;
}


void Genera_Tutto(Int_t nEvents){  
  delete gRandom;
  gRandom = new TRandom3(1234); //cambiare il seed col tempo!!!!!!!!!!!
  //--------------[Dist di pseudorapidità da file <kinem.root> ]--------------
TFile f("kinem.root");
TH1F* disteta = (TH1F*)f.Get("heta");
TH1F* distmult = (TH1F*)f.Get("hmul");
disteta->SetDirectory(0);
distmult->SetDirectory(0);
f.Close();
Int_t b1 = disteta->FindBin(-2); //eta
Int_t b2 = disteta->FindBin(2);
Int_t no_bins = b2-b1+1;
//Int_t nEvents = 10000;
//QUI VADO A DEFINIRE IL COUNTER DI DEBUG!!!!!!!!!!!!!!!!!!!
Int_t counter =0;

  //--------------[Istogramma da cui pescare gli <eta>]---------------
TH1F* disteta2 = new TH1F("Eta","test",no_bins,-2,2);
  //-----------[Riempio con punti secondo la distribuzione <disteta>]-------
  for(Int_t i=1;i<=no_bins;i++){
    disteta2->SetBinContent(i,disteta->GetBinContent(b1-1+i));
  }

  //------------[Loop sul numero di eventi]---------- 
  Double_t thetaphi[2];
  TH2F *debug = new TH2F("Cilindri","",nEvents,-80.,80.,nEvents,-80.,80.);
  TH2F *debug2 = new TH2F("Cilindri","",nEvents,-80.,80.,nEvents,-80.,80.);
  TH2F *debug3 = new TH2F("Cilindri","",nEvents,-80.,80.,nEvents,-80.,80.);
//SPOSTO LA GENERAZIONE DEL VERTICE
  Punto* vertice = Genera_Vertex();
  for(Int_t i=0;i<nEvents;i++){
    Double_t spit =1.;
    do{
    cout<<"angoli: \n"<<thetaphi[0]<<endl<<thetaphi[1]<<endl;
    spit = gRandom->Rndm();
    }//scarpa di Phi
    while(spit==1);
    thetaphi[0] = distTheta(disteta2->GetRandom()); //Theta
    thetaphi[1] = spit*TMath::Pi()*2; //Phi  
    cout<<"Angoli: " << thetaphi[0] << " " << thetaphi[1] << endl;
    Double_t m1 = TMath::Cos(thetaphi[1])*TMath::Sin(thetaphi[0]);
    Double_t m2 = TMath::Sin(thetaphi[0])*TMath::Sin(thetaphi[1]); //Estraggo i coseni direttori
    Double_t m3 = TMath::Cos(thetaphi[0]);
    cout<<m1<<endl<<m2<<endl<<m3<<endl;
    
    Retta line = Retta("Event",*vertice,m1,m2,m3);
    //line.Dump();
    Punto* point = line.Intersecate_Cylinder(rBP); // BeamPipe , /!\ ATTENZIONE, distruggilo!  /!\"
    line.~Retta();//-=-=-=-=[Distruggo]=-=-=-=-=-
    
    //Controllo della coerenza dell'hit!!
    
    if(TMath::Abs(point->Get_Punto_Z())<(L/2)){
      //Scatter(thetaphi,"Beryllium"); //ridefinisco gli angoli
      Double_t m11 = TMath::Cos(thetaphi[1])*TMath::Sin(thetaphi[0]);
      Double_t m22 = TMath::Sin(thetaphi[0])*TMath::Sin(thetaphi[1]); //Estraggo i coseni direttori
      Double_t m33 = TMath::Cos(thetaphi[0]);
      Retta lineBP = Retta("scattered_1",*point,m11,m22,m33);
      //lineBP.Dump();
      Punto* point2 = lineBP.Intersecate_Cylinder(rL1);
      point2->Set_Punto_Name("Hit2");
      //Controllo sul primo rivelatore
      if(TMath::Abs(point2->Get_Punto_Z())<(L/2)){
	//Scatter(thetaphi,"Silicon"); //ridefinisco gli angoli
	Double_t m111 = TMath::Cos(thetaphi[1])*TMath::Sin(thetaphi[0]);
	Double_t m222 = TMath::Sin(thetaphi[0])*TMath::Sin(thetaphi[1]); //Estraggo i coseni direttori
	Double_t m333 = TMath::Cos(thetaphi[0]);
	Retta lineL1 = Retta("scattered_2",*point2,m111,m222,m333);
	//lineL1.Dump();
	Punto* point3 = lineL1.Intersecate_Cylinder(rL2);
	point3->Set_Punto_Name("Hit3");
	if(TMath::Abs(point3->Get_Punto_Z())<(L/2)){
	  cout<<"E'Fatta\n";
	  counter++;
	  cout<<"Il Counter è a: "<<counter<<endl;
	}// fine controllo sul terzo rivelatore
     
      debug->Fill(point->Get_Punto_X(),point->Get_Punto_Y());
      debug2->Fill(point2->Get_Punto_X(),point2->Get_Punto_Y());
      debug3->Fill(point3->Get_Punto_X(),point3->Get_Punto_Y());
      debug2->SetFillColor(kRed);
      debug3->SetFillColor(kBlue);
      /*point->Dump();,
      point2->Dump(); //DEBUG
      point3->Dump();
      vertice->Dump();
      */TPolyLine3D *g = new TPolyLine3D(3);/*
      thetaphi[0]=thetaphi[1]=0;
      cout<<thetaphi[0]<<"    "<<thetaphi[1]<<endl;
      */
      //TPolyLine3D *d = new TPolyLine3D(2);
      g->SetPoint(1,point->Get_Punto_X(),point->Get_Punto_Y(),point->Get_Punto_Z());
      g->SetPoint(2,point2->Get_Punto_X(),point2->Get_Punto_Y(),point2->Get_Punto_Z());
      g->SetPoint(3,point3->Get_Punto_X(),point3->Get_Punto_Y(),point3->Get_Punto_Z());
      //d->SetPoint(1,point->Get_Punto_X(),point->Get_Punto_Y(),point->Get_Punto_Z());
      //d->SetPoint(2,point3->Get_Punto_X(),point3->Get_Punto_Y(),point3->Get_Punto_Z());
      //d->Draw();
      /*thetaphi[0]=thetaphi[1]=0;
      cout<<thetaphi[0]<<"    "<<thetaphi[1]<<endl;*/
      
      g->Draw("same");
      point->~Punto();
      point2->~Punto();
      point3->~Punto();
      vertice->~Punto();
      }//fine del controllo sul primo rivelatore
    } // fine del controllo sul BeamPipe
  }
  //debug->Draw();
  //debug2->Draw("SAME");
  //debug3->Draw("SAME");
  
}
