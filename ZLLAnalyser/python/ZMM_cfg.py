import FWCore.ParameterSet.Config as cms

process = cms.Process("MAIN")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("ZMM_output.root"),
    closeFileFast = cms.untracked.bool(True)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
                                #'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/SingleMuon/MINIAOD/16Dec2015-v1/10000/00006301-CAA8-E511-AD39-549F35AD8BC9.root'
                                'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleMuon/MINIAOD/16Dec2015-v1/10000/00039A2E-D7A7-E511-98EE-3417EBE64696.root'
                )
                            )
process.GlobalTag.globaltag = cms.string('76X_dataRun2_16Dec2015_v0')

process.zMMAnalyser = cms.EDAnalyzer('ZMMAnalyser',
   muons    = cms.InputTag('slimmedMuons')
                              )

process.p = cms.Path(process.zMMAnalyser)
