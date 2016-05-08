#include "UserCode/IIHETree/interface/HEEPCut.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

//////////////////////////////////////////////////////////////////////////////////////////
//                                     Base classes                                     //
//////////////////////////////////////////////////////////////////////////////////////////
HEEPParameter::HEEPParameter(int cutflow, std::string name, std::string pset_name, float defValue){
  cutflow_   = cutflow ;
  name_      = name ;
  pset_name_ = pset_name ;
  defValue_  = defValue ;
  value_     = defValue ;
}
void HEEPParameter::setValue(const edm::ParameterSet& iConfig){
  value_ = iConfig.getUntrackedParameter<double>(pset_name_, defValue_) ;
}
bool HEEPParameter::makeBranch(IIHEAnalysis* analysis){
  return analysis->addValueToMetaTree(name_, value_) ;
}
std::string HEEPParameter::cutflowName(){
  switch(cutflow_){
    case kHC4      : return "4X"      ;
    case kHC5      : return "5X"      ;
    case kHC6      : return "6X"      ;
    case kHC41     : return "41"      ;
    case kHC50_50ns: return "50_50ns" ;
    case kHC50_25ns: return "50_25ns" ;
    case kHC50     : return "50"      ;
    case kHC51     : return "51"      ;
    case kHC60     : return "60"      ;
    default        : return "NOTSET"  ;
  }
}

HEEPCutBase::HEEPCutBase(int cutflow, std::string name, IIHEModuleHEEP* mod){
  cutflow_ = cutflow ;
  name_ = name ;
  index_ = -1 ;
  nPass_ = 0 ;
  nPassCumulative_ = 0 ;
  value_ = -999 ;
  reset() ;
  parent_mod_ = mod ;
  branchName_n_           = name_ + "_n" ;
  branchName_nCumulative_ = name_ + "_nCumulative" ;
  branchName_value_       = name_ + "_value" ;
}
HEEPCutBase::~HEEPCutBase(){}
void HEEPCutBase::setParentCollection(HEEPCutCollection* parent){ parent_collection_ = parent ; }
void HEEPCutBase::setStatus(bool value, bool cumulativeSuccess){
  status_ = value ;
  if(status_) nPass_++ ;
  if(status_ && cumulativeSuccess) nPassCumulative_++ ;
}
bool HEEPCutBase::getStatus(){ return status_ ; }
void HEEPCutBase::reset(){ status_ = true ; }
bool HEEPCutBase::addBranches(){
  bool success = true ;
  success = (success && parent_mod_->addBranch(name_                  , kVectorInt )) ;
  success = (success && parent_mod_->addBranch(branchName_n_          , kInt        )) ;
  success = (success && parent_mod_->addBranch(branchName_nCumulative_, kInt        )) ;
  success = (success && parent_mod_->addBranch(branchName_value_      , kVectorFloat)) ;
  return success ;
}
void HEEPCutBase::store(){
  parent_mod_->store(name_, status_) ;
  parent_mod_->store(branchName_value_, value_) ;
}
bool HEEPCutBase::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){ return status_ ; }
void HEEPCutBase::print(){
  std::cout << std::setw(50) ;
  std::cout << name_ ;
  std::cout << std::setw(3) ;
  std::cout << " : " ;
  std::cout << std::setw(20) ;
  std::cout << value_ ;
  std::cout << std::endl ;
}

void HEEPCutBase::beginEvent(){
  nPass_           = 0 ;
  nPassCumulative_ = 0 ;
}
void HEEPCutBase::endEvent(){
  parent_mod_->store(branchName_n_          , nPass_          ) ;
  parent_mod_->store(branchName_nCumulative_, nPassCumulative_) ;
}

bool HEEPCutBase::isBarrel(reco::GsfElectron* gsfiter){
  return (fabs(gsfiter->superCluster()->eta()) < barrelEtaUpper_ ) ;
}
bool HEEPCutBase::isEndcap(reco::GsfElectron* gsfiter){
  float eta = gsfiter->superCluster()->eta() ;
  return (fabs(eta) > endcapEtaLower_ && fabs(eta) < endcapEtaUpper_) ;
}
int  HEEPCutBase::detectorRegion(reco::GsfElectron* gsfiter){
  if(isBarrel(gsfiter)) return kBarrel ;
  if(isEndcap(gsfiter)) return kEndcap ;
  return kNone ;
}
void HEEPCutBase::config(std::vector<HEEPParameter*> parameters_){ return ; }


//////////////////////////////////////////////////////////////////////////////////////////
//                                    Base HEEP cuts                                    //
//////////////////////////////////////////////////////////////////////////////////////////
//                                          ET                                          //
HEEPCut_Et::HEEPCut_Et(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_Et::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="EtThresholdBarrel"){ thresholdBarrel_ = par->value() ; }
    if(par->name()=="EtThresholdEndcap"){ thresholdEndcap_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_Et::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->caloEnergy()*sin(gsfiter->p4().theta())) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (value() > thresholdBarrel_) ; break ;
    case kEndcap: result = (value() > thresholdEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                          eta                                         //
HEEPCut_eta::HEEPCut_eta(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
bool HEEPCut_eta::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->superCluster()->eta()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = (region==kBarrel || region==kEndcap) ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

HEEPCut_EcalDriven::HEEPCut_EcalDriven(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_EcalDriven::config(std::vector<HEEPParameter*> parameters){ return ; }
bool HEEPCut_EcalDriven::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  bool result = gsfiter->ecalDrivenSeed() ;
  setStatus(result, cumulativeSuccess) ;
  setValue((float) result) ;
  return getStatus() ;
}

//                                         dPhiIn                                       //
HEEPCut_dPhiIn::HEEPCut_dPhiIn(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_dPhiIn::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="dPhiInThresholdBarrel"){ thresholdBarrel_ = par->value() ; }
    if(par->name()=="dPhiInThresholdEndcap"){ thresholdEndcap_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_dPhiIn::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->deltaPhiSuperClusterTrackAtVtx()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(value()) < thresholdBarrel_) ; break ;
    case kEndcap: result = (fabs(value()) < thresholdEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                     SigmaIetaIeta                                    //
HEEPCut_SigmaIetaIeta::HEEPCut_SigmaIetaIeta(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){} ;
void HEEPCut_SigmaIetaIeta::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="SigmaIetaIetaThreshold"){ threshold_ = par->value() ; }
  }
  return ;
}

bool HEEPCut_SigmaIetaIeta::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  float sigmaIetaIeta = -1 ;
  switch(cutflow()){
    case kHC60:{
      sigmaIetaIeta = gsfiter->full5x5_sigmaIetaIeta() ;
      break ;
    }
    default:
      sigmaIetaIeta = gsfiter->sigmaIetaIeta() ;
      break ;
  }
  setValue(sigmaIetaIeta) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = true ; break ;
    case kEndcap: result = (value() < threshold_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                      E1x5 , E2x5                                     //
// These cuts go together
HEEPCut_E1x5OverE5x5::HEEPCut_E1x5OverE5x5(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
HEEPCut_E2x5OverE5x5::HEEPCut_E2x5OverE5x5(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_E1x5OverE5x5::config(std::vector<HEEPParameter*> parameters){ return ; }
bool HEEPCut_E1x5OverE5x5::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  float e1x5 = -1 ;
  float e5x5 =  1 ;
  switch(cutflow()){
    case kHC60:{
      reco::CaloCluster seed = *(gsfiter->superCluster()->seed()) ;
      e1x5 = gsfiter->full5x5_e1x5() ;
      e5x5 = gsfiter->full5x5_e5x5() ;
      break ;
    }
    default:
      e1x5 = gsfiter->scE1x5() ;
      e5x5 = gsfiter->scE5x5() ;
      break ;
  }
  setValue(e1x5/e5x5) ;
  
  bool result = true ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_E2x5OverE5x5::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="E1x5threshold"){ thresholdE1x5_ = par->value() ; }
    if(par->name()=="E2x5threshold"){ thresholdE2x5_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_E2x5OverE5x5::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  float e1x5 = -1 ;
  float e2x5 = -1 ;
  float e5x5 =  1 ;
  switch(cutflow()){
    case kHC60:{
      reco::CaloCluster seed = *(gsfiter->superCluster()->seed()) ;
      e1x5 = gsfiter->full5x5_e1x5()    ;
      e2x5 = gsfiter->full5x5_e2x5Max() ;
      e5x5 = gsfiter->full5x5_e5x5()    ;
      break ;
    }
    default:{
      e1x5 = gsfiter->scE1x5() ;
      e2x5 = gsfiter->scE2x5Max() ;
      e5x5 = gsfiter->scE5x5() ;
      break ;
    }
  }
  setValue(e2x5/e5x5) ;
  
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (e2x5/e5x5 > thresholdE2x5_) || (e1x5/e5x5 > thresholdE1x5_) ; break ;
    case kEndcap: result = true ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                    isolEMHadDepth1                                   //
HEEPCut_isolEMHadDepth1::HEEPCut_isolEMHadDepth1(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_isolEMHadDepth1::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="isolEMHadDepth1ConstantTermBarrel"      ){ constantTermBarrel_       = par->value() ; }
    if(par->name()=="isolEMHadDepth1ConstantTermEndcapLowEt" ){ constantTermEndcapLowEt_  = par->value() ; }
    if(par->name()=="isolEMHadDepth1ConstantTermEndcapHighEt"){ constantTermEndcapHighEt_ = par->value() ; }
    if(par->name()=="isolEMHadDepth1LinearTermBarrel"        ){ linearTermBarrel_         = par->value() ; }
    if(par->name()=="isolEMHadDepth1LinearTermEndcap"        ){ linearTermEndcap_         = par->value() ; }
    if(par->name()=="isolEMHadDepth1OffsetTermEndcap"        ){ offsetTermEndcap_         = par->value() ; }
    if(par->name()=="isolEMHadDepth1EcalHcal1EffAreaBarrel"  ){ EcalHcal1EffAreaBarrel_   = par->value() ; }
    if(par->name()=="isolEMHadDepth1EcalHcal1EffAreaEndcap"  ){ EcalHcal1EffAreaEndcap_   = par->value() ; }
  }
  return ;
}
bool HEEPCut_isolEMHadDepth1::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  float gsf_ecaliso  = gsfiter->dr03EcalRecHitSumEt() ;
  float gsf_hcaliso1 = gsfiter->dr03HcalDepth1TowerSumEt() ;
  float gsf_gsfet    = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  setValue(gsf_ecaliso+gsf_hcaliso1) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel:
      result = (value()) < (constantTermBarrel_ + linearTermBarrel_*gsf_gsfet + rho_*EcalHcal1EffAreaBarrel_) ;
      break ;
    case kEndcap:
      if(gsf_gsfet<offsetTermEndcap_){ result = value() <  constantTermEndcapLowEt_                                                    + rho_*EcalHcal1EffAreaEndcap_   ; }
      else                           { result = value() < (constantTermEndcapHighEt_ + linearTermEndcap_*(gsf_gsfet-offsetTermEndcap_) + rho_*EcalHcal1EffAreaEndcap_ ) ; }
      break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_isolEMHadDepth1::setRho(float rho){ rho_ = rho ; }

//                                      isolPtTrks                                      //
HEEPCut_IsolPtTrks::HEEPCut_IsolPtTrks(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_IsolPtTrks::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="IsolPtTrksThresholdBarrel"){ thresholdBarrel_ = par->value() ; }
    if(par->name()=="IsolPtTrksThresholdEndcap"){ thresholdEndcap_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_IsolPtTrks::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->dr03TkSumPt()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (value() < thresholdBarrel_) ; break ;
    case kEndcap: result = (value() < thresholdEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                     Missing hits                                     //
HEEPCut_missingHits::HEEPCut_missingHits(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_missingHits::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="missingHitsThreshold"){ threshold_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_missingHits::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue((float) (gsfiter->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS))) ;
  bool result = (value() <= threshold_+0.5) ;
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                      dxyFirstPV                                      //
HEEPCut_dxyFirstPV::HEEPCut_dxyFirstPV(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_dxyFirstPV::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="dPhiInThresholdBarrel"){ thresholdBarrel_ = par->value() ; }
    if(par->name()=="dPhiInThresholdEndcap"){ thresholdEndcap_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_dxyFirstPV::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->gsfTrack()->dxy(firstPV_)) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(value()) < thresholdBarrel_) ; break ;
    case kEndcap: result = (fabs(value()) < thresholdEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
void HEEPCut_dxyFirstPV::setFirstPV(math::XYZPoint* PV){ firstPV_ = math::XYZPoint(*PV) ; }


//////////////////////////////////////////////////////////////////////////////////////////
//                                     HEEP 4.1 cuts                                    //
//////////////////////////////////////////////////////////////////////////////////////////
//                                     dEtaIn (4.1)                                     //
HEEPCut_41_dEtaIn::HEEPCut_41_dEtaIn(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_41_dEtaIn::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="dEtaInThresholdBarrel"){ thresholdBarrel_ = par->value() ; }
    if(par->name()=="dEtaInThresholdEndcap"){ thresholdEndcap_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_41_dEtaIn::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->deltaEtaSuperClusterTrackAtVtx()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(value()) < thresholdBarrel_) ; break ;
    case kEndcap: result = (fabs(value()) < thresholdEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                     HOverE (4.1)                                     //
HEEPCut_41_HOverE::HEEPCut_41_HOverE(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_41_HOverE::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="HOverEThresholdBarrel"){ thresholdBarrel_ = par->value() ; }
    if(par->name()=="HOverEThresholdEndcap"){ thresholdEndcap_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_41_HOverE::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->hadronicOverEm()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (value() < thresholdBarrel_) ; break ;
    case kEndcap: result = (value() < thresholdEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                 HEEP 5.0  startup cuts                               //
//////////////////////////////////////////////////////////////////////////////////////////
//                                  dEtaIn (5.0 50ns)                                   //
HEEPCut_50_50ns_dEtaIn::HEEPCut_50_50ns_dEtaIn(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_50_50ns_dEtaIn::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="dEtaInConstantTermBarrel"){ constantTermBarrel_ = par->value() ; }
    if(par->name()=="dEtaInLinearTermBarrel"  ){ linearTermBarrel_   = par->value() ; }
    if(par->name()=="dEtaInCutoffTermBarrel"  ){ cutoffTermBarrel_   = par->value() ; }
    if(par->name()=="dEtaInThresholdEndcap"   ){ thresholdEndcap_    = par->value() ; }
  }
  return ;
}
bool HEEPCut_50_50ns_dEtaIn::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->deltaEtaSuperClusterTrackAtVtx()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel:{
      float Et  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
      float threshold = std::max(constantTermBarrel_ - linearTermBarrel_*Et, cutoffTermBarrel_) ;
      result = (fabs(value()) < threshold) ;
      break ;
    }
    case kEndcap:{
      result = (fabs(value()) < thresholdEndcap_) ;
      break ;
    }
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                  HOverE (5.0 50ns)                                   //
HEEPCut_50_50ns_HOverE::HEEPCut_50_50ns_HOverE(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_50_50ns_HOverE::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="HOverEReciprocalTermBarrel"){ reciprocalTermBarrel_ = par->value() ; }
    if(par->name()=="HOverEReciprocalTermEndcap"){ reciprocalTermEndcap_ = par->value() ; }
    if(par->name()=="HOverEConstantTermBarrel"  ){ constantTermBarrel_   = par->value() ; }
    if(par->name()=="HOverEConstantTermEndcap"  ){ constantTermEndcap_   = par->value() ; }
  }
  return ;
}
bool HEEPCut_50_50ns_HOverE::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->hadronicOverEm()) ;
  float E = gsfiter->caloEnergy() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (value() < reciprocalTermBarrel_/E + constantTermBarrel_) ; break ;
    case kEndcap: result = (value() < reciprocalTermEndcap_/E + constantTermEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                   HEEP 5.0 25ns cuts                                 //
//////////////////////////////////////////////////////////////////////////////////////////
//                                  dEtaIn (5.0 50ns)                                   //
HEEPCut_50_25ns_dEtaIn::HEEPCut_50_25ns_dEtaIn(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_50_25ns_dEtaIn::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="dEtaInConstantTermBarrel"){ constantTermBarrel_ = par->value() ; }
    if(par->name()=="dEtaInLinearTermBarrel"  ){ linearTermBarrel_   = par->value() ; }
    if(par->name()=="dEtaInCutoffTermBarrel"  ){ cutoffTermBarrel_   = par->value() ; }
    if(par->name()=="dEtaInConstantTermEndcap"){ constantTermEndcap_ = par->value() ; }
    if(par->name()=="dEtaInLinearTermEndcap"  ){ linearTermEndcap_   = par->value() ; }
    if(par->name()=="dEtaInCutoffTermEndcap"  ){ cutoffTermEndcap_   = par->value() ; }
  }
  return ;
}
bool HEEPCut_50_25ns_dEtaIn::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->deltaEtaSuperClusterTrackAtVtx()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  float Et = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  switch(region){
    case kBarrel:{
      float threshold = std::max(constantTermBarrel_ - constantTermBarrel_*Et, cutoffTermBarrel_) ;
      result = (fabs(value()) < threshold) ;
      break ;
    }
    case kEndcap:{
      float threshold = std::max(constantTermEndcap_ - constantTermEndcap_*Et, cutoffTermEndcap_) ;
      result = (fabs(value()) < threshold) ;
      break ;
    }
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                  HOverE (5.0 25ns)                                   //
HEEPCut_50_25ns_HOverE::HEEPCut_50_25ns_HOverE(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_50_25ns_HOverE::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="HOverEReciprocalTermBarrel"){ reciprocalTermBarrel_ = par->value() ; }
    if(par->name()=="HOverEReciprocalTermEndcap"){ reciprocalTermEndcap_ = par->value() ; }
    if(par->name()=="HOverEConstantTermBarrel"  ){ constantTermBarrel_   = par->value() ; }
    if(par->name()=="HOverEConstantTermEndcap"  ){ constantTermEndcap_   = par->value() ; }
  }
  return ;
}
bool HEEPCut_50_25ns_HOverE::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->hadronicOverEm()) ;
  float E = gsfiter->caloEnergy() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (value() < reciprocalTermBarrel_/E + constantTermBarrel_) ; break ;
    case kEndcap: result = (value() < reciprocalTermEndcap_/E + constantTermEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                      HEEP 5.0 cuts                                   //
//////////////////////////////////////////////////////////////////////////////////////////
//                                     dEtaIn (5.0)                                     //
HEEPCut_50_dEtaIn::HEEPCut_50_dEtaIn(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_50_dEtaIn::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="dEtaInConstantTermBarrel"){ constantTermBarrel_ = par->value() ; }
    if(par->name()=="dEtaInLinearTermBarrel"  ){ linearTermBarrel_   = par->value() ; }
    if(par->name()=="dEtaInCutoffTermBarrel"  ){ cutoffTermBarrel_   = par->value() ; }
    if(par->name()=="dEtaInConstantTermEndcap"){ constantTermEndcap_ = par->value() ; }
    if(par->name()=="dEtaInLinearTermEndcap"  ){ linearTermEndcap_   = par->value() ; }
    if(par->name()=="dEtaInCutoffTermEndcap"  ){ cutoffTermEndcap_   = par->value() ; }
  }
  return ;
}
bool HEEPCut_50_dEtaIn::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->deltaEtaSuperClusterTrackAtVtx()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  float Et = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
  switch(region){
    case kBarrel:{
      float threshold = std::max(constantTermBarrel_ - constantTermBarrel_*Et, cutoffTermBarrel_) ;
      result = (fabs(value()) < threshold) ;
      break ;
    }
    case kEndcap:{
      float threshold = std::max(constantTermBarrel_ - constantTermBarrel_*Et, cutoffTermBarrel_) ;
      result = (fabs(value()) < threshold) ;
      break ;
    }
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                     HOverE (5.0)                                     //
HEEPCut_50_HOverE::HEEPCut_50_HOverE(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_50_HOverE::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="HOverEReciprocalTermBarrel"){ reciprocalTermBarrel_ = par->value() ; }
    if(par->name()=="HOverEReciprocalTermEndcap"){ reciprocalTermEndcap_ = par->value() ; }
    if(par->name()=="HOverEConstantTermBarrel"  ){ constantTermBarrel_   = par->value() ; }
    if(par->name()=="HOverEConstantTermEndcap"  ){ constantTermEndcap_   = par->value() ; }
  }
  return ;
}
bool HEEPCut_50_HOverE::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->hadronicOverEm()) ;
  float E = gsfiter->caloEnergy() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (value() < reciprocalTermBarrel_/E + constantTermBarrel_) ; break ;
    case kEndcap: result = (value() < reciprocalTermEndcap_/E + constantTermEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                      HEEP 5.1 cuts                                   //
//////////////////////////////////////////////////////////////////////////////////////////
//                                     dEtaIn (5.1)                                     //
HEEPCut_51_dEtaIn::HEEPCut_51_dEtaIn(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_51_dEtaIn::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="dEtaInThresholdBarrel"){ thresholdBarrel_ = par->value() ; }
    if(par->name()=="dEtaInThresholdEndcap"){ thresholdEndcap_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_51_dEtaIn::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->deltaEtaSuperClusterTrackAtVtx()) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(value()) < thresholdBarrel_) ; break ;
    case kEndcap: result = (fabs(value()) < thresholdEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}
//                                     HOverE (5.1)                                     //
HEEPCut_51_HOverE::HEEPCut_51_HOverE(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_51_HOverE::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="HOverEReciprocalTermBarrel"){ reciprocalTermBarrel_ = par->value() ; }
    if(par->name()=="HOverEReciprocalTermEndcap"){ reciprocalTermEndcap_ = par->value() ; }
    if(par->name()=="HOverEConstantTermBarrel"  ){ constantTermBarrel_   = par->value() ; }
    if(par->name()=="HOverEConstantTermEndcap"  ){ constantTermEndcap_   = par->value() ; }
  }
  return ;
}
bool HEEPCut_51_HOverE::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->hadronicOverEm()) ;
  float E = gsfiter->caloEnergy() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (value() < reciprocalTermBarrel_/E + constantTermBarrel_) ; break ;
    case kEndcap: result = (value() < reciprocalTermEndcap_/E + constantTermEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                      HEEP 6.0 cuts                                   //
//////////////////////////////////////////////////////////////////////////////////////////
//                                     dEtaIn (6.0)                                     //
HEEPCut_60_dEtaIn::HEEPCut_60_dEtaIn(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_60_dEtaIn::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="dEtaInThresholdBarrel"){ thresholdBarrel_ = par->value() ; }
    if(par->name()=="dEtaInThresholdEndcap"){ thresholdEndcap_ = par->value() ; }
  }
  return ;
}
bool HEEPCut_60_dEtaIn::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  float deltaEtaInSeed = std::numeric_limits<float>::max() ;
  if(gsfiter->superCluster().isNonnull() && gsfiter->superCluster()->seed().isNonnull()){
    deltaEtaInSeed = gsfiter->deltaEtaSuperClusterTrackAtVtx() - gsfiter->superCluster()->eta() + gsfiter->superCluster()->seed()->eta() ;
  }
  setValue(deltaEtaInSeed) ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (fabs(value()) < thresholdBarrel_) ; break ;
    case kEndcap: result = (fabs(value()) < thresholdEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//                                     HOverE (6.0)                                     //
HEEPCut_60_HOverE::HEEPCut_60_HOverE(int cutflow, std::string name, IIHEModuleHEEP* mod): HEEPCutBase(cutflow, name, mod){}
void HEEPCut_60_HOverE::config(std::vector<HEEPParameter*> parameters){
  for(unsigned int i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="HOverEReciprocalTermBarrel"){ reciprocalTermBarrel_ = par->value() ; }
    if(par->name()=="HOverEReciprocalTermEndcap"){ reciprocalTermEndcap_ = par->value() ; }
    if(par->name()=="HOverEConstantTermBarrel"  ){ constantTermBarrel_   = par->value() ; }
    if(par->name()=="HOverEConstantTermEndcap"  ){ constantTermEndcap_   = par->value() ; }
  }
  return ;
}
bool HEEPCut_60_HOverE::applyCut(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool cumulativeSuccess){
  setValue(gsfiter->hadronicOverEm()) ;
  float E = gsfiter->caloEnergy() ;
  int region = detectorRegion(gsfiter) ;
  bool result = true ;
  switch(region){
    case kBarrel: result = (value() < reciprocalTermBarrel_/E + constantTermBarrel_) ; break ;
    case kEndcap: result = (value() < reciprocalTermEndcap_/E + constantTermEndcap_) ; break ;
    default : break ;
  }
  setStatus(result, cumulativeSuccess) ;
  return getStatus() ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                   Cut collections                                    //
//////////////////////////////////////////////////////////////////////////////////////////
HEEPCutCollection::HEEPCutCollection(int cutflow, std::string name, IIHEModuleHEEP* mod, bool isActive){
  cutflow_      = cutflow ;
  name_         = name ;
  branchName_n_ = name + "_n" ;
  parent_mod_   = mod ;
  nPass_        = -1 ;
  isActive_     = isActive ;
  lazyTool_     = 0 ;
}
HEEPCutCollection::~HEEPCutCollection(){}
void HEEPCutCollection::config(std::vector<HEEPParameter*> parameters){
  cutIndex_        = 0 ;
  collectionIndex_ = 0 ;
  float barrelEtaUpper = 0 ;
  float endcapEtaLower = 0 ;
  float endcapEtaUpper = 0 ;
  for(unsigned i=0 ; i<parameters.size() ; ++i){
    HEEPParameter* par = parameters.at(i) ;
    if(par->cutflow()!=cutflow()) continue ;
    if(par->name()=="barrelEtaUpper"){ barrelEtaUpper = par->value() ; }
    if(par->name()=="endcapEtaLower"){ endcapEtaLower = par->value() ; }
    if(par->name()=="endcapEtaUpper"){ endcapEtaUpper = par->value() ; }
  }
  for(unsigned int i=0 ; i<cutTypes_.size() ; ++i){
    switch(cutTypes_.at(i)){
      case kCut:{
        HEEPCutBase* cut = listOfCuts_.at(cutIndex_) ;
        cut->addBranches() ;
        cut->setBarrelLimits(barrelEtaUpper) ;
        cut->setEndcapLimits(endcapEtaLower, endcapEtaUpper) ;
        cut->config(parameters) ;
        ++cutIndex_ ;
        break ;
      }
      case kCollection:{
        listOfCutCollections_.at(collectionIndex_)->config(parameters) ;
        ++collectionIndex_ ;
        break ;
      }
    }
  }
  parent_mod_->addBranch(name_, kVectorInt) ;
  parent_mod_->addBranch(branchName_n_, kInt) ;
}

void HEEPCutCollection::beginEvent(){
  nPass_           = 0 ;
  cutIndex_        = 0 ;
  collectionIndex_ = 0 ;
  for(unsigned int i=0 ; i<cutTypes_.size() ; ++i){
    switch(cutTypes_.at(i)){
      case kCut:{
        listOfCuts_.at(cutIndex_)->beginEvent() ;
        ++cutIndex_ ;
        break ;
      }
      case kCollection:{
        listOfCutCollections_.at(collectionIndex_)->beginEvent() ;
        ++collectionIndex_ ;
        break ;
      }
    }
  }
}
void HEEPCutCollection::endEvent()  {
  parent_mod_->store(branchName_n_, nPass_) ;
  cutIndex_        = 0 ;
  collectionIndex_ = 0 ;
  for(unsigned int i=0 ; i<cutTypes_.size() ; ++i){
    switch(cutTypes_.at(i)){
      case kCut:{
        listOfCuts_.at(cutIndex_)->endEvent() ;
        ++cutIndex_ ;
        break ;
      }
      case kCollection:{
        listOfCutCollections_.at(collectionIndex_)->endEvent() ;
        ++collectionIndex_ ;
        break ;
      }
    }
  }
}

void HEEPCutCollection::addCut(HEEPCutBase* cut){
  listOfCuts_.push_back(cut) ;
  cutTypes_.push_back(kCut) ;
  cut->setParentCollection(this) ;
}
void HEEPCutCollection::addCutCollection(HEEPCutCollection* collection){
  listOfCutCollections_.push_back(collection) ;
  cutTypes_.push_back(kCollection) ;
}
bool HEEPCutCollection::applyCuts(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool){
  return applyCuts(gsfiter, lazytool, true) ;
}
bool HEEPCutCollection::applyCuts(reco::GsfElectron* gsfiter, EcalClusterLazyTools lazytool, bool status_in){
  cutIndex_        = 0 ;
  collectionIndex_ = 0 ;
  status_ = status_in ;
  for(unsigned int i=0 ; i<cutTypes_.size() ; ++i){
    switch(cutTypes_.at(i)){
      case kCut:{
        HEEPCutBase* cut = (HEEPCutBase*) listOfCuts_.at(cutIndex_) ;
        cut->applyCut(gsfiter, lazytool, status_) ;
        cut->store() ;
        ++cutIndex_ ;
        status_ = (status_ && cut->getStatus()) ;
        break ;
      }
      case kCollection:{
        HEEPCutCollection* collection = listOfCutCollections_.at(collectionIndex_) ;
        collection->applyCuts(gsfiter, lazytool, status_in) ;
        ++collectionIndex_ ;
        status_ = (status_ && collection->getStatus()) ;
        break ;
      }
    }
  }
  parent_mod_->store(name_, status_) ;
  if(status_) nPass_++ ;
  return status_ ;
}

