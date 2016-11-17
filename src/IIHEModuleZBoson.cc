#include "UserCode/IIHETree/interface/IIHEModuleZBoson.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleZBoson::IIHEModuleZBoson(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  DeltaRCut_        = iConfig.getUntrackedParameter<double>("ZBosonDeltaRCut"          ,  0.3) ;
  ETThreshold_      = iConfig.getUntrackedParameter<double>("ZBosonEtThreshold"        , 10.0) ;
  mZAccept_         = iConfig.getUntrackedParameter<double>("ZBosonZMassAcceptLower"   , 60.0) ;
  mJpsiAcceptLower_ = iConfig.getUntrackedParameter<double>("ZBosonJPsiAcceptMassLower",  2.5) ;
  mJpsiAcceptUpper_ = iConfig.getUntrackedParameter<double>("ZBosonJPsiAcceptMassUpper",  3.5) ;
  mUpsAcceptLower_  = iConfig.getUntrackedParameter<double>("ZBosonUpsAcceptMassLower" ,  8.0) ;
  mUpsAcceptUpper_  = iConfig.getUntrackedParameter<double>("ZBosonUpsAcceptMassUpper" , 12.0) ;
  
  mZLowerCutoff_    = iConfig.getUntrackedParameter<double>("ZBosonZMassLowerCuttoff", 0.0) ;
  mZUpperCutoff_    = iConfig.getUntrackedParameter<double>("ZBosonZMassUpperCuttoff", 1e6) ;
  
  saveZee_  = iConfig.getUntrackedParameter<bool>("ZBosonSaveZee" , true) ;
  saveZmm_  = iConfig.getUntrackedParameter<bool>("ZBosonSaveZmm" , true) ;
  saveZem_  = iConfig.getUntrackedParameter<bool>("ZBosonSaveZem" , true) ;
  saveZeeg_ = iConfig.getUntrackedParameter<bool>("ZBosonSaveZeeg", true) ;
  saveZmmg_ = iConfig.getUntrackedParameter<bool>("ZBosonSaveZmmg", true) ;
}
IIHEModuleZBoson::~IIHEModuleZBoson(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleZBoson::beginJob(){
  nAcceptZee_  = 0 ;
  nAcceptZmm_  = 0 ;
  nAcceptZem_  = 0 ;
  nAcceptJmm_  = 0 ;
  nAcceptYmm_  = 0 ;
  nAcceptZeeg_ = 0 ;
  nAcceptZmmg_ = 0 ;
  nAcceptAll_  = 0 ;
  
  if(saveZee_){
    addBranch("Zee_n"          , kInt        ) ;
    addBranch("Zee_mass"       , kVectorFloat) ;
    addBranch("Zee_mass_HEEP"  , kVectorFloat) ;
    addBranch("Zee_i1"         , kVectorInt  ) ;
    addBranch("Zee_i2"         , kVectorInt  ) ;
    addBranch("Zee_highestMass", kInt  ) ;
  }
  
  if(saveZmm_){
    addBranch("Zmm_n"    , kInt        ) ;
    addBranch("Zmm_mass" , kVectorFloat) ;
    addBranch("Zmm_i1"   , kVectorInt  ) ;
    addBranch("Zmm_i2"   , kVectorInt  ) ;
    addBranch("Zmm_highestMass", kInt  ) ;
  }
  
  if(saveZem_){
    addBranch("Zem_n"          , kInt        ) ;
    addBranch("Zem_mass"       , kVectorFloat) ;
    addBranch("Zem_mass_HEEP"  , kVectorFloat) ;
    addBranch("Zem_i1"         , kVectorInt  ) ;
    addBranch("Zem_i2"         , kVectorInt  ) ;
    addBranch("Zem_highestMass", kInt  ) ;
  }
  
  if(saveZeeg_){
    addBranch("Zeeg_n"          , kInt        ) ;
    addBranch("Zeeg_mass"       , kVectorFloat) ;
    addBranch("Zeeg_i1"         , kVectorInt  ) ;
    addBranch("Zeeg_i2"         , kVectorInt  ) ;
    addBranch("Zeeg_iph"        , kVectorInt  ) ;
    addBranch("Zeeg_highestMass", kInt ) ;
  }
  
  if(saveZmmg_){
    addBranch("Zmmg_n"   , kInt        ) ;
    addBranch("Zmmg_mass", kVectorFloat) ;
    addBranch("Zmmg_i1"  , kVectorInt  ) ;
    addBranch("Zmmg_i2"  , kVectorInt  ) ;
    addBranch("Zmmg_iph" , kVectorInt  ) ;
    addBranch("Zmmg_highestMass", kInt ) ;
  }
}

// ------------ method called to for each event  ------------
void IIHEModuleZBoson::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  float mEl = 0.000511 ;
  float mMu = 0.105    ;
  
  // Get leptons and photons
  reco::GsfElectronCollection electrons = parent_->getElectronCollection() ;
  reco::PhotonCollection photons        = parent_->getPhotonCollection() ;
  reco::MuonCollection muons            = parent_->getMuonCollection() ;
  
  // Declare and fill four vectors
  std::vector<TLorentzVector> php4s ;
  std::vector<TLorentzVector> elp4s ;
  std::vector<TLorentzVector> mup4s ;
  std::vector<TLorentzVector> HEEPp4s ;
  
//  for(reco::PhotonCollection::const_iterator phiter = photons.begin() ; phiter!=photons.end() ; ++phiter){
  for(vector<reco::Photon>::const_iterator phiter = photons.begin() ; phiter!=photons.end() ; ++phiter){
    float px = phiter->px() ;
    float py = phiter->py() ;
    float pz = phiter->pz() ;
    float E = sqrt(px*px+py*py+pz*pz) ;
    float ET =  sqrt(px*px+py*py) ;
    if(ET<ETThreshold_) continue ;
    php4s.push_back(TLorentzVector(px, py, pz, E)) ;
  }
  
//  for(reco::GsfElectronCollection::const_iterator gsfiter=electrons.begin() ; gsfiter!=electrons.end() ; ++gsfiter){
  for(vector<reco::GsfElectron>::const_iterator gsfiter=electrons.begin() ; gsfiter!=electrons.end() ; ++gsfiter){
    float px = gsfiter->px() ;
    float py = gsfiter->py() ;
    float pz = gsfiter->pz() ;
    float E = sqrt(mEl*mEl+px*px+py*py+pz*pz) ;
    
    float HEEP_ET  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
    // float HEEP_E   = gsfiter->caloEnergy() ;
    TLorentzVector HEEPp4 ;
    HEEPp4.SetPtEtaPhiM(HEEP_ET, gsfiter->eta(), gsfiter->phi(), mEl) ;
    
    float ET =  sqrt(px*px+py*py) ;
    if(ET<ETThreshold_ && HEEP_ET<ETThreshold_) continue ;
    
    elp4s.push_back(TLorentzVector(px, py, pz, E)) ;
    HEEPp4s.push_back(HEEPp4) ;
  }
  

//  for(reco::MuonCollection::const_iterator muiter = muons.begin(); muiter!=muons.end() ; ++muiter){
  for(vector<reco::Muon>::const_iterator muiter = muons.begin() ; muiter != muons.end() ; ++muiter){
    float px = muiter->px() ;
    float py = muiter->py() ;
    float pz = muiter->pz() ;
    float E = sqrt(mMu*mMu+px*px+py*py+pz*pz) ;
    float ET =  sqrt(px*px+py*py) ;
    if(ET<ETThreshold_) continue ;
    mup4s.push_back(TLorentzVector(px, py, pz, E)) ;
  }
  
  // Decide if we keep the event based on mass ranges
  // Any mZ > 60 GeV
  // And m(mu mu) in range 8-12 GeV
  int acceptThisEvent = 0 ;
  bool acceptZee  = false ;
  bool acceptZmm  = false ;
  bool acceptZem  = false ;
  bool acceptJmm  = false ;
  bool acceptYmm  = false ;
  bool acceptZeeg = false ;
  bool acceptZmmg = false ;
  
  int Zee_n  = 0 ;
  int Zmm_n  = 0 ;
  int Zem_n  = 0 ;
  int Zeeg_n = 0 ;
  int Zmmg_n = 0 ;
  
  float Zee_highestMass  = 0 ;
  float Zmm_highestMass  = 0 ;
  float Zem_highestMass  = 0 ;
  float Zeeg_highestMass = 0 ;
  float Zmmg_highestMass = 0 ;
  
  int Zee_highestMassIndex  = -1 ;
  int Zmm_highestMassIndex  = -1 ;
  int Zem_highestMassIndex  = -1 ;
  int Zeeg_highestMassIndex = -1 ;
  int Zmmg_highestMassIndex = -1 ;
  
  // Now make Z bosons candidates
  for(unsigned int i1=0 ; i1<elp4s.size() ; ++i1){
    for(unsigned int i2=i1+1 ; i2<elp4s.size() ; ++i2){
      if(elp4s.at(i1).DeltaR(elp4s.at(i2)) < DeltaRCut_) continue ;
      TLorentzVector Zeep4     = elp4s  .at(i1) + elp4s  .at(i2) ;
      TLorentzVector ZeeHEEPp4 = HEEPp4s.at(i1) + HEEPp4s.at(i2) ;
      float mZee = Zeep4.M() ;
      float mZee_HEEP = ZeeHEEPp4.M() ;
      
      // Look for Z->eeg candidates
      for(unsigned iph=0 ; iph<php4s.size() ; ++iph){
        if(false==saveZeeg_) break ;
        if(php4s.at(iph).DeltaR(elp4s.at(i1)) < DeltaRCut_) continue ;
        if(php4s.at(iph).DeltaR(elp4s.at(i2)) < DeltaRCut_) continue ;
        TLorentzVector Zeegp4 = Zeep4 + php4s.at(iph) ;
        float mZeeg = Zeegp4.M() ;
        
        // Check to see if we're in the range we want
        if(mZeeg<mZLowerCutoff_) continue ;
        if(mZeeg>mZUpperCutoff_) continue ;
        
        store("Zeeg_mass", mZeeg) ;
        store("Zeeg_i1"  , i1) ;
        store("Zeeg_i2"  , i2) ;
        store("Zeeg_iph" , iph) ;
        if(mZeeg>mZAccept_){
          acceptThisEvent += pow(10, (int)kZeeg) ;
          acceptZeeg = true ;
        }
        if(mZeeg>Zeeg_highestMass){
          Zeeg_highestMass = mZeeg ;
          Zeeg_highestMassIndex = Zeeg_n ;
        }
        Zeeg_n++ ;
      }
      if(false==saveZee_) continue ;
      
      // Check to see if we're in the range we want
      if(mZee<mZLowerCutoff_ && mZee_HEEP<mZLowerCutoff_) continue ;
      if(mZee>mZUpperCutoff_ && mZee_HEEP>mZUpperCutoff_) continue ;
      
      store("Zee_mass", mZee) ;
      store("Zee_mass_HEEP", mZee_HEEP) ;
      store("Zee_i1"  , i1) ;
      store("Zee_i2"  , i2) ;
      if(mZee>mZAccept_){
        acceptThisEvent += pow(10, (int)kZee) ;
        acceptZee = true ;
      }
      if(mZee>Zee_highestMass){
        Zee_highestMass = mZee ;
        Zee_highestMassIndex = Zee_n ;
      }
      Zee_n++ ;
    }
  }
  for(unsigned int i1=0 ; i1<mup4s.size() ; ++i1){
    for(unsigned int i2=i1+1 ; i2<mup4s.size() ; ++i2){
      if(mup4s.at(i1).DeltaR(mup4s.at(i2)) < DeltaRCut_) continue ;
      TLorentzVector Zmmp4 = mup4s.at(i1) + mup4s.at(i2) ;
      float mZmm = Zmmp4.M() ;
      
      for(unsigned iph=0 ; iph<php4s.size() ; ++iph){
        if(false==saveZmmg_) break ;
        if(php4s.at(iph).DeltaR(mup4s.at(i1)) < DeltaRCut_) continue ;
        if(php4s.at(iph).DeltaR(mup4s.at(i2)) < DeltaRCut_) continue ;
        TLorentzVector Zmmgp4 = Zmmp4 + php4s.at(iph) ;
        float mZmmg = Zmmgp4.M() ;
        
        // Check to see if we're in the range we want
        if(mZmmg<mZLowerCutoff_) continue ;
        if(mZmmg>mZUpperCutoff_) continue ;
        
        store("Zmmg_mass", mZmmg) ;
        store("Zmmg_i1"  , i1) ;
        store("Zmmg_i2"  , i2) ;
        store("Zmmg_iph" , iph) ;
        if(mZmmg>mZAccept_){
          acceptThisEvent += pow(10, (int)kZmmg) ;
          acceptZmmg = true ;
        }
        if(mZmmg>Zmmg_highestMass){
          Zmmg_highestMass = mZmmg ;
          Zmmg_highestMassIndex = Zmmg_n ;
        }
        Zmmg_n++ ;
      }
      if(false==saveZmm_) continue ;
      
      // Check to see if we're in the range we want
      if(mZmm<mZLowerCutoff_) continue ;
      if(mZmm>mZUpperCutoff_) continue ;
      
      store("Zmm_mass", mZmm) ;
      store("Zmm_i1"  , i1) ;
      store("Zmm_i2"  , i2) ;
      if(mZmm>mZAccept_){
        acceptThisEvent += pow(10, (int)kZmm) ;
        acceptZmm = true ;
      }
      if(mZmm>mJpsiAcceptLower_ && mZmm<mJpsiAcceptUpper_){
        acceptThisEvent += pow(10, (int)kJmm) ;
        acceptJmm = true ;
      }
      if(mZmm>mUpsAcceptLower_ && mZmm<mUpsAcceptUpper_){
        acceptThisEvent += pow(10, (int)kYmm) ;
        acceptYmm = true ;
      }
      if (mZmm>Zmm_highestMass){
        Zmm_highestMass = mZmm ;
        Zmm_highestMassIndex = Zmm_n ;
      }
      Zmm_n++ ;
    }
  }
  for(unsigned int i1=0 ; i1<elp4s.size() ; ++i1){
    if(false==saveZem_) break ;
    for(unsigned int i2=i1+1 ; i2<mup4s.size() ; ++i2){
      if(elp4s.at(i1).DeltaR(mup4s.at(i2)) < DeltaRCut_) continue ;
      TLorentzVector Zemp4     = elp4s  .at(i1) + mup4s.at(i2) ;
      TLorentzVector ZemHEEPp4 = HEEPp4s.at(i1) + mup4s.at(i2) ;
      float mZem = Zemp4.M() ;
      float mZem_HEEP = ZemHEEPp4.M() ;
      
      // Check to see if we're in the range we want
      if(mZem<mZLowerCutoff_ && mZem_HEEP<mZLowerCutoff_) continue ;
      if(mZem>mZUpperCutoff_ && mZem_HEEP>mZUpperCutoff_) continue ;
      
      store("Zem_mass", mZem) ;
      store("Zem_mass_HEEP", mZem_HEEP) ;
      store("Zem_i1"  , i1) ;
      store("Zem_i2"  , i2) ;
      if(mZem>mZAccept_){
        acceptThisEvent += pow(10, (int)kZem) ;
        acceptZem = true ;
      }
      if(mZem>Zem_highestMass){
        Zem_highestMass = mZem ;
        Zem_highestMassIndex = Zem_n ;
      }
      Zem_n++ ;
    }
  }
  
  // Save the event if we see something we like
  if(acceptThisEvent>0){
    acceptEvent() ;
    nAcceptAll_++ ;
  }
  if(acceptZee ) nAcceptZee_ ++ ;
  if(acceptZmm ) nAcceptZmm_ ++ ;
  if(acceptZem ) nAcceptZem_ ++ ;
  if(acceptJmm ) nAcceptJmm_ ++ ;
  if(acceptYmm ) nAcceptYmm_ ++ ;
  if(acceptZeeg) nAcceptZeeg_++ ;
  if(acceptZmmg) nAcceptZmmg_++ ;
  
  if(saveZee_ ){
    store("Zee_n" , Zee_n ) ;
    store("Zee_highestMass" , Zee_highestMassIndex ) ;
  }
  if(saveZmm_ ){
    store("Zmm_n" , Zmm_n ) ;
    store("Zmm_highestMass" , Zmm_highestMassIndex ) ;
  }
  if(saveZem_ ){
    store("Zem_n" , Zem_n ) ;
    store("Zem_highestMass" , Zem_highestMassIndex ) ;
  }
  if(saveZeeg_){
    store("Zeeg_n", Zeeg_n) ;
    store("Zeeg_highestMass", Zeeg_highestMassIndex) ;
  }
  if(saveZmmg_){
    store("Zmmg_n", Zmmg_n) ;
    store("Zmmg_highestMass", Zmmg_highestMassIndex) ;
  }
}

void IIHEModuleZBoson::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleZBoson::beginEvent(){}
void IIHEModuleZBoson::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleZBoson::endJob(){
  std::cout << std::endl << "IIHEModuleZBoson report:" << std::endl ;
  std::cout << "  nAcceptZee  = " << nAcceptZee_  << std::endl ;
  std::cout << "  nAcceptZmm  = " << nAcceptZmm_  << std::endl ;
  std::cout << "  nAcceptZem  = " << nAcceptZem_  << std::endl ;
  std::cout << "  nAcceptJmm  = " << nAcceptJmm_  << std::endl ;
  std::cout << "  nAcceptYmm  = " << nAcceptYmm_  << std::endl ;
  std::cout << "  nAcceptZeeg = " << nAcceptZeeg_ << std::endl ;
  std::cout << "  nAcceptZmmg = " << nAcceptZmmg_ << std::endl ;
  std::cout << "  nAcceptAll  = " << nAcceptAll_  << std::endl ;
}

DEFINE_FWK_MODULE(IIHEModuleZBoson);

