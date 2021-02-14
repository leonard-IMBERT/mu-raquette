void Analysis() {
  TFile * file = new TFile("build/Results6.root");


  TH1D * EDepByDes = file->Get<TH1D>("EDepByDes");
  TH1D * EMuDetecteur = file->Get<TH1D>("EMuDetecteur");
  TH1D * EMuVetoA = file->Get<TH1D>("EMuVetoA");
  TH1D * EMuVetoB = file->Get<TH1D>("EMuVetoB");

  TH1D * FalsePositive = new TH1D("Faux positif (exemple)", "Faux positif (exemple); Temps(micro s); Energie (MeV)", 2000, 0, 20);
  TH1D * Signal = new TH1D("Temps de desintegration", "Time between two signal (simulation); Time (micro s); Number of events", 200, 0, 20);
  
  event Signals;

  int nS = Signals.Loop();
  int NFP = 0;
  int NTD = 0;
  int NRejected = 0;


  bool aFP = false;
  bool aTD = false;
  for(int ii = 0; ii < nS; ii ++) {
    Signals.GetEntry(ii);
    if(Signals.TrueDecay == false && aFP == false) {
      NFP += 1;
      //std::cout << "FP" << std::endl;
      //aFP = true;
      for(int yy = 0; yy < 2000; yy++) {
        if(Signals.EventsLog[yy] > 0) {
          FalsePositive->AddBinContent(yy+1, Signals.EventsLog[yy]);
        }
      }
    }
    //if(Signals.TrueDecay == true && aTD == false) {
      //std::cout << "TD" << std::endl;
      /* aTD = true;
      for(int yy = 0; yy < 200; yy++) {
        Signal->AddBinContent(yy + 1, Signals.EventsLog[yy]);
        std::cout << Signals.EventsLog[yy] << std::endl;
      } */
    if(Signals.TrueDecay == true) {
      NTD += 1;
      int start = -1;
      int nothing = false;
      int stop = -2;
      for(int yy = 0; yy < 2000; yy++) {
        if(Signals.EventsLog[yy] > 0 && start < 0 && stop < 0 && nothing == false) {
          start = yy;
        }
        if(Signals.EventsLog[yy] == 0 && start >= 0 && stop < 0 && nothing == false) {
          nothing = true;
        }
        if(Signals.EventsLog[yy] > 0 && start >= 0 && stop < 0 && nothing == true) {
          stop = yy;
        }
      }
      if (stop - start >= 0) {
        double t = ((double)(stop - start)) * 0.01;
        Signal->Fill(t);
      } else {
        NRejected += 1;
      }
      std::cout << "\r Nombre d'évenements traités: " << ii;
    }
  }

  std::cout << "Nombre d'entrée : " << nS << std::endl;
  std::cout << "Nombre de faux positifs : " << NFP << std::endl;
  std::cout << "Nombre de vrai signaux : " << NTD << std::endl;
  std::cout << "Nombre de vrai signaux rejetés : " << NRejected << std::endl;
    
  TCanvas * canvasFB = new TCanvas("FP", "Faux positif", 800, 600);
  gPad = canvasFB;
  FalsePositive->Draw();
  canvasFB->Draw();

  TCanvas * canvasTD = new TCanvas("TD", "Desintegration", 800, 600);
  gPad = canvasTD;
  gStyle->SetOptFit(0111);
  Signal->Draw("e");
  Signal->Fit("expo", "e", "", 0.1, 19.9);
  canvasTD->Draw();
}
