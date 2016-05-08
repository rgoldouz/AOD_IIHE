from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

#General section: In this section, the user specifies generic parameters about the request (e.g. request name). 
#config.section_('General')
#config.General.requestName = 'WZ_TuneCUETP8M1_13TeV-pythia8'
#config.General.requestName = 'ZToEE_NNPDF30_13TeV-powheg_M_4500_6000'
#config.General.requestName ='DoubleEG_Run2015D-16Dec2015-v2_MINIAOD'
config.General.requestName ='GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8'

config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

#JobType section: This section aims to contain all the parameters of the user job type and related configurables (e.g. CMSSW parameter-set configuration file, additional input files, etc.). 
#config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'IIHE.py'
config.JobType.pyCfgParams = ['DataProcessing=mc', 'grid=True']
#config.JobType.outputFiles = ['outfile.root']

#Data section: This section contains all the parameters related to the data to be analyzed, including the splitting parameters. 
#config.section_('Data')
#config.Data.inputDataset = '/WZ_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'
config.Data.inputDataset = '/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v3/MINIAODSIM'
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

