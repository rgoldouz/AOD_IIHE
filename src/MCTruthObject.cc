#include "UserCode/IIHETree/interface/MCTruthObject.h"

MCTruthObject::MCTruthObject(reco::Candidate* cand){
  candidate_ = cand ;
  DeltaRCut_ = 0.001 ;
  pdgId_  = cand->pdgId() ;
  pt_     = cand->pt() ;
  eta_    = cand->eta() ;
  phi_    = cand->phi() ;
  energy_ = cand->energy() ;
}
MCTruthObject::~MCTruthObject(){}
void MCTruthObject::addMother(const reco::Candidate* mother){
  mothers_.push_back(mother) ;
}
int MCTruthObject::matchMother(std::vector<MCTruthObject*> otherCands, unsigned int index){
  if(index>=mothers_.size()) return -2 ;
  const reco::Candidate* mother = mothers_.at(index) ;
  int pdgId = mother->pdgId() ;
  float best_DR = 1e6 ;
  int best_index = -1 ;
  for(unsigned int i=0 ; i<otherCands.size() ; ++i){
    const reco::Candidate* comp = otherCands.at(i)->getCandidate() ;
    if(comp->pdgId()!=pdgId) continue ;
    float DR = deltaR(comp->eta(),comp->phi(),mother->eta(),mother->phi()) ;
    if(DR<best_DR && DR<DeltaRCut_){
      best_DR = DR ;
      best_index = i ;
    }
  }
  return best_index ;
}
const reco::Candidate* MCTruthObject::getMother(unsigned int index){
  if(index>=mothers_.size()) return 0 ;
  return mothers_.at(index) ;
}

