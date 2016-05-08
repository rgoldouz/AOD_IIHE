#include "UserCode/IIHETree/interface/IIHEModuleLeptonsAccept.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleLeptonsAccept::IIHEModuleLeptonsAccept(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  ptThreshold_         = iConfig.getUntrackedParameter<double>("LeptonsAccept_pTThreshold", 30.0 ) ;
  nElectronsThreshold_ = iConfig.getUntrackedParameter<double>("LeptonsAccept_nElectrons" ,    1 ) ;
  nLeptonsThreshold_   = iConfig.getUntrackedParameter<double>("LeptonsAccept_nLeptons"   ,    2 ) ;
}
IIHEModuleLeptonsAccept::~IIHEModuleLeptonsAccept(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleLeptonsAccept::beginJob(){
  nAcceptAll_ = 0 ;
}

// ------------ method called to for each event  ------------
void IIHEModuleLeptonsAccept::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  reco::GsfElectronCollection electrons = parent_->getElectronCollection() ;
  reco::MuonCollection muons = parent_->getMuonCollection() ;

  int nEl = 0 ;
  int nMu = 0 ;
  
//  for(reco::GsfElectronCollection::const_iterator gsfiter=electrons.begin() ; gsfiter!=electrons.end() ; ++gsfiter){
  for(vector<reco::GsfElectron>::const_iterator gsfiter=electrons.begin() ; gsfiter!=electrons.end() ; ++gsfiter){
    float pt = gsfiter->pt() ;
    float HEEP_ET  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
    if(pt>ptThreshold_ || HEEP_ET>ptThreshold_) nEl++ ;
  }
//  for(reco::MuonCollection::const_iterator muiter = muons.begin(); muiter!=muons.end() ; ++muiter){
  for(vector<reco::Muon>::const_iterator muiter = muons.begin() ; muiter != muons.end() ; ++muiter){
    float pt = muiter->pt() ;
    if(pt>ptThreshold_) nMu++ ;
  }
  
  bool acceptEl      = (nEl     >= nElectronsThreshold_) ;
  bool acceptLeptons = (nEl+nMu >= nLeptonsThreshold_  ) ;
  bool acceptThisEvent = (acceptEl && acceptLeptons) ;
  
  // Save the event if we see something we like
  if(acceptThisEvent){
    acceptEvent() ;
    nAcceptAll_++ ;
  }
  else{
    //rejectEvent() ;
  }
}

void IIHEModuleLeptonsAccept::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleLeptonsAccept::beginEvent(){}
void IIHEModuleLeptonsAccept::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleLeptonsAccept::endJob(){
  std::cout << std::endl << "IIHEModuleLeptonsAccept report:" << std::endl ;
  std::cout << "  nAcceptAll  = " << nAcceptAll_   << std::endl ;
}

DEFINE_FWK_MODULE(IIHEModuleLeptonsAccept);
