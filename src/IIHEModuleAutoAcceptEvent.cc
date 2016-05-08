#include "UserCode/IIHETree/interface/IIHEModuleAutoAcceptEvent.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleAutoAcceptEvent::IIHEModuleAutoAcceptEvent(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){}
IIHEModuleAutoAcceptEvent::~IIHEModuleAutoAcceptEvent(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleAutoAcceptEvent::beginJob(){
}

// ------------ method called to for each event  ------------
void IIHEModuleAutoAcceptEvent::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
}

void IIHEModuleAutoAcceptEvent::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleAutoAcceptEvent::beginEvent(){}
void IIHEModuleAutoAcceptEvent::endEvent(){
  acceptEvent() ;
}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleAutoAcceptEvent::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleAutoAcceptEvent);
