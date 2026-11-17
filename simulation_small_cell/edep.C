
#include <map>

void edep(const char* input  = "/work/eic/users/aabhishe/Calorimeter_Data_Gen/simulation_small_cell/Data/pion100.root",
          const char* output = "/work/eic/users/aabhishe/Calorimeter_Data_Gen/simulation_small_cell/edep_histogram_pi.png")
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

  // Sum edepo per event using a map keyed on event_id. This is robust even
  // in multithreaded runs, where Geant4 interleaves rows from different
  // events in the merged ntuple: a sequential boundary check would split
  // each event into fragments and give wrongly small sums.
  std::map<Long64_t, double> edepPerEvent;
  Long64_t nEntries = t->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++)
  {
    t->GetEntry(i);
    edepPerEvent[(Long64_t)event_id] += edepo;
  }

  // Histogram of the TOTAL energy deposited per event.
  // edepo is in MeV (Geant4 internal units), so 0-100000 MeV = 0-100 GeV.
  TH1D *h1 = new TH1D("h1", "Energy deposition per event;E_{dep} (MeV);Events", 100, 0, 100000);
  for (const auto& ev : edepPerEvent) {
    h1->Fill(ev.second);
  }

  std::cout << "Processed " << edepPerEvent.size() << " events from " << nEntries
            << " steps; mean E_dep = " << h1->GetMean() << " MeV" << std::endl;

  TCanvas *c1 = new TCanvas("c1", "Energy Deposition", 800, 600);
  h1->Draw();
  c1->SetLogy();
  gSystem->mkdir(gSystem->DirName(output), kTRUE);
  c1->SaveAs(output);
}
  