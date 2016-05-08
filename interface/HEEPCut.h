#ifndef UserCode_IIHETree_HEEPCut_h
#define UserCode_IIHETree_HEEPCut_h

#include <algorithm>

#include "UserCode/IIHETree/interface/IIHEModuleHEEP.h"
// These classes are used to make it easier to keep track of HEEP cuts
// Each class corresponds to a different cut
// The cuts can be arranged into cut collections (eg HEEP41, HEEP50) to process a whole list of cuts at once

class IIHEModuleHEEP ;

class HEEPParameter{
private:
  int cutflow_ ;
  std::string name_ ;
  std::string pset_name_ ;
  float defValue_ ;  
  float value_ ;
public:
  HEEPParameter(int, std::string name, std::string, float) ;
  void setValue(const edm::ParameterSet&) ;
  bool makeBranch(IIHEAnalysis*) ;
  int cutflow(){ return cutflow_ ; }
  std::string cutflowName() ;
  std::string name(){ return name_ ; }
  float value(){ return value_ ; }
  void print(){ std::cout << name_ << ": " << value_ << std::endl ; }
};


class HEEPCutBase{
private:
  int cutflow_ ;
  std::string name_ ;
  std::string branchName_n_ ;
  std::string branchName_nCumulative_ ;
  std::string branchName_value_ ;
  float value_ ; // Value of the variable
  bool status_ ;
  int index_ ;
  int nPass_ ;
  int nPassCumulative_ ;
  IIHEModuleHEEP* parent_mod_ ;
  HEEPCutCollection* parent_collection_ ;
  
  float barrelEtaUpper_ ;
  float endcapEtaLower_ ;
  float endcapEtaUpper_ ;
  
public:
  HEEPCutBase(int, std::string, IIHEModuleHEEP*) ;
  ~HEEPCutBase() ;
  virtual bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
  bool addBranches() ;
  void store() ;
  void reset() ;
  void setStatus(bool, bool) ;
  bool getStatus() ;
  void setValue(float value){ value_ = value ; }
  float value(){ return value_ ; }
  std::string name(){ return name_ ; }
  int cutflow(){ return cutflow_ ; }
  void setParentCollection(HEEPCutCollection*) ;
  void print() ;
  
  void beginEvent() ;
  void endEvent() ;
  
  bool isBarrel(reco::GsfElectron*) ;
  bool isEndcap(reco::GsfElectron*) ;
  int detectorRegion(reco::GsfElectron*) ;
  
  void setBarrelLimits(float barrelEtaUpper){ barrelEtaUpper_ = barrelEtaUpper ; }
  void setEndcapLimits(float endcapEtaLower, float endcapEtaUpper){
    endcapEtaLower_ = endcapEtaLower ;
    endcapEtaUpper_ = endcapEtaUpper ;
  }
  virtual void config(std::vector<HEEPParameter*> parameters_) ;
  HEEPCutCollection* parentCollection(){ return parent_collection_ ; }
};

class HEEPCut_Et: HEEPCutBase{
public:
  HEEPCut_Et(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float thresholdBarrel_ ;
  float thresholdEndcap_ ;
} ;
class HEEPCut_eta: HEEPCutBase{
public:
  HEEPCut_eta(int, std::string, IIHEModuleHEEP*) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
} ;
class HEEPCut_EcalDriven: HEEPCutBase{
public:
  HEEPCut_EcalDriven(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
} ;
class HEEPCut_dPhiIn: HEEPCutBase{
public:
  HEEPCut_dPhiIn(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float thresholdBarrel_ ;
  float thresholdEndcap_ ;
} ;
class HEEPCut_SigmaIetaIeta: HEEPCutBase{
public:
  HEEPCut_SigmaIetaIeta(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float threshold_ ;
} ;
class HEEPCut_E1x5OverE5x5: HEEPCutBase{
public:
  HEEPCut_E1x5OverE5x5(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
} ;
class HEEPCut_E2x5OverE5x5: HEEPCutBase{
public:
  HEEPCut_E2x5OverE5x5(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float thresholdE1x5_ ;
  float thresholdE2x5_ ;
} ;
class HEEPCut_isolEMHadDepth1: HEEPCutBase{
private:
  float rho_ ;
  float EcalHcal1EffAreaBarrel_ ;
  float EcalHcal1EffAreaEndcap_ ;
  float constantTermBarrel_ ;
  float constantTermEndcapLowEt_ ;
  float constantTermEndcapHighEt_ ;
  float linearTermBarrel_ ;
  float linearTermEndcap_ ;
  float offsetTermEndcap_ ;
public:
  HEEPCut_isolEMHadDepth1(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
  void setRho(float) ;
} ;
class HEEPCut_IsolPtTrks: HEEPCutBase{
public:
  HEEPCut_IsolPtTrks(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float thresholdBarrel_ ;
  float thresholdEndcap_ ;
} ;
class HEEPCut_missingHits: HEEPCutBase{
public:
  HEEPCut_missingHits(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float threshold_ ;
} ;
class HEEPCut_dxyFirstPV: HEEPCutBase{
private:
  math::XYZPoint firstPV_ ;
  float thresholdBarrel_ ;
  float thresholdEndcap_ ;
public:
  HEEPCut_dxyFirstPV(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
  void setFirstPV(math::XYZPoint*) ;
} ;


class HEEPCut_41_dEtaIn: HEEPCutBase{
public:
  HEEPCut_41_dEtaIn(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float thresholdBarrel_ ;
  float thresholdEndcap_ ;
} ;
class HEEPCut_41_HOverE: HEEPCutBase{
public:
  HEEPCut_41_HOverE(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float thresholdBarrel_ ;
  float thresholdEndcap_ ;
} ;


class HEEPCut_50_50ns_dEtaIn: HEEPCutBase{
public:
  HEEPCut_50_50ns_dEtaIn(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float constantTermBarrel_ ;
  float linearTermBarrel_   ;
  float cutoffTermBarrel_   ;
  float thresholdEndcap_    ;
} ;
class HEEPCut_50_50ns_HOverE: HEEPCutBase{
public:
  HEEPCut_50_50ns_HOverE(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float reciprocalTermBarrel_ ;
  float reciprocalTermEndcap_ ;
  float constantTermBarrel_ ;
  float constantTermEndcap_ ;
} ;


class HEEPCut_50_25ns_dEtaIn: HEEPCutBase{
public:
  HEEPCut_50_25ns_dEtaIn(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float constantTermBarrel_ ;
  float linearTermBarrel_   ;
  float cutoffTermBarrel_   ;
  float constantTermEndcap_ ;
  float linearTermEndcap_   ;
  float cutoffTermEndcap_   ;
} ;
class HEEPCut_50_25ns_HOverE: HEEPCutBase{
public:
  HEEPCut_50_25ns_HOverE(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float reciprocalTermBarrel_ ;
  float reciprocalTermEndcap_ ;
  float constantTermBarrel_ ;
  float constantTermEndcap_ ;
} ;

class HEEPCut_50_dEtaIn: HEEPCutBase{
public:
  HEEPCut_50_dEtaIn(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float constantTermBarrel_ ;
  float linearTermBarrel_   ;
  float cutoffTermBarrel_   ;
  float constantTermEndcap_ ;
  float linearTermEndcap_   ;
  float cutoffTermEndcap_   ;
} ;
class HEEPCut_50_HOverE: HEEPCutBase{
public:
  HEEPCut_50_HOverE(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float reciprocalTermBarrel_ ;
  float reciprocalTermEndcap_ ;
  float constantTermBarrel_ ;
  float constantTermEndcap_ ;
} ;

class HEEPCut_51_dEtaIn: HEEPCutBase{
public:
  HEEPCut_51_dEtaIn(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float thresholdBarrel_ ;
  float thresholdEndcap_ ;
} ;
class HEEPCut_51_HOverE: HEEPCutBase{
public:
  HEEPCut_51_HOverE(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float reciprocalTermBarrel_ ;
  float reciprocalTermEndcap_ ;
  float constantTermBarrel_ ;
  float constantTermEndcap_ ;
} ;

class HEEPCut_60_dEtaIn: HEEPCutBase{
public:
  HEEPCut_60_dEtaIn(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float thresholdBarrel_ ;
  float thresholdEndcap_ ;
} ;
class HEEPCut_60_HOverE: HEEPCutBase{
public:
  HEEPCut_60_HOverE(int, std::string, IIHEModuleHEEP*) ;
  void config(std::vector<HEEPParameter*> parameters_) ;
  bool applyCut(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
private:
  float reciprocalTermBarrel_ ;
  float reciprocalTermEndcap_ ;
  float constantTermBarrel_ ;
  float constantTermEndcap_ ;
} ;


class HEEPCutCollection{
private:
  int cutflow_ ;
  bool status_ ;
  std::string name_ ;
  std::string branchName_n_ ;
  
  IIHEModuleHEEP* parent_mod_ ;
  
  std::vector<HEEPCutBase*      > listOfCuts_ ;
  std::vector<HEEPCutCollection*> listOfCutCollections_ ;
  
  // These variables keep track of the order of cuts/collections to preserve cutflow order
  std::vector<int> cutTypes_ ;
  int cutIndex_ ;
  int collectionIndex_ ;
  
  EcalClusterLazyTools* lazyTool_ ;
  
  bool isActive_ ;
  int nPass_ ;
  
  enum cutType{ kCut , kCollection } ;
public:
  HEEPCutCollection(int, std::string, IIHEModuleHEEP*, bool) ;
  ~HEEPCutCollection() ;
  
  void beginEvent() ;
  void endEvent() ;
  
  void setLazyTool(const edm::Event&, const edm::EventSetup&, edm::EDGetTokenT<edm::SortedCollection<EcalRecHit> >, edm::EDGetTokenT<edm::SortedCollection<EcalRecHit> >) ;
  EcalClusterLazyTools* getLazyTool(){
    EcalClusterLazyTools* lt = *(&lazyTool_) ;
    std::cout << "%%" << lazyTool_ << " " << lt << std::endl ;
    return lt ;
  }
  float e1x5         (reco::CaloCluster) ;
  float e2x5Max      (reco::CaloCluster) ;
  float e5x5         (reco::CaloCluster) ;
  float sigmaIetaIeta(reco::CaloCluster) ;
  
  void addCut(HEEPCutBase*) ;
  void addCutCollection(HEEPCutCollection*) ;
  bool applyCuts(reco::GsfElectron*, EcalClusterLazyTools, bool) ;
  bool applyCuts(reco::GsfElectron*, EcalClusterLazyTools) ;
  bool getStatus(){ return status_ ; }
  void makeCutflowHistogram() ;
  void config(std::vector<HEEPParameter*>) ;
  int cutflow(){ return cutflow_ ; }
  bool isActive(){ return isActive_ ; }
  const int nPass(){ return nPass_ ; }
};

#endif
