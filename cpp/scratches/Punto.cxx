#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TMath.h>
#include "Punto.h"
#include <TObject.h>
#include <TString.h>
#include <TSystem.h>
#endif


const Double_t zero=1/TMath::Tan(TMath::Pi()/2)/*0.00000001*/;
Double_t CorrMachinePrecision(const Double_t fValue) {
   if(fValue-(Int_t)fValue>-zero||fValue-(Int_t)fValue<zero) 
      return static_cast<Double_t>(static_cast<Int_t>(fValue+0.5));
   else return fValue;
}

ClassImp(Punto)

//___________{De,Con}structors_______________
Punto::Punto() : TObject(),
   X(0.),
   Y(0.),    
   Z(0.),
   Theta(0.),
   Phi(0.),
   CRadius(0.),
   SRadius(0.) { }

Punto::Punto(Double_t fX, Double_t fY, Double_t fZ): 
   TObject(),
   X(fX),
   Y(fY),
   Z(fZ) { 
      CartesiantoSpherical();   // Set Spherical coords.
      CartesiantoCylindrical(); // Set Cylindrical coords.
   }

//_________Setters___________________________
void Punto::SetPuntoCartesians(const Double_t fX, const Double_t fY, 
   const Double_t fZ) {
      X=fX;
      Y=fY;
      Z=fZ;
      CartesiantoSpherical();
      CartesiantoCylindrical();
}

void Punto::SetPuntoCylindricals(const Double_t fZ, 
   const Double_t fCRadius, const Double_t fPhi) {
      Z=fZ;
      Phi=fPhi;
      CRadius=fCRadius;
      CylindricaltoCartesian();
      CartesiantoSpherical();
}

void Punto::SetPuntoSphericals(const Double_t fTheta, 
   const Double_t fPhi, const Double_t fSRadius) {
      Theta=fTheta;
      Phi=fPhi;
      SRadius=fSRadius;
      SphericaltoCartesian();
      CartesiantoCylindrical();
}

void Punto::SetPuntoX(const Double_t fX) {
   X=fX;
   CartesiantoSpherical();
   CartesiantoCylindrical();
}

void Punto::SetPuntoY(const Double_t fY) {
   Y=fY;
   CartesiantoSpherical();
   CartesiantoCylindrical();
}

void Punto::SetPuntoZ(const Double_t fZ) {
   Z=fZ;
   CartesiantoSpherical();
   CartesiantoCylindrical(); 
   // Please note that is not necessary to update cylindrical 
   // coordinates because Z is the same coord.    
}

void Punto::SetPuntoTheta(const Double_t fTheta) {

   ///////////////////////////////////////////////////////////////////
   //Theta is a spherical parameter. We have to invert the spherical 
   //relations due to be effective the cartesian coords update. 
   //For some reason one might have to modify just one spherical 
   //coordinate. It is fine, if just working with sph coordinates. 
   //To keep consistency it is necessary to update the cartesian ones.
   //Then it's possible apply a CartesiantoCylindrical conversion for 
   //an update to cylindrical coords.
   ///////////////////////////////////////////////////////////////////
   Theta=fTheta;
   SphericaltoCartesian();
   CartesiantoCylindrical();
}

void Punto::SetPuntoPhi(const Double_t fPhi) {

   ///////////////////////////////////////////////////////////////////
   //fPhi is a spherical and a cylindrical parameter. 
   //We have to invert the cylindrical relations due to make effective 
   //the cartesian coords update. 
   //For some reason one might have to modify just a cylindrical 
   //coordinate. It is fine, if just working with cyl coordinates. 
   //To keep consistency it is necessary to update the cartesian ones.
   //Then it's possible apply a CartesiantoCylindrical conversion for 
   //an update to cylindrical coords.
   ///////////////////////////////////////////////////////////////////
   Phi=fPhi;
   CylindricaltoCartesian();
   CartesiantoSpherical();
}

void Punto::SetPuntoCRadius(const Double_t fCRadius) {
   CRadius=fCRadius;
   if (gDebug && X==0 && Y==0) 
      Printf("Warning: Cartesian variables X=Y=0! \
               \nThis assignment is probably \
               \ngoing to make a mess with datamembers!\\n\
               \nPlease, check your intentions and the source code!");
   CylindricaltoCartesian();
   CartesiantoSpherical();
   if (gDebug) Printf("Debug: \n X=%f\n Y=%f\n Z=%f",X,Y,Z); 
}

void Punto::SetPuntoSRadius(const Double_t fSRadius) {
   SRadius=fSRadius;
   if (gDebug && X==0 && Y==0 && Z==0) 
      Printf("Warning: Cartesian variables X=Y=Z=0! \
               \nThis assignment is probably \
               \ngoing to make a mess with datamembers!\n\
               \nPlease, check your tasks and the source code!");
   SphericaltoCartesian();
   CartesiantoCylindrical();
   if (gDebug) Printf("Debug: \n X=%f\n Y=%f\n Z=%f",X,Y,Z);
}

//___________Coordinates_Tools__________
void Punto::CartesiantoSpherical() { 
   /* Update spherical coordinates, managing exceptions and 
   singularities.*/

   if ( (SRadius=TMath::Sqrt(X*X+Y*Y+Z*Z))!=0 ) {

      // Theta
      Theta=TMath::ACos(Z/SRadius);

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
               else Phi=TMath::Pi()+TMath::ATan(Y/X);  // X<0. et Y<0.
            }
         }
      }
   }
   else {
      Phi=0.;
      Theta=0.;
   }
}

void Punto::CartesiantoCylindrical() { 
   /* Update spherical coordinates, managing exceptions and 
   singularities. Note that the third component [i.e "Z"] is not
   update, because is the same datamember used as cartesian coord.*/
   
   CRadius=CorrMachinePrecision(TMath::Sqrt(X*X+Y*Y));
   /* Phi is the angular coordinate. Please note that is equal to 
   the spherical one. */
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

void Punto::SphericaltoCartesian() {
   X=SRadius*TMath::Sin(Theta)*TMath::Cos(Phi);
   Y=SRadius*TMath::Sin(Theta)*TMath::Sin(Phi);
   Z=SRadius*TMath::Cos(Theta);
}

void Punto::CylindricaltoCartesian() {
   X=CRadius*TMath::Cos(Phi);
   Y=CRadius*TMath::Sin(Phi);
}
