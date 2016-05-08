#include "UserCode/IIHETree/interface/IIHEModuleExample.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleExample::IIHEModuleExample(const edm::ParameterSet& iConfig): IIHEModule(iConfig){}
IIHEModuleExample::~IIHEModuleExample(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleExample::beginJob(){
  setBranchType(kInt) ;
  addBranch("exampleIntVar1") ;
  addBranch("exampleIntVar2") ;
  addBranch("exampleVectorIntVar", kVectorInt) ;
}

// ------------ method called to for each event  ------------
void IIHEModuleExample::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  int value = 1 ;
  store("exampleIntVar1"     , value) ;
  store("exampleIntVar2"     , value) ;
  store("exampleVectorIntVar", value) ;
  store("exampleVectorIntVar", value) ;
  store("exampleVectorIntVar", value) ;
}

void IIHEModuleExample::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleExample::beginEvent(){}
void IIHEModuleExample::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleExample::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleExample);
