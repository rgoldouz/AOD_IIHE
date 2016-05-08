#include "UserCode/IIHETree/interface/IIHEModuleEvent.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleEvent::IIHEModuleEvent(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC):IIHEModule(iConfig)
{
  rhoTokenAll_ =  iC.consumes<double> (InputTag("fixedGridRhoAll"));
  rhoTokenFastjetAll_ =  iC.consumes<double> (InputTag("fixedGridRhoFastjetAll"));
  rhoTokenFastjetAllCalo_ =  iC.consumes<double> (InputTag("fixedGridRhoFastjetAllCalo")); 
  rhoTokenFastjetCentralCalo_ =  iC.consumes<double> (InputTag("fixedGridRhoFastjetCentralCalo"));
  rhoTokenFastjetCentralChargedPileUp_ =  iC.consumes<double> (InputTag("fixedGridRhoFastjetCentralChargedPileUp"));
  rhoTokenFastjetCentralNeutral_ =  iC.consumes<double> (InputTag("fixedGridRhoFastjetCentralNeutral"));

}

IIHEModuleEvent::~IIHEModuleEvent(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleEvent::beginJob(){
  setBranchType(kUInt) ;
  addBranch("ev_event"                 ) ;
  addBranch("ev_run"                   ) ;
  addBranch("ev_luminosityBlock"       ) ;
  addBranch("ev_time"                  ) ;
  addBranch("ev_time_unixTime"         ) ;
  addBranch("ev_time_microsecondOffset") ;
  
  addBranch("ev_fixedGridRhoAll", kFloat) ;
  addBranch("ev_fixedGridRhoFastjetAll", kFloat) ;
  addBranch("ev_fixedGridRhoFastjetAllCalo", kFloat) ;
  addBranch("ev_fixedGridRhoFastjetCentralCalo", kFloat) ;
  addBranch("ev_fixedGridRhoFastjetCentralChargedPileUp", kFloat) ;
  addBranch("ev_fixedGridRhoFastjetCentralNeutral", kFloat) ;


}

// ------------ method called to for each event  ------------
void IIHEModuleEvent::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  store("ev_event"          , ((unsigned int) (iEvent.id().event()          ))) ;
  store("ev_run"            , ((unsigned int) (iEvent.id().run()            ))) ;
  store("ev_luminosityBlock", ((unsigned int) (iEvent.id().luminosityBlock()))) ;
  
  edm::Timestamp time = iEvent.time() ;
  int timestamp_value = time.value() ;
  store("ev_time"                  , timestamp_value         ) ;
  store("ev_time_unixTime"         , time.unixTime()         ) ;
  store("ev_time_microsecondOffset", time.microsecondOffset()) ;
 
  edm::Handle<double> rhoHandleAll ;
  iEvent.getByToken(rhoTokenAll_, rhoHandleAll) ;
  float rhoAll = *rhoHandleAll ;
  store("ev_fixedGridRhoAll", rhoAll) ;

  
  edm::Handle<double> rhoHandlejetAll ;
  iEvent.getByToken(rhoTokenFastjetAll_, rhoHandlejetAll) ;
  float rhojetAll = *rhoHandlejetAll ;
  store("ev_fixedGridRhoFastjetAll", rhojetAll) ;

  edm::Handle<double> rhoHandlejetAllCalo; 
  iEvent.getByToken(rhoTokenFastjetAllCalo_, rhoHandlejetAllCalo) ;
  float rhojetAllCalo = *rhoHandlejetAllCalo;
  store("ev_fixedGridRhoFastjetAllCalo",rhojetAllCalo) ;

  edm::Handle<double> rhoHandlejetCentralCalo;
  iEvent.getByToken(rhoTokenFastjetCentralCalo_, rhoHandlejetCentralCalo) ;
  float rhojetCentralCalo = *rhoHandlejetCentralCalo;
  store("ev_fixedGridRhoFastjetCentralCalo", rhojetCentralCalo) ;

  edm::Handle<double> rhoHandlejetCentralChargedPileUp;
  iEvent.getByToken(rhoTokenFastjetCentralChargedPileUp_, rhoHandlejetCentralChargedPileUp) ;
  float rhojetCentralChargedPileUp = *rhoHandlejetCentralChargedPileUp;
  store("ev_fixedGridRhoFastjetCentralChargedPileUp",rhojetCentralChargedPileUp) ;

  edm::Handle<double> rhoHandlejetCentralNeutral_;
  iEvent.getByToken(rhoTokenFastjetCentralNeutral_,rhoHandlejetCentralNeutral_) ;
  float rhojetCentralNeutral_ = *rhoHandlejetCentralNeutral_;
  store("ev_fixedGridRhoFastjetCentralNeutral",rhojetCentralNeutral_) ;



}

void IIHEModuleEvent::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleEvent::beginEvent(){}
void IIHEModuleEvent::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleEvent::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleEvent);
