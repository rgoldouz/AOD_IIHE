from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

#General section: In this section, the user specifies generic parameters about the request (e.g. request name). 
#config.section_('General')
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_50_120'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_120_200'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_200_400'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_400_800'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_800_1400'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_1400_2300'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_2300_3500'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_3500_4500'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_4500_6000'
#config.General.requestName ='ZToEE_NNPDF30_13TeV-powheg_M_6000_Inf'
#config.General.requestName ='DYToEE_NNPDF30_13TeV-powheg-pythia8'
#config.General.requestName ='DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_ext1'
#config.General.requestName ='DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8'
config.General.requestName ='DoubleElectron_FlatPt-300To6500'


config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

#JobType section: This section aims to contain all the parameters of the user job type and related configurables (e.g. CMSSW parameter-set configuration file, additional input files, etc.). 
#config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'IIHE.py'
#if it is not reHLT use
#config.JobType.pyCfgParams = ['DataProcessing=mc']
#if you are running on reHLT use
config.JobType.pyCfgParams = ['DataProcessing=mcreHLT']
#config.JobType.outputFiles = ['outfile.root']

#Data section: This section contains all the parameters related to the data to be analyzed, including the splitting parameters. 
#config.section_('Data')
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_50_120/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_120_200/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_200_400/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_400_800/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_800_1400/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_1400_2300/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_2300_3500/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_3500_4500/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_4500_6000/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_6000_Inf/RunIISpring16reHLT80-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM'
#config.Data.inputDataset = '/DYToEE_NNPDF30_13TeV-powheg-pythia8/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext1-v1/AODSIM'
#config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_50_120/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_120_200/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_200_400/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v2/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_400_800/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_800_1400/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_1400_2300/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_2300_3500/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_3500_4500/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_4500_6000/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
#config.Data.inputDataset = '/ZToEE_NNPDF30_13TeV-powheg_M_6000_Inf/RunIISpring16DR80-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'
config.Data.inputDataset = '/DoubleElectron_FlatPt-300To6500/RunIISpring16DR80-PUFlat0to50_80X_mcRun2_asymptotic_2016_v3-v1/AODSIM'

#config.Data.inputDataset = '/DoubleEG/Run2015D-16Dec2015-v2/MINIAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.ignoreLocality = True
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
#config.Data.runRange = '193093-193999' # '193093-194075'
#config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
#config.Data.publication = True
#config.Data.outputDatasetTag = 'CRAB3_tutorial_May2015_Data_analysis'

#Site section: Grid site parameters are defined in this section, including the stage out information (e.g. stage out destination site, white/black lists, etc.). 
config.section_("Site")
config.Site.storageSite = 'T2_BE_IIHE'

