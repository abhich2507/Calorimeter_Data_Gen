

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

namespace B4
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_PbWO4");//PbWO4

  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;
  G4double density;
 
  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters of the calorimeter
  G4double cellSizeXY =  3.*mm;
  G4double cellSizeZ  = 6.*mm;
  G4int    nCellsXY = 100;
  G4int    nCellsZ  = 200;
  G4double calorSizeXY = nCellsXY * cellSizeXY;
  G4double calorSizeZ  = nCellsZ  * cellSizeZ;
  auto worldSizeXY = calorSizeXY * 1.1;
  auto worldSizeZ  = calorSizeZ  * 1.1;

  // Get materials
  auto defaultMaterial = G4Material::GetMaterial("Galactic");
  auto detectorMaterial = G4Material::GetMaterial("G4_PbWO4");//PbWO4

  if ( ! defaultMaterial || ! detectorMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }

  //
  // World
  //
  auto worldS = new G4Box("World", worldSizeXY/2, worldSizeXY/2, worldSizeZ/2);

  auto worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");

  auto worldPV = new G4PVPlacement(
    0,                                // no rotation 
    G4ThreeVector(0.,0.,0.),          // at (0,0,0)
    worldLV,                          // its logical volume
    "World",                          // its name
    0,                                // its mother  volume
    false,                            // no boolean operation
    0,                                // copy number
    false                             // checking overlaps
  );


  auto solidDetector=new G4Box("solidDetector", cellSizeXY/2, cellSizeXY/2, cellSizeZ/2);
  auto logicDetector= new G4LogicalVolume(solidDetector,detectorMaterial,"logicDetector");

  //
  // The calorimeter is voxelised as 3 nested replicas:
  //   Z: nCellsZ slabs -> X: nCellsXY rods -> Y: nCellsXY cells
  // This replaces the previous nCellsXY*nCellsXY*nCellsZ explicit G4PVPlacement
  // loop (2,000,000 physical volumes) with three lightweight G4PVReplica
  // objects, making detector construction orders of magnitude faster and far
  // less memory-hungry.
  //
  auto solidCalor = new G4Box("solidCalorimeter", calorSizeXY/2, calorSizeXY/2, calorSizeZ/2);
  auto logicCalor = new G4LogicalVolume(solidCalor, defaultMaterial, "Calorimeter");
  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicCalor, "Calorimeter",
                    worldLV, false, 0, false);

  // Z slices: full transverse size, one cell thick along z
  auto solidSlab = new G4Box("solidSlab", calorSizeXY/2, calorSizeXY/2, cellSizeZ/2);
  auto logicSlab = new G4LogicalVolume(solidSlab, defaultMaterial, "Slab");
  new G4PVReplica("ReplicaZ", logicSlab, logicCalor, kZAxis, nCellsZ, cellSizeZ);

  // X rods: one cell wide in x, full y, one cell thick along z
  auto solidRod = new G4Box("solidRod", cellSizeXY/2, calorSizeXY/2, cellSizeZ/2);
  auto logicRod = new G4LogicalVolume(solidRod, defaultMaterial, "Rod");
  new G4PVReplica("ReplicaX", logicRod, logicSlab, kXAxis, nCellsXY, cellSizeXY);

  // Y cells: one cell in x, one cell in y, one cell thick along z
  new G4PVReplica("ReplicaY", logicDetector, logicRod, kYAxis, nCellsXY, cellSizeXY);

  //
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());
  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  //
  // Always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

} // end namespace

