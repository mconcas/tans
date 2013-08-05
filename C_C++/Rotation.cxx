#include "TMath.h"

/*///////////////////////////////////////////////////
  //           Ancora da Controllare !!!!          //
  /////////////////////////////////////////////////// */

void rotate(Double_t th,Double_t ph,Double_t thp,Double_t php,Double_t *cd){
  Double_t mr[3][3];
  mr[0][0]=-TMath::Sin(ph);
  mr[1][1]=TMath::Cos(ph);
  mr[2][0]=0.;
  mr[0][1]=-TMath::Cos(ph)*TMath::Cos(th);
  mr[1][1]=-TMath::Cos(th)*TMath::Sin(ph);
  mr[2][1]=TMath::Sin(th);
  mr[0][2]=TMath::Sin(th)*TMath::Cos(ph);
  mr[1][2]=TMath::Sin(th)*TMath::Sin(ph);
  mr[2][2]=TMath::Cos(th);
  
  Double_t cdp[3];
  cdp[0]=TMath::Sin(thp)*TMath::Cos(php);
  cdp[1]=TMath::Sin(thp)*TMath::Sin(php);
  cdp[2]=TMath::Cos(thp);
  for(Int_t i=0;i<3;i++){
    cd[i]=0.;
    for(Int_t j=0;j<3;j++){
      cd[i]+=mr[i][j]*cdp[j];
    }
  }
}