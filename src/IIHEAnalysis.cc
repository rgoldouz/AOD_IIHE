// System includes
#include <iostream>
#include <TMath.h>
#include <vector>

#include <boost/algorithm/string.hpp>

// IIHE includes
#include "UserCode/IIHETree/interface/IIHEAnalysis.h"

#include "UserCode/IIHETree/interface/EtSort.h"
#include "UserCode/IIHETree/interface/BranchWrapper.h"
#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "UserCode/IIHETree/interface/IIHEModuleEvent.h"
#include "UserCode/IIHETree/interface/IIHEModuleVertex.h"
#include "UserCode/IIHETree/interface/IIHEModuleSuperCluster.h"
#include "UserCode/IIHETree/interface/IIHEModulePhoton.h"
#include "UserCode/IIHETree/interface/IIHEModuleGedGsfElectron.h"
#include "UserCode/IIHETree/interface/IIHEModuleMuon.h"
#include "UserCode/IIHETree/interface/IIHEModuleMCTruth.h"
#include "UserCode/IIHETree/interface/IIHEModuleHEEP.h"
#include "UserCode/IIHETree/interface/IIHEModuleMET.h"
#include "UserCode/IIHETree/interface/IIHEModuleTrigger.h"
#include "UserCode/IIHETree/interface/IIHEModuleZBoson.h"
#include "UserCode/IIHETree/interface/IIHEModuleLeptonsAccept.h"
#include "UserCode/IIHETree/interface/IIHEModuleAutoAcceptEvent.h"

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEAnalysis::IIHEAnalysis(const edm::ParameterSet& iConfig):
hltPrescaleProvider_(iConfig, consumesCollector(), *this)
{
  currentVarType_ = -1 ;
  debug_     = iConfig.getParameter<bool  >("debug"    ) ;
  git_hash_  = iConfig.getParameter<string>("git_hash" ) ;
  globalTag_ = iConfig.getParameter<string>("globalTag") ;
  nEvents_ = 0 ;
  nEventsStored_ = 0 ;
  acceptEvent_ = false ;
  
  edm::Service<TFileService> fs ;
  fs->file().cd() ;
  
  //mainFile_ = TFile("outfile.root", "RECREATE") ;
  dataTree_ = new TTree("IIHEAnalysis", "IIHEAnalysis") ;
  metaTree_ = new TTree("meta", "Information about globalTag etc") ;
  metaTree_->Branch("git_hash" , &git_hash_ ) ;
  metaTree_->Branch("globalTag", &globalTag_) ;
  
//CHOOSE_RELEASE_START DEFAULT CMSSW_7_4_4 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_7_0_6_patch1 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1
//CHOOSE_RELEASE_END DEFAULT CMSSW_7_4_4 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_7_0_6_patch1 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1
/*CHOOSE_RELEASE_START CMSSW_5_3_11
  beamSpotLabel_      = iConfig.getParameter<edm::InputTag>("beamSpot") ;
CHOOSE_RELEASE_END CMSSW_5_3_11*/
  
  primaryVertexLabel_          = iConfig.getParameter<edm::InputTag>("primaryVertex") ;
  superClusterCollectionLabel_ = iConfig.getParameter<edm::InputTag>("superClusterCollection"  ) ;
  photonCollectionLabel_       = iConfig.getParameter<edm::InputTag>("photonCollection"        ) ;
  electronCollectionLabel_     = iConfig.getParameter<edm::InputTag>("electronCollection"      ) ;
  muonCollectionLabel_         = iConfig.getParameter<edm::InputTag>("muonCollection"          ) ;
  beamSpotToken_      = consumes<reco::BeamSpot>(iConfig.getParameter<InputTag>("beamSpot")) ; 
  trigEventTag_ = iConfig.getParameter<InputTag>("triggerEvent");

 
  reducedBarrelRecHitCollection_ = iConfig.getParameter<edm::InputTag>("ebReducedRecHitCollection") ;
  reducedEndcapRecHitCollection_ = iConfig.getParameter<edm::InputTag>("eeReducedRecHitCollection") ;

//CHOOSE_RELEASE_START DEFAULT CMSSW_7_4_4 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_7_0_6_patch1 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1
  reducedBarrelRecHitCollectionToken_ = mayConsume<EcalRecHitCollection>(reducedBarrelRecHitCollection_) ;
  reducedEndcapRecHitCollectionToken_ = mayConsume<EcalRecHitCollection>(reducedEndcapRecHitCollection_) ;
  esReducedRecHitCollection_ = mayConsume<EcalRecHitCollection> (iConfig.getParameter<InputTag>("esReducedRecHitCollection"));  
//CHOOSE_RELEASE_END DEFAULT CMSSW_7_4_4 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_7_0_6_patch1 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1
/*CHOOSE_RELEASE_START CMSSW_5_3_11
CHOOSE_RELEASE_END CMSSW_5_3_11  */

  vtxToken_ = consumes<reco::VertexCollection>(primaryVertexLabel_);
  electronCollectionToken_ =  consumes<View<reco::GsfElectron> > (electronCollectionLabel_);
  muonCollectionToken_ =  consumes<View<reco::Muon> > (muonCollectionLabel_);
  photonCollectionToken_ =  consumes<View<reco::Photon> > (photonCollectionLabel_); 
  superClusterCollectionToken_ =  consumes<reco::SuperClusterCollection> (superClusterCollectionLabel_);
  trigEvent_ = consumes<trigger::TriggerEvent>(trigEventTag_);

 
  firstPrimaryVertex_ = new math::XYZPoint(0.0,0.0,0.0) ;
  beamspot_           = new math::XYZPoint(0.0,0.0,0.0) ;
  MCTruthModule_ = 0 ;
  
  includeLeptonsAcceptModule_   = iConfig.getUntrackedParameter<bool>("includeLeptonsAcceptModule"  , true ) ;
  includeTriggerModule_         = iConfig.getUntrackedParameter<bool>("includeTriggerModule"        , true ) ;
  includeEventModule_           = iConfig.getUntrackedParameter<bool>("includeEventModule"          , true ) ;
  includeVertexModule_          = iConfig.getUntrackedParameter<bool>("includeVertexModule"         , true ) ;
  includeSuperClusterModule_    = iConfig.getUntrackedParameter<bool>("includeSuperClusterModule"   , true ) ;
  includePhotonModule_          = iConfig.getUntrackedParameter<bool>("includePhotonModule"         , true ) ;
  includeElectronModule_        = iConfig.getUntrackedParameter<bool>("includeElectronModule"       , true ) ;
  includeMuonModule_            = iConfig.getUntrackedParameter<bool>("includeMuonModule"           , true ) ;
  includeMETModule_             = iConfig.getUntrackedParameter<bool>("includeMETModule"            , true ) ;
  includeMCTruthModule_         = iConfig.getUntrackedParameter<bool>("includeMCTruthModule"        , true ) ;
  includeHEEPModule_            = iConfig.getUntrackedParameter<bool>("includeHEEPModule"           , true ) ;
  includeZBosonModule_          = iConfig.getUntrackedParameter<bool>("includeZBosonModule"         , true ) ;
  includeAutoAcceptEventModule_ = iConfig.getUntrackedParameter<bool>("includeAutoAcceptEventModule", true ) ;
  
  if(includeLeptonsAcceptModule_  ) childModules_.push_back(new IIHEModuleLeptonsAccept(iConfig ,consumesCollector())  ) ;   
  if(includeTriggerModule_        ) childModules_.push_back(new IIHEModuleTrigger(iConfig,consumesCollector())        ) ;
  if(includeEventModule_          ) childModules_.push_back(new IIHEModuleEvent(iConfig   ,consumesCollector()       )) ;
  if(includeMCTruthModule_        ){
    MCTruthModule_ = new IIHEModuleMCTruth(iConfig ,consumesCollector()) ;
    childModules_.push_back(MCTruthModule_) ;
  }
  if(includeVertexModule_         ) childModules_.push_back(new IIHEModuleVertex(iConfig ,consumesCollector())         ) ;
  if(includeSuperClusterModule_   ) childModules_.push_back(new IIHEModuleSuperCluster(iConfig ,consumesCollector())   ) ;
  if(includePhotonModule_         ) childModules_.push_back(new IIHEModulePhoton(iConfig ,consumesCollector())         ) ;
  if(includeElectronModule_       ) childModules_.push_back(new IIHEModuleGedGsfElectron(iConfig ,consumesCollector()) ) ;
  if(includeMuonModule_           ) childModules_.push_back(new IIHEModuleMuon(iConfig ,consumesCollector())           ) ;
  if(includeMETModule_            ) childModules_.push_back(new IIHEModuleMET(iConfig ,consumesCollector())            ) ;
  if(includeHEEPModule_           ) childModules_.push_back(new IIHEModuleHEEP(iConfig ,consumesCollector())           ) ;
  if(includeZBosonModule_         ) childModules_.push_back(new IIHEModuleZBoson(iConfig ,consumesCollector())         ) ;  
  if(includeAutoAcceptEventModule_) childModules_.push_back(new IIHEModuleAutoAcceptEvent(iConfig ,consumesCollector())) ;  
}

IIHEAnalysis::~IIHEAnalysis(){}

const MCTruthObject* IIHEAnalysis::MCTruth_getRecordByIndex(int index){
  if(MCTruthModule_){
    return MCTruthModule_->getRecordByIndex(index) ;
  }
  return 0 ;
}
const MCTruthObject* IIHEAnalysis::MCTruth_matchEtaPhi(float eta, float phi){
  if(MCTruthModule_){
    return MCTruthModule_->matchEtaPhi(eta, phi) ;
  }
  return 0 ;
}
int IIHEAnalysis::MCTruth_matchEtaPhi_getIndex(float eta, float phi){
  if(MCTruthModule_){
    return MCTruthModule_->matchEtaPhi_getIndex(eta, phi) ;
  }
  return -1 ;
}

bool IIHEAnalysis::addValueToMetaTree(std::string parName, float value){
  BranchWrapperF* bw = new BranchWrapperF(parName) ;
  metaTreePars_.push_back(bw) ;
  bw->config(metaTree_) ;
  bw->set(value) ;
  return true ;
}

bool IIHEAnalysis::addFVValueToMetaTree(std::string parName, std::vector<float> value){
  BranchWrapperFV* bw = new BranchWrapperFV(parName) ;
  for (unsigned int i=0 ; i<value.size() ; ++i){
    bw->push(value[i]);
  }
  bw->config(metaTree_) ;
  return true ;
}

bool IIHEAnalysis::branchExists(std::string name){
  for(unsigned int i=0 ; i<allVars_.size() ; ++i){
    if(allVars_.at(i)->name()==name) return true ;
  }
  return false ;
}

void IIHEAnalysis::setBranchType(int type){ currentVarType_ = type ; }
int  IIHEAnalysis::getBranchType(){ return currentVarType_ ; }

bool IIHEAnalysis::addBranch(std::string name){ return addBranch(name, currentVarType_) ; }
bool IIHEAnalysis::addBranch(std::string name, int type){
  // First check to see if this branch name has already been used
  bool success = !(branchExists(name)) ;
  if(debug_) std::cout << "Adding a branch named " << name << " " << success << endl ;
  if(success==false){
    return false ;
  }
  listOfBranches_.push_back(std::pair<std::string,int>(name,type)) ;
  switch(type){
    case kBool:{
      BranchWrapperB*  bw = new BranchWrapperB(name) ;
      vars_B_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kDouble:{
      BranchWrapperD*  bw = new BranchWrapperD(name) ;
      vars_D_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
    }
    case kFloat:{
      BranchWrapperF*  bw = new BranchWrapperF(name) ;
      vars_F_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kInt:{
      BranchWrapperI*  bw = new BranchWrapperI(name) ;
      vars_I_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kUInt:{
      BranchWrapperU*  bw = new BranchWrapperU(name) ;
      vars_U_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorBool:{
      BranchWrapperBV* bw = new BranchWrapperBV(name) ;
      vars_BV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorDouble:{
      BranchWrapperDV* bw = new BranchWrapperDV(name) ;
      vars_DV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorFloat:{
      BranchWrapperFV* bw = new BranchWrapperFV(name) ;
      vars_FV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorInt:{
      BranchWrapperIV* bw = new BranchWrapperIV(name) ;
      vars_IV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorUInt:{
      BranchWrapperUV* bw = new BranchWrapperUV(name) ;
      vars_UV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorBool:{
      BranchWrapperBVV* bw = new BranchWrapperBVV(name) ;
      vars_BVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorDouble:{
      BranchWrapperDVV* bw = new BranchWrapperDVV(name) ;
      vars_DVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorFloat:{
      BranchWrapperFVV* bw = new BranchWrapperFVV(name) ;
      vars_FVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorInt:{
      BranchWrapperIVV* bw = new BranchWrapperIVV(name) ;
      vars_IVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorUInt:{
      BranchWrapperUVV* bw = new BranchWrapperUVV(name) ;
      vars_UVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    default :
      std::cout << "Failed to make a branch" << std::endl ;
      return false ; // Bail out if we don't know the type of branch
  }
  return true ;
}

// ------------ method called once each job just before starting event loop  -------------
void IIHEAnalysis::beginJob(){
  for(unsigned int i=0 ; i<childModules_.size() ; ++i){
    childModules_.at(i)->config(this) ;
    childModules_.at(i)->pubBeginJob() ;
  }
  
  // We have to call this last because other modules can add particles to the whitelist.
  // However the other modules must follow MCTruthModule to use the whitelist.  Here is
  // where we break the chicken and egg problem.
  if(MCTruthModule_) MCTruthModule_->setWhitelist() ;
  
  configureBranches() ;
}

void IIHEAnalysis::listBranches(){
  dataTree_->GetListOfLeaves()->ls() ;
}

int IIHEAnalysis::saveToFile(TObject* obj){
  edm::Service<TFileService> fs ;
  fs->file().cd() ;
  return obj->Write() ;
}

void IIHEAnalysis::configureBranches(){
  for(unsigned int i=0 ; i<allVars_.size() ; ++i){
    allVars_.at(i)->config(dataTree_) ;
  }
  return ;
}

void IIHEAnalysis::addToMCTruthWhitelist(std::vector<int> pdgIds){
  for(unsigned int i=0 ; i<pdgIds.size() ; ++i){
    bool add = true ;
    for(unsigned int j=0 ; j<MCTruthWhitelist_.size() ; j++){
      if(abs(MCTruthWhitelist_.at(j))==abs(pdgIds.at(i))){
        add = false ;
        break ;
      }
    }
    if(add){
      MCTruthWhitelist_.push_back(pdgIds.at(i)) ;
    }
  }
}

// ------------ method called to for each event  -----------------------------------------

void IIHEAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  beginEvent() ;



  preScaleIndex_ = hltPrescaleProvider_.prescaleSet(iEvent,iSetup);
  // Get the default collections
  // These should be harmonised across submodules, where possible
//  iEvent.getByLabel(superClusterCollectionLabel_, superClusterCollection_) ;
  iEvent.getByToken(superClusterCollectionToken_, superClusterCollection_) ;
  iEvent.getByToken( photonCollectionToken_, photonCollection_) ;
  iEvent.getByToken( electronCollectionToken_ , electronCollection_) ;
  iEvent.getByToken( muonCollectionToken_, muonCollection_) ;
//  iEvent.getByLabel(         primaryVertexLabel_,           pvCollection_) ;
  iEvent.getByToken( vtxToken_ , pvCollection_);  
  //iEvent.getByLabel(    electronCollectionLabel_,     electronCollectionMiniAOD_) ;
  
//CHOOSE_RELEASE_START DEFAULT CMSSW_7_4_4 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_7_0_6_patch1 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1
  iEvent.getByToken(beamSpotToken_, beamspotHandle_) ;
// CHOOSE_RELEASE_END DEFAULT CMSSW_7_4_4 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_7_0_6_patch1 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1
/*CHOOSE_RELEASE_START CMSSW_5_3_11
  iEvent.getByLabel(beamSpotLabel_, beamspotHandle_) ;
CHOOSE_RELEASE_END CMSSW_5_3_11*/
  
  beamspot_->SetXYZ(beamspotHandle_->position().x(),beamspotHandle_->position().y(),beamspotHandle_->position().z()) ;
  
  // We take only the first primary vertex
  firstPrimaryVertex_->SetXYZ(0,0,0);
  const reco::VertexCollection* primaryVertices = getPrimaryVertices() ;
  if(primaryVertices->size()>0){
    reco::VertexCollection::const_iterator firstpv = primaryVertices->begin();
    firstPrimaryVertex_->SetXYZ(firstpv->x(),firstpv->y(),firstpv->z());
  }
  
  for(unsigned i=0 ; i<childModules_.size() ; ++i){
    childModules_.at(i)->pubAnalyze(iEvent, iSetup) ;
    if(rejectEvent_) break ;
  }
  endEvent() ;
}

void IIHEAnalysis::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){

  bool changed = true ;
  hltPrescaleProvider_.init(iRun, iSetup, trigEventTag_.process(), changed);

  nRuns_.push_back(iRun.run());
  for(unsigned int i=0 ; i<childModules_.size() ; ++i){
    childModules_.at(i)->pubBeginRun(iRun, iSetup) ;
  }
}

void IIHEAnalysis::beginEvent(){
  acceptEvent_ = false ;
  rejectEvent_ = false ;
  for(unsigned int i=0 ; i<childModules_.size() ; ++i){ childModules_.at(i)->pubBeginEvent() ; }
  for(unsigned int i=0 ; i<allVars_.size()      ; ++i){ allVars_.at(i)->beginEvent()         ; }
}
void IIHEAnalysis::endEvent(){
  for(unsigned int i=0 ; i<childModules_.size() ; ++i){ childModules_.at(i)->pubEndEvent() ; }
  for(unsigned int i=0 ; i<allVars_.size()      ; ++i){      allVars_.at(i)->endEvent()    ; }
  if(true==acceptEvent_ && false==rejectEvent_){
    dataTree_->Fill() ;
    nEventsStored_++ ;
  }
  nEvents_++ ;
}

// ------------ method called once each job just after ending the event loop  ------------
void IIHEAnalysis::endJob(){

  for(unsigned int i=0 ; i<childModules_.size() ; ++i){ childModules_.at(i)->pubEndJob() ; }
  std::vector<std::string> untouchedBranchNames ;
  for(unsigned int i=0 ; i<allVars_.size() ; ++i){
    if(allVars_.at(i)->is_touched()==false) untouchedBranchNames.push_back(allVars_.at(i)->name()) ;
  }
  if(debug_==true){
    if(untouchedBranchNames.size()>0){
      std::cout << "The following branches were never touched:" << std::endl ;
      for(unsigned int i=0 ; i<untouchedBranchNames.size() ; ++i){
        std::cout << "  " << untouchedBranchNames.at(i) << std::endl ;
      }
    }
  }
  
  addValueToMetaTree("nEventsRaw"   , nEvents_      ) ;
  addValueToMetaTree("nEventsStored", nEventsStored_) ;
  addFVValueToMetaTree("nRuns", nRuns_) ;
  metaTree_->Fill() ;
  
  std::cout << "There were " << nEvents_ << " total events of which " << nEventsStored_ << " were stored to file." << std::endl ;

}

// ------------ method for storing information into the TTree  ------------
bool IIHEAnalysis::store(std::string name, bool value){
  for(unsigned int i=0 ; i<vars_B_.size() ; ++i){
    if(vars_B_.at(i)->name()==name){
      vars_B_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_BV_.size() ; ++i){
    if(vars_BV_.at(i)->name()==name){
      vars_BV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (bool) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, double value){
  // Try to fill doubles, then floats
  for(unsigned int i=0 ; i<vars_D_.size() ; ++i){
    if(vars_D_.at(i)->name()==name){
      vars_D_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_DV_.size() ; ++i){
    if(vars_DV_.at(i)->name()==name){
      vars_DV_ .at(i)->push(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_F_.size() ; ++i){
    if(vars_F_.at(i)->name()==name){
      vars_F_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_FV_.size() ; ++i){
    if(vars_FV_.at(i)->name()==name){
      vars_FV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (double) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, float value){
  // Try to fill floats, then doubles
  for(unsigned int i=0 ; i<vars_F_.size() ; ++i){
    if(vars_F_.at(i)->name()==name){
      vars_F_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_FV_.size() ; ++i){
    if(vars_FV_.at(i)->name()==name){
      vars_FV_ .at(i)->push(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_D_.size() ; ++i){
    if(vars_D_.at(i)->name()==name){
      vars_D_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_DV_.size() ; ++i){
    if(vars_DV_.at(i)->name()==name){
      vars_DV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (float) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, int value){
  for(unsigned int i=0 ; i<vars_I_.size() ; ++i){
    if(vars_I_.at(i)->name()==name){
      vars_I_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_IV_.size() ; ++i){
    if(vars_IV_.at(i)->name()==name){
      vars_IV_ .at(i)->push(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_U_.size() ; ++i){
    if(vars_U_.at(i)->name()==name){
      vars_U_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_UV_.size() ; ++i){
    if(vars_UV_.at(i)->name()==name){
      vars_UV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (int) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, unsigned int value){
  for(unsigned int i=0 ; i<vars_U_.size() ; ++i){
    if(vars_U_.at(i)->name()==name){
      vars_U_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_UV_.size() ; ++i){
    if(vars_UV_.at(i)->name()==name){
      vars_UV_ .at(i)->push(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_I_.size() ; ++i){
    if(vars_I_.at(i)->name()==name){
      vars_I_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_IV_.size() ; ++i){
    if(vars_IV_.at(i)->name()==name){
      vars_IV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (uint) branch named " << name << std::endl ;
  return false ;
}

bool IIHEAnalysis::store(std::string name, std::vector<bool> values){
  for(unsigned int i=0 ; i<vars_BVV_.size() ; ++i){
    if(vars_BVV_.at(i)->name()==name){
      vars_BVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_BV_.size() ; ++i){
    if(vars_BV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_BV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector bool) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, std::vector<float> values){
  for(unsigned int i=0 ; i<vars_FVV_.size() ; ++i){
    if(vars_FVV_.at(i)->name()==name){
      vars_FVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_FV_.size() ; ++i){
    if(vars_FV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_FV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector float) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, std::vector<double> values){
  for(unsigned int i=0 ; i<vars_DVV_.size() ; ++i){
    if(vars_DVV_.at(i)->name()==name){
      vars_DVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_DV_.size() ; ++i){
    if(vars_DV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_DV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector double) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, std::vector<int> values){
  for(unsigned int i=0 ; i<vars_IVV_.size() ; ++i){
    if(vars_IVV_.at(i)->name()==name){
      vars_IVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_IV_.size() ; ++i){
    if(vars_IV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_IV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector int) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, std::vector<unsigned int> values){
  for(unsigned int i=0 ; i<vars_UVV_.size() ; ++i){
    if(vars_UVV_.at(i)->name()==name){
      vars_UVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_UV_.size() ; ++i){
    if(vars_UV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_UV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector uint) branch named " << name << std::endl ;
  return false ;
}

// Function to split strings.  Required for passing comma separated arguments via the pset
std::vector<std::string> IIHEAnalysis::splitString(const string &text, const char* sep){
  vector<string> results ;
  boost::split(results, text, boost::is_any_of(","));
  return results ;
}

DEFINE_FWK_MODULE(IIHEAnalysis);
