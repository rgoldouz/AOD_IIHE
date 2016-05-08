#ifndef UserCode_IIHETree_MCTruthObject_h
#define UserCode_IIHETree_MCTruthObject_h

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/Candidate.h"

class MCTruthObject{
private:
  const reco::Candidate* candidate_ ;
  float DeltaRCut_ ;
  std::vector<const reco::Candidate*> mothers_ ;
  
  // These variables are required to truth match after the destructor of the
  // candidate_ has been called.
  int pdgId_    ;
  float pt_     ;
  float eta_    ;
  float phi_    ;
  float energy_ ;
public:
  MCTruthObject(reco::Candidate*) ;
  ~MCTruthObject() ;
  void addMother(const reco::Candidate*) ;
  int matchMother(std::vector<MCTruthObject*>, unsigned int) ;
  const reco::Candidate* getCandidate(){ return candidate_ ; }
  const reco::Candidate* getMother(unsigned int) ;
  unsigned nMothers(){ return mothers_.size() ; }
  
  float     pt() const { return pt_     ; }
  float    eta() const { return eta_    ; }
  float    phi() const { return phi_    ; }
  float energy() const { return energy_ ; }
};

#endif
