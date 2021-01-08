#include "RaquetteHit.hh"

#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "RootData.hh"

RaquetteHit::RaquetteHit():
  G4VHit(),
  _trackId(-1),
  _edep(0.),
  _pos(G4ThreeVector()),
  _vertex(G4ThreeVector()),
  _length(0.),
  _detecteur(Detect::other),
  _procName("")
{}

RaquetteHit::RaquetteHit(const RaquetteHit& right)
  : G4VHit()
{
  _trackId   = right._trackId;
  _edep      = right._edep;
  _pos       = right._pos;
  _name      = right._name;
  _vertex    = right._vertex;
  _length    = right._length;
  _detecteur = right._detecteur;
  _procName  = right._procName;
}

RaquetteHit::~RaquetteHit() {}

const RaquetteHit& RaquetteHit::operator=(const RaquetteHit& right) {
  _trackId   = right._trackId;
  _edep      = right._edep;
  _pos       = right._pos;
  _name      = right._name;
  _vertex    = right._vertex;
  _length    = right._length;
  _detecteur = right._detecteur;
  _procName  = right._procName;

  return *this;
}

G4bool RaquetteHit::operator==(const RaquetteHit& right) const {
  return ( this == &right ) ? true : false;
}

void RaquetteHit::Draw() {
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) {
    // Draw a circle where there was a hit
    G4Circle circle(_pos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,1.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void RaquetteHit::Print() {
  G4cout
    << "  name: " << _name
    << " trackId: " << _trackId
    << " Edep: " << std::setw(7) << G4BestUnit(_edep, "Energy")
    << " Position: " << std::setw(7) << G4BestUnit(_pos, "Length")
    << " Vertex: " << std::setw(7) << G4BestUnit(_vertex, "Length")
    << " Longeur: " << std::setw(7) << G4BestUnit(_length, "Length")
    << G4endl;
}
