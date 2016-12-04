#include "UserCode/IIHETree/interface/TriggerObject.h"

// The trigger filter is used for matching objects to the trigger.  In general we match to
// the final filter in the trigger, but it doesn't really matter.
TriggerFilter::TriggerFilter(std::string name, std::string triggerName){
  // Each filter has its own name, but that's not enough to identify it (one filter may be
  // used with multiple triggers) so we need to pass the trigger name if we want to be
  // able to cross reference the filters and triggers.
  name_ = name ;
  triggerName_ = triggerName ;
  
  // So far we only store eta and phi, but we can also all ET if we want to.
  etaBranchName_ = "trig_" + triggerName_.substr(0, triggerName_.find("_v")) + "_" + name_ + "_eta" ;
  phiBranchName_ = "trig_" + triggerName_.substr(0, triggerName_.find("_v")) + "_" + name_ + "_phi" ;
}
int TriggerFilter::createBranches(IIHEAnalysis* analysis){
  // Try to add the branches, and return the number of added branches.
  // These methods return 0 if the branches already exist, or if there was an error
  // creating the TTree.
  int result = 0 ;
  result += analysis->addBranch(etaBranchName_, kVectorFloat) ;
  result += analysis->addBranch(phiBranchName_, kVectorFloat) ;
  return result ;
}
int TriggerFilter::setIndex(edm::Handle<trigger::TriggerEvent> trigEvent, edm::InputTag trigEventTag){
  // This is actually just CMSSW voodoo.  We read the index from the trigEvent.  We need
  // to this at least once per run, as the structure of the trigger may change from one
  // run to another.
  index_ = trigEvent->filterIndex(edm::InputTag(name_,"",trigEventTag.process())) ;
  return index_ ;
}
int TriggerFilter::setValues(edm::Handle<trigger::TriggerEvent> trigEvent, IIHEAnalysis* analysis){
  // Clear the values so we can fill them.
  etaValues_.clear() ;
  phiValues_.clear() ;
  
  // Quick sanity check!
  if(index_<0) return 2 ;
  
  // Now choose the filter object using the index.
  if(index_<trigEvent->sizeFilters()){
    const trigger::Keys& trigKeys = trigEvent->filterKeys(index_) ; 
    const trigger::TriggerObjectCollection & trigObjColl(trigEvent->getObjects()) ;
    
    // Now loop over the trigger objects passing filter.  Each object represents a single
    // physics candidate (eg electron, muon).
    for(trigger::Keys::const_iterator keyIt = trigKeys.begin(); keyIt!=trigKeys.end(); ++keyIt){ 
      const trigger::TriggerObject& obj = trigObjColl[*keyIt] ;
      
      // Push the values to the ntuple.
      analysis->store(etaBranchName_, obj.eta()) ;
      analysis->store(phiBranchName_, obj.phi()) ;
      
      // Store them in case we want to do some analysis with them.  (We don't, so we can
      // remove these lines.  It may be useful for debugging though.)
      etaValues_.push_back(obj.eta()) ;
      phiValues_.push_back(obj.phi()) ;
    }
    // Return success.
    return 0 ;
  }
  // Return error- the index was out of range.
  return 1 ;
}
bool TriggerFilter::store(IIHEAnalysis* analysis){
  // Actually I'm not sure this should be here, since we already saved the values above.
  // There's no harm doing this though, it will just save things twice.
  bool etaSuccess = analysis->store(etaBranchName_, etaValues_) ;
  bool phiSuccess = analysis->store(phiBranchName_, phiValues_) ;
  return (etaSuccess && phiSuccess) ;
}

// L1Trigger is not used in the main analysis.  It has coarse matching.  See the
// HLTrigger class which shares the same functionality, and more.
L1Trigger::L1Trigger(std::string name, std::string prefix){
  filterIndex_ = -1 ;
  barrelEnd_ = 1.4791 ;
  regionEtaSizeEB_ = 0.522 ;
  regionEtaSizeEE_ = 1.0 ;
  regionPhiSize_ = 1.044 ;
  name_ = name ;
  branchName_ = prefix + name_ ;
  reset() ;
}
L1Trigger::~L1Trigger(){}
void L1Trigger::reset(){
  accept_ = false ;
  touched_ = false ;
  prescale_ = -999 ;
}
int L1Trigger::setFilterIndex(edm::Handle<trigger::TriggerEvent> trigEvent, edm::InputTag trigEventTag){
  filterIndex_ = trigEvent->filterIndex(edm::InputTag(name_,"",trigEventTag.process())) ;
  return filterIndex_ ;
}
bool L1Trigger::matchObject(edm::Handle<trigger::TriggerEvent> trigEvent, float eta, float phi){
  // Careful that L1 triggers only have discrete eta phi. Need to be extremely loose. 
  // See here: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/SHarper/SHNtupliser/src/SHTrigInfo.cc?revision=1.5&view=markup&pathrev=HEAD
  // It is important to specify the right HLT process for the filter, not doing this is a common bug
  if(filterIndex_<0) return false ;
  if(filterIndex_<trigEvent->sizeFilters()){ 
    const trigger::Keys& trigKeys = trigEvent->filterKeys(filterIndex_) ;
    
    const trigger::TriggerObjectCollection& trigObjColl(trigEvent->getObjects()) ;
    
    // Now loop of the trigger objects passing filter
    for(trigger::Keys::const_iterator keyIt=trigKeys.begin() ; keyIt!=trigKeys.end() ; ++keyIt){ 
      
      const trigger::TriggerObject& obj = trigObjColl[*keyIt] ;
      
      float objeta = obj.eta() ;
      float objphi = obj.phi() ;
      
      double etaBinLow  = 0.0 ;
      double etaBinHigh = 0.0 ;
      
      if(fabs(objeta)<barrelEnd_){
        etaBinLow  = objeta    - regionEtaSizeEB_/2.0 ;
        etaBinHigh = etaBinLow + regionEtaSizeEB_     ;
      }
      else{
        etaBinLow  = objeta    - regionEtaSizeEE_/2.0 ;
        etaBinHigh = etaBinLow + regionEtaSizeEE_     ;
      }
      
      if(eta<etaBinHigh && eta>etaBinLow){
        return true ;
      }
      float dPhi = reco::deltaPhi(phi,objphi) ;
      if(eta<etaBinHigh && eta>etaBinLow &&  dPhi<regionPhiSize_/2.0){
        return true ;
      }
    }
  }
  return false ;
}

// Each HLTrigger has a name and a list of filters.  We pass the hltConfig so we can find
// its filters.
HLTrigger::HLTrigger(std::string name, HLTConfigProvider hltConfig){
  name_ = name ;
  
  // The index tells us how to find the trigger quickly for each event.
  index_ = -1 ;
  
  // This is set so that we do not attempt to use a trigger that has not been found in the
  // event.  (For example, if we create an HLTrigger with a typo somehow.)
  searchStatus_ = notSearchedFor ;
  
  // Reset to the default values.  This must be done at each new run, so it is done in its
  // own method that can be called multiple times.
  reset() ;

  // Declare the branch names.
  acceptBranchName_   = "trig_" + name.substr(0, name.find("_v")) + "_accept"   ;
  prescaleBranchName_ = "trig_" + name.substr(0, name.find("_v")) + "_prescale" ;
  
  // Now parse the name to see how many superclusters, electrons etc are in the trigger.
  nSC_     = nSuperclustersInTriggerName() ;
  nPh_     = nPhotonsInTriggerName() ;
  nEl_     = nElectronsInTriggerName() ;
  nMu_     = nMuonsInTriggerName() ;
  nTau_    = nTausInTriggerName() ;
  nJet_    = nJetsInTriggerName() ;
  nBJet_   = nBJetsInTriggerName() ;
  hasMET_  = METInTriggerName() ;
  hasHT_   = HTInTriggerName() ;
  hasALCa_ = ALCaInTriggerName() ;
  nSCEl_   = nSC_+nEl_ ;
  
  // Slightly easier way to handle multiple objects.  In principle you should be able to
  // print this to the screen and read off the topology.  For example, a double electron
  // trigger would be 200, and an electron-muon trigger would be 1100.  This is never
  // used in the analysis, but can be used in debugging, and could be developed into an
  // alternative way to select triggers.
  nTypes_ =     nSC_*pow(10,(int)kSuperCluster)
          +     nPh_*pow(10,(int)kPhoton)
          +     nEl_*pow(10,(int)kElectron)
          +     nMu_*pow(10,(int)kMuon)
          +    nTau_*pow(10,(int)kTau)
          +    nJet_*pow(10,(int)kJet)
          +   nBJet_*pow(10,(int)kBJet)
          +  hasMET_*pow(10,(int)kMET) 
          +   hasHT_*pow(10,(int)kHT) 
          + hasALCa_*pow(10,(int)kALCa) ;
  
  // Find the trigger index from the config.  (This allows for much faster lookup per
  // event.)
  findIndex(hltConfig) ;
  
  // Now loop through all the filters to get the ones for this trigger.  We use the
  // trigger index to get the filters (called modules) and only save them if they are
  // enabled.  Most filters are not enabled, so the saveTags() evaluates to false.
  std::vector<std::string> moduleNames = hltConfig.moduleLabels(index_) ;
  std::vector<std::string> moduleNamesWithTags ;
  for(unsigned int j=0 ; j<moduleNames.size() ; ++j){
    if(hltConfig.saveTags(moduleNames.at(j))){
      moduleNamesWithTags.push_back(moduleNames.at(j)) ;
    }
  }
  // Now add the filters we saved.  This is done as a separate step in case we want to add
  // a flag to enable the user to not save filters.
  for(unsigned int i=0 ; i<moduleNamesWithTags.size() ; ++i){
    filters_.push_back(new TriggerFilter(moduleNamesWithTags.at(i), name_)) ;
  }
}
HLTrigger::~HLTrigger(){}

void HLTrigger::reset(){
  // Default values.  Set accept_ to neither 0 (fail) or 1 (pass), but to -1 (not set).
  accept_ = -1 ;
  
  // The ntuples hasn't see this value be filled yet, so set touched_ to false.
  touched_ = false ;
  
  // Set the prescale to a suitably crazy value so we don't take it seriously.
  prescale_ = -999 ;
}

int HLTrigger::nSuperclustersInTriggerName(){
  // Count the number of superclusters in the trigger.  We never used this, but it's here
  // in case we need backup triggers, or a trigger study.
  int scCount = nSubstringInString(name_, "_SC") ;
  return scCount ;
}
int HLTrigger::nPhotonsInTriggerName(){
  // Count the number of photons.  Note that "DoublePhoton" contains "Photon", we must be
  // careful not to count that more than once.  That's why there's an offset in the
  // expression for totalPhotonCount.  The same applies to electrons and muons.
  int singlePhotonCount = nSubstringInString(name_, "Photon"      ) ;
  int doublePhotonCount = nSubstringInString(name_, "DoublePhoton") ;
  int triplePhotonCount = nSubstringInString(name_, "TriplePhoton") ;
  int totalPhotonCount = 2*triplePhotonCount + 1*doublePhotonCount + singlePhotonCount ;
  return totalPhotonCount ;
}
int HLTrigger::nElectronsInTriggerName(){
  int singleElectronCount = nSubstringInString(name_, "Ele"      ) ;
  int doubleElectronCount = nSubstringInString(name_, "DoubleEle") + nSubstringInString(name_, "DiEle") ;
  int tripleElectronCount = nSubstringInString(name_, "TripleEle") ;
  int totalElectronCount = 2*tripleElectronCount + 1*doubleElectronCount + singleElectronCount ;
  return totalElectronCount ;
}
int HLTrigger::nMuonsInTriggerName(){
  int singleMuonCount = nSubstringInString(name_, "Mu"      ) ;
  int doubleMuonCount = nSubstringInString(name_, "DoubleMu") + nSubstringInString(name_, "DiMu") + nSubstringInString(name_, "Dimuon") ;
  int tripleMuonCount = nSubstringInString(name_, "TripleMu") ;
  int totalMuonCount = 2*tripleMuonCount + 1*doubleMuonCount + singleMuonCount ;
  return totalMuonCount ;
}
int HLTrigger::nTausInTriggerName(){
  int tauCount = nSubstringInString(name_, "Tau") ;
  return tauCount ;
}
int HLTrigger::nJetsInTriggerName(){
  // Jets are a pain.  They have so many jets.
  int ignoreJetCount = nSubstringInString(name_, "NoJetId"  ) ;
  int singleJetCount = nSubstringInString(name_, "Jet"  ) ;
  int doubleJetCount = nSubstringInString(name_, "DiJet") + nSubstringInString(name_, "DiPFJet") + nSubstringInString(name_, "DoubleJet") + nSubstringInString(name_, "DiCentralJet") + nSubstringInString(name_, "DiCentralPFJet") ;
  int tripleJetCount = nSubstringInString(name_, "TriCentralPFJet" ) ;
  int quadJetCount  = nSubstringInString(name_, "QuadJet" ) + nSubstringInString(name_, "QuadPFJet" ) ;
  int sixJetCount   = nSubstringInString(name_, "SixJet"  ) + nSubstringInString(name_, "SixPFJet"  ) ;
  int eightJetCount = nSubstringInString(name_, "EightJet") + nSubstringInString(name_, "EightPFJet") ;
  int totalJetCount = 7*eightJetCount + 5*sixJetCount + 3*quadJetCount + 2*tripleJetCount + 1*doubleJetCount + singleJetCount - 1*ignoreJetCount ;
  return totalJetCount ;
}
int HLTrigger::nBJetsInTriggerName(){
  int singleBJetCount = nSubstringInString(name_, "BJet"  ) ;
  int doubleBJetCount = nSubstringInString(name_, "DiBJet"  ) + nSubstringInString(name_,  "DiCentral") ;
  int tripleBJetCount = nSubstringInString(name_, "TriiBJet") + nSubstringInString(name_, "TriCentral") ;
  int totalBJetCount = 2*tripleBJetCount + 1*doubleBJetCount + singleBJetCount ;
  return totalBJetCount ;
}
int HLTrigger::METInTriggerName(){
  int METCount = nSubstringInString(name_, "MET") ;
  return METCount ;
}
int HLTrigger::HTInTriggerName(){
  int HTCount = nSubstringInString(name_, "HT") ;
  return HTCount ;
}
int HLTrigger::ALCaInTriggerName(){
  int ALCaCount = nSubstringInString(name_, "ALCa") ;
  return ALCaCount ;
}
int HLTrigger::nSubstringInString(const std::string& str, const std::string& sub){
  // Taken from https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_X/HLTriggerOffline/Egamma/src/EmDQM.cc#L1064
  // Thanks, Thomas!
  if(sub.length()==0) return 0 ;
  int count = 0 ;
  for (size_t offset=str.find(sub) ; offset!=std::string::npos ; offset=str.find(sub, offset + sub.length())){ ++count ; }
  return count;
}

//int HLTrigger::status(const edm::Event& iEvent, edm::EventSetup const& iSetup, HLTConfigProvider const& hltConfig, HLTPrescaleProvider const& hltPrescale, Handle<TriggerResults> const& triggerResults, edm::Handle<trigger::TriggerEvent> trigEvent, edm::InputTag trigEventTag, IIHEAnalysis* analysis){
  // Okay, here we pass a lot of information to the method!
int HLTrigger::status(const edm::Event& iEvent, const edm::EventSetup & iSetup, const HLTConfigProvider & hltConfig,int  Prescale, const Handle<TriggerResults> & triggerResults, edm::Handle<trigger::TriggerEvent> trigEvent, edm::InputTag trigEventTag, IIHEAnalysis* analysis){  

  // First check to see if we have successfully found the trigger.
  if(searchStatus_==searchedForAndFound && index_>=0){
    // Let the analyser know the ntuple has touched the member, so it doesn't report it as
    // an error during debugging.
    touched_  = true ;
    
    // Now get the variables we care about.
    accept_   = triggerResults->accept(index_) ;
//    prescale_ = 1;
//    Prescale.prescaleValue (iEvent,iSetup,name_);
//    int preScaleColumn = (Prescale.hltConfigProvider().inited()) ? Prescale.prescaleSet(iEvent,iSetup) : -1;
    prescale_ = hltConfig.prescaleValue(Prescale, name_);
 
    // And finally loop over the filters.
    for(unsigned i=0 ; i<filters_.size() ; ++i){
      TriggerFilter* filter = filters_.at(i) ;
      filter->setIndex(trigEvent, trigEventTag) ;
      filter->setValues(trigEvent, analysis) ;
    }
    // Return success!
    return 0 ;
  }
  // Return failure.
  return 2 ;
}
void HLTrigger::store(IIHEAnalysis* analysis){
  // Save the values to the ntuple.
  analysis->store(  acceptBranchName_, accept_  ) ;
  analysis->store(prescaleBranchName_, prescale_) ;
}

int HLTrigger::createBranches(IIHEAnalysis* analysis){
  // Create the branches we need.
  int result = 0 ;
  
  // Only report where we were successful, in case the TTree doesn't exist or something.
  result += analysis->addBranch(  acceptBranchName_, kInt) ;
  result += analysis->addBranch(prescaleBranchName_, kInt) ;
  
  // Now loop over the filters.
  for(unsigned i=0 ; i<filters_.size() ; ++i){
    result += filters_.at(i)->createBranches(analysis) ;
  }
  return result ;
}

bool HLTrigger::beginRun(HLTConfigProvider const& hltConfig){
  // Just report success if we find the index.
  bool success = findIndex(hltConfig) ;
  return success ;
}
int HLTrigger::findIndex(HLTConfigProvider const& hltConfig){
  // Set the flag to let the trigger know we haven't searched for it.
  searchStatus_ = notSearchedFor ;
  
  // Loop over the triggers that exist in the event.
  std::vector<std::string> names = hltConfig.triggerNames() ;
  for(unsigned int i=0 ; i<names.size() ; ++i){
    if(names.at(i)==name_){
      index_ = i ;
      
      // Woohoo!  We found it.  Update the flag appropriately.
      searchStatus_ = searchedForAndFound ;
      
      // Return success.
      return 0 ;
    }
  }
  
  // Boo!  We failed to find it.  Update the flag appropriately.
  index_ = -1 ;
  searchStatus_ = searchedForAndNotFound ;
  return 1 ;
}

