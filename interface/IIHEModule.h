#ifndef UserCode_IIHETree_IIHEModule_h
#define UserCode_IIHETree_IIHEModule_h

// system include files
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// user include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEgamma/EgammaElectronAlgos/interface/ElectronHcalHelper.h"

#include "UserCode/IIHETree/interface/BranchWrapper.h"
#include "UserCode/IIHETree/interface/IIHEAnalysis.h"
#include "UserCode/IIHETree/interface/MCTruthObject.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TH1F.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class IIHEAnalysis ; // Forward declaration

// class decleration
class IIHEModule : public edm::EDAnalyzer {

private:

public:
  explicit IIHEModule(const edm::ParameterSet& iConfig);
  ~IIHEModule();
  
  bool addBranch(std::string);
  bool addBranch(std::string,int);
  void config(IIHEAnalysis*);
  void begin();
  void store(std::string, bool        );
  void store(std::string, double      );
  void store(std::string, float       );
  void store(std::string, int         );
  void store(std::string, unsigned int);
  void store(std::string, std::vector<bool        >);
  void store(std::string, std::vector<double      >);
  void store(std::string, std::vector<float       >);
  void store(std::string, std::vector<int         >);
  void store(std::string, std::vector<unsigned int>);
  void setBranchType(int);
  
  bool addValueToMetaTree(std::string, float) ;
  
  void   vetoEvent() ;
  void acceptEvent() ;
  void rejectEvent() ;
  
  void addToMCTruthWhitelist(std::vector<int>) ;
  const MCTruthObject* MCTruth_matchEtaPhi(float, float) ;
  const MCTruthObject* MCTruth_getRecordByIndex(int) ;
  int MCTruth_matchEtaPhi_getIndex(float, float) ;  
  
  void   pubBeginJob(){   beginJob() ; } ;
  void pubBeginEvent(){ beginEvent() ; } ;
  void   pubEndEvent(){   endEvent() ; } ;
  void     pubEndJob(){     endJob() ; } ;
  virtual void pubBeginRun(edm::Run   const& iRun  , edm::EventSetup const& iSetup){ beginRun(iRun  , iSetup) ; } ;
  virtual void  pubAnalyze(edm::Event const& iEvent, edm::EventSetup const& iSetup){  analyze(iEvent, iSetup) ; } ;
  
  std::vector<std::string> splitString(const std::string&, const char*) ;

protected:
  IIHEAnalysis* parent_;
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  
  virtual void beginEvent() ;
  virtual void endEvent() ;
  
  std::vector<BranchWrapperBase*> all_branches_ ;
  std::vector<BranchWrapperBase*> live_branches_;
  
  // ----------member data ---------------------------
  bool debug;
};
#endif
//define this as a plug-in

