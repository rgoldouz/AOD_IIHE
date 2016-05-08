#ifndef UserCode_IIHETree_IIHEModuleGedGsfElectron_h
#define UserCode_IIHETree_IIHEModuleGedGsfElectron_h

#include "UserCode/IIHETree/interface/IIHEModule.h"

// class decleration
class IIHEModuleGedGsfElectron : public IIHEModule {
private:
  inline float etacorr(float eta, float pvz, float scz){ return asinh(sinh(eta)*(1.0-pvz/scz)) ; }
  edm::EDGetTokenT<EcalRecHitCollection>        ebReducedRecHitCollection_;
  edm::EDGetTokenT<EcalRecHitCollection>        eeReducedRecHitCollection_;
  edm::EDGetTokenT<reco::TrackCollection>       generalTracksToken_;
  edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_ ;
public:
  explicit IIHEModuleGedGsfElectron(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleGedGsfElectron(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleGedGsfElectron() ;
  
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
  
  float ETThreshold_ ;
};
#endif
