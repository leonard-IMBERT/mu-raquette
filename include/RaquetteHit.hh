#ifndef RaquetteHit_H
#define RaquetteHit_H 1

#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"

class RaquetteHit : public G4VHit {
  public:
    RaquetteHit();
    RaquetteHit(const RaquetteHit&);
    ~RaquetteHit();

    // operators
    const RaquetteHit& operator=(const RaquetteHit&);
    G4bool operator==(const RaquetteHit&) const;

    // Display functions
    virtual void Print();
    virtual void Draw();

    // Setters
    void SetTrackID  (G4int track)       { _trackId = track; };
    void SetEdep     (G4double edep)     { _edep = edep; };
    void SetPos      (G4ThreeVector pos) { _pos = pos; };
    void SetName     (G4String name)     { _name = name; };
    void SetVertex   (G4ThreeVector vert){ _vertex = vert; };
    void SetLength   (G4double len)      { _length = len; };
    void SetIsPrimary(G4bool prim)       { _isPrimary = prim; };

    // Getters
    G4int         GetTrackID  () { return _trackId; };
    G4double      GetEdep     () { return _edep; };
    G4ThreeVector GetPos      () { return _pos; };
    G4String      GetName     () { return _name; };
    G4ThreeVector GetVertex   () { return _vertex; };
    G4double      GetLength   () { return _length; };
    G4bool        GetIsPrimary() { return _isPrimary; };

  private:
    G4int         _trackId;    // Id of the track
    G4double      _edep;       // Deposit energy (MeV)
    G4ThreeVector _pos;        // Impact position
    G4String      _name;       // Particle Name
    G4ThreeVector _vertex;     // Vertex de creation de la particle
    G4double      _length;     // Longeur de la trace
    G4bool        _isPrimary;
};

typedef G4THitsCollection<RaquetteHit> HitsCollection;

#endif
