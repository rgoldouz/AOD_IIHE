#ifndef UserCode_IIHETree_IIHEModuleMET_h
#define UserCode_IIHETree_IIHEModuleMET_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/PatCandidates/interface/MET.h"

// class decleration
class IIHEModuleMET : public IIHEModule {
public:
  explicit IIHEModuleMET(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleMET(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleMET();
  
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

private:
 edm::EDGetTokenT<edm::View<pat::MET> > pfMETToken_;


};
#endif
