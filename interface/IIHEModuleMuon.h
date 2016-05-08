#ifndef UserCode_IIHETree_IIHEModuleMuon_h
#define UserCode_IIHETree_IIHEModuleMuon_h

#include "UserCode/IIHETree/interface/IIHEModule.h"

// class declerations
class IIHEMuonTrackVariableBase{
public:
  IIHEMuonTrackVariableBase(std::string, std::string, int) ;
  ~IIHEMuonTrackVariableBase(){} ;
  
  const std::string       Name(){ return       name_ ; }
  const std::string BranchName(){ return branchName_ ; }
  const int         branchType(){ return branchType_ ; }
  
  virtual void reset(){} ;
  virtual void store(IIHEAnalysis*){} ;
  bool addBranch(IIHEAnalysis*) ;
  
private:
  std::string       name_ ;
  std::string branchName_ ;
  int         branchType_ ;
};

class IIHEMuonTrackVariableInt: IIHEMuonTrackVariableBase{
public:
  IIHEMuonTrackVariableInt(std::string, std::string) ;
  ~IIHEMuonTrackVariableInt(){} ;
  void reset(){ value_ = -999 ; }
  void fill(int value){ value_ = value ; }
  void store(IIHEAnalysis*) ;
private:
  int value_ ;
};

class IIHEMuonTrackVariableFloat: IIHEMuonTrackVariableBase{
public:
  IIHEMuonTrackVariableFloat(std::string, std::string) ;
  ~IIHEMuonTrackVariableFloat(){} ;
  void reset(){ value_ = -999.0 ; }
  void fill(float value){ value_ = value ; }
  void store(IIHEAnalysis*) ;
private:
  float value_ ;
};

class IIHEMuonTrackWrapper{
public:
  explicit IIHEMuonTrackWrapper(std::string);
  ~IIHEMuonTrackWrapper(){};
  
  void addBranches(IIHEAnalysis*) ;
  void reset() ;
  void fill(reco::TrackRef&, math::XYZPoint*, math::XYZPoint*) ;
  void store(IIHEAnalysis*) ;
  
  // Taken from DataFormats/MuonReco/interface/Muon.h
  enum MuonTrackType {None, InnerTrack, OuterTrack, CombinedTrack, TPFMS, Picky, DYT} ;
private:
  int type_ ;
  std::string prefix_ ;
  
  IIHEMuonTrackVariableInt*   charge_        ;
  IIHEMuonTrackVariableFloat* qoverp_        ;
  IIHEMuonTrackVariableFloat* pt_            ;
  IIHEMuonTrackVariableFloat* eta_           ;
  IIHEMuonTrackVariableFloat* phi_           ;
  IIHEMuonTrackVariableFloat* p_             ;
  IIHEMuonTrackVariableFloat* px_            ;
  IIHEMuonTrackVariableFloat* py_            ;
  IIHEMuonTrackVariableFloat* pz_            ;
  IIHEMuonTrackVariableFloat* theta_         ;
  IIHEMuonTrackVariableFloat* lambda_        ;
  IIHEMuonTrackVariableFloat* d0_            ;
  IIHEMuonTrackVariableFloat* dz_            ;
  IIHEMuonTrackVariableFloat* dz_beamspot_   ;
  IIHEMuonTrackVariableFloat* dz_firstPVtx_  ;
  IIHEMuonTrackVariableFloat* dxy_           ;
  IIHEMuonTrackVariableFloat* dxy_beamspot_  ;
  IIHEMuonTrackVariableFloat* dxy_firstPVtx_ ;
  IIHEMuonTrackVariableFloat* dsz_           ;
  IIHEMuonTrackVariableFloat* vx_            ;
  IIHEMuonTrackVariableFloat* vy_            ;
  IIHEMuonTrackVariableFloat* vz_            ;
  IIHEMuonTrackVariableFloat* qoverpError_   ;
  IIHEMuonTrackVariableFloat* ptError_       ;
  IIHEMuonTrackVariableFloat* thetaError_    ;
  IIHEMuonTrackVariableFloat* lambdaError_   ;
  IIHEMuonTrackVariableFloat* phiError_      ;
  IIHEMuonTrackVariableFloat* dxyError_      ;
  IIHEMuonTrackVariableFloat* d0Error_       ;
  IIHEMuonTrackVariableFloat* dszError_      ;
  IIHEMuonTrackVariableFloat* dzError_       ;
  IIHEMuonTrackVariableFloat* etaError_      ;
  IIHEMuonTrackVariableFloat* chi2_          ;
  IIHEMuonTrackVariableFloat* ndof_          ;
  IIHEMuonTrackVariableFloat* normalizedChi2_;
  
  std::vector<IIHEMuonTrackVariableBase*> variables_ ;
};

class IIHEModuleMuon : public IIHEModule {
public:
  explicit IIHEModuleMuon(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleMuon(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleMuon() ;
  
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
  IIHEMuonTrackWrapper* globalTrackWrapper_ ;
  IIHEMuonTrackWrapper* outerTrackWrapper_  ;
  IIHEMuonTrackWrapper* innerTrackWrapper_  ;
  
  bool storeGlobalTrackMuons_ ;
  bool storeStandAloneMuons_  ;
  bool storeInnerTrackMuons_  ;
  float ptThreshold_ ;
};
#endif
