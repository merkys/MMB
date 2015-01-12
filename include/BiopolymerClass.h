/* -------------------------------------------------------------------------- *
 *                           MMB (MacroMoleculeBuilder)                       *
 * -------------------------------------------------------------------------- *
 *                                                                            *
 * Copyright (c) 2011-12 by the Author.                                       *
 * Author: Samuel Flores                                                      *
 *                                                                            *
 * See RNABuilder.cpp for the copyright and usage agreement.                  *
 * -------------------------------------------------------------------------- */

#ifndef BiopolymerClass_H_
#define BiopolymerClass_H_
#define MUTATIONMINORSEPARATOR "-"
#include <string>

#include <utility>
//#include <Compound.h>
#include "Mutation.h"
#include "SimTKmolmodel.h"


#include "Utils.h"           
#include "WaterDroplet.h"
#include "BaseInteractionParameterReader.h"
#include "ConstraintContainer.h"
#include "ReferenceNeighborList.h"

void         printBiopolymerSequenceInfo(const Biopolymer & myBiopolymer) ;
bool         letterIsRNA    (String);
bool         letterIsDNA    (String);
bool         letterIsProtein(String);
template <class ResidueStretchType>
class ResidueStretchContainer; // Don't you just love forward declarations?

class MMB_EXPORT BiopolymerClass {
private:

    ResidueID   firstResidueID    ;
    String      sequence;
    String      originalSequence;
    String      chainID;
    String      firstResidueMobilizerType;
    bool        proteinCapping;
    vector<MMBAtomInfo> atomInfoVector;
    vector<ResidueID> residueIDVector; // the element index should match the residue index for fast retrieval
    vector<Mutation> mutationVector; // this manages the vector of substitution mutations.  Not used for MMB, but breeder uses it extensively.
    String  pdbFileName;
    const PdbStructure * pdbStructure;
    bool    loadFromPdb;
    
    void        clear(); // sets all methods to empty values
    void        validateChainID();
    void        validateResidueNumbersAndInsertionCodes();
    bool        residueIsPurine (int residueIndex, String mySequence);
    bool        residueIsPurine (int residueIndex);
    int         validateSequence() ;  
    int         validateBiopolymerType() ;  
    int         validateProteinCapping();
    ResidueID   getResidueID    (  int residueIndex)  ;

    // Does not work because of incomplete template class decleration (forward declaration)
    // ResidueStretchContainer<ResidueStretch> inactiveResidueStretches;

    //Temporary
    bool activePhysics;
                                       
public:

    Biopolymer  myBiopolymer;

    BiopolymerType::BiopolymerTypeEnum biopolymerType;
    String  getSequence(){return sequence;}; // gets the sequence
    String  getSubSequence(const ResidueID startResidue, const ResidueID endResidue); 
    vector<Mutation> getMutationVector() { return mutationVector; }
    void    setMutationVector(vector<Mutation> myMutationVector ) { mutationVector = myMutationVector; }
    void    validateMutation( Mutation myMutation);
    void    addMutationToVector(Mutation myMutation);
    void    setOriginalSequence(String); // sets the original sequence
    String  getOriginalSequence(){return originalSequence;}; // gets the original (pre-mutation) sequence
    void    setProteinCapping(bool);
    void    setFirstResidueMobilizerType(String myFirstResidueMobilizerType);
    String  getFirstResidueMobilizerType();
    void    renumberPdbResidues(ResidueID firstResidueID);
    void    setBiopolymerType(String); // sets and validates the biopolymerType
    void    setBiopolymerType(BiopolymerType::BiopolymerTypeEnum); // sets and validates the biopolymerType

    void    setPdbFileName(String pdbFileName);
    String  getPdbFileName();

    void    setPdbStructure(const PdbStructure *);
    const   PdbStructure* getPdbStructure();
    
    void    setLoadFromPdb(bool yesno);
    bool    getLoadFromPdb();

    void    setSequence(String); // sets and validates the sequence
    void    changeSequence(String myNewSequence);

    BiopolymerClass(); // sets all properties to empty values.
    BiopolymerClass(String mySequence, String myChainID, ResidueID myFirstResidueNumber, String myBiopolymerType, bool proteinCapping ); // validates and sets the listed properties.          
    
    void initializeBiopolymer(CompoundSystem & system, 
                              bool myProteinCapping, 
                              bool matchExact, bool matchIdealized , 
                              const bool matchOptimize,
                              bool matchHydrogenAtomLocations, 
                              bool matchPurineN1AtomLocations, 
                              bool guessCoordinates,
                              int biopolymerClassIndex, double initialSeparation, 
                              const vector<Displacement> displacementVector,
                              double matchingMinimizerTolerance, 
                              double myPlanarityThreshold,
                              vector<SecondaryStructureStretch> secondaryStructureStretchVector 
                             ) ; //    Should  everything currently done by ConstrainedDynamics::initializeMolecules.  the latter should stop treating biopolymers altogether.  it really should stop treating MonoAtoms as well.

    void    matchCoordinates(String inputFileName, 
                             bool matchExact, bool matchIdealized,
                             const bool matchOptimize ,  
                             bool matchHydrogenAtomLocations, 
                             bool matchPurineN1AtomLocations,
                             bool guessCoordinates ,  
                             double matchingMinimizerTolerance, 
                             double myPlanarityThreshold);   // this parameter sets the out-of-planarity tolerance for identifying planar bonds.  Units: radians.
    void    matchCoordinates(istream & inputFile, 
                             bool matchExact, bool matchIdealized,
                             const bool matchOptimize ,  
                             bool matchHydrogenAtomLocations, 
                             bool matchPurineN1AtomLocations,
                             bool guessCoordinates ,  
                             double matchingMinimizerTolerance, 
                             double myPlanarityThreshold);   // this parameter sets the out-of-planarity tolerance for identifying planar bonds.  Units: radians.
    void    matchCoordinates(const PdbStructure & pdbStructure, 
                             bool matchExact, bool matchIdealized,
                             const bool matchOptimize ,  
                             bool matchHydrogenAtomLocations, 
                             bool matchPurineN1AtomLocations,
                             bool guessCoordinates ,  
                             double matchingMinimizerTolerance, 
                             double myPlanarityThreshold);   // this parameter sets the out-of-planarity tolerance for identifying planar bonds.  Units: radians.

    int         getChainLength();
    size_t      getNumAtoms();
    String      getChainID() const{ return chainID;};
    ResidueID   getFirstResidueID   ();
    ResidueID   getLastResidueID   ();
    BiopolymerType::BiopolymerTypeEnum  getBiopolymerType();
    String  getBiopolymerTypeAsString();
      bool  getProteinCapping(){return proteinCapping;}
    ResidueID   residueID(map<const String,double> myUserVariables, const   char* value);
    ResidueID   residueID(String inputString);  // this method of converting string to ResidueID has the advantage that it validates against the corresponding biopolymer
    void        validateResidueID(ResidueID myResidueID    );
    void        validateResidueIndex(int myResidueIndex);
    void        validateAtomInfoVector();
    bool        hasAtom(  ResidueID myResidueID,   String myAtomName);
    int         validateAtomPathName(  Compound::AtomPathName);
    Compound::AtomPathName atomPathString(  ResidueID myResidueID,   String myAtomName);
    Compound::AtomIndex    atomIndex(  ResidueID ,   String );
    DuMM::AtomIndex    getDuMMAtomIndex(  ResidueID ,   String );
    Vec3        getAtomLocationInMobilizedBodyFrame(  ResidueID myResidueID,   String myAtomName); 
    MobilizedBody & updAtomMobilizedBody(SimbodyMatterSubsystem & matter,   ResidueID myResidueID,   String myAtomName);
    MobilizedBodyIndex getAtomMobilizedBodyIndex( SimbodyMatterSubsystem & matter,   ResidueID myResidueID    ,   String myAtomName);
    Vec3        calcAtomLocationInGroundFrame(const  State & ,    ResidueID residueID,   String atomName);
    Vec3        calcDefaultAtomLocationInGroundFrame(   ResidueID residueID,   String atomName);
    void        loadResidueIDVector();
    void        loadResidueIDVectorAscending(ResidueID firstResidueID);
    const   ResidueInfo::Index   getResidueIndex(   ResidueID residueID  ); // called by getPdbResidueName 
    String  getPdbResidueName(   ResidueID residueID);
    String      getRepresentativeAtomName(); // returns the name of an atom which is typically used to represent the entire residue, e.g. CA or C4*.
    double      getRepresentativeAtomMassThreshold(); // gets a number slightly larger than the maximum expected mass of the representative atom's mobilized body.
    void        setContactParameters(GeneralContactSubsystem & contacts, HuntCrossleyForce & hc, double excludedVolumeStiffness, bool active );
    void        addGeneralSterics (GeneralContactSubsystem & ,ContactSetIndex contactSet, HuntCrossleyForce & hc,SimbodyMatterSubsystem & matter,  double excludedVolumeRadius,double excludedVolumeStiffness    ,    ResidueID startResidue  ,   ResidueID endResidue,   bool endCapsOn ,   bool addHydrogens);
    void        addCustomSterics (GeneralContactSubsystem & contacts, ContactSetIndex contactSet, HuntCrossleyForce & hc,SimbodyMatterSubsystem & matter,LeontisWesthofClass myLeontisWesthofClass,String leontisWesthofInteractionType,   ResidueID startResidue,   ResidueID endResidue,   bool endCapsOn);
    void        setProteinBondMobility (   BondMobility::Mobility  mobility,   ResidueID startResidue,   ResidueID endResidue);
    void        rigidifyTargetedBonds(Compound::AtomTargetLocations  & biopolymerAtomTargets);
    void        setSingleBondMobility(  ResidueID residueID1,  String atomName1,  ResidueID residueID2,   String atomName2,  String mobilityString ); // sets BondMobility for a single bond in the chain.
    Biopolymer & updBiopolymer();
    void        includeNonBondAtom(  ResidueID residueID,   String atomName, State & state, DuMMForceFieldSubsystem & dumm) ;
    ResidueInfo updResidueInfo (  ResidueID residueID) ;
    void        includeAllNonBondAtomsInResidue(  ResidueID residueID, State & state, DuMMForceFieldSubsystem & dumm) ;
    void        includeAllResiduesWithin (  vector<AllResiduesWithin> & includeAllResiduesWithinVector,    vector<IncludeAllNonBondAtomsInResidue> & includeAllNonBondAtomsInResidueVector, const State state);
    void        constrainRigidSegmentsToGround(CompoundSystem & system,  SimbodyMatterSubsystem & matter,State & state, ConstraintToGroundContainer & myConstraintToGroundContainer, bool toGround, ResidueID rootResidue   );
    void        physicsZone(vector<AllResiduesWithin> & myIncludeAllResiduesWithinVector , double radius, SimbodyMatterSubsystem & matter,State & state);
    void        multiplySmallGroupInertia(  ResidueID residueID,   String atomName,   double multiplier, CompoundSystem & system,  SimbodyMatterSubsystem & matter,State & state);
    void        multiplySmallGroupInertia(   double multiplier, CompoundSystem & system, SimbodyMatterSubsystem & matter,State & state) ;
    MMBAtomInfo mmbAtomInfo(  ResidueID myResidueID,   ResidueInfo::AtomIndex myResidueInfoAtomIndex,  SimbodyMatterSubsystem& matter  );
    MMBAtomInfo mmbAtomInfo(  ResidueID myResidueID,   ResidueInfo::AtomIndex myResidueInfoAtomIndex,  SimbodyMatterSubsystem& matter, DuMMForceFieldSubsystem & dumm );
    //MMBAtomInfo mmbAtomInfo(  ResidueID myResidueID,   ResidueInfo::AtomIndex myResidueInfoAtomIndex,  SimbodyMatterSubsystem& matter, DuMMForceFieldSubsystem & dumm , State & state);
    void        initializeAtomInfoVector(SimbodyMatterSubsystem & matter);
    void        initializeAtomInfoVector(SimbodyMatterSubsystem & matter,DuMMForceFieldSubsystem & dumm);

    const       vector<MMBAtomInfo> getAtomInfoVector();
    const       void printAtomInfoVector(){for (int i = 0 ; i < atomInfoVector.size(); i++) atomInfoVector[i].print(); };
    const       vector<MMBAtomInfo>  calcAtomInfoVector(ResidueStretch myResidueStretch, SimbodyMatterSubsystem& matter, DuMMForceFieldSubsystem & dumm);
    void        addRingClosingBond(CovalentBondClass myCovalentBondClass); 
    void        addRingClosingBond( ResidueID residueID1, String atomName1, String bondCenterName1,  ResidueID residueID2, String atomName2,String bondCenterName2, SimTK::BondMobility::Mobility bondMobility) ; 
 
    void        setResidueIDsAndInsertionCodesFromBiopolymer(const Biopolymer & inputBiopolymer, bool endCaps);
    void        setResidueIDsAndInsertionCodesFromBiopolymer(const Biopolymer & inputBiopolymer, Mutation myInsertion, bool endCaps);
    void        setResidueIDsAndInsertionCodesFromBiopolymerWithDeletion(const Biopolymer & oldBiopolymer, ResidueInfo::Index  myDeletionIndex, bool endCaps  );
    void        printBiopolymerInfo() ;
    void        loadMutationVectorFromSequence(); 
    void        writeMutationFlexibilizers(std::ofstream & output, const int offset,const double radius);
    void        writeMutationBackboneRigidifier(std::ofstream & output, const int offset);
    void        writePhysicsZones(std::ofstream & output, const int offset);
    void        writeSubstituteResidueCommands(std::ofstream & output);
    String      getFormattedMutationsString( String minorSeparator );
    void        setCurrentSequencesFromOriginalSequences();
    bool    allMutationsDifferFromWildType();
    void        updateMutationResidueTypesFromCurrentSequence() ;
    bool        residueIDLessThanOrEqualTo(ResidueID  residueA, ResidueID  residueB);
    int         getNumMutationVectorElements() {return mutationVector.size();}; 
    bool        residueIDGreaterThanOrEqualTo(ResidueID  residueA, ResidueID  residueB);
    ResidueID   incrementResidueID(ResidueID & residueID);
    ResidueID   decrementResidueID(ResidueID & residueID);
    void        setDefaultPeptideDihedralAngle (ResidueID residueID1, ResidueID residueID2, Angle dihedral);
    void    setDefaultPhiAngle (ResidueID residueID, Angle phi);
    void    setDefaultPsiAngle (ResidueID residueID, Angle psi);
    void    setAlphaHelicalDefaultBackboneAngles(ResidueID startResidue, ResidueID endResidue); 
    void        setParallelBetaSheetDefaultBackboneAngles(ResidueID startResidue, ResidueID endResidue); 
    void        setAntiParallelBetaSheetDefaultBackboneAngles(ResidueID startResidue, ResidueID endResidue); 
    
    int difference(ResidueID  residueA, ResidueID  residueB );
    //ResidueID testSum(ResidueID  oldResidueID, int  increment );
    bool safeSum(ResidueID  inputResidueID, int  increment, ResidueID outputResidueID);
    ResidueID safeSum(ResidueID  inputResidueID, int  increment );
    ResidueID sum(ResidueID  oldResidueID, int  increment );

    /** 
    * Get all residues within "distance" (nm) of location
    * @return a vector of ResidueID
    */ 
    std::vector<ResidueID> getResiduesWithin(Vec3 location, double distance);
    //void lockBiopolymerMobilizedBodies ();

    void        constrainRigidSegmentsToGroundForAllChains(CompoundSystem & system,  SimbodyMatterSubsystem & matter,State & state, ConstraintToGroundContainer & myConstraintToGroundContainer   ) ;
    //void        mutateCurrentSequenceFromMutationVector();

    bool hasResidueStretch(ResidueStretch & residues);

    // Unused for now
    void AddInactiveResidues(ResidueStretch & residues);
    void RemoveInactiveResidues(ResidueStretch & residues);
    vector<ResidueID> getInactiveResiduesVector();

    void setActivePhysics(bool yesno);
    bool getActivePhysics() const;


    template<class ResidueStretchType> 
    void selectivelyRemoveResidueStretchFromContainer(ResidueStretch & residueStretch, ResidueStretchContainer <ResidueStretchType> & residueStretchContainer){    
        // This command crops or deletes residue stretches in the range "residueStretch" from residueStretchVector.  This was intended to cancel any modifications to certain resiude stretches.    
        // We treat three cases:
        // 1. residueStretchVector[i] is a subset of (or is identical to) residueStretch
        //        -> erase residueStretchVector[i]
        // 2. residueStretch is a subset of residueStretchVector[i], with neither endpoint in common, splitting residueStretchVector[i] in two
        //        -> split residueStretchVector[i] into two disjoint residue stretches
        // 3. residueStretch is a subset of residueStretchVector[i], but the start point of residueStretch coincides with that of residueStretchVector[i] .
        //        -> trim  residueStretchVector[i] on left 
        // 4. residueStretch is a subset of residueStretchVector[i], but the end point of residueStretch coincides with that of residueStretchVector[i] .
        //        -> trim  residueStretchVector[i] on right
        // 5. residueStretch and residueStretchVector[i] overlap, with residueStretch starting before residueStretchVector[i].
        //        -> trim  residueStretchVector[i] on left 
        // 6. residueStretch and residueStretchVector[i] overlap, with residueStretchVector[i] starting before residueStretch.
        //        -> trim  residueStretchVector[i] on right
        //const int ResidueStretchContainer::getNumResidueStretches();
        cout<<__FILE__<<":"<<__LINE__<<" the Default stretch is :"<<endl;
        residueStretch.printStretch();
        cout<<__FILE__<<":"<<__LINE__<<" Now checking "<<residueStretchContainer.getNumResidueStretches()<<" stretches: "<<endl;
        for (int i = 0; i < residueStretchContainer.getNumResidueStretches(); i++) 
        {
            residueStretchContainer.residueStretchVector[i].printStretch();   

            if (residueStretchContainer.residueStretchVector[i].getChain().compare((residueStretch.getChain() )) != 0) {continue;} // in other words, only make modificatiosn to residueStretchContainer if chain ID's match.
            else if ((residueStretch.getStartResidue() <= residueStretchContainer.residueStretchVector[i].getStartResidue()) &&
                (residueStretch.getEndResidue() >= residueStretchContainer.residueStretchVector[i].getEndResidue()))
               {   //case = 1
                   residueStretchContainer.residueStretchVector.erase(residueStretchContainer.residueStretchVector.begin() + i);
                   i--; // vector has been shortened, so make sure we don't skip the next residueStretchContainer.residueStretchVector[i].
                   if (i < -1) {ErrorManager::instance <<__FILE__<<":"<<__LINE__<<"Unexplained error!"<<endl; ErrorManager::instance.treatError();}
               }
            else if ((residueStretch.getStartResidue() >  residueStretchContainer.residueStretchVector[i].getStartResidue()) &&
                (residueStretch.getEndResidue() <  residueStretchContainer.residueStretchVector[i].getEndResidue()))
               {   // case = 2 ;
                   cout<<__FILE__<<":"<<__LINE__<<"  "<<endl;
                   MobilizerStretch secondResidueStretch = residueStretchContainer.residueStretchVector[i];
                   ResidueID tempStartResidueID = (residueStretch).getStartResidue(); // getStartResidue() returns a temporary, whereas decrementResidueID expects a reference. can't convert a temporary to a reference.  This is because decrementResidueID might (and will!) try to modify ResidueID (as the name of the function suggests!).
                   //residueStretchContainer.residueStretchVector[i].setEndResidue(decrementResidueID((residueStretch).getStartResidue() ));
                   residueStretchContainer.residueStretchVector[i].setEndResidue(decrementResidueID(tempStartResidueID));//((residueStretch).getStartResidue() )));
                   cout<<__FILE__<<":"<<__LINE__<<" Just decreased endpoint of stretch "<<i<<".  New stretch is:"<<endl;
                   residueStretchContainer.residueStretchVector[i].printStretch();
                   ResidueID tempEndResidueID = (residueStretch).getEndResidue();
                   secondResidueStretch.setStartResidue(incrementResidueID(tempEndResidueID));//  residueStretch.getEndResidue()));
                   residueStretchContainer.addResidueStretchToVector(secondResidueStretch);
                   cout<<__FILE__<<":"<<__LINE__<<" Just added new  stretch :"<<endl;
                   residueStretchContainer.residueStretchVector[residueStretchContainer.getNumResidueStretches()-1].printStretch();
                   cout<<__FILE__<<":"<<__LINE__<<" Moving on to check next stretch. "<<endl;


               }
            else if ((residueStretch.getStartResidue() == residueStretchContainer.residueStretchVector[i].getStartResidue()) &&
                (residueStretch.getEndResidue() <  residueStretchContainer.residueStretchVector[i].getEndResidue()))
               {   // case = 3;
                   cout<<__FILE__<<":"<<__LINE__<<"  Case 3"<<endl;
                   ResidueID tempEndResidueID = (residueStretch).getEndResidue();
                   residueStretchContainer.residueStretchVector[i].setStartResidue(incrementResidueID(tempEndResidueID));//residueStretch.getEndResidue() ))  ;
               }
            else if ((residueStretch.getEndResidue() == residueStretchContainer.residueStretchVector[i].getEndResidue()) &&
                (residueStretch.getStartResidue() >  residueStretchContainer.residueStretchVector[i].getStartResidue()))
               {   // case = 4;
                   cout<<__FILE__<<":"<<__LINE__<<"  Case 4"<<endl;
                   
                   ResidueID tempStartResidueID = (residueStretch).getStartResidue();
                   residueStretchContainer.residueStretchVector[i].setEndResidue(decrementResidueID(tempStartResidueID));//residueStretch.getStartResidue()));
               }
            else if ((residueStretch.getStartResidue() <   residueStretchContainer.residueStretchVector[i].getStartResidue()) &&
                (residueStretch.getEndResidue()        >=  residueStretchContainer.residueStretchVector[i].getStartResidue()) &&
                     (residueStretch.getEndResidue()        <   residueStretchContainer.residueStretchVector[i].getEndResidue()))
            {   // case = 5;
                cout<<__FILE__<<":"<<__LINE__<<"  Case 5"<<endl;
                
                ResidueID tempEndResidueID = (residueStretch).getEndResidue();
                residueStretchContainer.residueStretchVector[i].setStartResidue(incrementResidueID(tempEndResidueID));//residueStretch.getEndResidue()))  ;
            }
            else if ((residueStretch.getEndResidue() >  residueStretchContainer.residueStretchVector[i].getEndResidue()) &&
                     (residueStretch.getStartResidue() >  residueStretchContainer.residueStretchVector[i].getStartResidue())     &&
                     (residueStretch.getStartResidue() <=  residueStretchContainer.residueStretchVector[i].getEndResidue()))
            {    // case = 6;
                cout<<__FILE__<<":"<<__LINE__<<"  Case 6"<<endl;
                
                ResidueID tempStartResidueID = (residueStretch).getStartResidue();
                residueStretchContainer.residueStretchVector[i].setEndResidue(decrementResidueID(tempStartResidueID));//  residueStretch.getStartResidue()));
            }
            else if (residueStretch.getEndResidue() < residueStretchContainer.residueStretchVector[i].getStartResidue()) {} // do nothing, stretches are disjoint
            else if (residueStretch.getStartResidue() > residueStretchContainer.residueStretchVector[i].getEndResidue()) {} // do nothing, stretches are disjoint
            else {
                // this should never happen
                        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<"Unexplained error!"<<endl; ErrorManager::instance.treatError();
                }
        }
    }

};

class MMB_EXPORT BiopolymerClassContainer {
private :
    map <const String, BiopolymerClass> biopolymerClassMap   ;
    //BiopolymerClassContainer(const BiopolymerClassContainer&); //never define; copy suppressed.
    //BiopolymerClassContainer& operator=(const BiopolymerClassContainer&); // never define; copy assignment suppressed.
    map <const String, PdbStructure> pdbStructureMap;

public :    
    BiopolymerClassContainer(){};
    map <const String, BiopolymerClass> getBiopolymerClassMap () const {return biopolymerClassMap;};
    void        clear(); //: deletes all BiopolymerClass's in biopolymerClassMap
    int         getNumBiopolymers(){return biopolymerClassMap.size();}
    size_t      getTotalNumAtoms();
    vector<SecondaryStructureStretch> secondaryStructureStretchVector;
    void        addBiopolymerClass(String mySequence, 
                                   String myChainID, ResidueID myFirstResidueID, 
                                   String myBiopolymerType, bool proteinCapping,
                                   String pdbFileName="", bool loadFromPdb=false
                                  ); // validates and sets the listed properties.        
    void        deleteBiopolymerClass(String myChainID);
    // Should  everything currently done by ConstrainedDynamics::initializeMolecules.  
    // Should probably split into or call multiple methods for coordinate matching, etc.   
    // The latter should stop treating biopolymers altogether.  it really should stop treating MonoAtoms as well.:
    void        initializeBiopolymers(CompoundSystem & system,
                                      bool myProteinCapping, 
                                      bool matchExact, 
                                      bool matchIdealized ,
                                      const bool matchOptimize ,
                                      bool matchHydrogenAtomLocations, 
                                      bool matchPurineN1AtomLocations,
                                      bool guessCoordinates, 
                                      double initialSeparation,
                                      const vector<Displacement> displacementVector,
                                      double matchingMinimizerTolerance,
                                      double myPlanarityThreshold
                                     );

    /** 
        Initialize one biopolymer identified by chainID
    */
    void initializeBiopolymer(String chainID, CompoundSystem & system,
                              bool myProteinCapping, bool matchExact, 
                              bool matchIdealized, const bool matchOptimize, 
                              bool matchHydrogenAtomLocations, 
                              bool matchPurineN1AtomLocations,
                              bool guessCoordinates,
                              double initialSeparation, 
                              const vector<Displacement> displacementVector,
                              double matchingMinimizerTolerance,
                              double myPlanarityThreshold,
                              vector<SecondaryStructureStretch> secondaryStructureStretchVector
                             );

    void        validateAtomInfoVectors();
    BiopolymerClass &   updBiopolymerClass(String myChainID);
    int                 getBiopolymerClassIndex(String myChainID);
    BiopolymerClass &   updBiopolymerClass(int biopolymerClassIndex);
    //void      setBondMobility  (const MobilizerContainer myMobilizerContainer ); 
    void        setBondMobility  (vector<BasePair> & ); 
    void        rigidifyAllChains();
    Vec3        getAtomLocationInMobilizedBodyFrame(String myChainID, ResidueID myResidueID, String myAtomName);
    MobilizedBody &     updAtomMobilizedBody(SimbodyMatterSubsystem & matter, String myChainID, ResidueID myResidueID, String myAtomName);
    //void      addContacts(ContactContainer myContactContainer , GeneralContactSubsystem &,GeneralForceSubsystem &, SimbodyMatterSubsystem &,CompoundSystem & , LeontisWesthofClass & myLeontisWesthofClass ,double excludedVolumeRadius,double excludedVolumeStiffness );
    void        writeDefaultPdb(std::ostream& outputStream);
    void        writePdb(State & state, CompoundSystem & system, std::ostream& outputStream, int modelNumber=1, bool calcEnergy=false, int satisfiedBasePairs=0, int unSatisfiedBasePairs=0);
    bool        hasChainID(String);
    int         validateChainID(String);
    Vec3        calcAtomLocationInGroundFrame(const State & , String chainID, ResidueID , String );
    void        newCalcAxes(const State& state,  LeontisWesthofBondRow myLeontisWesthofBondRow,ResidueID residueID1,ResidueID residueID2,String chain1 , String chain2,Vec3 & xAxisVector1,Vec3 & yAxisVector1, Vec3 & zAxisVector1,Vec3 & xAxisVector2,Vec3 & yAxisVector2 , Vec3 & zAxisVector2,Vec3 & glycosidicNitrogenAtom1LocationInGround,Vec3 & glycosidicNitrogenAtom2LocationInGround, Vec3 & ring1CenterLocationInGround, Vec3 & ring2CenterLocationInGround) ; 
    void        computeCorrection(LeontisWesthofClass &,vector<BaseInteraction> &,State &,SimbodyMatterSubsystem &);
    String      getPdbResidueName( const String chainID, ResidueID resID);
    String      getResidueSingleLetterCode( ResidueID myResidueID) ;
    void        setSingleBondMobility(const String chainID,const  ResidueID residueID1,const String atomName1,const ResidueID residueID2,const  String atomName2,const String mobilityString ); // sets BondMobility for a single bond in the chain.
    void        setSingleBondMobility(vector<SingleBondMobility>);  
    void        printAllIncludedResidues (vector<IncludeAllNonBondAtomsInResidue> & includeAllNonBondAtomsInResidueVector );
    std::vector< std::pair<const BiopolymerClass*, const ResidueID*> > getResiduesWithin(const String & chainID, const ResidueID & resID, double radius, const State & state, OpenMM::NeighborList * neighborList=NULL);
    std::vector< std::pair<const BiopolymerClass*, const ResidueID*> > getResiduesWithin(const String & chainID, const ResidueID & resID, double radius, OpenMM::NeighborList * neighborList=NULL);
    std::vector< std::pair<const BiopolymerClass*, const ResidueID*> > getResiduesWithin(vector<MMBAtomInfo>& concatenatedAtomInfoVector, const String & chainID, const ResidueID & resID, double radius, OpenMM::NeighborList * neighborList=NULL);
    OpenMM::NeighborList getNeighborList(const vector<MMBAtomInfo>& concatenatedAtomInfoVector, double radius);
    void        setNeighborsFromList(vector<MMBAtomInfo>& concatenatedAtomInfoVector, OpenMM::NeighborList& neighborList, double radius);
    void        includeAllNonBondAtomsInResidues(vector<IncludeAllNonBondAtomsInResidue>  myIncludeAllNonBondAtomsInResidueVector, State & state, DuMMForceFieldSubsystem & dumm) ;
    void        includeAllResiduesWithin (const vector<AllResiduesWithin> & includeAllResiduesWithinVector,    vector<IncludeAllNonBondAtomsInResidue> & includeAllNonBondAtomsInResidueVector, const State state);
    void        includeNonBondAtoms(  vector<IncludeNonBondAtomInBiopolymerStruct> includeNonBondAtomInBiopolymerVector,  State & state, DuMMForceFieldSubsystem & dumm) ;
    void        includeNonBondAtom(const String chain ,const  ResidueID residue,const  String atomName ,  State & state, DuMMForceFieldSubsystem & dumm) ;
    void        waterDropletAboutResidues (const vector <WaterDropletAboutResidueStruct> waterDropletAboutResidueVector,    WaterDropletContainer & waterDropletContainer );


    void        physicsZone(vector<AllResiduesWithin> & myIncludeAllResiduesWithinVector , double radius, SimbodyMatterSubsystem & matter,State & state);
    void        multiplySmallGroupInertia(const double multiplier, CompoundSystem & system,SimbodyMatterSubsystem & matter,State & state);
    void        initializeAtomInfoVectors(SimbodyMatterSubsystem & matter);
    void        initializeAtomInfoVectors(SimbodyMatterSubsystem & matter,DuMMForceFieldSubsystem & dumm);
    String      extractSequenceFromBiopolymer(const Biopolymer & myBiopolymer, bool endCaps);
    const bool  isRNA    (const Biopolymer & inputBiopolymer) ;
    const bool  isDNA    (const Biopolymer & inputBiopolymer) ;
    const bool  isProtein(const Biopolymer & inputBiopolymer, bool endCaps) ;
    void        loadSequencesFromPdb(String inPDBFilename,bool proteinCapping); 
    const PdbStructure & getPdbStructure(String fileName);
    void        printBiopolymerInfo() ;
    void setResidueIDsAndInsertionCodesFromBiopolymer(const String chain, const Biopolymer & inputBiopolymer,const  bool endCaps);
    ResidueID   residueID(map<const String,double> myUserVariables, const char* value , const String chain); // just like that below, except it can handle user-defined integer variables
    //ResidueID residueID(String inputResidueID, String chain); // this method of converting String to ResidueID has the advantage that it validates against the corresponding biopolymer.  Deprecated!  To be supplanted by the above.
    void addConstraintToGround(map<const String,double> myUserVariables, 
                               const String inputResidueString, 
                               const String chain, 
                               ConstraintToGroundContainer & constraintToGroundContainer);
    void addConstraintToGroundRange(map<const String,double> myUserVariables, const String inputResidueString1, const String inputResidueString2, const String chain, ConstraintToGroundContainer & constraintToGroundContainer);
    void addConstraint(map<const String,double> myUserVariables,
                       const String inputResidueString, const String chain, 
                       const String inputResidueString2, const String chain2, 
                       ConstraintToGroundContainer & constraintToGroundContainer);
    void addConstraint(map<const String,double> myUserVariables,
                       const String atomName, const String inputResidueString,const  String chain, 
                       const String atomName2, const String inputResidueString2,const  String chain2, 
                       ConstraintToGroundContainer & constraintToGroundContainer);
    void addConstraint(map<const String,double> myUserVariables,
               const String atomName1, const String inputResidueString1,const  String chain1, 
               const String atomName2, const String inputResidueString2,const  String chain2, 
               ConstraintType myConstraintType,
               ConstraintToGroundContainer & constraintToGroundContainer);

    void constrainRigidSegmentsToGroundForAllChains(CompoundSystem & system,  SimbodyMatterSubsystem & matter,State & state, ConstraintToGroundContainer & myConstraintToGroundContainer  );
    
    void        loadResidueIDVector();
    void        setFirstResidueMobilizerType(const String myFirstResidueMobilizerType);
    void        setContactParameters ( GeneralContactSubsystem & contacts,  HuntCrossleyForce & hc, double excludedVolumeStiffness, bool active );

    void        setOriginalSequence(String myChainID, String myOriginalSequence) {updBiopolymerClass(myChainID).setOriginalSequence(myOriginalSequence);}
    void        setOriginalSequencesFromCurrentSequences()   ; 
    void        replaceBiopolymerWithMutatedBiopolymerClass(BiopolymerClass & myOldBiopolymerClass, 
                                             String & myNewSequence);
    void        substituteResidue(String myChain , ResidueID myResidue, String mySubstitution, bool proteinCapping) ;
    void        insertResidue(Mutation myInsertion,  bool proteinCapping) ;
    void        loadMutationVectorsFromSequence(); 
    //void        loadMutationVectorsFromString(String myMutationString); 
    void        writeMutationFlexibilizers(std::ofstream & output, const int offset, const double radius);
    void        writeMutationBackboneRigidifier(std::ofstream & output, const int offset);
    void        writePhysicsZones(std::ofstream & output, const int offset);
    const       vector<MMBAtomInfo> getConcatenatedAtomInfoVector(bool activeChainsOnly=false);
    const       vector<MMBAtomInfo> getConcatenatedAtomInfoVector(const State & state,bool activeChainsOnly=false);
    void        printAtomInfoVector();
    void        writeSubstituteResidueCommands(std::ofstream & output);
    int         getNumMutationVectorElements(); 
    String      getFormattedSequencesString();
    String      getFormattedMutationsString( String minorSeparator  );
    void        setCurrentSequencesFromOriginalSequences();     
    bool        allMutationsDifferFromWildType();
    void        updateMutationResidueTypesFromCurrentSequence() ;
    void        substituteResidue(Mutation myMutation, bool safeParameters, bool matchPurineN1AtomLocations, bool proteinCapping );
    void        deleteResidue(Mutation myDeletion,   bool proteinCapping);
    vector<Mutation> getCompositeMutationVector();  
    void        addIntraChainInterfaceResidues(String chain, vector<IncludeAllNonBondAtomsInResidue> & myIncludeAllNonBondAtomsInResidueVector , double radius, SimbodyMatterSubsystem & matter,State & state);
};
    
#endif
