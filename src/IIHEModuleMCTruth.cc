#include "UserCode/IIHETree/interface/IIHEModuleMCTruth.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleMCTruth::IIHEModuleMCTruth(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  pt_threshold_            = iConfig.getUntrackedParameter<double>("MCTruth_ptThreshold"            , 10.0) ;
  m_threshold_             = iConfig.getUntrackedParameter<double>("MCTruth_mThreshold"             , 20.0) ;
  DeltaROverlapThreshold_  = iConfig.getUntrackedParameter<double>("MCTruth_DeltaROverlapThreshold" , 1e-3) ;
  puInfoSrc_               = iConfig.getUntrackedParameter<edm::InputTag>("PileUpSummaryInfo") ;
  generatorLabel_ = iC.consumes<GenEventInfoProduct> (iConfig.getParameter<InputTag>("generatorLabel"));
  puCollection_ = iC.consumes<vector<PileupSummaryInfo> > (puInfoSrc_);
  genParticlesCollection_ = iC.consumes<vector<reco::GenParticle> > (iConfig.getParameter<InputTag>("genParticleSrc"));
}
IIHEModuleMCTruth::~IIHEModuleMCTruth(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleMCTruth::beginJob(){
 std::vector<int> MCPdgIdsToSave ;
  MCPdgIdsToSave.push_back(11) ; // Electron
  MCPdgIdsToSave.push_back(12) ; // Electron neutrino
  MCPdgIdsToSave.push_back(13) ; // Muon
  MCPdgIdsToSave.push_back(14) ; // Muon neutrino
  MCPdgIdsToSave.push_back(15) ; // Tau
  MCPdgIdsToSave.push_back(16) ; // Tau neytrino
  MCPdgIdsToSave.push_back(21) ; // gluon
  MCPdgIdsToSave.push_back( 1) ; // d quark
  MCPdgIdsToSave.push_back( 2) ; // u quark
  MCPdgIdsToSave.push_back( 3) ; // s quark
  MCPdgIdsToSave.push_back( 4) ; // c quark
  MCPdgIdsToSave.push_back( 5) ; // b quark
  MCPdgIdsToSave.push_back( 6) ; // t quark
  MCPdgIdsToSave.push_back(22) ; // Photon
  MCPdgIdsToSave.push_back(23) ; // Z boson
  MCPdgIdsToSave.push_back(24) ; // W boson
  MCPdgIdsToSave.push_back(25) ; // BEH boson
  MCPdgIdsToSave.push_back(32) ; // Z'  boson
  MCPdgIdsToSave.push_back(33) ; // Z'' boson
  MCPdgIdsToSave.push_back(34) ; // W'  boson
  addToMCTruthWhitelist(MCPdgIdsToSave) ;


  addBranch("mc_n", kUInt) ;
  addBranch("mc_weight", kFloat) ;
  addBranch("mc_w_sign", kFloat) ;
  setBranchType(kVectorInt) ;
  addBranch("mc_index") ;
  addBranch("mc_pdgId") ;
  addBranch("mc_charge") ;
  addBranch("mc_status") ;
  setBranchType(kVectorFloat) ;
  addBranch("mc_mass") ;
  addBranch("mc_px") ;
  addBranch("mc_py") ;
  addBranch("mc_pz") ;
  addBranch("mc_pt") ;
  addBranch("mc_eta") ;
  addBranch("mc_phi") ;
  addBranch("mc_energy") ;
  
  setBranchType(kInt) ;
  addBranch("mc_trueNumInteractions") ;
  addBranch("mc_PU_NumInteractions" ) ;
  
  addValueToMetaTree("MCTruth_ptThreshold"           , pt_threshold_          ) ;
  addValueToMetaTree("MCTruth_mThreshold"            , m_threshold_           ) ;
  addValueToMetaTree("MCTruth_DeltaROverlapThreshold", DeltaROverlapThreshold_) ;

  nEventsWeighted_ = 0.0 ;
}

// ------------ method called to for each event  ------------
void IIHEModuleMCTruth::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<GenEventInfoProduct> genEventInfoHandle;
  iEvent.getByToken(generatorLabel_, genEventInfoHandle);
  float weight = genEventInfoHandle->weight() ;
  float w_sign = (weight>=0) ? 1 : -1 ;
  store("mc_weight"                  ,weight);
  store("mc_w_sign"             , w_sign) ;
  nEventsWeighted_ += w_sign ;
  // Fill pile-up related informations
  // --------------------------------
  edm::Handle<std::vector< PileupSummaryInfo > >  puInfo ;
  iEvent.getByToken(puCollection_, puInfo) ;
  int trueNumInteractions = -1 ;
  int PU_NumInteractions  = -1 ;
 if(puInfo.isValid()){
    std::vector<PileupSummaryInfo>::const_iterator PVI;
    for(PVI = puInfo->begin() ; PVI != puInfo->end() ; ++PVI){
      int BX = PVI->getBunchCrossing() ;
      if(BX==0){ // "0" is the in-time crossing, negative values are the early crossings, positive are late
        trueNumInteractions = PVI->getTrueNumInteractions() ;
        PU_NumInteractions  = PVI->getPU_NumInteractions() ;
      }
    }
  }

  Handle<GenParticleCollection> pGenParticles ;
  iEvent.getByToken(genParticlesCollection_, pGenParticles) ;
  GenParticleCollection genParticles(pGenParticles->begin(),pGenParticles->end()) ;
  
  // These variables are used to match up mothers to daughters at the end.
  int counter = 0 ;
  MCTruthRecord_.clear() ;
  

  for(GenParticleCollection::const_iterator mc_iter = genParticles.begin() ; mc_iter!=genParticles.end() ; ++mc_iter){
    counter = counter + 1;
    store("mc_px"     , mc_iter->px()    ) ;
    store("mc_py"     , mc_iter->py()    ) ;
    store("mc_pz"     , mc_iter->pz()    ) ;
    store("mc_pt"     , mc_iter->pt()    ) ;
    store("mc_eta"    , mc_iter->eta()   ) ;
    store("mc_phi"    , mc_iter->phi()   ) ;
    store("mc_energy" , mc_iter->energy()) ;
    store("mc_mass"   , mc_iter->mass()  ) ;

    store("mc_index"  , counter ) ;
    store("mc_pdgId"  , mc_iter->pdgId() ) ;
    store("mc_charge" , mc_iter->charge()) ;
    store("mc_status" , mc_iter->status()) ;
  }
  
  store("mc_trueNumInteractions", trueNumInteractions) ;
  store("mc_PU_NumInteractions" , PU_NumInteractions ) ;
  
  store("mc_n", (unsigned int)(MCTruthRecord_.size())) ;
}

int IIHEModuleMCTruth::matchEtaPhi_getIndex(float eta, float phi){
  float bestDR = 1e6 ;
  int bestIndex = -1 ;
  for(unsigned int i=0 ; i<MCTruthRecord_.size() ; ++i){
    MCTruthObject* MCTruth = MCTruthRecord_.at(i) ;
    float DR = deltaR(MCTruth->eta(),MCTruth->phi(),eta,phi) ;
    if(DR<bestDR){
      bestDR = DR ;
      bestIndex = i ;
    }
  }
  return bestIndex ;
}

const MCTruthObject* IIHEModuleMCTruth::matchEtaPhi(float eta, float phi){
  int index = matchEtaPhi_getIndex(eta, phi) ;
  return getRecordByIndex(index) ;
}

const MCTruthObject* IIHEModuleMCTruth::getRecordByIndex(int index){
  if(index<0) return 0 ;
  return MCTruthRecord_.at(index) ;
}

void IIHEModuleMCTruth::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleMCTruth::beginEvent(){}
void IIHEModuleMCTruth::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleMCTruth::endJob(){
  addValueToMetaTree("mc_nEventsWeighted", nEventsWeighted_) ;
}

DEFINE_FWK_MODULE(IIHEModuleMCTruth);
