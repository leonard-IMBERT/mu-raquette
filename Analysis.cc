void Analysis() {
  TFile * file = new TFile("build/Results.root");
  TH1D * HMuEDep = file->Get<TH1D>("HMuEDep");
  TH1D * HGEDep = file->Get<TH1D>("HGEDep");
  TH1D * HTotEDep = file->Get<TH1D>("HTotEDep");

  TCanvas * canvas = new TCanvas("HMuEDep", "Energy deposited by muons", 800, 600);

  const Int_t nBin = 300;
  const Int_t maxE = 65;

  Double_t x[nBin], siBruit[nBin], eff[nBin];

  for(Int_t ii = 0; ii < nBin; ii++) {
    x[ii] = (Double_t)ii * (Double_t)maxE / (Double_t)nBin;
    eff[ii] = (Double_t)HMuEDep->Integral(ii, nBin-1) / (Double_t)HMuEDep->Integral(0, nBin - 1);
    if(HTotEDep->GetBinContent(ii) == 0) {
      siBruit[ii] = 1;
    } else {
      siBruit[ii] = (Double_t)HMuEDep->GetBinContent(ii) / (Double_t)HTotEDep->GetBinContent(ii);
    }
  }

  
  TGraph * EffMuon = new TGraph(nBin, x, eff);
  EffMuon->SetTitle("Efficacite de detection des muons en fonction du seuil;Energie seuil (MeV)");
  EffMuon->SetMarkerColor(kBlue);
  EffMuon->SetMarkerStyle(kFullDiamond);
  EffMuon->SetMarkerSize(0.6);

  TGraph * SignalBruit = new TGraph(nBin, x, siBruit);
  SignalBruit->SetTitle("Rapport Signal muons sur signal total;Energie (MeV)");
  SignalBruit->SetMarkerColor(kRed);
  SignalBruit->SetMarkerStyle(kFullTriangleUp);
  SignalBruit->SetMarkerSize(0.6);

  TMultiGraph * mg = new TMultiGraph();
  mg->Add(EffMuon);
  mg->Add(SignalBruit);
  mg->SetTitle("Efficacite de detection des muons et rapport signal muons sur signal total; Energie(MeV)");
  mg->GetYaxis()->SetNdivisions(1010);
  mg->GetYaxis()->SetRangeUser(0, 1);
  mg->GetXaxis()->SetRangeUser(0, 65);

  Double_t plateau = HMuEDep->Integral(0, 40);
  std::cout<< " Nombre de muons dans le plateau; ";
  std::cout<<plateau<<std::endl;

  std::cout<< " Efficacite et rapport a " << (Double_t)maxE / (Double_t)nBin * 37. << " MeV: " << eff[37] << " ; " << siBruit[37] << std::endl;
  std::cout<< " Efficacite et rapport a " << (Double_t)maxE / (Double_t)nBin * 43. << " MeV: " << eff[43] << " ; " << siBruit[43] << std::endl;

  //HSignalSBruit->Draw();
  //EffMuon->Draw("AP");
  //SignalBruit->Draw("P");
  mg->Draw("AP");
  canvas->BuildLegend();
  canvas->Draw();
}
