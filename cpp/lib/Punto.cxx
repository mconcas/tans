#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TMath.h>
#include "Punto.h"
#include <TObject.h>
#include <TString.h>
#include <TSystem.h>
#endif

// Redefine zero due to machine precision issues.
const Double_t kZero=1/TMath::Tan(TMath::Pi()/2);
Double_t CorrMachinePrecision(const Double_t value) 
{
   if( value<0 && value - (Int_t)value>-kZero )
   return static_cast<Double_t>(static_cast<Int_t>(value+0.5));
   else {
      if( value>0 && value - (Int_t)value<kZero )
      return static_cast<Double_t>(static_cast<Int_t>(value+0.5));
      else return value;
   }
}

ClassImp(Punto)

//___________{De,Con}structors_______________
Punto::Punto() :
   TObject(),
   fX(0.),
   fY(0.),
   fZ(0.),
   fTheta(0.),
   fPhi(0.),
   fCRadius(0.),
   fSRadius(0.) { }

Punto::Punto(Double_t X, Double_t Y, Double_t Z):
   TObject(),
   fX(X),
   fY(Y),
   fZ(Z) 
{
      CartesiantoSpherical();   // Set Spherical coords.
      CartesiantoCylindrical(); // Set Cylindrical coords.
}

//_________Setters___________________________
void Punto::SetPuntoCartesians(const Double_t X, const Double_t Y,
   const Double_t Z) 
{
      fX=X;
      fY=Y;
      fZ=Z;
      CartesiantoSpherical();
      CartesiantoCylindrical();
}

void Punto::SetPuntoCylindricals(const Double_t Z,
   const Double_t CRadius, const Double_t Phi) 
{
      fZ=Z;
      fPhi=Phi;
      fCRadius=CRadius;
      CylindricaltoCartesian();
      CartesiantoSpherical();
}

void Punto::SetPuntoSphericals(const Double_t Theta,
   const Double_t Phi, const Double_t SRaSRadius) 
{
      fTheta=Theta;
      fPhi=Phi;
      fSRadius=SRaSRadius;
      SphericaltoCartesian();
      CartesiantoCylindrical();
}

void Punto::SetPuntoX(const Double_t X) 
{
   fX=X;
   CartesiantoSpherical();
   CartesiantoCylindrical();
}

void Punto::SetPuntoY(const Double_t Y) 
{
   fY=Y;
   CartesiantoSpherical();
   CartesiantoCylindrical();
}

void Punto::SetPuntoZ(const Double_t Z) 
{
   fZ=Z;
   CartesiantoSpherical();
   CartesiantoCylindrical();
   // Please note that is not necessary to update cylindrical
   // coordinates because Z is the same coord.
}

void Punto::SetPuntoTheta(const Double_t Theta) 
{

   ///////////////////////////////////////////////////////////////////
   //Theta is a spherical parameter. We have to invert the spherical
   //relations due to be effective the cartesian coords update.
   //For some reason one might have to modify just one spherical
   //coordinate. It is fine, if just working with sph coordinates.
   //To keep consistency it is necessary to update the cartesian ones.
   //Then it's possible apply a CartesiantoCylindrical conversion for
   //an update to cylindrical coords.
   ///////////////////////////////////////////////////////////////////

   fTheta=Theta;
   SphericaltoCartesian();
   CartesiantoCylindrical();
}

void Punto::SetPuntoPhi(const Double_t Phi) 
{

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

   fPhi=Phi;
   CylindricaltoCartesian();
   CartesiantoSpherical();
}

void Punto::SetPuntoCRadius(const Double_t CRadius) 
{
   fCRadius=CRadius;
   CylindricaltoCartesian();
   CartesiantoSpherical();
   if (gDebug) Printf("Debug: \n X=%f\n Y=%f\n Z=%f",fX,fY,fZ);
   if (gDebug && fX==0 && fY==0)
      Printf("Warning: Cartesian variables X=Y=0! \
             \n This assignment is probably \
             \n going to make a mess with datamembers! \
             \n Please, check your intentions and the source code!");
}

void Punto::SetPuntoSRadius(const Double_t SRadius) 
{
   fSRadius=SRadius;
   SphericaltoCartesian();
   CartesiantoCylindrical();
   if (gDebug) Printf("Debug: \n X=%f\n Y=%f\n Z=%f",fX,fY,fZ);
   if (gDebug && fX==0 && fY==0 && fZ==0)
      Printf("Warning: Cartesian variables X=Y=Z=0! \
               \n This assignment is probably \
               \n going to make a mess with datamembers! \
               \n Please, check your tasks and the source code!");
}

//___________Coordinates_Tools__________
void Punto::CartesiantoSpherical() 
{
   // Update spherical coordinates, managing exceptions and
   // singularities.
   if ((fSRadius=CorrMachinePrecision( TMath::Sqrt(fX*fX+fY*fY+fZ*fZ)
      ))!=0 ) {

      // Theta
      fTheta=TMath::ACos(fZ/fSRadius);

      // Phi
      if (fX>0.) {
         if (fY>0. || fY==0.) fPhi=TMath::ATan(fY/fX);// X>0. et Y>=0.
         else {
            fPhi=2*TMath::Pi()+TMath::ATan(fY/fX);    // X>0. et Y<0.
         }
      }
      else {
         if (fX==0.) {
            if (fY>0.) fPhi=TMath::Pi()/2;             // X=0. et Y>0.
            else {
               if (fY==0.) fPhi=fTheta=0.;             // X=0. et Y=0.
               else fPhi=TMath::Pi()+TMath::Pi()/2;    // X=0. et Y<0.
            }
         } else {
            fPhi=TMath::Pi()+TMath::ATan(fY/fX);       // X<0. et ∀Y
         }
      }
   } else {
      fPhi=fTheta=0.;   // Default value, to avoid non-initialization.
   }
}

void Punto::CartesiantoCylindrical() {
   // Update spherical coordinates, managing exceptions and
   // singularities. Note that the third component [i.e "Z"] is not
   // update, because is the same datamember used as cartesian coord.
   fCRadius=CorrMachinePrecision(TMath::Sqrt(fX*fX+fY*fY));

   // Phi is the azimuthal angle. Please note that is equal to
   // the spherical one.
   if (fX>0.) {
      if (fY>0. || fY==0.) fPhi=TMath::ATan(fY/fX); // X>0. et Y>=0.
      else {
         fPhi=2*TMath::Pi()+TMath::ATan(fY/fX);     // X>0. et Y<0.
      }
   } else {
      if (fX==0.) {
         if (fY>0.) fPhi=TMath::Pi()/2;             // X=0. et Y>0.
         else {
            if (fY==0.) fPhi=fTheta=0.;             // X=0. et Y=0.
            else fPhi=TMath::Pi()+TMath::Pi()/2;    // X=0. et Y<0.
         }
      } else {
         fPhi=TMath::Pi()+TMath::ATan(fY/fX);       // X<0. et ∀Y
      }
   }
}

void Punto::SphericaltoCartesian() {
   fX=fSRadius*TMath::Sin(fTheta)*TMath::Cos(fPhi);
   fY=fSRadius*TMath::Sin(fTheta)*TMath::Sin(fPhi);
   fZ=fSRadius*TMath::Cos(fTheta);
}

void Punto::CylindricaltoCartesian() {
   fX=fCRadius*TMath::Cos(fPhi);
   fY=fCRadius*TMath::Sin(fPhi);
}
