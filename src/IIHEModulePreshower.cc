#include "UserCode/IIHETree/interface/IIHEModulePreshower.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/Records/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/EcalPreshowerTopology.h"
#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"
#include "RecoCaloTools/Navigation/interface/EcalPreshowerNavigator.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"

#include <iostream>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModulePreshower::IIHEModulePreshower(const edm::ParameterSet& iConfig): IIHEModule(iConfig),
geometryPreshower_(0),
topologyPreshower_(0)
{}
IIHEModulePreshower::~IIHEModulePreshower(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModulePreshower::beginJob(){
  addBranch("es_stripsX", kVectorVectorFloat) ;
  addBranch("es_stripsY", kVectorVectorFloat) ;
}

// ------------ method called to for each event  ------------
void IIHEModulePreshower::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  edm::ESHandle<CaloGeometry> pGeometry ;
  iSetup.get<CaloGeometryRecord>().get(pGeometry) ;
  CaloGeometry* geometry = (CaloGeometry*) pGeometry.product() ;
  geometryPreshower_ = (CaloSubdetectorGeometry*) geometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower) ;
  if(geometry) topologyPreshower_ = new EcalPreshowerTopology(geometry) ;
}

void IIHEModulePreshower::printPreshowerCells(int start){
  // This function walks through the preshower, dumping the information about the individual strips
  // It should be run four times- once for each side of the detector, and once for each plane
  // Never call this when running over many events, it produced ~50MB of output for just a few thousand events!
  
  std::vector<ESDetId> detIds ;
  std::vector<bool>    detStatuses ;
  
  // Typical hit id is 906621265
  ESDetId esDetId = ESDetId(start) ;
  
  detIds.push_back(esDetId) ;
  detStatuses.push_back(false) ;
  
  bool done = false ;
  while(false==done){
    done = true ;
    for(unsigned int i=0 ; i<detStatuses.size() ; ++i){
      if(detStatuses.at(i)==false){
        // Update status, take detId, continue to loop
        detStatuses.at(i) = true ;
        esDetId = detIds.at(i) ;
        done = false ;
        
        // Make a navigator to move around
        EcalPreshowerNavigator theESNav(esDetId, topologyPreshower_) ;
        std::vector<ESDetId> neighbours ;
        
        theESNav.setHome(esDetId) ;
        neighbours.push_back(theESNav.west() ) ;
        theESNav.setHome(esDetId) ;
        neighbours.push_back(theESNav.east() ) ;
        theESNav.setHome(esDetId) ;
        neighbours.push_back(theESNav.north()) ;
        theESNav.setHome(esDetId) ;
        neighbours.push_back(theESNav.south()) ;
        
        for(unsigned int j=0 ; j<neighbours.size() ; ++j){
          if(neighbours.at(j)!=ESDetId(0)){
            bool addDetId = true ;
            for(unsigned k=0 ; k<detIds.size() ; ++k){
              if(neighbours.at(j)==detIds.at(k)){
                addDetId = false ;
                break ;
              }
            }
            if(addDetId){
              detIds.push_back(neighbours.at(j)) ;
              detStatuses.push_back(false) ;
            }
          }
        }
        break ;
      }
    }
  }
}

void IIHEModulePreshower::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModulePreshower::beginEvent(){}
void IIHEModulePreshower::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModulePreshower::endJob(){}

DEFINE_FWK_MODULE(IIHEModulePreshower);
