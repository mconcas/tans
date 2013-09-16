void ent() {

  TString fn;
  Int_t sum = 0;
  Punto *p = new Punto();

  for (Int_t i=0; i<=9; i++) {
    fn.Form("htree_%03d.root", i);
    TFile *f = TFile::Open(fn.Data());
    TTree *t = (TTree *)( f->Get("T") );
    t->SetBranchAddress("VertMult", &p);
    Int_t e = (Int_t)(t->GetEntries());
    for (Int_t j=0; j<e; j++) {
      t->GetEntry(j);
      sum += p->GetMult();
    }
  }

  Printf("SUM: %d", sum);

}
