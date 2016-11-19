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
  MCPdgIdsToSave.push_back(13) ; // Muon
  MCPdgIdsToSave.push_back(15) ; // Tau
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
  addBranch("mc_id_first", kInt) ;
  addBranch("mc_id_second", kInt) ;
  addBranch("mc_x_first", kFloat) ;
  addBranch("mc_x_second", kFloat) ;
  addBranch("mc_xPDF_first", kFloat) ;
  addBranch("mc_xPDF_second", kFloat) ;
  addBranch("mc_scalePDF", kFloat) ;
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
  setBranchType(kVectorUInt) ;
  addBranch("mc_numberOfDaughters") ;
  addBranch("mc_numberOfMothers"  ) ;
  setBranchType(kVectorVectorInt) ;
  addBranch("mc_mother_index") ;
  addBranch("mc_mother_pdgId") ;
  setBranchType(kVectorVectorFloat) ;
  addBranch("mc_mother_px"    ) ;
  addBranch("mc_mother_py"    ) ;
  addBranch("mc_mother_pz"    ) ;
  addBranch("mc_mother_pt"    ) ;
  addBranch("mc_mother_eta"   ) ;
  addBranch("mc_mother_phi"   ) ;
  addBranch("mc_mother_energy") ;
  addBranch("mc_mother_mass"  ) ;
  
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

  store("mc_id_first" , genEventInfoHandle->pdf()->id.first);    // PDG ID of incoming parton #1
  store("mc_id_second" , genEventInfoHandle->pdf()->id.second);   // PDG ID of incoming parton #2
  store("mc_x_first", genEventInfoHandle->pdf()->x.first);     // x value of parton #1
  store("mc_x_second" , genEventInfoHandle->pdf()->x.second);    // x value of parton #2
  store("mc_xPDF_first" , genEventInfoHandle->pdf()->xPDF.first);  // PDF weight for parton #1
  store("mc_xPDF_second" , genEventInfoHandle->pdf()->xPDF.second); // PDF weight for parton #2
  store("mc_scalePDF" , genEventInfoHandle->pdf()->scalePDF);    // scale of the hard interaction
   
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

  MCTruthObject* MCTruth ;
//  const Candidate* parent ;
  const Candidate* child ;

  //we should save all outgoing particle from hard interaction
  for(GenParticleCollection::const_iterator mc_iter = genParticles.begin() ; mc_iter!=genParticles.end() ; ++mc_iter){
    if(mc_iter->status()!=23) continue;
    child  = mc_iter->clone()  ;
    // Create a truth record instance.
    MCTruth = new MCTruthObject((reco::Candidate*)&*mc_iter) ;
    //
    // Add all the mothers
    for(unsigned int mother_iter=0 ; mother_iter<child->numberOfMothers() ; ++mother_iter){
        MCTruth->addMother(child->mother(mother_iter)) ;
    }    

    MCTruthRecord_.push_back(MCTruth) ;
    counter++ ;
  }

  for(GenParticleCollection::const_iterator mc_iter = genParticles.begin() ; mc_iter!=genParticles.end() ; ++mc_iter){
    if(mc_iter->status()==23) continue;
    int pdgId = mc_iter->pdgId() ;
    float pt  = mc_iter->pt()    ;
    // First check the whitelist.
    bool whitelist_accept = false ;
    for(unsigned int i=0 ; i<whitelist_.size() ; ++i){
      if(abs(pdgId)==abs(whitelist_.at(i))){
        whitelist_accept = true ;
        break ;
      }
    }
    
    // Ignore particles with exactly one daughter (X => X => X etc)
    bool daughters_accept = (mc_iter->numberOfDaughters()!=1) ;
    
    // Remove objects with zero pT.
    bool nonZeroPt_accept = (pt>1e-3) ;
    
    // Now check the thresholds.
    bool thresholds_accept = false ;
    float pt_threshold = (pdgId==21 || abs(pdgId)<5) ? 10 : pt_threshold_ ;
    if(             pt>pt_threshold ) thresholds_accept = true  ;
    if(mc_iter->mass()> m_threshold_) thresholds_accept = true  ;
    
    // Now combine them all.
    bool accept = (whitelist_accept && daughters_accept && thresholds_accept && nonZeroPt_accept) ;
    if(false==accept) continue ;
    // Now go up the ancestry until we find the real parent
//    parent = mc_iter->mother() ;
    child  = mc_iter->clone()  ;
//    while(parent->pdgId()==pdgId){
//      child  = parent ;
//      parent = parent->mother() ;
//    }
    
    // Create a truth record instance.
    MCTruth = new MCTruthObject((reco::Candidate*)&*mc_iter) ;
    
    // Add all the mothers
//    for(unsigned int mother_iter=0 ; mother_iter<child->numberOfMothers() ; ++mother_iter){
//      MCTruth->addMother(child->mother(mother_iter)) ;
//    }
    
    // Finally check to see if this overlaps with an existing truth particle.
    bool overlap = false ;
    for(unsigned int i=0 ; i<MCTruthRecord_.size() ; ++i){
      const reco::Candidate* comp = MCTruthRecord_.at(i)->getCandidate() ;
      float DR = deltaR(comp->eta(),comp->phi(),MCTruth->getCandidate()->eta(),MCTruth->getCandidate()->phi()) ;
      if(DR<DeltaROverlapThreshold_){
        overlap = true ;
        break ;
      }
    }
    if(true==overlap) continue ;
    
    // Then push back the MC truth information
    MCTruthRecord_.push_back(MCTruth) ;
    counter++ ;
  }
  for(unsigned int i=0 ; i<MCTruthRecord_.size() ; ++i){
    MCTruthObject* ob = MCTruthRecord_.at(i) ;
    std::vector<int  > mc_mother_index ;
    std::vector<int  > mc_mother_pdgId ;
    std::vector<float> mc_mother_px ;
    std::vector<float> mc_mother_py ;
    std::vector<float> mc_mother_pz ;
    std::vector<float> mc_mother_pt ;
    std::vector<float> mc_mother_eta ;
    std::vector<float> mc_mother_phi ;
    std::vector<float> mc_mother_energy ;
    std::vector<float> mc_mother_mass ;
    for(unsigned int j=0 ; j<ob->nMothers() ; ++j){
      const reco::Candidate* mother = ob->getMother(j) ;
      if(mother){
        int mother_index_tmp = ob->matchMother(MCTruthRecord_, j) ;
        mc_mother_index .push_back(mother_index_tmp) ;
        mc_mother_pdgId .push_back(mother->pdgId() ) ;
        mc_mother_px    .push_back(mother->px()    ) ;
        mc_mother_py    .push_back(mother->py()    ) ;
        mc_mother_pz    .push_back(mother->pz()    ) ;
        mc_mother_pt    .push_back(mother->pt()    ) ;
        mc_mother_eta   .push_back(mother->eta()   ) ;
        mc_mother_phi   .push_back(mother->phi()   ) ;
        mc_mother_energy.push_back(mother->energy()) ;
        mc_mother_mass  .push_back(mother->mass()  ) ;
      }
    }
    if(mc_mother_index.size()==0) mc_mother_index.push_back(0) ;
    store("mc_mother_index" , mc_mother_index ) ;
    store("mc_mother_pdgId" , mc_mother_pdgId ) ;
    store("mc_mother_px"    , mc_mother_px    ) ;
    store("mc_mother_py"    , mc_mother_py    ) ;
    store("mc_mother_pz"    , mc_mother_pz    ) ;
    store("mc_mother_pt"    , mc_mother_pt    ) ;
    store("mc_mother_eta"   , mc_mother_eta   ) ;
    store("mc_mother_phi"   , mc_mother_phi   ) ;
    store("mc_mother_energy", mc_mother_energy) ;
    store("mc_mother_mass"  , mc_mother_mass  ) ;
    
    store("mc_numberOfDaughters", (unsigned int)(ob->getCandidate()->numberOfDaughters())) ;
    store("mc_numberOfMothers"  , (unsigned int)(ob->nMothers())) ;
    
    store("mc_px"     , ob->getCandidate()->px()    ) ;
    store("mc_py"     , ob->getCandidate()->py()    ) ;
    store("mc_pz"     , ob->getCandidate()->pz()    ) ;
    store("mc_pt"     , ob->getCandidate()->pt()    ) ;
    store("mc_eta"    , ob->getCandidate()->eta()   ) ;
    store("mc_phi"    , ob->getCandidate()->phi()   ) ;
    store("mc_energy" , ob->getCandidate()->energy()) ;
    store("mc_mass"   , ob->getCandidate()->mass()  ) ;
    
    store("mc_index"  , i ) ;
    store("mc_pdgId"  , ob->getCandidate()->pdgId() ) ;
    store("mc_charge" , ob->getCandidate()->charge()) ;
    store("mc_status" , ob->getCandidate()->status()) ;
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
