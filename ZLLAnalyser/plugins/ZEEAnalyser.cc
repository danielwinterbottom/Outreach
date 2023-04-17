// system include files
#include <memory>
#include <vector>
#include "TTree.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class ZEEAnalyser : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit ZEEAnalyser(const edm::ParameterSet&);
      ~ZEEAnalyser();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::EDGetTokenT<std::vector<pat::Electron>> elecToken_;
      TTree* tree_;
      float mass_;
      float pt_1_;
      float pt_2_;
      float iso_1_;
      float iso_2_;
      float trackiso_1_;
      float trackiso_2_;
      bool os_;
      float HoverE_1_;
      float HoverE_2_;
      unsigned long long event_;
      unsigned lumi_; 
      unsigned run_; 

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
//ZEEAnalyser::ZEEAnalyser(const edm::ParameterSet& iConfig)
//
//{
//   //now do what ever initialization is needed
//   usesResource("TFileService");
#include "DataFormats/Math/interface/LorentzVector.h"
//
//}

ZEEAnalyser::ZEEAnalyser(const edm::ParameterSet& iConfig)
    : elecToken_(consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("elecs")))
{
  //usesResource("TFileService");
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("tree", "Z boson electron pairs");
  tree_->Branch("mass", &mass_, "mass/F");
  tree_->Branch("pt_1", &pt_1_, "pt_1/F");
  tree_->Branch("pt_2", &pt_2_, "pt_2/F");
  tree_->Branch("iso_1", &iso_1_, "iso_1/F");
  tree_->Branch("iso_2", &iso_2_, "iso_2/F");
  tree_->Branch("trackiso_1", &trackiso_1_, "trackiso_1/F");
  tree_->Branch("trackiso_2", &trackiso_2_, "trackiso_2/F");
  tree_->Branch("HoverE_1", &HoverE_1_, "HoverE_1/F");
  tree_->Branch("HoverE_2", &HoverE_2_, "HoverE_2/F");
  tree_->Branch("os", &os_);
  tree_->Branch("event", &event_);
  tree_->Branch("lumi", &lumi_);
  tree_->Branch("run", &run_);
}

ZEEAnalyser::~ZEEAnalyser()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------

void ZEEAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<std::vector<pat::Electron>> elecs_handle;
  iEvent.getByToken(elecToken_, elecs_handle);

  if (elecs_handle->size()>1) {

    event_=iEvent.id().event();
    lumi_=iEvent.luminosityBlock();
    run_=iEvent.run();

    const pat::Electron elec1 = elecs_handle->at(0);
    const pat::Electron elec2 = elecs_handle->at(1);
    const auto p4 = elec1.p4() + elec2.p4();
    mass_ = p4.M();
    pt_1_=elec1.pt();
    pt_2_=elec2.pt();

    trackiso_1_= elec1.trackIso();
    trackiso_2_= elec2.trackIso();
    trackiso_1_/=pt_1_;
    trackiso_2_/=pt_2_;

    iso_1_ = elec1.pfIsolationVariables().sumChargedHadronPt + std::max(0.,elec1.pfIsolationVariables().sumNeutralHadronEt+elec1.pfIsolationVariables().sumPhotonEt-0.5*elec1.pfIsolationVariables().sumPUPt);
    iso_2_ = elec2.pfIsolationVariables().sumChargedHadronPt + std::max(0.,elec2.pfIsolationVariables().sumNeutralHadronEt+elec2.pfIsolationVariables().sumPhotonEt-0.5*elec2.pfIsolationVariables().sumPUPt);
    iso_1_/=pt_1_;
    iso_2_/=pt_2_;

    HoverE_1_ =elec1.hadronicOverEm();
    HoverE_2_ =elec2.hadronicOverEm();
   
    os_ = elec1.charge() * elec2.charge() < 0;

    if (/*elec1.charge() * elec2.charge() < 0 && pt_1_ > 10. && pt_2_ > 10. && */ std::fabs(elec1.eta())<2.5 && std::fabs(elec2.eta())<2.5) {
      tree_->Fill();
    }    
  }

}

// ------------ method called once each job just before starting event loop  ------------
void 
ZEEAnalyser::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZEEAnalyser::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ZEEAnalyser::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ZEEAnalyser);
