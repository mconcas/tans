#if !defined (__CINT__) || defined (__MAKECINT__)
#include "TMath.h"
#include "Riostream.h"
#include "TSystem.h"
#endif

void CartesiantoSpherical(const Double_t &X, const Double_t &Y, 
				const Double_t &Z) 
{
	Double_t Rho, Theta, Phi;
	if ( (Rho=TMath::Sqrt(X*X+Y*Y+Z*Z))!=0 ) {
	   // Theta
	   Theta=TMath::ACos(Z/Rho);
	   // Phi
	   if (X>0.) {
	   	if (Y>0.) Phi=TMath::ATan(Y/X);               // X>0. et Y>0.
	   	else {
	   		if (Y==0.) Phi=0.;                         // X>0. et Y=0.
	   		else Phi=2*TMath::Pi()+TMath::ATan(Y/X);   // X>0. et Y<0.
	   	}
	   }
	   else {
	   	if (X==0.) {
	   		if (Y>0.) Phi=TMath::Pi()/2;               // X=0. et Y>0.
	   		else {
	   			if (Y==0.) Phi=Theta=0.;                // X=0. et Y=0. 
               else Phi=TMath::Pi()+TMath::Pi()/2;     // X=0. et Y<0.
	   	   }
	      }
	   	else { 
	   		if (Y>0) Phi=TMath::Pi()+TMath::ATan(Y/X); // X<0. et Y>0.
	   		else {
	   			if (Y==0.) Phi=TMath::Pi();             // X<0. et Y=0. 
	   			else Phi=TMath::Pi()+TMath::ATan(Y/X);  // X<0. et Y>0.
	   		}
	   	}
	   }
	}
   else Phi=Theta=0;
   // end Theta, Phi
   Printf("Rho= %f, Theta= %f, Phi= %f", Rho, Theta, Phi);
}

void ProvaCtoS(){
	for(Double_t i=0;i<=0.9;i=i+0.01) CartesiantoSpherical(1-i,0+i,0);
	for(Double_t i=0;i<=0.9;i=i+0.01) CartesiantoSpherical(0-i,1-i,0);
	for(Double_t i=0;i<=0.9;i=i+0.01) CartesiantoSpherical(-1+i,0-i,0);
	for(Double_t i=0;i<1;i=i+0.01) CartesiantoSpherical(0+i,-1+i,0);	
}

void CartesiantoCylindrical(const Double_t &X, const Double_t &Y, 
				const Double_t &Z)
{
	Double_t CRho, Zeta, Theta;
	Zeta=Z;
	CRho=TMath::Sqrt(X*X+Y*Y);
	if (X>0.) {
	   	if (Y>0.) Theta=TMath::ATan(Y/X);              //X>0. et Y>0.
	   	else {
	   		if (Y==0.) Theta=0.;                        //X>0. et Y=0.
	   		else Theta=2*TMath::Pi()+TMath::ATan(Y/X);  //X>0. et Y<0.
	   	}
	   }
	else {
	   if (X==0.) {
	   	if (Y>0.) Theta=TMath::Pi()/2;                 //X=0. et Y>0.
	   	else {
	   		if (Y==0.) Theta=0.;                        //X=0. et Y=0. 
            else Theta=TMath::Pi()+TMath::Pi()/2;       //X=0. et Y<0.
	   	}
	   }
	   else { 
	   	if (Y>0) Theta=TMath::Pi()+TMath::ATan(Y/X);   //X<0. et Y>0.
	   	else {
	   		if (Y==0.) Theta=TMath::Pi();               //X<0. et Y=0. 
	   		else Theta=TMath::Pi()+TMath::ATan(Y/X);    //X<0. et Y>0.
	   	}
	   }
	}
	Printf("CRho= %f, Zeta= %f, Theta= %f", CRho, Zeta, Theta);
}
void ProvaCtoC(){
	for(Double_t i=0;i<=0.9;i=i+0.01) CartesiantoCylindrical(1-i,0+i,0);
	for(Double_t i=0;i<=0.9;i=i+0.01) CartesiantoCylindrical(0-i,1-i,0);
	for(Double_t i=0;i<=0.9;i=i+0.01) CartesiantoCylindrical(-1+i,0-i,0);
	for(Double_t i=0;i<1;i=i+0.01) CartesiantoCylindrical(0+i,-1+i,0);	
}