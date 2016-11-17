#ifndef UserCode_IIHETree_TriggerObject_h
#define UserCode_IIHETree_TriggerObject_h

#include "UserCode/IIHETree/interface/Types.h"
#include "UserCode/IIHETree/interface/IIHEAnalysis.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace std ;
using namespace reco;
using namespace edm ;

class IIHEAnalysis ;

class TriggerFilter{
  std::string name_ ;
  std::string triggerName_ ;
  std::vector<float> etaValues_ ;
  std::vector<float> phiValues_ ;
  std::string etaBranchName_ ;
  std::string phiBranchName_ ;
  int index_ ;
public:
  TriggerFilter(std::string, std::string);
  ~TriggerFilter(){} ;
  int createBranches(IIHEAnalysis*) ;
  int setIndex(edm::Handle<trigger::TriggerEvent>, edm::InputTag) ;
  int setValues(edm::Handle<trigger::TriggerEvent>, IIHEAnalysis*) ;
  bool store(IIHEAnalysis* analysis) ;
};

class L1Trigger{
private:
  std::string name_ ;
  std::string branchName_ ;
  int filterIndex_;
  bool accept_  ;
  bool touched_ ;
  int prescale_ ;
  int index_ ;
  
  double barrelEnd_       ;
  double regionEtaSizeEB_ ;
  double regionEtaSizeEE_ ;
  double regionPhiSize_   ;
  
  bool matchObject(edm::Handle<trigger::TriggerEvent>, float, float) ;
public:
  L1Trigger(std::string, std::string) ;
  ~L1Trigger() ;
  void reset() ;
  
  std::string       name(){ return       name_ ; }
  std::string branchName(){ return branchName_ ; }
  int setFilterIndex(edm::Handle<trigger::TriggerEvent>, edm::InputTag) ;
  bool matchElectron(edm::Handle<trigger::TriggerEvent>, reco::GsfElectron*) ;
  bool matchMuon    (edm::Handle<trigger::TriggerEvent>, reco::Muon*       ) ;
};

class HLTrigger{
private:
  std::string name_ ;
  int  accept_  ;
  bool touched_ ;
  bool error_ ;
  int  prescale_ ;
  int  index_ ;
  int  searchStatus_ ;
  
  std::vector<float> etaValues_ ;
  std::vector<float> phiValues_ ;
  
  int nSC_     ;
  int nPh_     ;
  int nEl_     ;
  int nMu_     ;
  int nTau_    ;
  int nJet_    ;
  int nBJet_   ;
  int hasMET_  ;
  int hasHT_   ;
  int hasALCa_ ;
  int nSCEl_   ;
  int nSCPh_   ;
  long nTypes_ ;
  
  // Branch names for accept, prescale, eta and phi values.  They must be unique.
  std::string acceptBranchName_   ;
  std::string prescaleBranchName_ ;
  std::string etaBranchName_      ;
  std::string phiBranchName_      ;
  
  enum searchStatuses{ notSearchedFor , searchedForAndFound , searchedForAndNotFound } ;
  
  int nSubstringInString(const std::string&, const std::string&) ;
  int nMuonsInTriggerName() ;
  int nSuperclustersInTriggerName() ;
  int nPhotonsInTriggerName() ;
  int nElectronsInTriggerName() ;
  int nTausInTriggerName() ;
  int nJetsInTriggerName() ;
  int nBJetsInTriggerName() ;
  int METInTriggerName() ;
  int HTInTriggerName() ;
  int ALCaInTriggerName() ;
  
public:
  HLTrigger(std::string, HLTConfigProvider) ;
  ~HLTrigger() ;
  void reset() ;
  int createBranches(IIHEAnalysis*) ;
  bool  beginRun(HLTConfigProvider const&) ;
  
  int findIndex(HLTConfigProvider const&) ;
  int status(const edm::Event&, edm::EventSetup const&, HLTConfigProvider const&, int, Handle<TriggerResults> const&, edm::Handle<trigger::TriggerEvent>, edm::InputTag, IIHEAnalysis*) ;
  void store(IIHEAnalysis*) ;
  
  std::string name(){ return name_ ; }
  void setIndex(int index){ index_ = index ; }
  int index(){ return index_ ; }
  
  bool isSingleElectron(){ return nEl_==1 ; }
  bool isDoubleElectron(){ return nEl_==2 ; }
  bool isTripleElectron(){ return nEl_==3 ; }
  bool isSingleMuon(){ return nMu_==1 ; }
  bool isDoubleMuon(){ return nMu_==2 ; }
  bool isTripleMuon(){ return nMu_==3 ; }
  bool isSinglePhoton(){ return nPh_==1 ; }
  bool isSingleElectronSingleMuon(){ return (nEl_==1 && nMu_==1) ; }
  bool isSingleElectronDoubleMuon(){ return (nEl_==1 && nMu_==2) ; }
  bool isDoubleElectronSingleMuon(){ return (nEl_==2 && nMu_==1) ; }
  bool isOnlySingleElectron(){ return (nTypes_==1*pow(10,(int)kElectron)) ; }
  bool isOnlyDoubleElectron(){ return (nTypes_==2*pow(10,(int)kElectron)) ; }
  bool isOnlyTripleElectron(){ return (nTypes_==3*pow(10,(int)kElectron)) ; }
  bool isOnlySingleMuon(){ return (nTypes_==1*pow(10,(int)kMuon)) ; }
  bool isOnlyDoubleMuon(){ return (nTypes_==2*pow(10,(int)kMuon)) ; }
  bool isOnlyTripleMuon(){ return (nTypes_==3*pow(10,(int)kMuon)) ; }
  bool isOnlySingleElectronSingleMuon(){ return (nTypes_==1*pow(10,(int)kElectron)+1*pow(10,(int)kMuon)) ; }
  bool isOnlySingleElectronDoubleMuon(){ return (nTypes_==1*pow(10,(int)kElectron)+2*pow(10,(int)kMuon)) ; }
  bool isOnlyDoubleElectronSingleMuon(){ return (nTypes_==2*pow(10,(int)kElectron)+1*pow(10,(int)kMuon)) ; }
  
  std::vector<TriggerFilter*> filters_ ;
};

#endif
