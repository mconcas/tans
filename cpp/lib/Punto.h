//////////////////////////////////////////////////////////////////////
//class Punto: basic geometric class.
//
//Written by Matteo Concas: mett1990@gmail.com 
//Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////

#ifndef PUNTO_H
#define PUNTO_H
#include <TObject.h>

Double_t CorrMachinePrecision(const Double_t fValue);
class Punto : public TObject {
   
   public:
      //____________{Con,Des}tructors____________
      Punto();
      Punto(Double_t fX, Double_t fY, Double_t fZ);
      virtual ~Punto() { };
      
      //____________Setters______________________  
      void SetPuntoCartesians(const Double_t fX, const Double_t fY, 
         const Double_t fZ);
      void SetPuntoCylindricals(const Double_t fZ,
         const Double_t fCRadius, const Double_t fPhi);
      void SetPuntoSphericals(const Double_t fTheta,
         const Double_t fPhi, const Double_t fCRadius);
      void SetPuntoX(const Double_t fX);
      void SetPuntoY(const Double_t fY);
      void SetPuntoZ(const Double_t fZ);
      void SetPuntoTheta(const Double_t fTheta);
      void SetPuntoPhi(const Double_t fPhi);
      void SetPuntoCRadius(const Double_t fCRadius);
      void SetPuntoSRadius(const Double_t fSRadius);
  
      //____________Inline_Getters_______________
      Double_t GetPuntoX() const { return X; }
      Double_t GetPuntoY() const { return Y; }
      Double_t GetPuntoZ() const { return Z; }
      Double_t GetPuntoTheta() const { return Theta; }
      Double_t GetPuntoPhi() const { return Phi; }
      Double_t GetPuntoSRadius() const { return SRadius; }
      Double_t GetPuntoCRadius() const { return CRadius; }
   
   private:

   //___________Special_Setters_______________
      void CartesiantoCylindrical();
      void CartesiantoSpherical();
      void SphericaltoCartesian();
      void CylindricaltoCartesian();   
   
   protected:
      Double_t X;        // Cartesian coordinate "X"
      Double_t Y;        // Cartesian coordinate "Y"
      Double_t Z;        // Cartesian coordinate "Z"
      Double_t Theta;    // Spherical coordinate "ϑ"
      Double_t Phi;      // Spherical and Cylindrical coordinate "φ"
      Double_t CRadius;  // Cylindrical Radius
      Double_t SRadius;  // Spherical Radius

      ClassDef(Punto,1)        
};
#endif

