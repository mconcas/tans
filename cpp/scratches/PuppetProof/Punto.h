#ifndef PUNTO_H
#define PUNTO_H

#include "TObject.h"

class Punto : public TObject
{

public:

Punto();
Punto(Double_t X, Double_t Y, Double_t Z);

virtual ~Punto();

 Double_t GetX() const {return fX;}
 Double_t GetY() const {return fY;}
 Double_t GetZ() const {return fZ;}
 // Int_t GetM() const {return fM}


private:


Double_t fX;
Double_t fY;
Double_t fZ;
// Int_t fM;

ClassDef(Punto,1)
};


#endif
