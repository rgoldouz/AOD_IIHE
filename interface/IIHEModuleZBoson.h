#ifndef UserCode_IIHETree_IIHEModuleZBoson_h
#define UserCode_IIHETree_IIHEModuleZBoson_h

#include "UserCode/IIHETree/interface/IIHEModule.h"

enum ZTypes{
  kZee,
  kZmm,
  kJmm,
  kYmm,
  kZeeg,
  kZmmg,
  kZem
};

// class decleration
class IIHEModuleZBoson : public IIHEModule {
private:
  float ETThreshold_ ;
  float DeltaRCut_ ;
  
  float mZAccept_ ;
  float mJpsiAcceptLower_ ;
  float mJpsiAcceptUpper_ ;
  float mUpsAcceptLower_  ;
  float mUpsAcceptUpper_  ;
  
  float mZLowerCutoff_ ;
  float mZUpperCutoff_ ;
   
  int nAcceptZee_  ;
  int nAcceptZmm_  ;
  int nAcceptZem_  ;
  int nAcceptJmm_  ;
  int nAcceptYmm_  ;
  int nAcceptZeeg_ ;
  int nAcceptZmmg_ ;
  int nAcceptAll_  ;
  
  bool saveZee_  ;
  bool saveZmm_  ;
  bool saveZem_  ;
  bool saveZeeg_ ;
  bool saveZmmg_ ;
  
public:
  explicit IIHEModuleZBoson(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleZBoson(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleZBoson();
  
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
