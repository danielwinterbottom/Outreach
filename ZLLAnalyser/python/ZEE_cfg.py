import FWCore.ParameterSet.Config as cms

process = cms.Process("MAIN")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("ZEE_output.root"),
    closeFileFast = cms.untracked.bool(True)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
                                'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/SingleElectron/MINIAOD/08Jun2016-v1/10000/0201576F-DE2E-E611-8663-7845C4FC38ED.root'
                                #'root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleMuon/MINIAOD/16Dec2015-v1/10000/00039A2E-D7A7-E511-98EE-3417EBE64696.root'
                )
                            )
process.GlobalTag.globaltag = cms.string('76X_dataRun2_16Dec2015_v0')

process.zEEAnalyser = cms.EDAnalyzer('ZEEAnalyser',
   elecs    = cms.InputTag('slimmedElectrons')
                              )

process.p = cms.Path(process.zEEAnalyser)
