from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

#General section: In this section, the user specifies generic parameters about the request (e.g. request name). 
#config.section_('General')
#config.General.requestName = 'DoubleEG_Run2016B-23Sep2016-v3_AOD'
#config.General.requestName = 'DoubleEG_Run2016C-23Sep2016-v1_AOD'
config.General.requestName = 'DoubleEG_Run2016D-23Sep2016-v1_AOD'
#config.General.requestName = 'DoubleEG_Run2016E-23Sep2016-v1_AOD'
#config.General.requestName = 'DoubleEG_Run2016F-23Sep2016-v1_AOD'
#config.General.requestName = 'DoubleEG_Run2016G-23Sep2016-v1_AOD'
#config.General.requestName = 'DoubleEG_Run2016H-PromptReco-v2_AOD'
#config.General.requestName = 'DoubleEG_Run2016H-PromptReco-v3_AOD'
#config.General.requestName = 'DoubleEG_Run2016B-23Sep2016-v3_AOD_resubmit'
#config.General.requestName = 'DoubleEG_Run2016C-23Sep2016-v1_AOD_resubmit'
#config.General.requestName = 'DoubleEG_Run2016D-23Sep2016-v1_AOD_resubmit'
#config.General.requestName = 'DoubleEG_Run2016E-23Sep2016-v1_AOD_resubmit'
config.General.workArea = 'crab_projects_23Sep2016'
config.General.transferOutputs = True
config.General.transferLogs = False

#JobType section: This section aims to contain all the parameters of the user job type and related configurables (e.g. CMSSW parameter-set configuration file, additional input files, etc.). 
#config.section_('JobType')
config.JobType.pluginName = 'Analysis'

#If rereco RUNS B-G
config.JobType.psetName = 'IIHE.py'
#If rereco RUNS B-G
config.JobType.pyCfgParams = ['DataProcessing=rerecodata']
#If promptreco RUN H
#config.JobType.pyCfgParams = ['DataProcessing=promptdata']
#config.JobType.outputFiles = ['outfile.root']

#Data section: This section contains all the parameters related to the data to be analyzed, including the splitting parameters. 
#config.section_('Data')
#config.Data.inputDataset = '/DoubleEG/Run2016B-23Sep2016-v3/AOD'
#config.Data.inputDataset = '/DoubleEG/Run2016C-23Sep2016-v1/AOD'
config.Data.inputDataset = '/DoubleEG/Run2016D-23Sep2016-v1/AOD'
#config.Data.inputDataset = '/DoubleEG/Run2016E-23Sep2016-v1/AOD'
#config.Data.inputDataset = '/DoubleEG/Run2016F-23Sep2016-v1/AOD'
#config.Data.inputDataset = '/DoubleEG/Run2016G-23Sep2016-v1/AOD'
#config.Data.inputDataset = '/DoubleEG/Run2016H-PromptReco-v2/AOD'
#config.Data.inputDataset = '/DoubleEG/Run2016H-PromptReco-v3/AOD'




config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 15
config.Data.ignoreLocality = True
#config.Site.whitelist = ['']
config.Data.lumiMask = 'Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
#config.Data.lumiMask = 'crab_projects_23Sep2016/crab_DoubleEG_Run2016B-23Sep2016-v3_AOD/results/notFinishedLumis.json'
#config.Data.lumiMask = 'crab_projects_23Sep2016/crab_DoubleEG_Run2016C-23Sep2016-v1_AOD/results/notFinishedLumis.json'
#config.Data.lumiMask = 'crab_projects_23Sep2016/crab_DoubleEG_Run2016D-23Sep2016-v1_AOD/results/notFinishedLumis.json'
#config.Data.lumiMask = 'crab_projects_23Sep2016/crab_DoubleEG_Run2016E-23Sep2016-v1_AOD/results/notFinishedLumis.json'
#config.Data.runRange = '273150-284068'  #  this is runF and G in 9 of Sep
#config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
#config.Data.publication = True
#config.Data.outputDatasetTag = 'CRAB3_tutorial_May2015_Data_analysis'

#Site section: Grid site parameters are defined in this section, including the stage out information (e.g. stage out destination site, white/black lists, etc.). 
config.section_("Site")
config.Site.storageSite = 'T2_BE_IIHE'

