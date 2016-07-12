#include "UserCode/IIHETree/interface/IIHEModuleTrigger.h"
#include "UserCode/IIHETree/interface/TriggerObject.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;
IIHEModuleTrigger::IIHEModuleTrigger(const edm::ParameterSet& iConfig):IIHEModule(iConfig),
hltPrescaleProvider_(iConfig, consumesCollector(), *this){}

IIHEModuleTrigger::IIHEModuleTrigger(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig),
 hltPrescaleProvider_(iConfig, consumesCollector(), *this) 
{
//  hlTriggerResultsTag_ = iConfig.getParameter<edm::InputTag>("hltTriggerSummaryAOD","","HLT") ;
  nEvents_ = 0 ;
  nWasRun_ = 0 ;
  nAccept_ = 0 ;
  nErrors_ = 0 ;

 triggerBits_ = iC.consumes<edm::TriggerResults>(InputTag("TriggerResults","","HLT"));
 trigEvent_ = iC.consumes<trigger::TriggerEvent>(InputTag("hltTriggerSummaryAOD","","HLT"));
  
  // Comments are given with "steps" that should be read in order.  The code for the
  // triggers is complicated, so we need some help with navigation!
  
  // Step 1: Read the list of triggers provided by the pset.  This consists of a
  // semicolon separated list of triggers and trigger topologies.  If we want to save
  // double electrons, we add the "doubleElectron" to the argument in the pset.
  std::string triggersIn = iConfig.getUntrackedParameter<std::string>("triggers" , "") ;
  
  // We can also specify individual triggers using a comma separated list.
  triggerNamesFromPSet_ = splitString(triggersIn, ",") ;
  
  std::cout << triggersIn << std::endl ;
  
  // We have a series of flags for different topologies, so we parse the argument from
  // the pset to set them.
  includeSingleElectronTriggers_ = (triggersIn.find("singleElectron")!=std::string::npos) ;
  includeDoubleElectronTriggers_ = (triggersIn.find("doubleElectron")!=std::string::npos) ;
  includeTripleElectronTriggers_ = (triggersIn.find("tripleElectron")!=std::string::npos) ;
  includeSingleMuonTriggers_     = (triggersIn.find("singleMuon"    )!=std::string::npos) ;
  includeDoubleMuonTriggers_     = (triggersIn.find("doubleMuon"    )!=std::string::npos) ;
  includeTripleMuonTriggers_     = (triggersIn.find("tripleMuon"    )!=std::string::npos) ;
  includeSingleElectronSingleMuonTriggers_ = (triggersIn.find("singleElectronSingleMuon")!=std::string::npos) ;
  includeSingleElectronDoubleMuonTriggers_ = (triggersIn.find("singleElectronDoubleMuon")!=std::string::npos) ;
  includeDoubleElectronSingleMuonTriggers_ = (triggersIn.find("doubleElectronSingleMuon")!=std::string::npos) ;
  
  // Then tell the user which topologies we will save.
  std::cout << "Including single electron triggers: " << includeSingleElectronTriggers_ << std::endl ;
  std::cout << "Including double electron triggers: " << includeDoubleElectronTriggers_ << std::endl ;
  std::cout << "Including triple electron triggers: " << includeTripleElectronTriggers_ << std::endl ;
  std::cout << "Including single muon triggers:     " << includeSingleMuonTriggers_     << std::endl ;
  std::cout << "Including double muon triggers:     " << includeDoubleMuonTriggers_     << std::endl ;
  std::cout << "Including triple muon triggers:     " << includeTripleMuonTriggers_     << std::endl ;
  std::cout << "Including single electron single muon triggers: " << includeSingleElectronSingleMuonTriggers_ << std::endl ;
  std::cout << "Including single electron double muon triggers: " << includeSingleElectronDoubleMuonTriggers_ << std::endl ;
  std::cout << "Including double electron single muon triggers: " << includeDoubleElectronSingleMuonTriggers_ << std::endl ;
  
  // At this point we have loaded all the information about the triggers we want to save.
  // Note that it's much safer to save topologies instead of individual triggers,
  // because trigger names change in data with little or no warning!

}
IIHEModuleTrigger::~IIHEModuleTrigger(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleTrigger::beginJob(){
}

bool IIHEModuleTrigger::addHLTrigger(HLTrigger* hlt){
  // Check to see if the HLTrigger instance is already in the vector, and if not, add it.
  for(unsigned int i=0 ; i<HLTriggers_.size() ; ++i){
    if(HLTriggers_.at(i)->name()==hlt->name()){
      return false ;
    }
  }
  HLTriggers_.push_back(hlt) ;
  return true ;
}

int IIHEModuleTrigger::addBranches(){
  // Just loop over the HLTrigger objects and make branches for each one, including its
  // filters.  Return the number of branches we've added so that we can get an idea of
  // the trigger overhead.  (This is not used in the main analysis, but can be trivially
  // printed to screen.)
  int result = 0 ;
  IIHEAnalysis* analysis = parent_ ;
  for(unsigned int i=0 ; i<HLTriggers_.size() ; i++){
    result += HLTriggers_.at(i)->createBranches(analysis) ;
  }
  return result ;
}

// ------------ method called to for each event  ------------
void IIHEModuleTrigger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){





  // Declare some variables to hold the trigger information.
  edm::Handle<trigger::TriggerEvent> trigEvent ;
  iEvent.getByToken(trigEvent_,trigEvent) ;
  
  // Get hold of TriggerResults, using the normal getByLabel method.
 edm::Handle<TriggerResults> HLTR ;
 iEvent.getByToken(triggerBits_, HLTR) ;
 edm::InputTag trigEventTag("hltTriggerSummaryAOD","","HLT") ;  
  // Now fill the values.
  IIHEAnalysis* analysis = parent_ ;
  for(unsigned int i=0 ; i<HLTriggers_.size() ; i++){
    // Here we loop over the triggers one by one.
    HLTrigger* hlt = HLTriggers_.at(i) ;
    // Then pass the trigger information to the trigger so that it can pick out the
    // results from the event.
    hlt->status(iEvent, iSetup, hltConfig_, hltPrescaleProvider_, HLTR, trigEvent, trigEventTag, analysis) ;
    // Finally store the information to the ntuple.
    hlt->store(analysis) ;
  }
  nEvents_++ ;
}

void IIHEModuleTrigger::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){
  // Step 2: Read a new run.
  // The trigger names may change between runs, so we need to check the trigger names
  // with each run.
  
  // First, tell the module that things have changed.  (This is just a sanity check in
  // case we ever try to save information to ntuple before we've set this flag.)
  bool changed = true ;

 edm::InputTag trigEventTag("hltTriggerSummaryAOD","","HLT") ;
  hltPrescaleProvider_.init(iRun, iSetup, trigEventTag.process(), changed);
  
  // First check to see that we can initialise the hltconfig...
  if(hltConfig_.init(iRun, iSetup, trigEventTag.process(), changed)){
    // ...and that we've noticed that things have changed...
    if(changed){
      // This is debug information.  It's very verbose, so I commented it out.
      if(false) hltConfig_.dump("Modules") ;
      
      // Get the updated list of trigger names.
      HLTNamesFromConfig_ = hltConfig_.triggerNames() ;
      
      // Then loop over the triggers one by one.
      for(unsigned int i=0 ; i<HLTNamesFromConfig_.size() ; ++i){
        // Get the names.
        std::string name = HLTNamesFromConfig_.at(i) ;
        
        // Attempt to add the trigger.
        // The default action is not to add the trigger, unless its name matches.
        bool addThisTrigger = false ;
        
        // Create a new instance HLTrigger so that we can use its methods to find out its
        // topology.
        HLTrigger* hlt = new HLTrigger(name, hltConfig_) ;
        
        // First check to see if it's in the list of requested triggers.  Each line asks
        // if we want to save a given topology, and if the trigger has that topology.
        // For example, the first clause evaluates to true if and only if we ask to save
        // single electron triggers, and this trigger's name matches a single electron
        // trigger (eg HLT_SingleEle27_WP80).
        if(hlt->isOnlySingleElectron()           && includeSingleElectronTriggers_          ) addThisTrigger = true ;
        if(hlt->isOnlyDoubleElectron()           && includeDoubleElectronTriggers_          ) addThisTrigger = true ;
        if(hlt->isOnlyTripleElectron()           && includeTripleElectronTriggers_          ) addThisTrigger = true ;
        if(hlt->isOnlySingleMuon()               && includeSingleMuonTriggers_              ) addThisTrigger = true ;
        if(hlt->isOnlyDoubleMuon()               && includeDoubleMuonTriggers_              ) addThisTrigger = true ;
        if(hlt->isOnlyTripleMuon()               && includeTripleMuonTriggers_              ) addThisTrigger = true ;
        if(hlt->isOnlySingleElectronSingleMuon() && includeSingleElectronSingleMuonTriggers_) addThisTrigger = true ;
        if(hlt->isOnlySingleElectronDoubleMuon() && includeSingleElectronDoubleMuonTriggers_) addThisTrigger = true ;
        if(hlt->isOnlyDoubleElectronSingleMuon() && includeDoubleElectronSingleMuonTriggers_) addThisTrigger = true ;
        
        // Finally, if we haven't added the trigger, we compare it to the list of
        // individual triggers to see if it exists there.  (I don't think is ever used in
        // the analysis, but it is included in case someone wants to use a small number
        // of specific triggers.)
        if(addThisTrigger==false){
          for(unsigned int j=0 ; j<triggerNamesFromPSet_.size() ; ++j){
            // Here we do the comparison, although it might be safer to use find(), as we
            // do above.
            if(triggerNamesFromPSet_.at(j)==name){
              addThisTrigger = true ;
              break ;
            }
          }
        }
        
        // Check to see if this trigger matches the topologies or names, and if it does,
        // add it to the vector of triggers to save.
        if(addThisTrigger==false) continue ;
        addHLTrigger(hlt) ;
      }
      
      // This step may only work for AOD!
      // Now we need to re-map the indices to the names, given that some new triggers may
      // have been inserted to the menu.
      // This method loops over the HLTrigger objects and compares their names to the
      // CMSSW triggerObjects to get the indices and the filters.
      for(unsigned int i=0 ; i<HLTriggers_.size() ; ++i){
        HLTriggers_.at(i)->beginRun(hltConfig_) ;
      }
      
      // Attempt to add branches to the ntuple.  This reads the HLTrigger objects and
      // makes branches for the status, prescale, and filters.
      addBranches() ;
      parent_->configureBranches() ;
    
      // Now reset things to 0.
      nEvents_ = 0 ;
      nWasRun_ = 0 ;
      nAccept_ = 0 ;
      nErrors_ = 0 ;
    }
  }
  else{
    std::cout << "Failed to init hltConfig" << std::endl ;
  }
}

void IIHEModuleTrigger::beginEvent(){}
void IIHEModuleTrigger::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleTrigger::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleTrigger);
