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
// $Id: HodoscopeSD.cc 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file HodoscopeSD.cc
/// \brief Implementation of the HodoscopeSD class

#include "HodoscopeSD.hh"
#include "HodoscopeHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeSD::HodoscopeSD(G4String name)
: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1)
{
    G4String HCname = "hodoscopeColl";
    collectionName.insert(HCname);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeSD::~HodoscopeSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HodoscopeSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection = new HodoscopeHitsCollection
    (SensitiveDetectorName,collectionName[0]);
    if (fHCID<0)
    { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
    hce->AddHitsCollection(fHCID,fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HodoscopeSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // =============================================
    // Exercise 2 Step 2:
    // Collect time information from G4Step.
    // A simplified "ideal" hodoscope works as follows:
    //  when a charged particle leaves some energy in the scintillator
    //  tile, the hodoscope electronics registers the time when the hit
    //  happened. We want to simulate this behavior. Remember more than
    //  one hit can be registered, for the same event, in each sensitive
    //  detector component.
    // First: if there is no energy deposition in the tile, the detector
    // did not trigger. Just return.
    // Second: Get copyNo and hit-time from Step. You should use PreStepPoint
    // Third: check if hits collection has already one hit for tile "copyNo".
    //         if not create a new hit and add it to the detector, if hit
    //        already exists set as time of the hit
    //        min(thisHitTim,previousTime).
    
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
