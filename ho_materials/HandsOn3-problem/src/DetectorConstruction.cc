//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: DetectorConstruction.cc 77656 2013-11-27 08:52:57Z gcosmo $
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "MagneticField.hh"
#include "CellParameterisation.hh"
#include "HodoscopeSD.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = nullptr;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = nullptr;
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fMessenger(0),
  fHodoscope1Logical(0), fHodoscope2Logical(0),
  fWirePlane1Logical(0), fWirePlane2Logical(0),
  fCellLogical(0), fHadCalScintiLogical(0),
  fMagneticLogical(0),
  fVisAttributes(),
  fArmAngle(30.*deg), fArmRotation(0), fSecondArmPhys(0)

{
    fArmRotation = new G4RotationMatrix();
    fArmRotation->rotateY(fArmAngle);
    
    // define commands for this class
    DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fArmRotation;
    delete fMessenger;
    
    for (G4int i=0; i<G4int(fVisAttributes.size()); ++i) 
    {
      delete fVisAttributes[i];
    }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Construct materials
    ConstructMaterials();
    G4Material* air = G4Material::GetMaterial("G4_AIR");
    //G4Material* argonGas = G4Material::GetMaterial("_Ar");
    G4Material* argonGas = G4Material::GetMaterial("G4_Ar");
    G4Material* scintillator 
      = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    G4Material* csI = G4Material::GetMaterial("G4_CESIUM_IODIDE");
    G4Material* lead = G4Material::GetMaterial("G4_Pb");
    
    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    // geometries --------------------------------------------------------------
    // experimental hall (world volume)
    G4VSolid* worldSolid 
      = new G4Box("worldBox",10.*m,3.*m,10.*m);
    G4LogicalVolume* worldLogical
      = new G4LogicalVolume(worldSolid,air,"worldLogical");
    G4VPhysicalVolume* worldPhysical
      = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,
                          false,0,checkOverlaps);
    
    // Tube with Local Magnetic field
    
    G4VSolid* magneticSolid 
      = new G4Tubs("magneticTubs",0.,1.*m,1.*m,0.,360.*deg);

    fMagneticLogical
      = new G4LogicalVolume(magneticSolid, air, "magneticLogical");

    // placement of Tube
    
    G4RotationMatrix* fieldRot = new G4RotationMatrix();
    fieldRot->rotateX(90.*deg);
    new G4PVPlacement(fieldRot,G4ThreeVector(),fMagneticLogical,
                      "magneticPhysical",worldLogical,
                      false,0,checkOverlaps);
    
    // set step limit in tube with magnetic field  
    G4UserLimits* userLimits = new G4UserLimits(1*m);
    fMagneticLogical->SetUserLimits(userLimits);
    
    // first arm
    G4VSolid* firstArmSolid 
      = new G4Box("firstArmBox",1.5*m,1.*m,3.*m);
    G4LogicalVolume* firstArmLogical
      = new G4LogicalVolume(firstArmSolid,air,"firstArmLogical");
    new G4PVPlacement(0,G4ThreeVector(0.,0.,-5.*m),firstArmLogical,
                      "firstArmPhysical",worldLogical,
                      false,0,checkOverlaps);
    
    // second arm
    G4VSolid* secondArmSolid 
      = new G4Box("secondArmBox",2.*m,2.*m,3.5*m);
    G4LogicalVolume* secondArmLogical
      = new G4LogicalVolume(secondArmSolid,air,"secondArmLogical");
    G4double x = -5.*m * std::sin(fArmAngle);
    G4double z = 5.*m * std::cos(fArmAngle);
    fSecondArmPhys
      = new G4PVPlacement(fArmRotation,G4ThreeVector(x,0.,z),secondArmLogical,
                          "fSecondArmPhys",worldLogical,
                          false,0,checkOverlaps);
    
    // hodoscopes in first arm
    G4VSolid* hodoscope1Solid 
      = new G4Box("hodoscope1Box",5.*cm,20.*cm,0.5*cm);
    fHodoscope1Logical
      = new G4LogicalVolume(hodoscope1Solid,scintillator,"hodoscope1Logical");
    for (G4int i=0;i<15;i++)
    {
        G4double x1 = (i-7)*10.*cm;
        new G4PVPlacement(0,G4ThreeVector(x1,0.,-1.5*m),fHodoscope1Logical,
                          "hodoscope1Physical",firstArmLogical,
                          false,i,checkOverlaps);
    }
    
    // drift chambers in first arm
    G4VSolid* chamber1Solid 
      = new G4Box("chamber1Box",1.*m,30.*cm,1.*cm);
    G4LogicalVolume* chamber1Logical
      = new G4LogicalVolume(chamber1Solid,argonGas,"chamber1Logical");
    for (G4int i=0;i<5;i++)
    {
        G4double z1 = (i-2)*0.5*m;
        new G4PVPlacement(0,G4ThreeVector(0.,0.,z1),chamber1Logical,
                          "chamber1Physical",firstArmLogical,
                          false,i,checkOverlaps);
    }
    
    // "virtual" wire plane
    G4VSolid* wirePlane1Solid 
      = new G4Box("wirePlane1Box",1.*m,30.*cm,0.1*mm);
    fWirePlane1Logical
      = new G4LogicalVolume(wirePlane1Solid,argonGas,"wirePlane1Logical");
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fWirePlane1Logical,
                      "wirePlane1Physical",chamber1Logical,
                      false,0,checkOverlaps);
    
    // =============================================
    // Exercise 1
    // Complete full geometry.
    // Note that second arm, by default is rotated of
    // 30 deg.
    // Step 1: Add an hodoscope with dimensions (X,Y,Z):
    //         (10,40,1)cm made of scintillator.
    //         There are 25 planes placed at Y,Z=0 (w.r.t. mother volume)

    // Step 2: Add 5 drift chambers made of argon, with dimensions (X,Y,Z):
    //         (300,60,2)cm
    //         These are placed equidistant inside the second arm at distances from -2.5m
    //         to -0.5m
    
    // Step 3: Add a "virtual" wire plane of (300,60,0.02)cm
    //         at (0,0,0) in the drift chamber

    // Step 4: Build CsI EM-calorimeter of (300,60,30)cm
    //         placed at (0,0,2)m in the second arm.
    //         The calorimeter is made of 80 cells,
    //         parametrised according to CellParametrisation
    //         G4VPVParameterisation concrete instance.
    //         This class paramtrises the position of each cell depending
    //         on its copy number.
    //         The cells are cubes of dimension 15 cm.
    //         (you could use placements or replicas, but here
    //         we show how to use parametrisations to build geometry)
    
    // Step 5: Add a "sandwich" hadronic calorimeter of dimensions:
    //         (300,60,100)cm.
    //         The calorimeter absorber is made of lead. It is divided in
    //         towers of (30,60,100)cm. Use replica along X-axis
    //         for towers.
    //         A tower is composed of cells, "stakced" along Y-axis
    //         Each cell has dimension (30,30,100)cm.
    //         A cells has "layers" along Z-axis. Each layer has dimensions
    //         (30,30,5)cm. Also in this case use replicas.
    //         Finally in each layer there is a tile of scintillator material
    //         of dimensions (30,30,1)cm    //         (300,60,100)cm.
    
    // Step 6: uncomment visualization attributes of the newly created volumes.
    // You may need to review the names of the logical volumes
    // visualization attributes ------------------------------------------------
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    visAttributes->SetVisibility(false);
    worldLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.9));   // LightGray
    fMagneticLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    visAttributes->SetVisibility(false);
    firstArmLogical->SetVisAttributes(visAttributes);
    secondArmLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
    fHodoscope1Logical->SetVisAttributes(visAttributes);
    //fHodoscope2Logical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    chamber1Logical->SetVisAttributes(visAttributes);
    //chamber2Logical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.0,0.8888,0.0));
    visAttributes->SetVisibility(false);
    fWirePlane1Logical->SetVisAttributes(visAttributes);
    //fWirePlane2Logical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0.0));
    visAttributes->SetVisibility(false);
    //emCalorimeterLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.0));
    //fCellLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    //hadCalorimeterLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    visAttributes->SetVisibility(false);
    //HadCalColumnLogical->SetVisAttributes(visAttributes);
    //HadCalCellLogical->SetVisAttributes(visAttributes);
    //HadCalLayerLogical->SetVisAttributes(visAttributes);
    //fHadCalScintiLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    // return the world physical volume ----------------------------------------
    
    return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
    // =============================================
    // Exercise 2 Step 3:
    // Add a Sensitive detector to the two hodoscopes
    //   with names /hodoscope{1,2}.
       
    // magnetic field ----------------------------------------------------------
    fMagneticField = new MagneticField();
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(fMagneticField);
    fFieldMgr->CreateChordFinder(fMagneticField);
    G4bool forceToAllDaughters = true;
    fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    // Air 
    nistManager->FindOrBuildMaterial("G4_AIR");
  
    // Argon gas
    nistManager->FindOrBuildMaterial("G4_Ar");
    // With a density different from the one defined in NIST
    // G4double density = 1.782e-03*g/cm3; 
    // nistManager->BuildMaterialWithNewDensity("_Ar","G4_Ar",density);
    // !! cases segmentation fault

    // Scintillator
    // (PolyVinylToluene, C_9H_10)
    nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    
    // CsI
    nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");
    
    // Lead
    nistManager->FindOrBuildMaterial("G4_Pb");
    
    // Vacuum "Galactic"
    // nistManager->FindOrBuildMaterial("G4_Galactic");

    // Vacuum "Air with low density"
    // G4Material* air = G4Material::GetMaterial("G4_AIR");
    // G4double density = 1.0e-5*air->GetDensity();
    // nistManager
    //   ->BuildMaterialWithNewDensity("Air_lowDensity", "G4_AIR", density);

    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetArmAngle(G4double val)
{
    if (!fSecondArmPhys)
    {
        G4cerr << "Detector has not yet been constructed." << G4endl;
        return;
    }
    
    fArmAngle = val;
    *fArmRotation = G4RotationMatrix();  // make it unit vector
    fArmRotation->rotateY(fArmAngle);
    G4double x = -5.*m * std::sin(fArmAngle);
    G4double z = 5.*m * std::cos(fArmAngle);
    fSecondArmPhys->SetTranslation(G4ThreeVector(x,0.,z));
    
    // tell G4RunManager that we change the geometry
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineCommands()
{
    // Define //detector command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, 
                                        "/tutorial/detector/",
                                        "Detector control");

    // armAngle command
    G4GenericMessenger::Command& armAngleCmd
      = fMessenger->DeclareMethodWithUnit("armAngle","deg",
                                  &DetectorConstruction::SetArmAngle, 
                                  "Set rotation angle of the second arm.");
    armAngleCmd.SetParameterName("angle", true);
    armAngleCmd.SetRange("angle>=0. && angle<180.");
    armAngleCmd.SetDefaultValue("30.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
