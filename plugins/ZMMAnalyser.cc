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
#include "DataFormats/PatCandidates/interface/Muon.h"
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

class ZMMAnalyser : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit ZMMAnalyser(const edm::ParameterSet&);
      ~ZMMAnalyser();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::EDGetTokenT<std::vector<pat::Muon>> muonToken_;
      TTree* tree_;
      float mass_;
      float pt_1_;
      float pt_2_;
      bool isLoose_1_;
      bool isLoose_2_;
      bool isMedium_1_;
      bool isMedium_2_;
      float iso_1_;
      float iso_2_;
      bool os_;
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
//ZMMAnalyser::ZMMAnalyser(const edm::ParameterSet& iConfig)
//
//{
//   //now do what ever initialization is needed
//   usesResource("TFileService");
#include "DataFormats/Math/interface/LorentzVector.h"
//
//}

ZMMAnalyser::ZMMAnalyser(const edm::ParameterSet& iConfig)
    : muonToken_(consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muons")))
{
  //usesResource("TFileService");
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("tree", "Z boson muon pairs");
  tree_->Branch("mass", &mass_, "mass/F");
  tree_->Branch("pt_1", &pt_1_, "pt_1/F");
  tree_->Branch("pt_2", &pt_2_, "pt_2/F");
  tree_->Branch("isLoose_1", &isLoose_1_);
  tree_->Branch("isLoose_2", &isLoose_2_);
  tree_->Branch("isMedium_1", &isMedium_1_);
  tree_->Branch("isMedium_2", &isMedium_2_);
  tree_->Branch("iso_1", &iso_1_, "iso_1/F");
  tree_->Branch("iso_2", &iso_2_, "iso_2/F");
  tree_->Branch("os", &os_);
  tree_->Branch("event", &event_);
  tree_->Branch("lumi", &lumi_);
  tree_->Branch("run", &run_);
}

ZMMAnalyser::~ZMMAnalyser()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------

void ZMMAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
//  edm::Handle<edm::View<reco::Muon> > muons_handle;
  edm::Handle<std::vector<pat::Muon>> muons_handle;
  iEvent.getByToken(muonToken_, muons_handle);

  // Select two muons with opposite charges
  //std::cout << muons_handle->size() << std::endl;
  if (muons_handle->size()>1) {

    event_=iEvent.id().event();
    lumi_=iEvent.luminosityBlock();
    run_=iEvent.run();

    const pat::Muon muon1 = muons_handle->at(0);
    const pat::Muon muon2 = muons_handle->at(1);
    const auto p4 = muon1.p4() + muon2.p4();
    mass_ = p4.M();
    pt_1_=muon1.pt();
    pt_2_=muon2.pt();
    isLoose_1_ = muon1.isLooseMuon();
    isLoose_2_ = muon2.isLooseMuon();
    isMedium_1_ = muon1.isMediumMuon();
    isMedium_2_ = muon2.isMediumMuon();
    iso_1_ = muon1.pfIsolationR04().sumChargedHadronPt + std::max(0.,muon1.pfIsolationR04().sumNeutralHadronEt+muon1.pfIsolationR04().sumPhotonEt-0.5*muon1.pfIsolationR04().sumPUPt);
    iso_2_ = muon2.pfIsolationR04().sumChargedHadronPt + std::max(0.,muon2.pfIsolationR04().sumNeutralHadronEt+muon2.pfIsolationR04().sumPhotonEt-0.5*muon2.pfIsolationR04().sumPUPt);
    iso_1_/=pt_1_;
    iso_2_/=pt_2_;

    os_ = muon1.charge() * muon2.charge() < 0;

    if (/*muon1.charge() * muon2.charge() < 0 && pt_1_ > 10. && pt_2_ > 10. && */ std::fabs(muon1.eta())<2.4 && std::fabs(muon2.eta())<2.4) {
      tree_->Fill();
    }    
  }

}

// ------------ method called once each job just before starting event loop  ------------
void 
ZMMAnalyser::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZMMAnalyser::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ZMMAnalyser::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ZMMAnalyser);
