//////////////////////////////////////////////////////////////////////
//class Punto: basic geometric class.
//
//Written by Matteo Concas: mett1990@gmail.com
//Exam: «Tecniche di analisi numerica e simulazione»
//////////////////////////////////////////////////////////////////////

#ifndef PUNTO_H
#define PUNTO_H
#include <TObject.h>

Double_t CorrMachinePrecision(const Double_t value);
class Punto : public TObject {

   public:
      //____________{Con,Des}tructors____________
      Punto();
      Punto(Double_t X, Double_t Y, Double_t Z);
      virtual ~Punto() { };

      //____________Setters______________________
      void SetPuntoCartesians(const Double_t X, const Double_t Y,
         const Double_t Z);
      void SetPuntoCylindricals(const Double_t Z,
         const Double_t CRadius, const Double_t Phi);
      void SetPuntoSphericals(const Double_t Theta,
         const Double_t Phi, const Double_t CRadius);
      void SetPuntoX(const Double_t X);
      void SetPuntoY(const Double_t Y);
      void SetPuntoZ(const Double_t Z);
      void SetPuntoTheta(const Double_t Theta);
      void SetPuntoPhi(const Double_t Phi);
      void SetPuntoCRadius(const Double_t CRadius);
      void SetPuntoSRadius(const Double_t SRadius);

      //____________Inline_Getters_______________
      Double_t GetPuntoX() { return fX; }
      Double_t GetPuntoY() { return fY; }
      Double_t GetPuntoZ() { return fZ; }
      Double_t GetPuntoTheta() { return fTheta; }
      Double_t GetPuntoPhi() { return fPhi; }
      Double_t GetPuntoSRadius() { return fSRadius; }
      Double_t GetPuntoCRadius() { return fCRadius; }

   private:

   //___________Special_Setters_______________
      void CartesiantoCylindrical();
      void CartesiantoSpherical();
      void SphericaltoCartesian();
      void CylindricaltoCartesian();

   protected:
      Double_t fX;        // Cartesian coordinate "X"
      Double_t fY;        // Cartesian coordinate "Y"
      Double_t fZ;        // Cartesian coordinate "Z"
      Double_t fTheta;    // Spherical coordinate "ϑ"
      Double_t fPhi;      // Spherical and Cylindrical coordinate "φ"
      Double_t fCRadius;  // Cylindrical Radius
      Double_t fSRadius;  // Spherical Radius

      ClassDef(Punto,1)
};
#endif
