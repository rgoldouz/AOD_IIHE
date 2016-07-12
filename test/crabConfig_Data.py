from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

#General section: In this section, the user specifies generic parameters about the request (e.g. request name). 
#config.section_('General')
#config.General.requestName = 'DoubleEG_Run2016B-PromptReco-v2_AOD'
config.General.requestName ='SingleElectron_Run2016B-PromptReco-v2_AOD'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

#JobType section: This section aims to contain all the parameters of the user job type and related configurables (e.g. CMSSW parameter-set configuration file, additional input files, etc.). 
#config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'IIHE.py'
#config.JobType.outputFiles = ['outfile.root']

#Data section: This section contains all the parameters related to the data to be analyzed, including the splitting parameters. 
#config.section_('Data')
#config.Data.inputDataset = '/DoubleEG/Run2016B-PromptReco-v2/AOD'
config.Data.inputDataset = '/SingleElectron/Run2016B-PromptReco-v2/AOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
config.Data.ignoreLocality = True
#config.Data.lumiMask = 'Json_totalRuns_160626_2.txt'
config.Data.runRange = '271184-276384' # '193093-194075'
#config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
#config.Data.publication = True
#config.Data.outputDatasetTag = 'CRAB3_tutorial_May2015_Data_analysis'

#Site section: Grid site parameters are defined in this section, including the stage out information (e.g. stage out destination site, white/black lists, etc.). 
config.section_("Site")
config.Site.storageSite = 'T2_BE_IIHE'

