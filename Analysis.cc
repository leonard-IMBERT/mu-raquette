void Analysis() {
  TFile * file = new TFile("build/Results.root");


  TH1D * EDepByDes = file->Get<TH1D>("EDepByDes");
  TH1D * EMuDetecteur = file->Get<TH1D>("EMuDetecteur");
  TH1D * EMuVetoA = file->Get<TH1D>("EMuVetoA");
  TH1D * EMuVetoB = file->Get<TH1D>("EMuVetoB");



  TCanvas * canvas = new TCanvas("EDepByDes", "Energy deposited  desintegration", 800, 600);
  gPad = canvas;
  EDepByDes->Draw();
  canvas->Draw();
  canvas->Print("images/EDepByDes.png", "png");

  TCanvas * canvasMuDect = new TCanvas("EMuDetecteur", "Energy deposited bu mu- in detector", 800, 600);
  gPad = canvasMuDect;
  EMuDetecteur->Draw();
  canvasMuDect->Draw();
  canvasMuDect->Print("images/EMuDetecteur.png", "png");

  TCanvas * canvasMuVA = new TCanvas("EMuVetoA", "Energy deposited bu mu- in VetoA", 800, 600);
  gPad = canvasMuVA;
  EMuVetoA->Draw();
  canvasMuVA->Draw();
  canvasMuVA->Print("images/EMuVetoA.png", "png");

  TCanvas * canvasMuVB = new TCanvas("EMuVetoB", "Energy deposited bu mu- in VetoB", 800, 600);
  gPad = canvasMuVB;
  EMuVetoB->Draw();
  canvasMuVB->Print("images/EMuVetoB.png", "png");
  canvasMuVB->Draw();
}
