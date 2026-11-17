
void edep(const char* input  = "/work/eic/users/aabhishe/HCAL_DATA/simulation_small_cell/data/proton100GeV.root",
          const char* output = "/work/eic/users/aabhishe/HCAL_DATA/Plots/energy_deposition.pdf")
{
  TFile *f = TFile::Open(input,"READ");
  if (!f || f->IsZombie()) {
    std::cerr << "Cannot open " << input << std::endl;
    return;
  }
  TTree *t = (TTree*)f->Get("part_info");
  if (!t) {
    std::cerr << "Tree \"part_info\" not found in " << input << std::endl;
    return;
  }

  double edepo;
  double event_id;
  t->SetBranchAddress("edepo", &edepo);
  t->SetBranchAddress("event_id", &event_id);

  // Histogram of the TOTAL energy deposited per event (sum over all its steps).
  // edepo is in MeV (Geant4 internal units), so 0-100000 MeV = 0-100 GeV.
  TH1D *h1 = new TH1D("h1", "Energy deposition per event;E_{dep} (MeV);Events", 100, 0, 100000);

  Long64_t nEntries = t->GetEntries();
  double currentEvent = -1;
  double eventTotal = 0;
  Long64_t nEvents = 0;

  for (Long64_t i = 0; i < nEntries; i++)
  {
    t->GetEntry(i);
    if (event_id != currentEvent) {
      // New event: store the previous event's total.
      if (currentEvent >= 0) {
        h1->Fill(eventTotal);
        nEvents++;
      }
      currentEvent = event_id;
      eventTotal = 0;
    }
    eventTotal += edepo;
  }
  // Flush the last event.
  if (nEntries > 0) {
    h1->Fill(eventTotal);
    nEvents++;
  }

  std::cout << "Processed " << nEvents << " events from " << nEntries
            << " steps; mean E_dep = " << h1->GetMean() << " MeV" << std::endl;

  TCanvas *c1 = new TCanvas("c1", "Energy Deposition", 800, 600);
  h1->Draw();
  c1->SetLogy();
  gSystem->mkdir(gSystem->DirName(output), kTRUE);
  c1->SaveAs(output);
}
  