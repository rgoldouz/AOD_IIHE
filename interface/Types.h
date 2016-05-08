#ifndef UserCode_IIHETree_Types_h
#define UserCode_IIHETree_Types_h

enum variableTypes{
  kBool,
  kDouble,
  kFloat,
  kInt,
  kUInt,
  kVectorBool,
  kVectorDouble,
  kVectorFloat,
  kVectorInt,
  kVectorUInt,
  kVectorVectorBool,
  kVectorVectorDouble,
  kVectorVectorFloat,
  kVectorVectorInt,
  kVectorVectorUInt
};

// These are used in triggers
enum particleTypes{
  kSuperCluster,
  kPhoton,
  kElectron,
  kMuon,
  kTau,
  kJet,
  kBJet,
  kMET,
  kHT,
  kALCa
};

enum triggerLevels{
  kLevel1,
  kHighLevel
};

enum HEEPCutflows{
  kHC4, // HEEP cutflow 4.X
  kHC5, // HEEP cutflow 5.X
  kHC6, // HEEP cutflow 6.X
  kHC41,
  kHC50_50ns,
  kHC50_25ns,
  kHC50,
  kHC51,
  kHC60
};

enum DetectorRegion{
  kNone,
  kBarrel,
  kEndcap,
  kGap,     // Not yet used
  kForward  // Not yet used
};

#endif
