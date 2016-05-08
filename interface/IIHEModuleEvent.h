#ifndef UserCode_IIHETree_IIHEModuleEvent_h
#define UserCode_IIHETree_IIHEModuleEvent_h
#include "FWCore/Utilities/interface/InputTag.h"
#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "TMath.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

// class decleration
class IIHEModuleEvent : public IIHEModule {
private:

   edm::EDGetTokenT<double> rhoTokenAll_;
   edm::EDGetTokenT<double> rhoTokenFastjetAll_;
   edm::EDGetTokenT<double> rhoTokenFastjetAllCalo_;
   edm::EDGetTokenT<double> rhoTokenFastjetCentralCalo_;
   edm::EDGetTokenT<double> rhoTokenFastjetCentralChargedPileUp_;
   edm::EDGetTokenT<double> rhoTokenFastjetCentralNeutral_;

public:
  explicit IIHEModuleEvent(const edm::ParameterSet& iConfig,  edm::ConsumesCollector && iC);
  explicit IIHEModuleEvent(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleEvent();
  
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
