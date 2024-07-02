{
 TFile* f=TFile::Open("Tutorial.root");
 TCanvas* c = new TCanvas();
 c->Divide(2,2);
 c->cd(1);
 f->Get("Chamber1")->Draw();
 c->cd(2);
 f->Get("Chamber2")->Draw();
 c->cd(3);
 f->Get("Chamber1_XY")->Draw();
 c->cd(4);
 f->Get("Chamber2_XY")->Draw();
}
