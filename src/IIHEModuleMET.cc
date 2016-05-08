#include "UserCode/IIHETree/interface/IIHEModuleMET.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h" 
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleMET::IIHEModuleMET(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  pfMETToken_ =  iC.consumes<View<pat::MET> > (iConfig.getParameter<edm::InputTag>("METCollection"));
}
IIHEModuleMET::~IIHEModuleMET(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleMET::beginJob(){
  setBranchType(kFloat) ;
  addBranch("MET_pfMet_et"   ) ;
  addBranch("MET_pfMet_phi"  ) ;
}

// ------------ method called to for each event  ------------
void IIHEModuleMET::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<edm::View<pat::MET> > pfMETHandle;
  iEvent.getByToken(pfMETToken_, pfMETHandle);

  if (pfMETHandle.isValid()) {
    const pat::MET *pfMET = 0;
    pfMET = &(pfMETHandle->front());
    store("MET_pfMet_et"   , pfMET->et()    ) ;
    store("MET_pfMet_phi"  , pfMET->phi()   ) ;
  }
}
void IIHEModuleMET::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleMET::beginEvent(){}
void IIHEModuleMET::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleMET::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleMET);
