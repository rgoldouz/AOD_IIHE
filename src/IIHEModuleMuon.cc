#include "UserCode/IIHETree/interface/IIHEModuleMuon.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

//////////////////////////////////////////////////////////////////////////////////////////
//                             IIHEMuonTrackVariable classes                            //
//////////////////////////////////////////////////////////////////////////////////////////
IIHEMuonTrackVariableBase::IIHEMuonTrackVariableBase(std::string prefix, std::string name, int type){
  name_       = name ;
  branchName_ = prefix + "_" + name_ ;
  branchType_ = type ;
}
bool IIHEMuonTrackVariableBase::addBranch(IIHEAnalysis* analysis){
  return analysis->addBranch(branchName_, branchType_) ;
}

IIHEMuonTrackVariableInt::IIHEMuonTrackVariableInt(std::string prefix, std::string name):
IIHEMuonTrackVariableBase(prefix, name, kVectorInt){
  reset() ;
}
void IIHEMuonTrackVariableInt::store(IIHEAnalysis* analysis){
  analysis->store(BranchName(), value_) ;
}

IIHEMuonTrackVariableFloat::IIHEMuonTrackVariableFloat(std::string prefix, std::string name):
IIHEMuonTrackVariableBase(prefix, name, kVectorFloat){
  reset() ;
}
void IIHEMuonTrackVariableFloat::store(IIHEAnalysis* analysis){
  analysis->store(BranchName(), value_ ) ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                  IIHEMuonTrack class                                 //
//////////////////////////////////////////////////////////////////////////////////////////
IIHEMuonTrackWrapper::IIHEMuonTrackWrapper(std::string prefix){
  prefix_ = prefix ;
  
  charge_         = new IIHEMuonTrackVariableInt  (prefix_, "charge"        ) ;
  qoverp_         = new IIHEMuonTrackVariableFloat(prefix_, "qoverp"        ) ;
  pt_             = new IIHEMuonTrackVariableFloat(prefix_, "pt"            ) ;
  eta_            = new IIHEMuonTrackVariableFloat(prefix_, "eta"           ) ;
  phi_            = new IIHEMuonTrackVariableFloat(prefix_, "phi"           ) ;
  p_              = new IIHEMuonTrackVariableFloat(prefix_, "p"             ) ;
  px_             = new IIHEMuonTrackVariableFloat(prefix_, "px"            ) ;
  py_             = new IIHEMuonTrackVariableFloat(prefix_, "py"            ) ;
  pz_             = new IIHEMuonTrackVariableFloat(prefix_, "pz"            ) ;
  theta_          = new IIHEMuonTrackVariableFloat(prefix_, "theta"         ) ;
  lambda_         = new IIHEMuonTrackVariableFloat(prefix_, "lambda"        ) ;
  d0_             = new IIHEMuonTrackVariableFloat(prefix_, "d0"            ) ;
  dz_             = new IIHEMuonTrackVariableFloat(prefix_, "dz"            ) ;
  dz_beamspot_    = new IIHEMuonTrackVariableFloat(prefix_, "dz_beamspot"   ) ;
  dz_firstPVtx_   = new IIHEMuonTrackVariableFloat(prefix_, "dz_firstPVtx"  ) ;
  dxy_            = new IIHEMuonTrackVariableFloat(prefix_, "dxy"           ) ;
  dxy_beamspot_   = new IIHEMuonTrackVariableFloat(prefix_, "dxy_beamspot"  ) ;
  dxy_firstPVtx_  = new IIHEMuonTrackVariableFloat(prefix_, "dxy_firstPVtx" ) ;
  dsz_            = new IIHEMuonTrackVariableFloat(prefix_, "dsz"           ) ;
  vx_             = new IIHEMuonTrackVariableFloat(prefix_, "vx"            ) ;
  vy_             = new IIHEMuonTrackVariableFloat(prefix_, "vy"            ) ;
  vz_             = new IIHEMuonTrackVariableFloat(prefix_, "vz"            ) ;
  qoverpError_    = new IIHEMuonTrackVariableFloat(prefix_, "qoverpError"   ) ;
  ptError_        = new IIHEMuonTrackVariableFloat(prefix_, "ptError"       ) ;
  thetaError_     = new IIHEMuonTrackVariableFloat(prefix_, "thetaError"    ) ;
  lambdaError_    = new IIHEMuonTrackVariableFloat(prefix_, "lambdaError"   ) ;
  phiError_       = new IIHEMuonTrackVariableFloat(prefix_, "phiError"      ) ;
  dxyError_       = new IIHEMuonTrackVariableFloat(prefix_, "dxyError"      ) ;
  d0Error_        = new IIHEMuonTrackVariableFloat(prefix_, "d0Error"       ) ;
  dszError_       = new IIHEMuonTrackVariableFloat(prefix_, "dszError"      ) ;
  dzError_        = new IIHEMuonTrackVariableFloat(prefix_, "dzError"       ) ;
  etaError_       = new IIHEMuonTrackVariableFloat(prefix_, "etaError"      ) ;
  chi2_           = new IIHEMuonTrackVariableFloat(prefix_, "chi2"          ) ;
  ndof_           = new IIHEMuonTrackVariableFloat(prefix_, "ndof"          ) ;
  normalizedChi2_ = new IIHEMuonTrackVariableFloat(prefix_, "normalizedChi2") ;
      
  variables_.push_back((IIHEMuonTrackVariableBase*) qoverp_        ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) charge_        ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) pt_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) eta_           ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) phi_           ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) p_             ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) px_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) py_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) pz_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) theta_         ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) lambda_        ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) d0_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dz_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dz_beamspot_   ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dz_firstPVtx_  ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dxy_           ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dxy_beamspot_  ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dxy_firstPVtx_ ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dsz_           ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) vx_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) vy_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) vz_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) qoverpError_   ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) ptError_       ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) thetaError_    ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) lambdaError_   ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) phiError_      ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dxyError_      ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) d0Error_       ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dszError_      ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dzError_       ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) etaError_      ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) chi2_          ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) ndof_          ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) normalizedChi2_) ;
}

void IIHEMuonTrackWrapper::addBranches(IIHEAnalysis* analysis){
  for(unsigned int i=0 ; i<variables_.size() ; ++i){
    variables_.at(i)->addBranch(analysis) ;
  }
}
void IIHEMuonTrackWrapper::reset(){
  for(unsigned int i=0 ; i<variables_.size() ; ++i){
    variables_.at(i)->reset() ;
  }
}
void IIHEMuonTrackWrapper::fill(TrackRef& track, math::XYZPoint* beamspot, math::XYZPoint* firstPrimaryVertex){
  float etaError = track->thetaError()/sin(track->theta()) ;

  charge_        ->fill(track->charge()                ) ;
  qoverp_        ->fill(track->qoverp()                ) ;
  pt_            ->fill(track->pt()                    ) ;
  eta_           ->fill(track->eta()                   ) ;
  phi_           ->fill(track->phi()                   ) ;
  p_             ->fill(track->p()                     ) ;
  px_            ->fill(track->px()                    ) ;
  py_            ->fill(track->py()                    ) ;
  pz_            ->fill(track->pz()                    ) ;
  theta_         ->fill(track->theta()                 ) ;
  lambda_        ->fill(track->lambda()                ) ;
  d0_            ->fill(track->d0()                    ) ;
  dz_            ->fill(track->dz()                    ) ;
  dz_beamspot_   ->fill(track->dz(*beamspot)           ) ;
  dz_firstPVtx_  ->fill(track->dz(*firstPrimaryVertex) ) ;
  dxy_           ->fill(track->dxy()                   ) ;
  dxy_beamspot_  ->fill(track->dxy(*beamspot)          ) ;
  dxy_firstPVtx_ ->fill(track->dxy(*firstPrimaryVertex)) ;
  dsz_           ->fill(track->dsz(*beamspot)          ) ;
  vx_            ->fill(track->vx()                    ) ;
  vy_            ->fill(track->vy()                    ) ;
  vz_            ->fill(track->vz()                    ) ;
  ptError_       ->fill(track->ptError()               ) ;
  thetaError_    ->fill(track->thetaError()            ) ;
  lambdaError_   ->fill(track->lambdaError()           ) ;
  phiError_      ->fill(track->phiError()              ) ;
  dxyError_      ->fill(track->dxyError()              ) ;
  d0Error_       ->fill(track->d0Error()               ) ;
  dszError_      ->fill(track->dszError()              ) ;
  dzError_       ->fill(track->dzError()               ) ;
  etaError_      ->fill(etaError                       ) ;
  chi2_          ->fill(track->chi2()                  ) ;
  ndof_          ->fill(track->ndof()                  ) ;
  normalizedChi2_->fill(track->normalizedChi2()        ) ;

}
void IIHEMuonTrackWrapper::store(IIHEAnalysis* analysis){
  for(unsigned int i=0 ; i<variables_.size() ; ++i){
    variables_.at(i)->store(analysis) ;
  }
}



//////////////////////////////////////////////////////////////////////////////////////////
//                                  Main IIHEMuonModule                                 //
//////////////////////////////////////////////////////////////////////////////////////////
IIHEModuleMuon::IIHEModuleMuon(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC):
  IIHEModule(iConfig),
  globalTrackWrapper_(new IIHEMuonTrackWrapper("mu_gt")),
  outerTrackWrapper_ (new IIHEMuonTrackWrapper("mu_ot")),
  innerTrackWrapper_ (new IIHEMuonTrackWrapper("mu_it")){
  
  storeGlobalTrackMuons_ = iConfig.getUntrackedParameter<bool>("storeGlobalTrackMuons", true ) ;
  storeStandAloneMuons_  = iConfig.getUntrackedParameter<bool>("storeStandAloneMuons" , true ) ;
  storeInnerTrackMuons_  = iConfig.getUntrackedParameter<bool>("storeInnerTrackMuons" , true ) ;
  ptThreshold_           = iConfig.getUntrackedParameter<double>("muons_pTThreshold"  ,  0.0 ) ;
}
IIHEModuleMuon::~IIHEModuleMuon(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleMuon::beginJob(){
  setBranchType(kUInt) ;
  addBranch("mu_n"   ) ;
  addBranch("mu_n_saved") ;
  addBranch("mu_gt_n") ;
  addBranch("mu_ot_n") ;
  addBranch("mu_it_n") ;
  
  IIHEAnalysis* analysis = parent_ ;
  if(storeGlobalTrackMuons_) globalTrackWrapper_->addBranches(analysis) ;
  if(storeStandAloneMuons_ )  outerTrackWrapper_->addBranches(analysis) ;
  if(storeInnerTrackMuons_ )  innerTrackWrapper_->addBranches(analysis) ;
  
  // Muon type block
  setBranchType(kVectorBool) ;
  addBranch("mu_isGlobalMuon"      ) ;
  addBranch("mu_isStandAloneMuon"  ) ;
  addBranch("mu_isTrackerMuon"     ) ;
  addBranch("mu_isPFMuon"          ) ;
  addBranch("mu_isPFIsolationValid") ;
  
  // Hits block
  setBranchType(kVectorInt) ;
  addBranch("mu_numberOfMatchedStations" ) ;
  addBranch("mu_numberOfValidPixelHits"  ) ;
  addBranch("mu_numberOfValidTrackerHits") ;
  addBranch("mu_numberOfValidMuonHits"   ) ;
/*CHOOSE_RELEASE_START CMSSW_6_2_0_SLHC23_patch1
  addBranch("mu_numberOfValidGEMHits"    ) ;
CHOOSE_RELEASE_END CMSSW_6_2_0_SLHC23_patch1*/
/*CHOOSE_RELEASE_START DEFAULT
CHOOSE_RELEASE_END DEFAULT*/
  
  // TeV optimized values
  addBranch("mu_tevOptimized_charge", kVectorInt) ;
  setBranchType(kVectorFloat) ;
  addBranch("mu_tevOptimized_pt"   ) ;
  addBranch("mu_tevOptimized_eta"  ) ;
  addBranch("mu_tevOptimized_phi"  ) ;
  addBranch("mu_tevOptimized_theta") ;
  addBranch("mu_tevOptimized_px"   ) ;
  addBranch("mu_tevOptimized_py"   ) ;
  addBranch("mu_tevOptimized_pz"   ) ;
  
  addBranch("mu_tevOptimized_d0"           ) ;
  addBranch("mu_tevOptimized_dz"           ) ;
  addBranch("mu_tevOptimized_dz_beamSpot"  ) ;
  addBranch("mu_tevOptimized_dz_firstPVtx" ) ;
  addBranch("mu_tevOptimized_dxy"          ) ;
  addBranch("mu_tevOptimized_dxy_beamSpot" ) ;
  addBranch("mu_tevOptimized_dxy_firstPVtx") ;
  
  addBranch("mu_tevOptimized_ptError"   ) ;
  addBranch("mu_tevOptimized_etaError"  ) ;
  addBranch("mu_tevOptimized_phiError"  ) ;
  addBranch("mu_tevOptimized_thetaError") ;
  addBranch("mu_tevOptimized_d0Error"   ) ;
  addBranch("mu_tevOptimized_dzError"   ) ;
  addBranch("mu_tevOptimized_dxyError"  ) ;
  
  // Isolation block
  setBranchType(kVectorFloat) ;
  addBranch("mu_isolationR03_sumPt"        ) ;
  addBranch("mu_isolationR03_trackerVetoPt") ;
  addBranch("mu_isolationR03_emEt"         ) ;
  addBranch("mu_isolationR03_emVetoEt"     ) ;
  addBranch("mu_isolationR03_hadEt"        ) ;
  addBranch("mu_isolationR03_hadVetoEt"    ) ;
  
  addBranch("mu_isolationR05_sumPt"        ) ;
  addBranch("mu_isolationR05_trackerVetoPt") ;
  addBranch("mu_isolationR05_emEt"         ) ;
  addBranch("mu_isolationR05_emVetoEt"     ) ;
  addBranch("mu_isolationR05_hadEt"        ) ;
  addBranch("mu_isolationR05_hadVetoEt"    ) ;
  
  addBranch("mu_pfIsolationR03_sumChargedHadronPt"             ) ;
  addBranch("mu_pfIsolationR03_sumChargedParticlePt"           ) ;
  addBranch("mu_pfIsolationR03_sumPhotonEt"                    ) ;
  addBranch("mu_pfIsolationR03_sumNeutralHadronEtHighThreshold") ;
  addBranch("mu_pfIsolationR03_sumPhotonEtHighThreshold"       ) ;
  addBranch("mu_pfIsolationR03_sumPUPt"                        ) ;
  
  addBranch("mu_pfIsolationR04_sumChargedHadronPt"             ) ;
  addBranch("mu_pfIsolationR04_sumChargedParticlePt"           ) ;
  addBranch("mu_pfIsolationR04_sumPhotonEt"                    ) ;
  addBranch("mu_pfIsolationR04_sumNeutralHadronEtHighThreshold") ;
  addBranch("mu_pfIsolationR04_sumPhotonEtHighThreshold"       ) ;
  addBranch("mu_pfIsolationR04_sumPUPt"                        ) ;
  
//CHOOSE_RELEASE_START DEFAULT CMSSW_7_4_4 CMSSW_7_0_6_patch1 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1 CMSSW_7_6_3 
  addBranch("mu_pfMeanDRIsoProfileR03_sumChargedHadronPt"             ) ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumChargedParticlePt"           ) ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumPhotonEt"                    ) ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumNeutralHadronEtHighThreshold") ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumPhotonEtHighThreshold"       ) ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumPUPt"                        ) ;
  
  addBranch("mu_pfMeanDRIsoProfileR04_sumChargedHadronPt"             ) ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumChargedParticlePt"           ) ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumPhotonEt"                    ) ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumNeutralHadronEtHighThreshold") ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumPhotonEtHighThreshold"       ) ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumPUPt"                        ) ;
  
  addBranch("mu_mc_bestDR", kVectorFloat) ;
  addBranch("mu_mc_index" , kVectorInt  ) ;
  addBranch("mu_mc_ERatio", kVectorFloat) ;
//CHOOSE_RELEASE_END DEFAULT CMSSW_7_4_4 CMSSW_7_0_6_patch1 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1 CMSSW_7_6_3
/*CHOOSE_RELEASE_START  CMSSW_5_3_11
CHOOSE_RELEASE_END CMSSW_5_3_11*/
}

// ------------ method called to for each event  ------------
void IIHEModuleMuon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  reco::MuonCollection muons = parent_->getMuonCollection() ;
  
  math::XYZPoint* beamspot = parent_->getBeamspot() ;
  math::XYZPoint* firstPrimaryVertex = parent_->getFirstPrimaryVertex() ;
  
  store("mu_n", (unsigned int)(muons.size())) ;
  
  // Muons come with three tracks:
  //   Standalone track.  This is made from the outer detector
  //   Inner track.  This is made from the tracking system
  //   Global track.  This is made from a combination of the inner and outer tracks.
  // So we need to be a little careful when we get the variables.
  
  IIHEAnalysis* analysis = parent_ ;
  
  unsigned int mu_gt_n = 0 ;
  unsigned int mu_ot_n = 0 ;
  unsigned int mu_it_n = 0 ;
  int mu_n_saved = 0 ;
  
//  for(reco::MuonCollection::const_iterator muIt = muons.begin(); muIt != muons.end(); ++muIt){
  for(vector<reco::Muon>::const_iterator muIt = muons.begin() ; muIt != muons.end() ; ++muIt){
    bool isGlobalMuon     = muIt->isGlobalMuon()     ;
    bool isStandAloneMuon = muIt->isStandAloneMuon() ;
    bool isTrackerMuon    = muIt->isTrackerMuon()    ;
    
    // Try to save some disk space and CPU time
    bool storeThisMuon = false ;
    if(isGlobalMuon     && storeGlobalTrackMuons_) storeThisMuon = true ;
    if(isStandAloneMuon && storeStandAloneMuons_ ) storeThisMuon = true ;
    if(isTrackerMuon    && storeInnerTrackMuons_ ) storeThisMuon = true ;
    if(storeThisMuon==false) continue ;
    
    store("mu_isGlobalMuon"      , isGlobalMuon              ) ;
    store("mu_isStandAloneMuon"  , isStandAloneMuon          ) ;
    store("mu_isTrackerMuon"     , isTrackerMuon             ) ;
    store("mu_isPFMuon"          , muIt->isPFMuon()          ) ;
    store("mu_isPFIsolationValid", muIt->isPFIsolationValid()) ;
    
    int numberOfMatchStations    = 0 ;
    int numberOfValidPixelHits   = 0 ;
    int numberOfValidTrackerHits = 0 ;
    int numberOfValidMuonHits    = 0 ;
    
    numberOfMatchStations = muIt->numberOfMatchedStations() ;
    if(isTrackerMuon){
      numberOfValidPixelHits   = muIt->innerTrack()->hitPattern().numberOfValidPixelHits() ;
      numberOfValidTrackerHits = muIt->innerTrack()->hitPattern().trackerLayersWithMeasurement() ;
    }
    
    if(isGlobalMuon){
      numberOfValidMuonHits = muIt->globalTrack()->hitPattern().numberOfValidMuonHits() ;
    }
    
    store("mu_numberOfMatchedStations" , numberOfMatchStations   ) ;
    store("mu_numberOfValidPixelHits"  , numberOfValidPixelHits  ) ;
    store("mu_numberOfValidTrackerHits", numberOfValidTrackerHits) ;
    store("mu_numberOfValidMuonHits"   , numberOfValidMuonHits   ) ;
    
/*CHOOSE_RELEASE_START CMSSW_6_2_0_SLHC23_patch1
    int numberOfValidGEMHits     = 0 ;
    if(isStandAloneMuon){
      numberOfValidGEMHits = muIt->standAloneMuon()->hitPattern().numberOfValidMuonGEMHits() ;
    }
    store("mu_numberOfValidGEMHits"    , numberOfValidGEMHits    ) ;
CHOOSE_RELEASE_END CMSSW_6_2_0_SLHC23_patch1*/
/*CHOOSE_RELEASE_START DEFAULT
CHOOSE_RELEASE_END DEFAULT*/
    
    globalTrackWrapper_->reset() ;
    outerTrackWrapper_ ->reset() ;
    innerTrackWrapper_ ->reset() ;
    
    reco::TrackRef globalTrack = muIt->globalTrack() ;
    reco::TrackRef  outerTrack = muIt->outerTrack()  ;
    reco::TrackRef  innerTrack = muIt->innerTrack()  ;
    
    bool saveMuon = false ;
    
    if(storeInnerTrackMuons_){
      if( innerTrack.isNonnull() && muIt->   isTrackerMuon()){
        if(innerTrack->pt() > ptThreshold_) saveMuon = true ;
      }
    }
    if(storeStandAloneMuons_){
      if( outerTrack.isNonnull() && muIt->isStandAloneMuon()){
        if(outerTrack->pt() > ptThreshold_) saveMuon = true ;
      }
    }
    if(storeGlobalTrackMuons_){
      if(globalTrack.isNonnull() && muIt->    isGlobalMuon()){
        if(globalTrack->pt() > ptThreshold_) saveMuon = true ;
      }
    }
    
    if(saveMuon){
      if(storeInnerTrackMuons_){
        if( innerTrack.isNonnull() && muIt->   isTrackerMuon()){
          innerTrackWrapper_->fill( innerTrack, beamspot, firstPrimaryVertex) ;
        }
        innerTrackWrapper_ ->store(analysis) ;
        mu_it_n++ ;
      }
      if(storeStandAloneMuons_){
        if( outerTrack.isNonnull() && muIt->isStandAloneMuon()){
          outerTrackWrapper_->fill( outerTrack, beamspot, firstPrimaryVertex) ;
        }
        outerTrackWrapper_ ->store(analysis) ;
        mu_ot_n++ ;
      }
      if(storeGlobalTrackMuons_){
        if(globalTrack.isNonnull() && muIt->    isGlobalMuon()){
          globalTrackWrapper_->fill(globalTrack, beamspot, firstPrimaryVertex) ;
        }
        globalTrackWrapper_->store(analysis) ;
        mu_gt_n++ ;
      }
      mu_n_saved++ ;
    }
   
    // get TeV optimized track
    bool makeTevOptimizedTrack = muIt->isGlobalMuon() ;
    if(makeTevOptimizedTrack){

//      reco::Muon::MuonTrackTypePair tevOptimizedTrack = muon::tevOptimized(*muIt, 200, 17., 40., 0.25) ;
//      reco::Muon::MuonTrackTypePair tevOptimizedTrack = muon::tevOptimized(muIt->globalTrack(),muIt->innerTrack(),muIt->tpfmsTrack(),muIt->pickyTrack(),muIt->dytTrack(), 200, 17., 40., 0.25) ;
/*
      store("mu_tevOptimized_charge"       , tevOptimizedTrack.first->charge()                ) ;
      store("mu_tevOptimized_pt"           , tevOptimizedTrack.first->pt()                    ) ;
      store("mu_tevOptimized_eta"          , tevOptimizedTrack.first->eta()                   ) ;
      store("mu_tevOptimized_phi"          , tevOptimizedTrack.first->phi()                   ) ;
      store("mu_tevOptimized_theta"        , tevOptimizedTrack.first->theta()                 ) ;
      store("mu_tevOptimized_px"           , tevOptimizedTrack.first->px()                    ) ;
      store("mu_tevOptimized_py"           , tevOptimizedTrack.first->py()                    ) ;
      store("mu_tevOptimized_pz"           , tevOptimizedTrack.first->pz()                    ) ;
      store("mu_tevOptimized_d0"           , tevOptimizedTrack.first->d0()                    ) ;
      store("mu_tevOptimized_dz"           , tevOptimizedTrack.first->dz()                    ) ;
      store("mu_tevOptimized_dz_beamSpot"  , tevOptimizedTrack.first->dz(*beamspot)           ) ;
      store("mu_tevOptimized_dz_firstPVtx" , tevOptimizedTrack.first->dz(*firstPrimaryVertex) ) ;
      store("mu_tevOptimized_dxy"          , tevOptimizedTrack.first->dxy()                   ) ;
      store("mu_tevOptimized_dxy_beamSpot" , tevOptimizedTrack.first->dxy(*beamspot)          ) ;
      store("mu_tevOptimized_dxy_firstPVtx", tevOptimizedTrack.first->dxy(*firstPrimaryVertex)) ;
      store("mu_tevOptimized_ptError"      , tevOptimizedTrack.first->ptError()               ) ;
      store("mu_tevOptimized_etaError"     , tevOptimizedTrack.first->etaError()              ) ;
      store("mu_tevOptimized_phiError"     , tevOptimizedTrack.first->phiError()              ) ;
      store("mu_tevOptimized_thetaError"   , tevOptimizedTrack.first->thetaError()            ) ;
      store("mu_tevOptimized_d0Error"      , tevOptimizedTrack.first->d0Error()               ) ;
      store("mu_tevOptimized_dzError"      , tevOptimizedTrack.first->dzError()               ) ;
      store("mu_tevOptimized_dxyError"     , tevOptimizedTrack.first->dxyError()              ) ;
*/
    }
    else{
      int   defValueInt   = -999.0 ;
      float defValueFloat = -999.0 ;
      store("mu_tevOptimized_charge"       , defValueInt  ) ;
      store("mu_tevOptimized_pt"           , defValueFloat) ;
      store("mu_tevOptimized_eta"          , defValueFloat) ;
      store("mu_tevOptimized_phi"          , defValueFloat) ;
      store("mu_tevOptimized_theta"        , defValueFloat) ;
      store("mu_tevOptimized_px"           , defValueFloat) ;
      store("mu_tevOptimized_py"           , defValueFloat) ;
      store("mu_tevOptimized_pz"           , defValueFloat) ;
      store("mu_tevOptimized_d0"           , defValueFloat) ;
      store("mu_tevOptimized_dz"           , defValueFloat) ;
      store("mu_tevOptimized_dz_beamSpot"  , defValueFloat) ;
      store("mu_tevOptimized_dz_firstPVtx" , defValueFloat) ;
      store("mu_tevOptimized_dxy"          , defValueFloat) ;
      store("mu_tevOptimized_dxy_beamSpot" , defValueFloat) ;
      store("mu_tevOptimized_dxy_firstPVtx", defValueFloat) ;
      store("mu_tevOptimized_ptError"      , defValueFloat) ;
      store("mu_tevOptimized_etaError"     , defValueFloat) ;
      store("mu_tevOptimized_phiError"     , defValueFloat) ;
      store("mu_tevOptimized_thetaError"   , defValueFloat) ;
      store("mu_tevOptimized_d0Error"      , defValueFloat) ;
      store("mu_tevOptimized_dzError"      , defValueFloat) ;
      store("mu_tevOptimized_dxyError"     , defValueFloat) ;
    }
    
    // Isolation variables
    const MuonIsolation   iso30       = muIt->isolationR03() ;
    const MuonIsolation   iso50       = muIt->isolationR05() ;
    const MuonPFIsolation pfIso30     = muIt->pfIsolationR03() ;
    const MuonPFIsolation pfIso40     = muIt->pfIsolationR04() ;
    
    store("mu_isolationR03_sumPt"        , iso30.sumPt        ) ;
    store("mu_isolationR03_trackerVetoPt", iso30.trackerVetoPt) ;
    store("mu_isolationR03_emEt"         , iso30.emEt         ) ;
    store("mu_isolationR03_emVetoEt"     , iso30.emVetoEt     ) ;
    store("mu_isolationR03_hadEt"        , iso30.hadEt        ) ;
    store("mu_isolationR03_hadVetoEt"    , iso30.hadVetoEt    ) ;
    
    store("mu_isolationR05_sumPt"        , iso50.sumPt        ) ;
    store("mu_isolationR05_trackerVetoPt", iso50.trackerVetoPt) ;
    store("mu_isolationR05_emEt"         , iso50.emEt         ) ;
    store("mu_isolationR05_emVetoEt"     , iso50.emVetoEt     ) ;
    store("mu_isolationR05_hadEt"        , iso50.hadEt        ) ;
    store("mu_isolationR05_hadVetoEt"    , iso50.hadVetoEt    ) ;
    
    store("mu_pfIsolationR03_sumChargedHadronPt"             , pfIso30.sumChargedHadronPt             ) ;
    store("mu_pfIsolationR03_sumChargedParticlePt"           , pfIso30.sumChargedParticlePt           ) ;
    store("mu_pfIsolationR03_sumPhotonEt"                    , pfIso30.sumPhotonEt                    ) ;
    store("mu_pfIsolationR03_sumNeutralHadronEtHighThreshold", pfIso30.sumNeutralHadronEtHighThreshold) ;
    store("mu_pfIsolationR03_sumPhotonEtHighThreshold"       , pfIso30.sumPhotonEtHighThreshold       ) ;
    store("mu_pfIsolationR03_sumPUPt"                        , pfIso30.sumPUPt                        ) ;
    
    store("mu_pfIsolationR04_sumChargedHadronPt"             , pfIso40.sumChargedHadronPt             ) ;
    store("mu_pfIsolationR04_sumChargedParticlePt"           , pfIso40.sumChargedParticlePt           ) ;
    store("mu_pfIsolationR04_sumPhotonEt"                    , pfIso40.sumPhotonEt                    ) ;
    store("mu_pfIsolationR04_sumNeutralHadronEtHighThreshold", pfIso40.sumNeutralHadronEtHighThreshold) ;
    store("mu_pfIsolationR04_sumPhotonEtHighThreshold"       , pfIso40.sumPhotonEtHighThreshold       ) ;
    store("mu_pfIsolationR04_sumPUPt"                        , pfIso40.sumPUPt                        ) ;

//CHOOSE_RELEASE_START DEFAULT CMSSW_7_4_4 CMSSW_7_0_6_patch1 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1 CMSSW_7_6_3
    const MuonPFIsolation pfMeanIso30 = muIt->pfMeanDRIsoProfileR03() ;
    const MuonPFIsolation pfMeanIso40 = muIt->pfMeanDRIsoProfileR04() ;

    store("mu_pfMeanDRIsoProfileR03_sumChargedHadronPt"             , pfMeanIso30.sumChargedHadronPt             ) ;
    store("mu_pfMeanDRIsoProfileR03_sumChargedParticlePt"           , pfMeanIso30.sumChargedParticlePt           ) ;
    store("mu_pfMeanDRIsoProfileR03_sumPhotonEt"                    , pfMeanIso30.sumPhotonEt                    ) ;
    store("mu_pfMeanDRIsoProfileR03_sumNeutralHadronEtHighThreshold", pfMeanIso30.sumNeutralHadronEtHighThreshold) ;
    store("mu_pfMeanDRIsoProfileR03_sumPhotonEtHighThreshold"       , pfMeanIso30.sumPhotonEtHighThreshold       ) ;
    store("mu_pfMeanDRIsoProfileR03_sumPUPt"                        , pfMeanIso30.sumPUPt                        ) ;
    
    store("mu_pfMeanDRIsoProfileR04_sumChargedHadronPt"             , pfMeanIso40.sumChargedHadronPt             ) ;
    store("mu_pfMeanDRIsoProfileR04_sumChargedParticlePt"           , pfMeanIso40.sumChargedParticlePt           ) ;
    store("mu_pfMeanDRIsoProfileR04_sumPhotonEt"                    , pfMeanIso40.sumPhotonEt                    ) ;
    store("mu_pfMeanDRIsoProfileR04_sumNeutralHadronEtHighThreshold", pfMeanIso40.sumNeutralHadronEtHighThreshold) ;
    store("mu_pfMeanDRIsoProfileR04_sumPhotonEtHighThreshold"       , pfMeanIso40.sumPhotonEtHighThreshold       ) ;
    store("mu_pfMeanDRIsoProfileR04_sumPUPt"                        , pfMeanIso40.sumPUPt                        ) ;
//CHOOSE_RELEASE_END DEFAULT CMSSW_7_4_4 CMSSW_7_0_6_patch1 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1 CMSSW_7_6_3
/*CHOOSE_RELEASE_START  CMSSW_5_3_11
CHOOSE_RELEASE_END CMSSW_5_3_11*/
    
    // Now apply truth matching.
    int index = MCTruth_matchEtaPhi_getIndex(muIt->eta(), muIt->phi()) ;
    if(index>=0){
      const MCTruthObject* MCTruth = MCTruth_getRecordByIndex(index) ;
      store("mu_mc_bestDR", deltaR(muIt->eta(), muIt->phi(), MCTruth->eta(), MCTruth->phi())) ;
      store("mu_mc_index" , index) ;
      store("mu_mc_ERatio", muIt->energy()/MCTruth->energy()) ;
    }
    else{
      store("mu_mc_bestDR", 999.0) ;
      store("mu_mc_index" ,    -1) ;
      store("mu_mc_ERatio", 999.0) ;
    }
  }
  store("mu_n_saved", mu_n_saved) ;
  store("mu_gt_n", mu_gt_n) ;
  store("mu_ot_n", mu_ot_n) ;
  store("mu_it_n", mu_it_n) ;
}

void IIHEModuleMuon::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleMuon::beginEvent(){}
void IIHEModuleMuon::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleMuon::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleMuon);
