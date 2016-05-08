#ifndef UserCode_IIHETree_IIHEModuleLeptonsAccept_h
#define UserCode_IIHETree_IIHEModuleLeptonsAccept_h

#include "UserCode/IIHETree/interface/IIHEModule.h"

// class decleration
class IIHEModuleLeptonsAccept : public IIHEModule {
private:
  int nAcceptAll_  = 0 ;
  
  float ptThreshold_ ;
  int   nElectronsThreshold_ ;
  int   nLeptonsThreshold_   ;
public:
  explicit IIHEModuleLeptonsAccept(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleLeptonsAccept(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleLeptonsAccept();
  
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
