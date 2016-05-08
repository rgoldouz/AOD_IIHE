#include "UserCode/IIHETree/interface/IIHEModuleVertex.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleVertex::IIHEModuleVertex(const edm::ParameterSet& iConfig , edm::ConsumesCollector && iC): IIHEModule(iConfig){}
IIHEModuleVertex::~IIHEModuleVertex(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleVertex::beginJob(){
  addBranch("pv_n", kUInt) ;
  setBranchType(kVectorFloat) ;
  addBranch("pv_x") ;
  addBranch("pv_y") ;
  addBranch("pv_z") ;
  addBranch("pv_isValid", kVectorInt) ;
  addBranch("pv_normalizedChi2", kVectorFloat) ;
  setBranchType(kVectorInt) ;
  addBranch("pv_ndof") ;
  addBranch("pv_nTracks") ;
  addBranch("pv_totTrackSize") ;
}

// ------------ method called to for each event  ------------
void IIHEModuleVertex::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  // Get the beamspot from the Event:
  // The beamspot is passed to the IIHEAnalysis class, so we call it from parent_
  // Don't forget to declare IIHEModuleVertex as a friend of IIHEAnalysis!

  // Retrieve primary vertex collection
  const reco::VertexCollection* pvcoll = parent_->getPrimaryVertices() ;
  store("pv_n", (unsigned int) pvcoll->size()) ;
  for(reco::VertexCollection::const_iterator pvIt = pvcoll->begin(); pvIt!=pvcoll->end(); ++pvIt){
    store("pv_x"             , pvIt->x()                ) ;
    store("pv_y"             , pvIt->y()                ) ;   
    store("pv_z"             , pvIt->z()                ) ;  
    store("pv_isValid"       , pvIt->isValid()          ) ;
    store("pv_ndof"          , (int)pvIt->ndof()        ) ;
    store("pv_nTracks"       , (int)(pvIt->nTracks())   ) ;
    store("pv_normalizedChi2", pvIt->normalizedChi2()   ) ;
    store("pv_totTrackSize"  , (int)(pvIt->tracksSize())) ;
  }
}

void IIHEModuleVertex::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleVertex::beginEvent(){}
void IIHEModuleVertex::endEvent(){}

// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleVertex::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleVertex);
