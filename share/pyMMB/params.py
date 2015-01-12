from ctypes import *
from pyMMB import *

class ParameterReader_wrapper(Structure):
    _fields_ = [ 
            # ('mmbID', c_int),
            #('// private:', : unknown ctype),
            #('// ParameterReader(const ParameterReader &);', : unknown ctype),
            #('&)', //// ParameterReader & operator = (const ParameterReader: unknown ctype),
            #('// ParameterReader & operator = (const ParameterReader &);', : unknown ctype),
            #('_errorManager', //ErrorManager &: unknown ctype),
            #('// public:', : unknown ctype),
            #('// ParameterReader();', : unknown ctype),
            #('ParameterReader()', ////: unknown ctype),
            #('additionalCovalentBondVector', //vector<CovalentBondClass>: unknown ctype),
            #('basePairContainer', //BasePairContainer: unknown ctype),
            #('basePairPartners', //map<const ChainResidueIndex, BasePairPartner,twoIndexCmp>: unknown ctype),
            #('// // variables previously declared and initialized in Repel.h:', : unknown ctype),
            ('addAllAtomSterics', c_bool),
            ('addAllHeavyAtomSterics', c_bool),
            ('addBackboneOxygenForces', c_bool),
            ('addProteinBackboneSterics', c_bool),
            ('addRNABackboneSterics', c_bool),
            ('addSelectedAtoms', c_bool),
            ('addTestSpring', c_bool),
            ('applyC1pSprings', c_bool),
            ('calcBaseBodyFramesAtEveryTimeStep', c_int),
            ('calcEnergy', c_bool),
            ('totalEnergy', c_double),
            ('potentialEnergy', c_double),
            ('kineticEnergy', c_double),
            ('checkSatisfied', c_bool),
            ('constrainRigidSegments', c_bool),
            ('constraintTolerance', c_double),
            ('guessCoordinates', c_bool),
            ('cutoffRadius', c_double),
            ('cutoffAngle', c_double),
            ('densityAtomFraction', c_double),
            ('densityFileName', c_char_p),
            ('densityForceConstant', c_double),
            #('densityMapActivate', ////bool: unknown ctype),
            ('excludedVolumeStiffness', c_double),
            #('firstResidueMobilizerType', ////String: unknown ctype),
            ('firstStage', c_int),
            ('fitDefaultTolerance', c_double),
            ('globalAmberImproperTorsionScaleFactor', c_double),
            ('globalBondBendScaleFactor', c_double),
            ('globalBondStretchScaleFactor', c_double),
            ('globalBondTorsionScaleFactor', c_double),
            ('globalCoulombScaleFactor', c_double),
            ('globalGbsaScaleFactor', c_double),
            ('globalVdwScaleFactor', c_double),
            ('hardSphereStiffnessMultiplier', c_double),
            ('inQVectorFileName', c_char_p),
            ('initialSeparation', c_double),
            ('integratorAccuracy', c_double),
            ('integratorStepSize', c_double),
            ('integratorType', c_char_p),
            ('kbBackboneTorsionGlobalScaleFactor', c_double),
            ('lastStage', c_int),
            ('leontisWesthofInFileName', c_char_p),
            ('loadTinkerParameterFile', c_bool),
            ('outQVectorFileName', c_char_p),
            ('magnesiumIonChainId', c_char_p),
            ('magnesiumIonRadius', c_double),
            ('matchDefaultSkipTopLevelTransform', c_int),
            ('matchHydrogenAtomLocations', c_bool),
            ('matchPurineN1AtomLocations', c_bool),
            ('matchProteinCarboxylOxygenLocations', c_bool),
            ('matchExact', c_bool),
            ('matchIdealized', c_bool),
            ('matchOptimize', c_bool),
            ('matchingMinimizerTolerance', c_double),
            ('numReportingIntervals', c_int),
            ('minimize', c_int),
            ('monteCarloRun', c_int),
            ('monteCarloTemperature', c_double),
            ('monteCarloTemperatureIncrement', c_double),
            ('nastGlobalBondTorsionScaleFactor', c_double),
            ('noseHooverTime', c_double),
            ('numMagnesiumIons', c_int),
            ('outMonteCarloFileName', c_char_p),
            ('outTrajectoryFileName', c_char_p),
            #('physicsWhereYouWantIt', ////bool: unknown ctype),
            ('physicsRadius', c_double),
            ('piecewiseRigidify', c_bool),
            #('// double planarityThreshold; // threshold for considering a bond center to be planar, in rads.', : unknown ctype),
            ('potentialType', c_char_p),
            #('prioritize', ////int: unknown ctype),
            ('proteinCapping', c_bool),
            ('excludedVolumeRadius', c_double),
            ('readInQVector', c_int),
            ('readPreviousFrameFile', c_bool),
            ('readMagnesiumPositionsFromFile', c_int),
            ('removeRigidBodyMomentum', c_bool),
            ('removeMomentumPeriod', c_double),
            ('reportingInterval', c_double),
            ('restrainingForceConstant', c_double),
            ('restrainingTorqueConstant', c_double),
            ('rigidifyFormedHelices', c_bool),
            ('rigidifyTermini', c_int),
            ('satisfiedBasePairs', c_int),
            ('unSatisfiedBasePairs', c_int),
            ('scrubberPeriod', c_double),
            ('safeParameters', c_bool),
            #('setChiBondAnti', ////int: unknown ctype),
            ('setChiBondMobility', c_int),
            #('setDefaultMDParameters', ////int: unknown ctype),
            ('setDefaultStructurePredictionParameters', c_int),
            ('setDefaultThreadingParameters', c_int),
            ('setForceAndStericScrubber', c_bool),
            ('setForceScrubber', c_bool),
            ('setHelicalStacking', c_bool),
            ('setInitialVelocities', c_bool),
            ('setLoopBondMobility', c_bool),
            ('setOverallBondMobility', c_bool),
            ('setRemoveBasePairsInRigidStretch', c_bool),
            ('setRepulsiveForce', c_bool),
            ('setTemperature', c_bool),
            ('smallGroupInertiaMultiplier', c_double),
            ('stackAllHelicalResidues', c_bool),
            ('thermostatType', c_char_p),
            ('tinkerParameterFileName', c_char_p),
            ('twoTransformForceMultiplier', c_double),
            ('useFixedStepSize', c_bool),
            ('useMultithreadedComputation', c_bool),
            ('useOpenMMAcceleration', c_bool),
            ('vanderWallSphereRadius', c_double),
            ('velocityRescalingInterval', c_double),
            ('verbose', c_bool),
            ('vmdOutput', c_int),
            ('waterDropletMake', c_bool),
            #('// double waterDropletRadius;//Angstroms', : unknown ctype),
            #('// double waterDropletX; //Angstroms', : unknown ctype),
            #('// double waterDropletY; //Angstroms', : unknown ctype),
            #('// double waterDropletZ; //Angstroms', : unknown ctype),
            ('waterInertiaMultiplier', c_double),
            ('weldToGround', c_bool),
            ('wkdpGlobalBondTorsionScaleFactor', c_double),
            ('writeCoordinates', c_bool),
            ('writeDoublePrecisionTrajectories', c_bool),
            ('writeFrameFile', c_bool),
            ('writeLastFrameFile', c_bool),
            ('detectConvergence',c_bool),
            ('converged',c_bool),
            ('convergenceTimeout',c_int),
            ('convergenceEpsilon',c_double),
            #('helixBondMobility', //BondMobility::Mobility: unknown ctype),
            #('loopBondMobility', //BondMobility::Mobility: unknown ctype),
            #('overallBondMobility', //BondMobility::Mobility: unknown ctype),
            #('chiBondMobility', //BondMobility::Mobility: unknown ctype),
            #('qVector', //Vector: unknown ctype),
            ('lastFrameFileName', c_char_p),
            ('previousFrameFileName', c_char_p),
            #('myLeontisWesthofClass', //LeontisWesthofClass: unknown ctype),
            ('enforceParallelness', c_int),
            #('// // end of variables improted from Repel.h', : unknown ctype),
            ('sequence', c_char_p),
            ('proteinSequence', c_char_p),
            ('coarseNucleicAcidSequence', c_char_p),
            #('numChains', ////int: unknown ctype),
            ('numFirstResidues', c_int),
            ('numResetBases', c_int),
            ('numProteinFirstResidues', c_int),
            #('numProteinChains', ////int: unknown ctype),
            ('numTemperatures', c_int),
            ('numGlobalCoulombScaleFactors', c_int),
            ('numGlobalVdwScaleFactors', c_int),
            #('numDutyCycles', ////int: unknown ctype),
            ('temperature', c_double),
            ('dutyCycle', c_double),
            ('periodicallyUpdateParameters', c_int),
            ('currentStage', c_int),
            ('priority', c_int),
            #('biopolymerVector', ////vector<Biopolymer>: unknown ctype),
            #('chainId', ////vector<String>: unknown ctype),
            #('residueNumber', //vector<int>: unknown ctype),
            #('residueNumberTwo', //map<const ChainResidueIndex, int,twoIndexCmp>: unknown ctype),
            #('temperatureArray', ////vector<double>: unknown ctype),
            #('temperaturePriority', ////vector<int>: unknown ctype),
            #('dutyCycleArray', ////vector<double>: unknown ctype),
            #('dutyCyclePriority', ////vector<int>: unknown ctype),
            #('globalCoulombScaleFactorArray', ///*vector<double>: unknown ctype),
            #('globalCoulombScaleFactorPriority', //vector<int>: unknown ctype),
            #('globalVdwScaleFactorArray', //vector<double>: unknown ctype),
            #('// vector<int> globalVdwScaleFactorPriority;*/', : unknown ctype),
            #('_leontisWesthofClass', //LeontisWesthofClass: unknown ctype),
            #('userVariables', //mutable map<const String,double>: unknown ctype),
            #('myDensityMap', //DensityMap: unknown ctype),
            #('mobilizerContainer', //MobilizerContainer: unknown ctype),
            #('constraintToGroundContainer', //ConstraintToGroundContainer: unknown ctype),
            #('displacementContainer', //DisplacementContainer: unknown ctype),
            #('atomSpringContainer', //AtomSpringContainer: unknown ctype),
            #('myBiopolymerClassContainer', //BiopolymerClassContainer: unknown ctype),
            #('waterDropletContainer', //WaterDropletContainer: unknown ctype),
            #('proteinSequences', //map<const String,String>: unknown ctype),
            #('coarseNucleicAcidSequences', //map<const String,String>: unknown ctype),
            #('// map<const String, int> numRigidSegments   ; // scf remove, phased out', : unknown ctype),
            #('firstResidueNumbersIterator', //map<const String,int>::iterator: unknown ctype),
            #('// //void addRingClosingBond(const String chainID, ResidueID residueID1, String atomName1,String bondCenterName1,  ResidueID residueID2, String atomName2,String bondCenterName2);', : unknown ctype),
            #('bondCenterName2)', //// //void addRingClosingBond(const String chainID, ResidueID residueID1, String atomName1,String bondCenterName1, ResidueID residueID2, String atomName2,String: unknown ctype),
            #('// void addC1pSprings (LeontisWesthofClass myLeontisWesthofClass);', : unknown ctype),
            #('myLeontisWesthofClass)', //// void addC1pSprings (LeontisWesthofClass: unknown ctype),
            #('// void applyAtomSprings (SimbodyMatterSubsystem & matter, GeneralForceSubsystem & forces);', : unknown ctype),
            #('forces)', //// void applyAtomSprings (SimbodyMatterSubsystem & matter, GeneralForceSubsystem &: unknown ctype),
            #('// void configureDumm( DuMMForceFieldSubsystem & dumm);', : unknown ctype),
            #('dumm)', //// void configureDumm( DuMMForceFieldSubsystem &: unknown ctype),
            #('// static double myAtoF(map<const String,double> myUserVariables,const char* value );', : unknown ctype),
            #(')', //// //bool chainIsBiopolymer(String myChainId: unknown ctype),
            #('// static bool aToBool( const String& name, const char* value );', : unknown ctype),
            #('// static bool compareUpper( const String& param, const char* symbol );', : unknown ctype),
            #('baseOperationVector', //vector<BasePair>: unknown ctype),
            #('contactContainer', //ContactContainer: unknown ctype),
            #('densityContainer', //DensityContainer: unknown ctype),
            #('singleBondMobilityVector', //vector<SingleBondMobility>: unknown ctype),
            #('atomSpringVector', ////vector<AtomSpring>: unknown ctype),
            #('basePairPartnerVector', //vector<BasePairPartner>: unknown ctype),
            #('includeAllNonBondAtomsInResidueVector', //vector<IncludeAllNonBondAtomsInResidue>: unknown ctype),
            #('includeAllResiduesWithinVector', //vector<AllResiduesWithin>: unknown ctype),
            #('includeNonBondAtomInBiopolymerVector', //vector<IncludeNonBondAtomInBiopolymerStruct>: unknown ctype),
            #('waterDropletAboutResidueVector', //vector <WaterDropletAboutResidueStruct>: unknown ctype),
            #('// void removeBasePairsInRigidStretch ();', : unknown ctype),
            #('()', //// // void initializeDefaults: unknown ctype),
            #('// void printAllSettings (   ostream  & myOstream = std::cout, String remarkString = "") ;', : unknown ctype),
            #('"")', //// void printAllSettings ( ostream & myOstream = std::cout, String remarkString =: unknown ctype),
            #('// void removeNonPriorityBasePairs (int priorityLevel);', : unknown ctype),
            #('priorityLevel)', //// void removeNonPriorityBasePairs (int: unknown ctype),
            #('// //int getFirstResidueNumbers(const String myChainId) const ;', : unknown ctype),
            #('const', //// int getNumBasePairs(): unknown ctype),
            #('// int getProteinFirstResidueNumbers(const String myProteinChainId) const ;', : unknown ctype),
            #('// //int getBasePriority(int baseResidueNumber,String baseChain, String basePairingEdge) const ;', : unknown ctype),
            #('// int getNumBasePairs() const;', : unknown ctype),
            #('// void updateBasePair(int index,', : unknown ctype),
            #('// String ch1, int res1, String edge1,', : unknown ctype),
            #('// String ch2, int res2, String edge2,', : unknown ctype),
            #('// String orient);', : unknown ctype),
            #('// void updateMobilizerStretch(int index,', : unknown ctype),
            #('// String chainId,', : unknown ctype),
            #('// int startRes,', : unknown ctype),
            #('// int endRes,', : unknown ctype),
            #('// String bondMobility);', : unknown ctype),
            #('// void addAllResiduesWithin(String chainID, int resID, double radius);', : unknown ctype),
            #('radius)', //// void updateAllResiduesWithin(int index, String chainID, int resID, double: unknown ctype),
            #('// void updateAllResiduesWithin(int index, String chainID, int resID, double radius);', : unknown ctype),
            #('// void deleteAllResiduesWithin(int index);', : unknown ctype),
            #('index)', //// void deleteIncludeAllNonBondAtomsInResidue(int: unknown ctype),
            #('// void updateIncludeAllNonBondAtomsInResidue(int index, String chainID, int resID);', : unknown ctype),
            #('resID)', //// void updateIncludeAllNonBondAtomsInResidue(int index, String chainID, int: unknown ctype),
            #('// void deleteIncludeAllNonBondAtomsInResidue(int index);', : unknown ctype),
            #('// //int calcHighestPriority();', : unknown ctype),
            #('calcHighestPriority()', //// //int: unknown ctype),
            #('// //int calcLowestBondingResidue(const String myChainId) ;', : unknown ctype),
            #('myChainId)', //// //bool chainIsMonoAtoms(String: unknown ctype),
            #('// //int calcHighestBondingResidue(const String myChainId);', : unknown ctype),
            #('// void setLeontisWesthofBondRowIndex();', : unknown ctype),
            #('setLeontisWesthofBondRowIndex()', //// void: unknown ctype),
            #('// void parameterStringInterpreter(const String & paramstr);', : unknown ctype),
            #('paramstr)', //// void parameterStringInterpreter(const String &: unknown ctype),
            #('// void parameterStringInterpreter(const ParameterStringClass & parameterStringClass,', : unknown ctype),
            #('// const int readStage = 0,', : unknown ctype),
            #('// const bool readAtOneStageOnly = false,', : unknown ctype),
            #('// const bool readOnlyUntilStage = false,', : unknown ctype),
            #('// const bool readExcept = false);', : unknown ctype),
            #('// void initializeFromFileOnly(const char * parameterFileName = "./commands.dat" ) ;', : unknown ctype),
            #('// void setFirstAndLastStage(const char * parameterFileName = "./commands.dat" ) ;', : unknown ctype),
            #('// void loadSequencesFromPdb(const char * pdbFileName);', : unknown ctype),
            #('pdbFileName)', //// void loadSequencesFromPdb(const char *: unknown ctype),
            #('// //void printRigidSegments();', : unknown ctype),
            #('printRigidSegments()', //// //void: unknown ctype),
            #('// void printBasePairs();', : unknown ctype),
            #('printBasePairs()', //// void: unknown ctype),
            #('// void printBaseAssignments();', : unknown ctype),
            #('printBaseAssignments()', //// void: unknown ctype),
            #('// void postInitialize();', : unknown ctype),
            #('postInitialize()', //// void: unknown ctype),
            #('// void clearContainers();', : unknown ctype),
            #('clearContainers()', //// void: unknown ctype),
            #('// void clearBiopolymers();', : unknown ctype),
            #('clearBiopolymers()', //// void: unknown ctype),
            #('// void clearForces();', : unknown ctype),
            #('clearForces()', //// void: unknown ctype),
            #('// void clearConstraints();', : unknown ctype),
            #('clearConstraints()', //// void: unknown ctype),
            #('// // void initializeDefaults ();', : unknown ctype),
            #('// void initializeDefaults(const char * leontisWesthofInFileName = "./parameters.csv");', : unknown ctype),
            #('"./parameters.csv")', //// void initializeDefaults(const char * leontisWesthofInFileName =: unknown ctype),
            #('// void initialize(const char * parameterFileName = "./commands.dat" );', : unknown ctype),
            #('// //bool chainIsBiopolymer(String myChainId );', : unknown ctype),
            #('// //bool chainIsMonoAtoms(String myChainId);', : unknown ctype),
            #('// //int getChainIndex(String myChainId , vector<Biopolymer> & tempChain);', : unknown ctype),
            #('tempChain)', //// //int getChainIndex(String myChainId , vector<Biopolymer> &: unknown ctype),
            #('myMonoAtomsContainer', //MonoAtomsContainer: unknown ctype),
            #('// //variables for internal use only:', : unknown ctype),
            # ('r', c_int),
            # ('ti', c_int),
            # ('gcsfi', c_int),
            # ('gvsfi', c_int),
            # ('d', c_int),
            # ('s', c_char_p),
            #('300', ////temperature =: unknown ctype),
            #('//outQVectorFileName', //: unknown ctype),
            #('1', ////dutyCycle =: unknown ctype),
            #('// //lastStage = 0;// calcHighestPriority();', : unknown ctype),
            #('0', //// //lastStage =: unknown ctype),
            #('// //priority = 0;  //  this will be set in removeNonPriorityBasePairs', : unknown ctype)
            ]

    def __init__(self):
        Structure.__init__(self)
        # print "Init"
        call('updateParameterReader_wrapper', byref(self))
        Structure.__setattr__(self,"nonDefaultParameters",set())

    def __setattr__(self, name, value):
        # print __file__, "Set", name
        #cmd(name + " " + str(value))
        # We synchronize the wrapper with MMB to avoid changing other attributes
        call('updateParameterReader_wrapper', byref(self))
        Structure.__setattr__(self,name, value)
        Structure.__getattribute__(self, "nonDefaultParameters").update(set([name]))
        call('updateParameterReader', byref(self))

    def __getattribute__(self, name):
        # print __file__,  "Get", name
        call('updateParameterReader_wrapper', byref(self))
        return Structure.__getattribute__(self, name)

ParameterReader_ptr = POINTER(ParameterReader_wrapper)
MMB.updateParameterReader_wrapper.argtypes = [c_void_p, c_char_p]
MMB.updateParameterReader.argtypes = [c_void_p, c_char_p]