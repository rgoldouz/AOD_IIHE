# This crabConfig file lists the data and MC samples that we are interested in.  You can
# make a semi-automated script that submits multiple crab tasks by adding a field
# that reads something like: "###JOB###" and then using sed to replace this with the
# name of the sample you like.
# Don't forget to update the date when you submit, and to update the pset (IIHE.py)
# depending on whether you are using MC or data to get the correct global tag.

date = '20151201_'

job_to_submit = 'RunIISpring15DR74_ZToEE_50_120_25ns'
###JOB###

class job_details:
    def __init__(self, name, inputDataset):
        self.name = name
        self.inputDataset = inputDataset
        self.publishDataName = self.inputDataset.replace('/','__')
        self.requestName = '%s_%s'%(date,self.name)

def add_job(name, DAS_string):
    jobs[name] = job_details(name, DAS_string)

jobs = {}

# 2015 data.
add_job('EGamma__Run2015A-PromptReco-v1', '/EGamma/Run2015A-PromptReco-v1/AOD')

add_job('DoubleEG__Run2015A-PromptReco-v1', '/DoubleEG/Run2015A-PromptReco-v1/AOD')
add_job('DoubleEG__Run2015B-PromptReco-v1', '/DoubleEG/Run2015B-PromptReco-v1/AOD')
add_job('DoubleEG__Run2015C-PromptReco-v1', '/DoubleEG/Run2015C-PromptReco-v1/AOD')
add_job('DoubleEG__Run2015D-PromptReco-v3', '/DoubleEG/Run2015D-PromptReco-v3/AOD')
add_job('DoubleEG__Run2015D-PromptReco-v4', '/DoubleEG/Run2015D-PromptReco-v4/AOD')

add_job('SingleMu__Run2015A-PromptReco-v1', '/SingleMu/Run2015A-PromptReco-v1/AOD')
add_job('SingleMu__Run2015B-PromptReco-v1', '/SingleMu/Run2015B-PromptReco-v1/AOD')

add_job('MuonEG__Run2015B-PromptReco-v1', '/MuonEG/Run2015B-PromptReco-v1/AOD')
add_job('MuonEG__Run2015C-PromptReco-v1', '/MuonEG/Run2015C-PromptReco-v1/AOD')
add_job('MuonEG__Run2015D-PromptReco-v3', '/MuonEG/Run2015D-PromptReco-v3/AOD')
add_job('MuonEG__Run2015D-PromptReco-v4', '/MuonEG/Run2015D-PromptReco-v4/AOD')

add_job('SingleElectron__Run2015A'   , '/SingleElectron/Run2015A-PromptReco-v1/AOD')
add_job('SingleElectron__Run2015B'   , '/SingleElectron/Run2015B-PromptReco-v1/AOD')
add_job('SingleElectron__Run2015C'   , '/SingleElectron/Run2015C-PromptReco-v1/AOD')
add_job('SingleElectron__Run2015D_v3', '/SingleElectron/Run2015D-PromptReco-v3/AOD')
add_job('SingleElectron__Run2015D_v4', '/SingleElectron/Run2015D-PromptReco-v4/AOD')

add_job('SingleElectron_0T__Run2015C_v2', '/SingleElectron_0T/Run2015C-PromptReco-v2/AOD')
add_job('SingleElectron_0T__Run2015C_v3', '/SingleElectron_0T/Run2015C-PromptReco-v3/AOD')
add_job('SingleElectron_0T__Run2015D_v3', '/SingleElectron_0T/Run2015D-PromptReco-v3/AOD')
add_job('SingleElectron_0T__Run2015D_v4', '/SingleElectron_0T/Run2015D-PromptReco-v4/AOD')

# CMSSW_7_4_X
add_job('PI_M2300', '/GammaGammaToEE_MassMin2300/lathomas-EXOMCRECO_RunIISpring15DR74_Asympt25ns_MCRUN2_74_V9-v1-v1-0b969f84d38afee522ac0dc4540e4506/USER')

# DY (incomplete)
add_job('RunIISpring15DR74_ZToEE_50_120_25ns'   , '/ZToEE_NNPDF30_13TeV-powheg_M_50_120/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZToEE_120_200_25ns'  , '/ZToEE_NNPDF30_13TeV-powheg_M_120_200/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM')
add_job('RunIISpring15DR74_ZToEE_200_400_25ns'  , '/ZToEE_NNPDF30_13TeV-powheg_M_200_400/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZToEE_400_800_25ns'  , '/ZToEE_NNPDF30_13TeV-powheg_M_400_800/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZToEE_800_1400_25ns' , '/ZToEE_NNPDF30_13TeV-powheg_M_800_1400/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZToEE_1400_2300_25ns', '/ZToEE_NNPDF30_13TeV-powheg_M_1400_2300/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZToEE_2300_3500_25ns', '/ZToEE_NNPDF30_13TeV-powheg_M_2300_3500/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZToEE_3500_4500_25ns', '/ZToEE_NNPDF30_13TeV-powheg_M_3500_4500/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZToEE_4500_6000_25ns', '/ZToEE_NNPDF30_13TeV-powheg_M_4500_6000/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZToEE_6000_25ns'     , '/ZToEE_NNPDF30_13TeV-powheg_M_6000_Inf/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')

# Z for tau tau
add_job('RunIISpring15DR74_DYJetsToLL_M50_50ns', '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/AODSIM')
add_job('RunIISpring15DR74_DYJetsToLL_M50_25ns', '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/AODSIM')

# Diboson
# 50 ns
add_job('RunIISpring15DR74_WW_50ns_v1'       , '/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/AODSIM')
add_job('RunIISpring15DR74_WZ_50ns_v2'       , '/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/AODSIM')
add_job('RunIISpring15DR74_ZZ_50ns_v2'       , '/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/AODSIM')

# 25 ns
add_job('RunIISpring15DR74_WW_25ns_v1'       , '/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_WZ_25ns_v1'       , '/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ZZ_25ns_v3'       , '/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/AODSIM')

# W+jets
add_job('RunIISpring15DR74_WJetsToLNu_25ns_v1'           , '/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_WJetsToLNu_50ns_v1'           , '/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/AODSIM')
add_job('RunIISpring15DR74_WJetsToLNu_HT_100_200_25ns_v1', '/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_WJetsToLNu_HT_200_400_25ns_v1', '/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_WJetsToLNu_HT_400_600_25ns_v3', '/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/AODSIM')
add_job('RunIISpring15DR74_WJetsToLNu_HT_600_25ns_v1'    , '/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')

# ttbar
add_job('RunIISpring15DR74_powheg_TTJets_50ns_v4', '/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v4/AODSIM')
add_job('RunIISpring15DR74_powheg_TTJets_25ns_v4', '/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM')

# Gamma plus jets
add_job('RunIISpring15DR74_GJets_HT40To100' , '/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM ')
add_job('RunIISpring15DR74_GJets_HT100To200', '/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/AODSIM ')
add_job('RunIISpring15DR74_GJets_HT200To400', '/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_GJets_HT400To600', '/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_GJets_HT600'     , '/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')

# Single top
# 50 ns
add_job('RunIISpring15DR74_ST_s_4f_leptonic_50ns_v1'          , '/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/AODSIM')
add_job('RunIISpring15DR74_ST_t_antitop_4f_leptonic_50ns_v1'  , '/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/AODSIM')
add_job('RunIISpring15DR74_ST_t_top_4f_leptonic_50ns_v1'      , '/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/AODSIM')
add_job('RunIISpring15DR74_ST_tW_antitop_5f_inclusive_50ns_v2', '/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/AODSIM')
add_job('RunIISpring15DR74_ST_tW_top_5f_inclusive_50ns_v1'    , '/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/AODSIM')

# 25 ns
add_job('RunIISpring15DR74_ST_s_4f_leptonic_25ns_v1'             , '/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ST_t_4f_leptonic_25ns_v1'             , '/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ST_t_5f_leptonic_25ns_v1'             , '/ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ST_t_antitop_4f_leptonic_25ns_v1'     , '/ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ST_t_top_4f_leptonic_25ns_v1'         , '/ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ST_tW_antitop_5f_inclusive_25ns_v1'   , '/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ST_tW_top_5f_inclusive_25ns_v1'       , '/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ST_tW_antitop_5f_DS_inclusive_25ns_v1', '/ST_tW_antitop_5f_DS_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')
add_job('RunIISpring15DR74_ST_tW_top_5f_DS_inclusive_25ns_v1'    , '/ST_tW_top_5f_DS_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM')

job = jobs[job_to_submit]

from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.requestName = '%s'%(job.requestName)
config.General.workArea = 'crab_projects'

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
if pick_events:
    config.JobType.psetName = '/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw-patch/CMSSW_7_4_4_patch2/src/PhysicsTools/Utilities/configuration/copyPickMerge_cfg.py'
    config.JobType.outputFiles = ['pickevents.root']
    config.JobType.pyCfgParams = ['eventsToProcess_load=pickevents_runEvents.txt','outputFile=pickevents.root']
else:
    config.JobType.psetName = 'IIHE.py'
    config.JobType.outputFiles = ['outfile.root']

config.section_('Data')
config.Data.unitsPerJob = 1

config.Data.inputDataset = job.inputDataset
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
if 'sharper' in job.name or 'PI' in job.name:
    config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader/'
config.Data.splitting = 'FileBased'
#config.Data.publication = True
#config.Data.publishDbsUrl = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
#config.Data.publishDataName = job.publishDataName
config.Data.ignoreLocality = True

config.section_('Site')
config.Site.storageSite = 'T2_BE_IIHE'
if 'sharper' in job.name:
    config.Site.whitelist = ['T2_UK_SGrid_RALPP']

config.section_('User')
config.User.voGroup = 'becms'
