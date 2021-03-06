import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runOnMC = True
config.runOnEventWeight = False
config.runPATSequences = True
config.usePAT = True
config.runOnAOD = False
config.runOnRECO = True
config.globalTagNameData = 'GR_R_42_V23::All' 
#config.instLumiROOTFile='/storage2/eliza/lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
config.instLumiROOTFile=''
config.globalTagNameMC = 'START42_V17D::All'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'

if config.runOnMC:
    config.hltPaths =('HLT_Jet30*','HLT_Jet60*','HLT_Jet80*','HLT_Jet110*','HLT_Jet150*','HLT_Jet190*','HLT_Jet240_v*','HLT_Jet370_v*')
else:
    config.hltPaths = ('HLT_ExclDiJet60_HFAND_v*','HLT_ExclDiJet60_HFOR_v*','HLT_Jet60_v*')
    #config.hltPaths = ('HLT_ExclDiJet30U_HFAND_v*','HLT_ExclDiJet30U_HFOR_v*','HLT_Jet30U*')

#config.generator = 'Pythia6'

#config.outputEdmFile = 'DijetsAnalysis.root'
config.outputTTreeFile = 'forwardQCDTTreeAnalysis.root'

if config.runOnMC:
    #config.inputFileName = '/storage2/eliza/samples_test/QCD_Pt-15to30_TuneZ2_7TeV_pythia6AODSIMS_3.root'# MC
    config.inputFileName = '/storage1/dmf/PrivateMCProduction/July2012Prod/Pythia/CMSSW_4_2_8_lowpupatch1/src/step3_RAW2DIGI_L1Reco_RECO.root'# MC
else:
 if config.runOnAOD:
     #config.inputFileName = '/storage2/eliza/samples_test/MultiJetPromptReco_v4.root'#data 2011
     config.inputFileName = '/storage2/antoniov/data1/MultiJet_Run2010B_Apr21ReReco-v1_AOD/MultiJet_Run2010B_Apr21ReReco-v1_AOD_7EA7B611-7371-E011-B164-002354EF3BDB.root' 
 else:
  if config.runOnRECO:
      config.inputFileName = 'rfio:/castor/cern.ch/cms/store/data/Run2011A/Jet/RECO/PromptReco-v6/000/175/576/D08A7488-32D9-E011-B32F-BCAEC518FF8D.root'

#=======================
process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.debugModules = cms.untracked.vstring('forwardQCDTTreeAnalysis')
#process.MessageLogger.debugModules = cms.untracked.vstring('*')
process.MessageLogger.destinations = cms.untracked.vstring('cerr')
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
    #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

#-------------------------------------------------------------------------------
# Import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.options.wantSummary = True
#--------------------------------------------------------------------------------
if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

#---------------------------------------------------------------------------------
##Jet Correction Service

#process.ak5CaloL1Offset.useCondDB = False
#process.ak5PFL1Offset.useCondDB = False
#process.ak5JPTL1Offset.useCondDB = False

#---------------------------------------------------------------------------------
# Output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))

###################################################################################
# CASTOR RecHit Corrector
if not config.runOnMC:
    from ForwardAnalysis.Utilities.addCastorRecHitCorrector import addCastorRecHitCorrector
    addCastorRecHitCorrector(process)
####################################################################################
# Analysis modules
#--------------------------------
from ForwardAnalysis.Utilities.countsAnalyzer_cfi import countsAnalyzer
if not config.runOnMC:
    process.load('ForwardAnalysis.Utilities.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)


#if config.runOnMC:
#     process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = True
#else:
#     process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = False
 
#process.load('RecoJets.Configuration.RecoPFJets_cff')
#process.load('RecoJets.Configuration.RecoJets_cff')
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

process.load("ForwardAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsAnalysisSequences_cff")

if config.runOnMC:
    process.exclusiveDijetsHLTFilter.HLTPaths = config.hltPaths 
else:
    process.exclusiveDijetsHLTFilter.HLTPaths = config.hltPaths 

process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis
pfjecService    = 'ak5PFL1FastL2L3'
calojecService  = 'ak5CaloL1L2L3'
if not config.runOnMC:
    pfjecService    = 'ak5PFL1FastL2L3Residual'
    calojecService  = 'ak5CaloL1L2L3Residual'
process.forwardQCDTTreeAnalysis = cms.EDAnalyzer('ProcessedTreeProducer',
    eventInfo = cms.PSet(),
    diffractiveAnalysis = DiffractiveAnalysis,
    exclusiveDijetsAnalysis = ExclusiveDijetsAnalysis,
    ## jet collections ###########################
    pfjets          = cms.InputTag('ak5PFJets'),
    calojets        = cms.InputTag('ak5CaloJets'),
    ## database entry for the uncertainties ######
    PFPayloadName   = cms.string('AK5PF'),
    CaloPayloadName = cms.string('AK5Calo'),
    ## calojet ID and extender for the JTA #######
    calojetID       = cms.InputTag('ak5JetID'),
    calojetExtender = cms.InputTag('ak5JetExtender'),
    ## set the conditions for good Vtx counting ##
    offlineVertices = cms.InputTag('offlinePrimaryVertices'),
    goodVtxNdof     = cms.double(4), 
    goodVtxZ        = cms.double(24),
    ## rho #######################################
    srcCaloRho      = cms.InputTag('kt6CaloJets','rho'),
    srcPFRho        = cms.InputTag('kt6PFJets','rho'),
    ## preselection cuts #########################
    maxY            = cms.double(5.0), 
    minPFPt         = cms.double(20),
    minPFFatPt      = cms.double(10),
    maxPFFatEta     = cms.double(2.5),
    minCaloPt       = cms.double(20),
    minNPFJets      = cms.int32(1),
    minNCaloJets    = cms.int32(1), 
    minJJMass       = cms.double(-1),
    ## trigger ###################################
    printTriggerMenu = cms.untracked.bool(True),
    processName     = cms.string('HLT'),
    triggerName     = cms.vstring('HLT_Jet30_v1','HLT_Jet30_v2','HLT_Jet30_v3','HLT_Jet30_v4','HLT_Jet30_v5','HLT_Jet30_v6',
                                  'HLT_Jet60_v1','HLT_Jet60_v2','HLT_Jet60_v3','HLT_Jet60_v4','HLT_Jet60_v5','HLT_Jet60_v6',
                                  'HLT_Jet80_v1','HLT_Jet80_v2','HLT_Jet80_v3','HLT_Jet80_v4','HLT_Jet80_v5','HLT_Jet80_v6',
                                  'HLT_Jet110_v1','HLT_Jet110_v2','HLT_Jet110_v3','HLT_Jet110_v4','HLT_Jet110_v5','HLT_Jet110_v6',
                                  'HLT_Jet150_v1','HLT_Jet150_v2','HLT_Jet150_v3','HLT_Jet150_v4','HLT_Jet150_v5','HLT_Jet150_v6',
                                  'HLT_Jet190_v1','HLT_Jet190_v2','HLT_Jet190_v3','HLT_Jet190_v4','HLT_Jet190_v5','HLT_Jet190_v6',
                                  'HLT_Jet240_v1','HLT_Jet240_v2','HLT_Jet240_v3','HLT_Jet240_v4','HLT_Jet240_v5','HLT_Jet240_v6',
                                  'HLT_Jet300_v1', 'HLT_Jet300_v2','HLT_Jet300_v3','HLT_Jet300_v4','HLT_Jet300_v5',
                                  'HLT_Jet370_v1','HLT_Jet370_v2','HLT_Jet370_v3','HLT_Jet370_v4','HLT_Jet370_v5','HLT_Jet370_v6'),
    triggerResults  = cms.InputTag("TriggerResults","","HLT"),
    triggerEvent    = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    ## jec services ##############################
    pfjecService    = cms.string(pfjecService),
    calojecService  = cms.string(calojecService)
)

# CASTOR 
config.castorTagName = "castorRecHitCorrector"
if config.runOnMC: config.castorTagName = "castorreco"
# Diffractive analysis
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.triggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.hltPath = ''
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.comEnergy = config.comEnergy
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.trackTag = config.trackTagName
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.vertexTag = "goodOfflinePrimaryVertices"
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.jetTag = "selectedPatJetsPFlow"
process.forwardQCDTTreeAnalysis.diffractiveAnalysis.castorRecHitTag = config.castorTagName
if config.runOnMC:
    process.forwardQCDTTreeAnalysis.diffractiveAnalysis.accessMCInfo = True

# Exclusive dijets analysis
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.TriggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = config.hltPaths
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.EBeam = config.comEnergy/2.
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.TrackTag = config.trackTagName
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.VertexTag = "goodOfflinePrimaryVertices"
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.JetNonCorrTag = "ak5PFJets"
process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.UsePAT = config.usePAT
if config.runOnMC:
    process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = True
else:
    process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = False

print "DiffractiveAnalysis configuration:"
print process.forwardQCDTTreeAnalysis.diffractiveAnalysis
print "ExclusiveDijetsAnalysis configuration:"
print process.forwardQCDTTreeAnalysis.exclusiveDijetsAnalysis


############# Turn-on the fastjet area calculation needed for the L1Fastjet ##############
############# applied only to PFJets because if CaloJets are re-recoed the JetID map will be lost #####
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.kt6CaloJets.doRhoFastjet = True
process.kt6CaloJets.Rho_EtaMax = cms.double(5.0)
process.ak7PFJets.doAreaFastjet = True
process.ak7PFJets.Rho_EtaMax = cms.double(5.0)
process.ak7PFJets.jetPtMin = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.jetPtMin = cms.double(5.0)
process.reco_step = cms.Path(process.kt6PFJets * process.kt6CaloJets * process.ak5PFJets)

if not config.runOnMC: process.castor_step = cms.Path(process.castorSequence)

if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process,config.runOnMC)

    if config.runOnMC:
	process.patTrigger.addL1Algos = cms.bool( False )
	process.patJets.addTagInfos   = cms.bool( False )
    else:
	process.patTrigger.addL1Algos = cms.bool( True )
	process.patJets.addTagInfos   = cms.bool( True )   

if not config.runOnMC and config.runOnEventWeight:
    process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
    process.eventWeight_step = cms.Path(process.eventWeightSequence) 

if config.runOnMC:
   process.gen_step = cms.Path(process.genChargedParticles+
                               process.genProtonDissociative*process.edmNtupleMxGen+
                               process.genStableParticles*
                               process.etaMaxGen+process.etaMinGen*
                               process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)
process.analysis_reco_step = cms.Path(process.analysisSequences)

if not config.runOnMC:
    process.analysis_forwardQCDAnalysis_step = cms.Path( process.eventSelectionHLT +
                                                         process.forwardQCDTTreeAnalysis )
else:
    process.analysis_forwardQCDAnalysis_step = cms.Path( process.eventSelection + 
                                                         process.forwardQCDTTreeAnalysis )

