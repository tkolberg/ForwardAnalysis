#ifndef ForwardAnalysisUtilities_CaloActivityFilter_h
#define ForwardAnalysisUtilities_CaloActivityFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

namespace forwardAnalysis {

class DefaultCaloTowerSelector {
   public:
      DefaultCaloTowerSelector(edm::ParameterSet const& pset) {}
      ~DefaultCaloTowerSelector() {}

      bool operator() (CaloTower const& calotower, edm::Event const& event){ return true; }
};

template <class Selector=DefaultCaloTowerSelector>
class CaloActivityFilter : public edm::EDFilter {
    public:
       explicit CaloActivityFilter( edm::ParameterSet const& );
       ~CaloActivityFilter();

       virtual void beginJob();
       virtual bool filter( edm::Event&, edm::EventSetup const& );
   private:
       Selector selector_;

       edm::InputTag caloTowerTag_; 

       double energyThresholdHB_;
       double energyThresholdHE_;
       double energyThresholdHF_;
       double energyThresholdEB_;
       double energyThresholdEE_;

       int nHBTowersMaxPlus_,nHBTowersMaxMinus_;
       int nHETowersMaxPlus_,nHETowersMaxMinus_;
       int nHFTowersMaxPlus_,nHFTowersMaxMinus_; 
       int nEBTowersMaxPlus_,nEBTowersMaxMinus_;
       int nEETowersMaxPlus_,nEETowersMaxMinus_;

       double sumEHBMaxPlus_,sumEHBMaxMinus_; 
       double sumEHEMaxPlus_,sumEHEMaxMinus_; 
       double sumEHFMaxPlus_,sumEHFMaxMinus_;
       double sumEEBMaxPlus_,sumEEBMaxMinus_;
       double sumEEEMaxPlus_,sumEEEMaxMinus_;

       double sumETHBMaxPlus_,sumETHBMaxMinus_;
       double sumETHEMaxPlus_,sumETHEMaxMinus_;
       double sumETHFMaxPlus_,sumETHFMaxMinus_; 
       double sumETEBMaxPlus_,sumETEBMaxMinus_;
       double sumETEEMaxPlus_,sumETEEMaxMinus_;
};

// Implementation
template <class Selector>
CaloActivityFilter<Selector>::CaloActivityFilter(edm::ParameterSet const& pset):
  selector_(pset),
  caloTowerTag_( pset.getParameter<edm::InputTag>("CaloTowerTag") ), 
  energyThresholdHB_( pset.getParameter<double>("EnergyThresholdHB") ),
  energyThresholdHE_( pset.getParameter<double>("EnergyThresholdHE") ),
  energyThresholdHF_( pset.getParameter<double>("EnergyThresholdHF") ),
  energyThresholdEB_( pset.getParameter<double>("EnergyThresholdEB") ),
  energyThresholdEE_( pset.getParameter<double>("EnergyThresholdEE") ),
  nHBTowersMaxPlus_( pset.getParameter<int>("NTowersMaxHBPlus") ),
  nHBTowersMaxMinus_( pset.getParameter<int>("NTowersMaxHBMinus") ),
  nHETowersMaxPlus_( pset.getParameter<int>("NTowersMaxHEPlus") ), 
  nHETowersMaxMinus_( pset.getParameter<int>("NTowersMaxHEMinus") ),
  nHFTowersMaxPlus_( pset.getParameter<int>("NTowersMaxHFPlus") ), 
  nHFTowersMaxMinus_( pset.getParameter<int>("NTowersMaxHFMinus") ),
  nEBTowersMaxPlus_( pset.getParameter<int>("NTowersMaxEBPlus") ),
  nEBTowersMaxMinus_( pset.getParameter<int>("NTowersMaxEBMinus") ),
  nEETowersMaxPlus_( pset.getParameter<int>("NTowersMaxEEPlus") ),
  nEETowersMaxMinus_( pset.getParameter<int>("NTowersMaxEEMinus") ),
  sumEHBMaxPlus_( pset.getParameter<double>("SumEMaxHBPlus") ),
  sumEHBMaxMinus_( pset.getParameter<double>("SumEMaxHBMinus") ),
  sumEHEMaxPlus_( pset.getParameter<double>("SumEMaxHEPlus") ), 
  sumEHEMaxMinus_( pset.getParameter<double>("SumEMaxHEMinus") ),
  sumEHFMaxPlus_( pset.getParameter<double>("SumEMaxHFPlus") ), 
  sumEHFMaxMinus_( pset.getParameter<double>("SumEMaxHFMinus") ),
  sumEEBMaxPlus_( pset.getParameter<double>("SumEMaxEBPlus") ),
  sumEEBMaxMinus_( pset.getParameter<double>("SumEMaxEBMinus") ),
  sumEEEMaxPlus_( pset.getParameter<double>("SumEMaxEEPlus") ),
  sumEEEMaxMinus_( pset.getParameter<double>("SumEMaxEEMinus") ),
  sumETHBMaxPlus_( pset.getParameter<double>("SumETMaxHBPlus") ),
  sumETHBMaxMinus_( pset.getParameter<double>("SumETMaxHBMinus") ),
  sumETHEMaxPlus_( pset.getParameter<double>("SumETMaxHEPlus") ),
  sumETHEMaxMinus_( pset.getParameter<double>("SumETMaxHEMinus") ),
  sumETHFMaxPlus_( pset.getParameter<double>("SumETMaxHFPlus") ),
  sumETHFMaxMinus_( pset.getParameter<double>("SumETMaxHFMinus") ),
  sumETEBMaxPlus_( pset.getParameter<double>("SumETMaxEBPlus") ),
  sumETEBMaxMinus_( pset.getParameter<double>("SumETMaxEBMinus") ),
  sumETEEMaxPlus_( pset.getParameter<double>("SumETMaxEEPlus") ),
  sumETEEMaxMinus_( pset.getParameter<double>("SumETMaxEEMinus") ) {} 

template <class Selector>
CaloActivityFilter<Selector>::~CaloActivityFilter() {}

template <class Selector>
void CaloActivityFilter<Selector>::beginJob() {}

template <class Selector>
bool CaloActivityFilter<Selector>::filter(edm::Event& event, edm::EventSetup const& setup){

  // Calo tower collection from event
  edm::Handle<CaloTowerCollection> towerCollectionH;
  event.getByLabel(caloTowerTag_,towerCollectionH);
  const CaloTowerCollection& towerCollection = *towerCollectionH;

  int nTowersHF_plus = 0;
  int nTowersHF_minus = 0;
  int nTowersHE_plus = 0;
  int nTowersHE_minus = 0;
  int nTowersHB_plus = 0;
  int nTowersHB_minus = 0;
  int nTowersEE_plus = 0;
  int nTowersEE_minus = 0;
  int nTowersEB_plus = 0;
  int nTowersEB_minus = 0; 
  //Sum(E)
  double sumEHF_plus = 0.;
  double sumEHF_minus = 0.;
  double sumEHE_plus = 0.;
  double sumEHE_minus = 0.;
  double sumEHB_plus = 0.;
  double sumEHB_minus = 0.;
  double sumEEE_plus = 0.;
  double sumEEE_minus = 0.;
  double sumEEB_plus = 0.;
  double sumEEB_minus = 0.;
  // Sum(ET)
  double sumETHF_plus = 0.;
  double sumETHF_minus = 0.;
  double sumETHE_plus = 0.;
  double sumETHE_minus = 0.;
  double sumETHB_plus = 0.;
  double sumETHB_minus = 0.;
  double sumETEE_plus = 0.;
  double sumETEE_minus = 0.;
  double sumETEB_plus = 0.;
  double sumETEB_minus = 0.;

  // Loop over calo towers
  CaloTowerCollection::const_iterator calotower = towerCollection.begin();
  CaloTowerCollection::const_iterator calotowers_end = towerCollection.end();
  for(; calotower != calotowers_end; ++calotower) {

     // Accept tower following policy
     if( !selector_(*calotower,event) ) continue;    

     bool hasHCAL = false;
     bool hasHF = false;
     bool hasHE = false;
     bool hasHB = false;
     bool hasHO = false;
     bool hasECAL = false;
     bool hasEE = false;
     bool hasEB = false;     
     for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
        DetId detId = calotower->constituent(iconst);
        if(detId.det()==DetId::Hcal){
           hasHCAL = true;
           HcalDetId hcalDetId(detId);
           if(hcalDetId.subdet()==HcalForward) hasHF = true;
           else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
           else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
           else if(hcalDetId.subdet()==HcalOuter) hasHO = true;  
        } else if(detId.det()==DetId::Ecal){
           hasECAL = true;
           EcalSubdetector ecalSubDet = (EcalSubdetector)detId.subdetId();
           if(ecalSubDet == EcalEndcap) hasEE = true;
           else if(ecalSubDet == EcalBarrel) hasEB = true;
        }
     }

     int zside = calotower->zside();
     double caloTowerEnergy = calotower->energy();
     double caloTowerEmEnergy = calotower->emEnergy();
     double caloTowerHadEnergy = calotower->hadEnergy();

     // FIXME
     //double caloTowerET = calotower->et(primVtx.position());
     //double caloTowerET = calotower->et(primVtx.z());
     double caloTowerEt = calotower->et();
     double caloTowerEmEt = calotower->emEt();
     double caloTowerHadEt = calotower->hadEt();

     // HCAL: Towers made of at least one component from HB,HE,HF
     if( hasHF && !hasHE ){
        if( caloTowerEnergy >= energyThresholdHF_ ){
           if(zside >= 0){
              ++nTowersHF_plus;
              sumEHF_plus += caloTowerEnergy; 
              sumETHF_plus += caloTowerEt;
           } else{
              ++nTowersHF_minus;
              sumEHF_minus += caloTowerEnergy;
              sumETHF_minus += caloTowerEt;
           } 
        }
     } else if( hasHE && !hasHF && !hasHB ){
        if( caloTowerEnergy >= energyThresholdHE_ ){
           if(zside >= 0){
              ++nTowersHE_plus;
              sumEHE_plus += caloTowerHadEnergy;
              sumETHE_plus += caloTowerHadEt;
           } else{
              ++nTowersHE_minus;
              sumEHE_minus += caloTowerHadEnergy;
              sumETHE_minus += caloTowerHadEt;
           }
        }
     } else if( hasHB && !hasHE ){
        if( caloTowerEnergy >= energyThresholdHB_ ){
           if(zside >= 0){
              ++nTowersHB_plus;
              sumEHB_plus += caloTowerHadEnergy;
              sumETHB_plus += caloTowerHadEt;
           } else{
              ++nTowersHB_minus;
              sumEHB_minus += caloTowerHadEnergy;
              sumETHB_minus += caloTowerHadEt;
           }
        }
     }

     // ECAL: Towers made of at least one component from EB,EE
     if( hasEE && !hasEB ){
        if( caloTowerEnergy >= energyThresholdEE_ ){
           if(zside >= 0){
              ++nTowersEE_plus;
              sumEEE_plus += caloTowerEmEnergy;
              sumETEE_plus += caloTowerEmEt;
           } else{
              ++nTowersEE_minus;
              sumEEE_minus += caloTowerEmEnergy;
              sumETEE_minus += caloTowerEmEt;
           }
        }
     } else if( hasEB && !hasEE ){
        if( caloTowerEnergy >= energyThresholdEB_ ){
           if(zside >= 0){
              ++nTowersEB_plus;
              sumEEB_plus += caloTowerEmEnergy;
              sumETEB_plus += caloTowerEmEt;
           } else{
              ++nTowersEB_minus;
              sumEEB_minus += caloTowerEmEnergy;
              sumETEB_minus += caloTowerEmEt;
           }
        }
     }
  }

  bool accept = true;
  if( (nHBTowersMaxPlus_ >= 0 && nTowersHB_plus > nHBTowersMaxPlus_) ||
      (nHBTowersMaxMinus_ >= 0 && nTowersHB_minus > nHBTowersMaxMinus_) ||
      (nHETowersMaxPlus_ >= 0 && nTowersHE_plus > nHETowersMaxPlus_) ||
      (nHETowersMaxMinus_ >= 0 && nTowersHE_minus > nHETowersMaxMinus_) ||
      (nHFTowersMaxPlus_ >= 0 && nTowersHF_plus > nHFTowersMaxPlus_) ||
      (nHFTowersMaxMinus_ >= 0 && nTowersHF_minus > nHFTowersMaxMinus_) ||
      (nEBTowersMaxPlus_ >= 0 && nTowersEB_plus > nEBTowersMaxPlus_) ||
      (nEBTowersMaxMinus_ >= 0 && nTowersEB_minus > nEBTowersMaxMinus_) ||
      (nEETowersMaxPlus_ >= 0 && nTowersEE_plus > nEETowersMaxPlus_) ||
      (nEETowersMaxMinus_ >= 0 && nTowersEE_minus > nEETowersMaxMinus_) ||
      (sumEHBMaxPlus_ >= 0. && sumEHB_plus > sumEHBMaxPlus_) ||
      (sumEHBMaxMinus_ >= 0. && sumEHB_minus > sumEHBMaxMinus_) ||
      (sumEHEMaxPlus_ >= 0. && sumEHE_plus > sumEHEMaxPlus_) ||
      (sumEHEMaxMinus_ >= 0. && sumEHE_minus > sumEHEMaxMinus_) ||
      (sumEHFMaxPlus_ >= 0. && sumEHF_plus > sumEHFMaxPlus_) ||
      (sumEHFMaxMinus_ >= 0. && sumEHF_minus > sumEHFMaxMinus_) ||
      (sumEEBMaxPlus_ >= 0. && sumEEB_plus > sumEEBMaxPlus_) ||
      (sumEEBMaxMinus_ >= 0. && sumEEB_minus > sumEEBMaxMinus_) ||
      (sumEEEMaxPlus_ >= 0. && sumEEE_plus > sumEEEMaxPlus_) ||
      (sumEEEMaxMinus_ >= 0. && sumEEE_minus > sumEEEMaxMinus_) ||
      (sumETHBMaxPlus_ >= 0. && sumETHB_plus > sumETHBMaxPlus_) ||
      (sumETHBMaxMinus_ >= 0. && sumETHB_minus > sumETHBMaxMinus_) ||
      (sumETHEMaxPlus_ >= 0. && sumETHE_plus > sumETHEMaxPlus_) ||
      (sumETHEMaxMinus_ >= 0. && sumETHE_minus > sumETHEMaxMinus_) ||
      (sumETHFMaxPlus_ >= 0. && sumETHF_plus > sumETHFMaxPlus_) ||
      (sumETHFMaxMinus_ >= 0. && sumETHF_minus > sumETHFMaxMinus_) ||
      (sumETEBMaxPlus_ >= 0. && sumETEB_plus > sumETEBMaxPlus_) ||
      (sumETEBMaxMinus_ >= 0. && sumETEB_minus > sumETEBMaxMinus_) ||
      (sumETEEMaxPlus_ >= 0. && sumETEE_plus > sumETEEMaxPlus_) ||
      (sumETEEMaxMinus_ >= 0. && sumETEE_minus > sumETEEMaxMinus_) ) accept = false;

  return accept;
}

} // namespace

#endif
