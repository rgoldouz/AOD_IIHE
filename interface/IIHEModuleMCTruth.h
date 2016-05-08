#ifndef UserCode_IIHETree_IIHEModuleMCTruth_h
#define UserCode_IIHETree_IIHEModuleMCTruth_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "UserCode/IIHETree/interface/MCTruthObject.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

// class decleration
class IIHEModuleMCTruth : public IIHEModule {
public:
  explicit IIHEModuleMCTruth(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleMCTruth(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleMCTruth();
  
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
  
  int matchEtaPhi_getIndex(float, float) ;
  const MCTruthObject* matchEtaPhi(float, float) ;
  const MCTruthObject* getRecordByIndex(int) ;
  
  void setWhitelist(){ whitelist_ = whitelist_ = parent_->getMCTruthWhitelist() ; }
private:
  std::vector<int> whitelist_ ;
  double pt_threshold_ ;
  double  m_threshold_ ;
  double DeltaROverlapThreshold_ ;
  std::vector<MCTruthObject*> MCTruthRecord_ ;
  
  edm::InputTag puInfoSrc_ ;
  edm::EDGetTokenT<GenEventInfoProduct> generatorLabel_;
  edm::EDGetTokenT<vector<PileupSummaryInfo> > puCollection_;
  edm::EDGetTokenT<vector<reco::GenParticle> > genParticlesCollection_;
  float nEventsWeighted_ ;
};
#endif
