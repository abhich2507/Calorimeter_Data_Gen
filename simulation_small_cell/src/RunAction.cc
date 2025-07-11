


#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace B4
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(const G4String& outputFile)
 : fOutputFile(outputFile)
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  auto analysisManager = G4AnalysisManager::Instance();

  // Create directories
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  // Use the configurable output file name
  analysisManager->OpenFile(fOutputFile);
  
  analysisManager->CreateNtuple("part_info","part_info");
  analysisManager->CreateNtupleDColumn("event_id");
  analysisManager->CreateNtupleDColumn("part_id");
  analysisManager->CreateNtupleDColumn("track_id");
  analysisManager->CreateNtupleDColumn("parent_id");
  analysisManager->CreateNtupleDColumn("mom");
  analysisManager->CreateNtupleDColumn("edepo");
  analysisManager->CreateNtupleDColumn("deltae");
  analysisManager->CreateNtupleDColumn("global_t");
  analysisManager->CreateNtupleDColumn("cublet_idx");
  analysisManager->CreateNtupleDColumn("cell_in_cub");
  // analysisManager->CreateNtupleDColumn("pos_x");
  // analysisManager->CreateNtupleDColumn("pos_y");
  // analysisManager->CreateNtupleDColumn("pos_z");
  // analysisManager->CreateNtupleDColumn("local_t");
  // analysisManager->CreateNtupleDColumn("picell_idx");
  analysisManager->FinishNtuple(0);
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file using the configurable filename
  analysisManager->OpenFile(fOutputFile); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  auto analysisManager = G4AnalysisManager::Instance();
  //
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}


