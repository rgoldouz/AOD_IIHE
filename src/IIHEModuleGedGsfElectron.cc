#include "UserCode/IIHETree/interface/IIHEModuleGedGsfElectron.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/EcalPreshowerTopology.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/Records/interface/CaloTopologyRecord.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include <iostream>
#include <TMath.h>
#include <vector>
#include <Math/VectorUtil.h>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleGedGsfElectron::IIHEModuleGedGsfElectron(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  ebReducedRecHitCollection_ = iC.consumes<EcalRecHitCollection> (iConfig.getParameter<InputTag>("ebReducedRecHitCollection"));
  eeReducedRecHitCollection_ = iC.consumes<EcalRecHitCollection> (iConfig.getParameter<InputTag>("eeReducedRecHitCollection"));
  generalTracksToken_ = iC.consumes<reco::TrackCollection> (iConfig.getParameter<InputTag>("generalTracksLabel"));
  beamSpotToken_      = consumes<reco::BeamSpot>(iConfig.getParameter<InputTag>("beamSpot")) ;
  rhoTokenAll_ = iC.consumes<double> (iConfig.getParameter<edm::InputTag>("eventRho"));
  ETThreshold_ = iConfig.getUntrackedParameter<double>("electrons_ETThreshold", 0.0 ) ;
}
IIHEModuleGedGsfElectron::~IIHEModuleGedGsfElectron(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleGedGsfElectron::beginJob(){
  addBranch("gsf_n", kUInt) ;
  addBranch("gsf_classification", kVectorInt) ;
  setBranchType(kVectorFloat) ;
  addBranch("gsf_energy") ;
  addBranch("gsf_p") ;
  addBranch("gsf_pt") ;
  addBranch("gsf_scE1x5") ;
  addBranch("gsf_scE5x5") ;
  addBranch("gsf_scE2x5Max") ;
  addBranch("gsf_eta") ;
  addBranch("gsf_phi") ;
  addBranch("gsf_theta") ;
  addBranch("gsf_px") ;
  addBranch("gsf_py") ;
  addBranch("gsf_pz") ;
  addBranch("gsf_superClusterEta") ;
  addBranch("gsf_superClusterEnergy") ;
  addBranch("gsf_caloEnergy") ;
  addBranch("gsf_deltaEtaSuperClusterTrackAtVtx") ;
  addBranch("gsf_deltaPhiSuperClusterTrackAtVtx") ;
  addBranch("gsf_hadronicOverEm") ;
  addBranch("gsf_hcalDepth1OverEcal") ;
  addBranch("gsf_hcalDepth2OverEcal") ;
  addBranch("gsf_dr03TkSumPt") ;
//  addBranch("gsf_dr03TkSumPtCorrected") ;
  addBranch("gsf_dr03EcalRecHitSumEt") ;
  addBranch("gsf_dr03HcalDepth1TowerSumEt") ;
  addBranch("gsf_dr03HcalDepth2TowerSumEt") ;
  addBranch("gsf_charge", kVectorInt) ;
  addBranch("gsf_sigmaIetaIeta") ;
  setBranchType(kVectorBool) ;
  addBranch("gsf_ecaldrivenSeed"   ) ;
  addBranch("gsf_trackerdrivenSeed") ;
  addBranch("gsf_isEB") ;
  addBranch("gsf_isEE") ;
  setBranchType(kVectorFloat) ;
  addBranch("gsf_deltaEtaSeedClusterTrackAtCalo") ;
  addBranch("gsf_deltaPhiSeedClusterTrackAtCalo") ;
  addBranch("gsf_ecalEnergy") ;
  addBranch("gsf_eSuperClusterOverP") ;
  addBranch("gsf_dxy") ;
  addBranch("gsf_dxy_beamSpot") ;
  addBranch("gsf_dxy_firstPVtx") ;
  addBranch("gsf_dxyError") ;
  addBranch("gsf_dz") ;
  addBranch("gsf_dz_beamSpot") ;
  addBranch("gsf_dz_firstPVtx") ;
  addBranch("gsf_dzError") ;
  addBranch("gsf_vz") ;
  setBranchType(kVectorInt) ;
  addBranch("gsf_numberOfValidHits") ;
  addBranch("gsf_nLostInnerHits"   ) ;
  addBranch("gsf_nLostOuterHits"   ) ;
  addBranch("gsf_convFlags"        ) ;
  setBranchType(kVectorFloat) ;
  addBranch("gsf_convDist") ;
  addBranch("gsf_convDcot") ;
  addBranch("gsf_convRadius") ;
  addBranch("gsf_fBrem") ;
  addBranch("gsf_e1x5") ;
  addBranch("gsf_e2x5Max") ;
  addBranch("gsf_e5x5") ;
  addBranch("gsf_r9") ;
  addBranch("gsf_deltaPhiSeedClusterTrackAtCalo") ;
  addBranch("gsf_deltaEtaSeedClusterTrackAtCalo") ;
  addBranch("gsf_deltaEtaSeedClusterTrackAtVtx") ;
  addBranch("gsf_hitsinfo", kVectorVectorInt) ;

  setBranchType(kVectorFloat) ;
  addBranch("gsf_pixelMatch_dPhi1") ;
  addBranch("gsf_pixelMatch_dPhi2") ;
  addBranch("gsf_pixelMatch_dRz1" ) ;
  addBranch("gsf_pixelMatch_dRz2" ) ;
  setBranchType(kVectorInt) ;
  addBranch("gsf_pixelMatch_subDetector1") ;
  addBranch("gsf_pixelMatch_subDetector2") ;
  
  addBranch("gsf_mc_bestDR", kVectorFloat) ;
  addBranch("gsf_mc_index" , kVectorInt  ) ;
  addBranch("gsf_mc_ERatio", kVectorFloat) ;

  setBranchType(kVectorFloat) ;
  addBranch("gsf_sc_energy") ;
  addBranch("gsf_sc_eta") ;
  addBranch("gsf_sc_etacorr") ;
  addBranch("gsf_sc_theta") ;
  addBranch("gsf_sc_thetacorr") ;
  addBranch("gsf_sc_et") ;
  addBranch("gsf_sc_phi") ;
  addBranch("gsf_sc_px") ;
  addBranch("gsf_sc_py") ;
  addBranch("gsf_sc_pz") ;
  addBranch("gsf_sc_x") ;
  addBranch("gsf_sc_y") ;
  addBranch("gsf_sc_z") ;
  addBranch("gsf_sc_phiWidth") ;
  addBranch("gsf_sc_etaWidth") ;
  addBranch("gsf_sc_seed_rawId", kVectorInt) ;
  addBranch("gsf_sc_seed_ieta", kVectorInt) ;
  addBranch("gsf_sc_seed_iphi", kVectorInt) ;

  setBranchType(kVectorFloat) ;
  addBranch("gsf_swissCross") ;
  addBranch("gsf_sc_rawEnergy") ;
  addBranch("gsf_sc_preshowerEnergy") ;
  addBranch("gsf_sc_lazyTools_e2x5Right") ;
  addBranch("gsf_sc_lazyTools_e2x5Left") ;
  addBranch("gsf_sc_lazyTools_e2x5Top") ;
  addBranch("gsf_sc_lazyTools_e2x5Bottom") ;
  addBranch("gsf_sc_lazyTools_eMax") ;
  addBranch("gsf_sc_lazyTools_e2nd") ;
  addBranch("gsf_sc_lazyTools_eRight") ;
  addBranch("gsf_sc_lazyTools_eLeft") ;
  addBranch("gsf_sc_lazyTools_eTop") ;
  addBranch("gsf_sc_lazyTools_eBottom") ;
  addBranch("gsf_sc_lazyTools_e2x2") ;
  addBranch("gsf_sc_lazyTools_e3x3") ;
  addBranch("gsf_sc_lazyTools_e4x4") ;
  addBranch("gsf_sc_lazyTools_e5x5") ;
  addBranch("gsf_sc_lazyTools_e1x3") ;
  addBranch("gsf_sc_lazyTools_e3x1") ;
  addBranch("gsf_sc_lazyTools_e1x5") ;
  addBranch("gsf_sc_lazyTools_e5x1") ;
  addBranch("gsf_sc_lazyTools_eshitsixix") ;
  addBranch("gsf_sc_lazyTools_eshitsiyiy") ;
  addBranch("gsf_sc_lazyTools_eseffsixix") ;
  addBranch("gsf_sc_lazyTools_eseffsiyiy") ;
  addBranch("gsf_sc_lazyTools_eseffsirir") ;
  addBranch("gsf_sc_lazyTools_BasicClusterSeedTime") ;


  setBranchType(kVectorBool) ;
  addBranch("gsf_isHeepV60");

}

// ------------ method called to for each event  ------------
void IIHEModuleGedGsfElectron::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  // Delegate default electron collection name to IIHEAnalysis class
  reco::GsfElectronCollection electrons = parent_->getElectronCollection() ;
 
  Handle<EcalRecHitCollection> EBHits;
  Handle<EcalRecHitCollection> EEHits;
  iEvent.getByToken(ebReducedRecHitCollection_, EBHits) ;
  iEvent.getByToken(eeReducedRecHitCollection_, EEHits) ;

  const EcalRecHitCollection* theBarrelEcalRecHits = EBHits.product () ;
  const EcalRecHitCollection* theEndcapEcalRecHits = EEHits.product () ;


  edm::Handle<reco::BeamSpot> beamspotHandle_ ;
  iEvent.getByToken(beamSpotToken_, beamspotHandle_) ;

  edm::ESHandle<CaloGeometry> pGeometry ;
  iSetup.get<CaloGeometryRecord>().get(pGeometry) ;
  CaloGeometry* geometry = (CaloGeometry*) pGeometry.product() ;
  const CaloSubdetectorGeometry* geometryES = geometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower) ;
  CaloSubdetectorTopology* topology_ES = (geometryES) ? new EcalPreshowerTopology(geometry) : 0 ;

  
  math::XYZPoint* beamspot     = parent_->getBeamspot() ;
  math::XYZPoint* firstpvertex = parent_->getFirstPrimaryVertex() ;
  float pv_z = firstpvertex->z() ; 
  EcalClusterLazyTools lazytool(iEvent, iSetup, parent_->getReducedBarrelRecHitCollectionToken(), parent_->getReducedEndcapRecHitCollectionToken(), parent_->getReducedESRecHitCollectionToken()) ;

  edm::Handle<double> rhoHandle ;
  iEvent.getByToken(rhoTokenAll_, rhoHandle) ;
  double rho = *rhoHandle ;
 
  unsigned int gsf_n = 0 ;
  for(vector<reco::GsfElectron>::const_iterator gsfiter=electrons.begin() ; gsfiter!=electrons.end() ; ++gsfiter){
    
    float ET = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
    if(ET<ETThreshold_ && gsfiter->pt()<ETThreshold_) continue ;
    gsf_n++ ;
    
    //Fill the gsf related variables

//CHOOSE_RELEASE_START DEFAULT CMSSW_7_4_4 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_7_6_3
    int gsf_nLostInnerHits = gsfiter->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS) ;
    int gsf_nLostOuterHits = gsfiter->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_OUTER_HITS) ;
//CHOOSE_RELEASE_END DEFAULT CMSSW_7_4_4 CMSSW_7_3_0 CMSSW_7_2_0 CMSSW_7_6_3
/*CHOOSE_RELEASE_START CMSSW_7_0_6_patch1 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1 CMSSW_5_3_11
    int gsf_nLostInnerHits = gsfiter->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() ;
    int gsf_nLostOuterHits = gsfiter->gsfTrack()->trackerExpectedHitsOuter().numberOfLostHits() ;
CHOOSE_RELEASE_END CMSSW_7_0_6_patch1 CMSSW_6_2_5 CMSSW_6_2_0_SLHC23_patch1 CMSSW_5_3_11*/
    
    store("gsf_energy"                        , gsfiter->energy()                        ) ;
    store("gsf_p"                             , gsfiter->p()                             ) ;
    store("gsf_pt"                            , gsfiter->pt()                            ) ;
    store("gsf_classification"                , gsfiter->classification()                ) ;
    store("gsf_scE1x5"                        , gsfiter->scE1x5()                        ) ;
    store("gsf_scE5x5"                        , gsfiter->scE5x5()                        ) ;
    store("gsf_scE2x5Max"                     , gsfiter->scE2x5Max()                     ) ;
    store("gsf_eta"                           , gsfiter->eta()                           ) ;
    store("gsf_phi"                           , gsfiter->phi()                           ) ;
    store("gsf_theta"                         , gsfiter->theta()                         ) ;
    store("gsf_px"                            , gsfiter->px()                            ) ;
    store("gsf_py"                            , gsfiter->py()                            ) ;
    store("gsf_pz"                            , gsfiter->pz()                            ) ;
    store("gsf_superClusterEta"               , gsfiter->superCluster()->eta()           ) ;
    store("gsf_superClusterEnergy"            , gsfiter->superCluster()->energy()        ) ;
    store("gsf_caloEnergy"                    , gsfiter->caloEnergy()                    ) ;
    store("gsf_deltaEtaSuperClusterTrackAtVtx", gsfiter->deltaEtaSuperClusterTrackAtVtx()) ;
    store("gsf_deltaPhiSuperClusterTrackAtVtx", gsfiter->deltaPhiSuperClusterTrackAtVtx()) ;
    store("gsf_hadronicOverEm"                , gsfiter->hadronicOverEm()                ) ;
    store("gsf_hcalDepth1OverEcal"            , gsfiter->hcalDepth1OverEcal()            ) ;
    store("gsf_hcalDepth2OverEcal"            , gsfiter->hcalDepth2OverEcal()            ) ;
    store("gsf_dr03TkSumPt"                   , gsfiter->dr03TkSumPt()                   ) ;
    store("gsf_dr03EcalRecHitSumEt"           , gsfiter->dr03EcalRecHitSumEt()           ) ;
    store("gsf_dr03HcalDepth1TowerSumEt"      , gsfiter->dr03HcalDepth1TowerSumEt()      ) ;
    store("gsf_dr03HcalDepth2TowerSumEt"      , gsfiter->dr03HcalDepth2TowerSumEt()      ) ;
    store("gsf_charge"                        , gsfiter->charge()                        ) ;
    store("gsf_sigmaIetaIeta"                 , gsfiter->sigmaIetaIeta()                 ) ;
    store("gsf_ecaldrivenSeed"                , gsfiter->ecalDrivenSeed()                ) ;
    store("gsf_trackerdrivenSeed"             , gsfiter->trackerDrivenSeed()             ) ;
    store("gsf_isEB"                          , gsfiter->isEB()                          ) ;
    store("gsf_isEE"                          , gsfiter->isEE()                          ) ;
    store("gsf_deltaEtaSeedClusterTrackAtCalo", gsfiter->deltaEtaSeedClusterTrackAtCalo()) ;
    store("gsf_deltaPhiSeedClusterTrackAtCalo", gsfiter->deltaPhiSeedClusterTrackAtCalo()) ;
    store("gsf_ecalEnergy"                    , gsfiter->ecalEnergy()                    ) ;
    store("gsf_eSuperClusterOverP"            , gsfiter->eSuperClusterOverP()            ) ;
    store("gsf_dxy"                           , gsfiter->gsfTrack()->dxy()               ) ;
    store("gsf_dxy_beamSpot"                  , gsfiter->gsfTrack()->dxy(*beamspot)      ) ;
    store("gsf_dxy_firstPVtx"                 , gsfiter->gsfTrack()->dxy(*firstpvertex)  ) ;
    store("gsf_dxyError"                      , gsfiter->gsfTrack()->dxyError()          ) ;
    store("gsf_dz"                            , gsfiter->gsfTrack()->dz()                ) ;
    store("gsf_dz_beamSpot"                   , gsfiter->gsfTrack()->dz(*beamspot)       ) ;
    store("gsf_dz_firstPVtx"                  , gsfiter->gsfTrack()->dz(*firstpvertex)   ) ;
    store("gsf_dzError"                       , gsfiter->gsfTrack()->dzError()           ) ; 
    store("gsf_vz"                            , gsfiter->gsfTrack()->vz()                ) ;
    store("gsf_numberOfValidHits"             , gsfiter->gsfTrack()->numberOfValidHits() ) ;
    store("gsf_nLostInnerHits"                , gsf_nLostInnerHits                       ) ;
    store("gsf_nLostOuterHits"                , gsf_nLostOuterHits                       ) ;
    store("gsf_convFlags"                     , gsfiter->convFlags()                     ) ;
    store("gsf_convDist"                      , gsfiter->convDist()                      ) ;
    store("gsf_convDcot"                      , gsfiter->convDcot()                      ) ;
    store("gsf_convRadius"                    , gsfiter->convRadius()                    ) ;
    store("gsf_fBrem"                         , gsfiter->fbrem()                         ) ;
    store("gsf_e1x5"                          , gsfiter->e1x5()                          ) ;
    store("gsf_r9"                            , gsfiter->r9()                            ) ;
    store("gsf_e2x5Max"                       , gsfiter->e2x5Max()                       ) ;
    store("gsf_e5x5"                          , gsfiter->e5x5()                          ) ;
    store("gsf_deltaPhiSeedClusterTrackAtCalo", gsfiter->deltaPhiSeedClusterTrackAtCalo()) ;
    store("gsf_deltaEtaSeedClusterTrackAtCalo", gsfiter->deltaEtaSeedClusterTrackAtCalo()) ;
    store("gsf_deltaEtaSeedClusterTrackAtVtx" , gsfiter->deltaEtaSeedClusterTrackAtVtx() ) ;

    float sc_energy = gsfiter->superCluster()->rawEnergy()+gsfiter->superCluster()->preshowerEnergy() ;
    float sc_et     = sc_energy*sin(2.*atan(exp(-1.*gsfiter->superCluster()->eta()))) ;
    float etaCorr = etacorr( gsfiter->superCluster()->eta(), pv_z, gsfiter->superCluster()->position().z()) ;

    store("gsf_sc_eta"        , gsfiter->superCluster()->eta()                    ) ;
    store("gsf_sc_etacorr"    , etaCorr                                           ) ;
    store("gsf_sc_theta"      , 2.*atan(exp(-1.*gsfiter->superCluster()->eta()))  ) ;
    store("gsf_sc_thetacorr"  , 2.*atan(exp(-1.*etaCorr))                         ) ;
    store("gsf_sc_phi"        , gsfiter->superCluster()->phi()                    ) ;
    store("gsf_sc_energy"     , sc_energy                                         ) ;
    store("gsf_sc_et"         , sc_et                                             ) ;
    store("gsf_sc_px"         , sc_et*cos(gsfiter->superCluster()->phi())         ) ;
    store("gsf_sc_py"         , sc_et*sin(gsfiter->superCluster()->phi())         ) ;
    store("gsf_sc_pz"         , sc_energy*tanh(gsfiter->superCluster()->eta())    ) ;
    store("gsf_sc_x"          , gsfiter->superCluster()->position().x()           ) ;
    store("gsf_sc_y"          , gsfiter->superCluster()->position().y()           ) ;
    store("gsf_sc_z"          , gsfiter->superCluster()->position().z()           ) ;
    store("gsf_sc_phiWidth"   , gsfiter->superCluster()->phiWidth()               ) ;
    store("gsf_sc_etaWidth"   , gsfiter->superCluster()->etaWidth()               ) ;

    store("gsf_sc_seed_rawId" , gsfiter->superCluster()->seed()->seed().rawId()   );

    const std::vector<std::pair<DetId,float> > & hits= gsfiter->superCluster()->hitsAndFractions();
    if (gsfiter->isEB()){
      EBDetId EBscID = EBDetId(gsfiter->superCluster()->seed()->seed().rawId());
      store("gsf_sc_seed_ieta" , EBscID.ieta()   );
      store("gsf_sc_seed_iphi" , EBscID.iphi()   );
      std::pair<DetId, float> id = EcalClusterTools::getMaximum(hits, theBarrelEcalRecHits); 
      store("gsf_swissCross"  , EcalTools::swissCross(id.first,*theBarrelEcalRecHits,0.));
    }
    else if (gsfiter->isEE()){
      EEDetId EEscID = EEDetId(gsfiter->superCluster()->seed()->seed().rawId());
      store("gsf_sc_seed_ieta" , EEscID.iy()   );
      store("gsf_sc_seed_iphi" , EEscID.ix()   );
      std::pair<DetId, float> id = EcalClusterTools::getMaximum(hits, theEndcapEcalRecHits); 
      store("gsf_swissCross"  , EcalTools::swissCross(id.first,*theEndcapEcalRecHits,0.));
    }

    store("gsf_sc_rawEnergy"          , gsfiter->superCluster()->rawEnergy()      ) ;
    store("gsf_sc_preshowerEnergy"          , gsfiter->superCluster()->preshowerEnergy()) ;

    reco::SuperClusterRef    cl_ref = gsfiter->superCluster() ;
    const reco::CaloClusterPtr seed = gsfiter->superCluster()->seed() ;

    store("gsf_sc_lazyTools_e2x5Right"   , lazytool.e2x5Right (*seed)            );
    store("gsf_sc_lazyTools_e2x5Left"    , lazytool.e2x5Left (*seed)             );
    store("gsf_sc_lazyTools_e2x5Top"     , lazytool.e2x5Top (*seed)              );
    store("gsf_sc_lazyTools_e2x5Bottom"  , lazytool.e2x5Bottom (*seed)           );
    store("gsf_sc_lazyTools_eMax"        , lazytool.eMax (*seed)                 );
    store("gsf_sc_lazyTools_e2nd"        , lazytool.e2nd (*seed)                 );
    store("gsf_sc_lazyTools_eRight"      , lazytool.eRight (*seed)               );
    store("gsf_sc_lazyTools_eLeft"       , lazytool.eLeft (*seed)                );
    store("gsf_sc_lazyTools_eTop"        , lazytool.eTop (*seed)                 );
    store("gsf_sc_lazyTools_eBottom"     , lazytool.eBottom (*seed)              );
    store("gsf_sc_lazyTools_e2x2"        , lazytool.e2x2 (*seed)                 );
    store("gsf_sc_lazyTools_e3x3"        , lazytool.e3x3 (*seed)                 );
    store("gsf_sc_lazyTools_e4x4"        , lazytool.e4x4 (*seed)                 );
    store("gsf_sc_lazyTools_e5x5"        , lazytool.e5x5 (*seed)                 );
    store("gsf_sc_lazyTools_e1x5"        , lazytool.e1x5 (*seed)                 );
    store("gsf_sc_lazyTools_e5x1"        , lazytool.e5x1 (*seed)                 );
    store("gsf_sc_lazyTools_e1x3"        , lazytool.e1x3 (*seed)                 );
    store("gsf_sc_lazyTools_e3x1"        , lazytool.e3x1 (*seed)                 );
    store("gsf_sc_lazyTools_BasicClusterSeedTime"        , lazytool.BasicClusterSeedTime (*seed)  );
    double x = gsfiter->superCluster()->x() ;
    double y = gsfiter->superCluster()->y() ;
    double z = gsfiter->superCluster()->z() ;
    store("gsf_sc_lazyTools_eshitsixix", lazytool.getESHits(x, y, z, lazytool.rechits_map_, geometry, topology_ES, 0, 1)) ;
    store("gsf_sc_lazyTools_eshitsiyiy", lazytool.getESHits(x, y, z, lazytool.rechits_map_, geometry, topology_ES, 0, 2)) ;
    store("gsf_sc_lazyTools_eseffsixix", lazytool.eseffsixix(*cl_ref)) ;
    store("gsf_sc_lazyTools_eseffsiyiy", lazytool.eseffsiyiy(*cl_ref)) ;
    store("gsf_sc_lazyTools_eseffsirir", lazytool.eseffsirir(*cl_ref)) ;

    reco::HitPattern kfHitPattern = gsfiter->gsfTrack()->hitPattern();
    int nbtrackhits = kfHitPattern.numberOfHits(reco::HitPattern::TRACK_HITS) ;
    std::vector<int> gsf_hitsinfo ;
    for(int hititer=0 ; hititer<25 ; hititer++){
      
      int myhitbin = (hititer<nbtrackhits) ? kfHitPattern.getHitPattern(reco::HitPattern::TRACK_HITS, hititer) : 0 ;
      
      gsf_hitsinfo.push_back(myhitbin) ;
    }
    store("gsf_hitsinfo", gsf_hitsinfo) ;
    
    store("gsf_pixelMatch_dPhi1"       , gsfiter->pixelMatchDPhi1()       ) ;
    store("gsf_pixelMatch_dPhi2"       , gsfiter->pixelMatchDPhi2()       ) ;
    store("gsf_pixelMatch_dRz1"        , gsfiter->pixelMatchDRz1()        ) ;
    store("gsf_pixelMatch_dRz2"        , gsfiter->pixelMatchDRz2()        ) ;
    store("gsf_pixelMatch_subDetector1", gsfiter->pixelMatchSubdetector1()) ;
    store("gsf_pixelMatch_subDetector2", gsfiter->pixelMatchSubdetector2()) ;
    
    // Now apply truth matching.
    int index = MCTruth_matchEtaPhi_getIndex(gsfiter->eta(), gsfiter->phi()) ;
    if(index>=0){
      const MCTruthObject* MCTruth = MCTruth_getRecordByIndex(index) ;
      store("gsf_mc_bestDR", deltaR(gsfiter->eta(), gsfiter->phi(), MCTruth->eta(), MCTruth->phi())) ;
      store("gsf_mc_index" , index) ;
      store("gsf_mc_ERatio", gsfiter->energy()/MCTruth->energy()) ;
    }
    else{
      store("gsf_mc_bestDR", 999.0) ;
      store("gsf_mc_index" ,    -1) ;
      store("gsf_mc_ERatio", 999.0) ;

    bool isHeep = false;
    //Barrel
    if ( ET > 35  && abs(gsfiter->superCluster()->eta()) < 1.4442  &&
      gsfiter->ecalDrivenSeed()                                            &&
      gsfiter->deltaEtaSuperClusterTrackAtVtx() < 0.004                    &&
      gsfiter->deltaPhiSuperClusterTrackAtVtx() < 0.06                     &&
      gsfiter->hadronicOverEm() < 0.05 + 1/ gsfiter->caloEnergy()          &&
      (gsfiter->full5x5_e1x5()/gsfiter->full5x5_e5x5() > 0.83 || gsfiter->full5x5_e2x5Max()/gsfiter->full5x5_e5x5() > 0.94) &&
      gsf_nLostInnerHits < 2                                               &&
      gsfiter->gsfTrack()->dxy(*firstpvertex) < 0.02                       &&
      gsfiter->dr03EcalRecHitSumEt() + gsfiter->dr03HcalDepth1TowerSumEt() < 2 + 0.03 * ET + 0.28 * rho   && 
      gsfiter->dr03TkSumPt() < 5) isHeep = true;
    //endcap
    if ( ET > 35  && (abs(gsfiter->superCluster()->eta()) > 1.4442  || (abs(gsfiter->superCluster()->eta()) < 2.5) )&&
      gsfiter->ecalDrivenSeed()                                            &&
      gsfiter->deltaEtaSuperClusterTrackAtVtx() < 0.006                    &&
      gsfiter->deltaPhiSuperClusterTrackAtVtx() < 0.06                     &&
      gsfiter->hadronicOverEm() < 0.05 + 5/ gsfiter->caloEnergy()          &&
      gsfiter->sigmaIetaIeta() <0.03                                         &&
      gsf_nLostInnerHits < 2                                               &&
      gsfiter->gsfTrack()->dxy(*firstpvertex) < 0.05                       &&
      (( ET < 50 && gsfiter->dr03EcalRecHitSumEt() + gsfiter->dr03HcalDepth1TowerSumEt() < 2.5 + 0.03 + 0.28 * rho) || 
      ( ET > 50 && gsfiter->dr03EcalRecHitSumEt() + gsfiter->dr03HcalDepth1TowerSumEt() < 2.5 + 0.03 * (ET-50) + 0.28 * rho)) &&
      gsfiter->dr03TkSumPt() < 5) isHeep = true;

    store("gsf_isHeepV60", isHeep);
    }
 }
  store("gsf_n", gsf_n) ;
}

void IIHEModuleGedGsfElectron::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleGedGsfElectron::beginEvent(){}
void IIHEModuleGedGsfElectron::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleGedGsfElectron::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleGedGsfElectron);
