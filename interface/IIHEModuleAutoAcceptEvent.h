#ifndef UserCode_IIHETree_IIHEModuleAutoAcceptEvent_h
#define UserCode_IIHETree_IIHEModuleAutoAcceptEvent_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"

// class decleration
class IIHEModuleAutoAcceptEvent : public IIHEModule {
public:
  explicit IIHEModuleAutoAcceptEvent(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  IIHEModuleAutoAcceptEvent(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleAutoAcceptEvent();


  
  void   pubBeginJob(){   beginJob() ; } ;
  void pubBeginEvent(){ beginEvent() ; } ;
  void   pubEndEvent(){   endEvent() ; } ;
  virtual void pubAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){ analyze(iEvent, iSetup) ; } ;
  
  virtual void beginEvent() ;
  virtual void endEvent() ;
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
};
#endif
