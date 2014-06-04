//////////////////////////////////////////////////////////////////////
// 
// Class Punto: basic geometric class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PUNTO_H
#define PUNTO_H
#include <TObject.h>

Double_t CorrMachinePrecision(const Double_t value);
class Punto : public TObject {

   public:
   
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

      //_____________Getters_______________
      Double_t GetPuntoX() const {return fX;}
      Double_t GetPuntoY() const {return fY;}
      Double_t GetPuntoZ() const {return fZ;}
      Double_t GetPuntoTheta() const {return fTheta;}
      Double_t GetPuntoPhi() const {return fPhi;}
      Double_t GetPuntoSRadius() const {return fSRadius;}
      Double_t GetPuntoCRadius() const {return fCRadius;}
      static Double_t GetDistance(Punto &PointOne, Punto &PointTwo);
      static Double_t GetDeltaPhi(Punto &FirstPunto, Punto &SecondPunto);

   private:
      void CartesiantoCylindrical();
      void CartesiantoSpherical();
      void SphericaltoCartesian();
      void CylindricaltoCartesian();

   protected:
      Double_t fX;                   // Cartesian coordinate "X"
      Double_t fY;                   // Cartesian coordinate "Y"
      Double_t fZ;                   // Cartesian coordinate "Z"
      Double_t fTheta;               // Spherical coordinate "ϑ"
      Double_t fPhi;                 // Spherical and Cylindrical coordinate "φ"
      Double_t fCRadius;             // Cylindrical Radius
      Double_t fSRadius;             // Spherical Radius

      ClassDef(Punto,1)
};
#endif
