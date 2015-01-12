/* -------------------------------------------------------------------------- *
 *                           MMB (MacroMoleculeBuilder)                       *
 * -------------------------------------------------------------------------- *
 *                                                                            *
 * Copyright (c) 2011-12 by the Author.                                       *
 * Author: Samuel Flores                                                      *
 *                                                                            *
 * See RNABuilder.cpp for the copyright and usage agreement.                  *
 * -------------------------------------------------------------------------- */


#include "AtomSpringContainer.h"
#include "BiopolymerClass.h"
#include <seqan/align.h>

using namespace std;
using namespace SimTK;

AtomSpring & AtomSpringContainer::initializeAtomSpring(AtomSpring & atomSpring) {
   atomSpring.atom1Name = ""           ;   
   atomSpring.atom2Name = ""           ;   
   atomSpring.atom1Residue =  ResidueID(0, ' ')        ;   
   atomSpring.atom2Residue =  ResidueID(0, ' ')        ;   
   atomSpring.atom1Chain = "";
   atomSpring.atom2Chain = "";
   atomSpring.toGround   = false;
   atomSpring.tether     = false;
   atomSpring.groundLocation = Vec3(0);
   atomSpring.forceConstant  = 0.0 ;
   atomSpring.deadLength     = 0.0 ;   
   return atomSpring;
};


void AtomSpringContainer::printAtomSpring(const AtomSpring atomSpring){
    cout<<__FILE__<<":"<<__LINE__
        <<" atom1Chain     = " << atomSpring.atom1Chain    
        <<" atom1Residue   = " << atomSpring.atom1Residue.outString()   
	<<" atom1Name      = " << atomSpring.atom1Name     
        <<" atom2Chain     = " << atomSpring.atom2Chain    
        <<" atom2Residue   = " << atomSpring.atom2Residue.outString()   
        <<" atom2Name      = " << atomSpring.atom2Name      
        <<" toGround       = " << atomSpring.toGround      
        <<" tether         = " << atomSpring.tether        
        <<" groundLocation = " << atomSpring.groundLocation <<" (nm,nm,nm) "
        <<" forceConstant  = " << atomSpring.forceConstant <<" (kJ/mol/nm/nm) "
        <<" deadLength     = " << atomSpring.deadLength <<" (nm) "
        <<endl;     
};

void AtomSpringContainer::printAtomSpring(int atomSpringIndex){
    AtomSpring myAtomSpring = getAtomSpring (atomSpringIndex);
    printAtomSpring(myAtomSpring);
};

void AtomSpringContainer::printAtomSprings(){
    for (int i = 0 ; i < numAtomSprings(); i++) 
        printAtomSpring(i); 
};


void AtomSpringContainer::validateAtomSpring(const AtomSpring & atomSpring){//,  BiopolymerClassContainer & myBiopolymerContainer ){
    // a significant amount of validation is already being done in ParameterReader.cpp, when the atomSpring or related command is read.
    // the following two calls will ensure that the desired atoms exist:
    //myBiopolymerContainer.updBiopolymerClass(atomSpring.atom1Chain).atomPathString(atomSpring.atom1Residue,atomSpring.atom1Name);
    //if (! atomSpring.toGround) // if it's .toGround, then there is no second atom
    //  myBiopolymerContainer.updBiopolymerClass(atomSpring.atom2Chain).atomPathString(atomSpring.atom2Residue,atomSpring.atom2Name);
    // make sure groundLocation is not NaN or Inf.  This applies even if .toGround is false, because it should anyhow be well formed:
    //cout<<__FILE__<<":"<<__LINE__<<" Validating atom spring : "<<endl;
    //printAtomSpring(  atomSpring);
    ValidateVec3(atomSpring.groundLocation);
    ValidateReal(atomSpring.forceConstant);
    ValidateReal(atomSpring.deadLength);
};

void AtomSpringContainer::validateAtomSpring(const AtomSpring & atomSpring,  BiopolymerClassContainer & myBiopolymerContainer ){
    // a significant amount of validation is already being done in ParameterReader.cpp, when the atomSpring or related command is read.
    // the following two calls will ensure that the desired atoms exist:
    myBiopolymerContainer.updBiopolymerClass(atomSpring.atom1Chain).atomPathString(atomSpring.atom1Residue,atomSpring.atom1Name);
    if (! atomSpring.toGround) // if it's .toGround, then there is no second atom
    	myBiopolymerContainer.updBiopolymerClass(atomSpring.atom2Chain).atomPathString(atomSpring.atom2Residue,atomSpring.atom2Name);
    // make sure groundLocation is not NaN or Inf.  This applies even if .toGround is false, because it should anyhow be well formed:
    //cout<<__FILE__<<":"<<__LINE__<<" Validating atom spring : "<<endl;
    //printAtomSpring(  atomSpring);
    ValidateVec3(atomSpring.groundLocation);
    ValidateReal(atomSpring.forceConstant);
    ValidateReal(atomSpring.deadLength);
};

void AtomSpringContainer::addAtomSpring(const AtomSpring & atomSpring, BiopolymerClassContainer & myBiopolymerClassContainer){
    validateAtomSpring(atomSpring, myBiopolymerClassContainer);
    atomSpringVector.push_back(atomSpring);
}

void AtomSpringContainer::deleteAtomSpring(int id){
    if(id < 0 || id >= atomSpringVector.size()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": you tried to delete a non existing AtomSpring." << endl;
        ErrorManager::instance.treatError();
    }
    atomSpringVector.erase(atomSpringVector.begin()+id);
}

void AtomSpringContainer::updateAtomSpring(const int id, const AtomSpring & newSpring, BiopolymerClassContainer & myBiopolymerClassContainer){
    if(id < 0 || id >= atomSpringVector.size()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": you tried to update a non existing AtomSpring." << endl;
        ErrorManager::instance.treatError();
    }
    validateAtomSpring(newSpring, myBiopolymerClassContainer);
    atomSpringVector[id] = newSpring;
}

void AtomSpringContainer::clear()
{
    atomSpringVector.clear();
    clearThreading();
    clearGappedThreading();
}

void AtomSpringContainer::clearThreading(){
    threadingStructVector.clear();
}

void AtomSpringContainer::validateThreading(const ThreadingStruct & thread, BiopolymerClassContainer & myBiopolymerClassContainer){
    BiopolymerClass & bpc1 = myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1);
    BiopolymerClass & bpc2 = myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2);

    if(bpc1.getBiopolymerType() != bpc2.getBiopolymerType())
    {
       ErrorManager::instance << __FILE__ << " " << __LINE__ << ": In the threading command, botch chains must be of the same type." << endl;
       ErrorManager::instance.treatError(); 
    }

    if( thread.residueStart1 > thread.residueEnd1)
    {
        ErrorManager::instance << __FILE__ << " " << __LINE__ << ": In the threading command, the end residue must be greater than or equal to the start residue for each chain." << endl;
        ErrorManager::instance.treatError();
    }
    if( thread.residueStart2 > thread.residueEnd2)
    {
        ErrorManager::instance << __FILE__ << " " << __LINE__ << ": In the threading command, the end residue must be greater than or equal to the start residue for each chain." << endl;
        ErrorManager::instance.treatError();
    }
    if( bpc1.difference(thread.residueEnd1,thread.residueStart1) != bpc2.difference(thread.residueEnd2,thread.residueStart2))
    {
        ErrorManager::instance << __FILE__ << " " << __LINE__ << ": In the threading command, the two threaded segments must be of the same length." << endl;
        ErrorManager::instance.treatError();
    }
}

void AtomSpringContainer::addThreading(const ThreadingStruct & threadingStruct, 
                                       BiopolymerClassContainer & myBiopolymerClassContainer){
    validateThreading(threadingStruct, myBiopolymerClassContainer);
    threadingStructVector.push_back(threadingStruct);
}

void AtomSpringContainer::addThreading(String chain1, ResidueID resStart1, ResidueID resEnd1, 
                                       String chain2, ResidueID resStart2, ResidueID resEnd2, 
                                       double forceConstant, bool backboneOnly, 
                                       BiopolymerClassContainer & myBiopolymerClassContainer){
    ThreadingStruct thread(chain1, resStart1, resEnd1, chain2, resStart2, resEnd2, forceConstant, backboneOnly);
    addThreading(thread, myBiopolymerClassContainer);
}

void AtomSpringContainer::deleteThreading(int id){
    if(id < 0 || id >= threadingStructVector.size()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": you tried to delete a non existing Threading." << endl;
        ErrorManager::instance.treatError();
    }
    threadingStructVector.erase(threadingStructVector.begin()+id);
}

void AtomSpringContainer::updateThreading(int id, const ThreadingStruct & newThread, 
                                          BiopolymerClassContainer & myBiopolymerClassContainer){
    if(id < 0 || id >= threadingStructVector.size()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": you tried to update a non existing Threading." << endl;
        ErrorManager::instance.treatError();
    }
    validateThreading(newThread, myBiopolymerClassContainer);
    threadingStructVector[id] = newThread;
}

void AtomSpringContainer::updateThreading(int id, String chain1, ResidueID resStart1, ResidueID resEnd1, 
                                          String chain2, ResidueID resStart2, ResidueID resEnd2, 
                                          double forceConstant, bool backboneOnly, 
                                          BiopolymerClassContainer & myBiopolymerClassContainer){
    if(id < 0 || id >= threadingStructVector.size()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": you tried to update a non existing Threading." << endl;
        ErrorManager::instance.treatError();
    }
    ThreadingStruct newThread(chain1, resStart1, resEnd1, chain2, resStart2, resEnd2, forceConstant, backboneOnly);
    this->updateThreading(id, newThread, myBiopolymerClassContainer);
}

void AtomSpringContainer::createSpringsFromThreading(BiopolymerClassContainer & myBiopolymerClassContainer)
{
    vector<ThreadingStruct>::iterator it;
    for(it = threadingStructVector.begin(); it != threadingStructVector.end(); it++){
        ThreadingStruct & thread = *it;
        BiopolymerClass & bp1 = myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1);
        int threadedLength = bp1.difference (thread.residueEnd1 , thread.residueStart1) + 1;

        if(bp1.getBiopolymerType() == BiopolymerType::Protein && thread.backboneOnly)
        {
            for (int i = 0; i < threadedLength; i++) 
            {
                AtomSpring myAtomSpring1(thread.chainID1, myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1).sum(thread.residueStart1 , i), String("N"),
                                         thread.chainID2, myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).sum(thread.residueStart2 , i), String("N"),
                                         thread.forceConstant
                                        );

                AtomSpring myAtomSpring2(thread.chainID1, myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1).sum(thread.residueStart1 , i), String("CA"),
                                         thread.chainID2, myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).sum(thread.residueStart2 , i), String("CA"),
                                         thread.forceConstant
                                        );

                AtomSpring myAtomSpring3(thread.chainID1, myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1).sum(thread.residueStart1 , i), String("C"),
                                         thread.chainID2, myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).sum(thread.residueStart2 , i), String("C"),
                                         thread.forceConstant
                                        );
                this->add   (myAtomSpring1);
                this->add   (myAtomSpring2);
                this->add   (myAtomSpring3);

            }        
        }
        else
        {
            for (int i = 0; i < threadedLength; i++) 
            {
                ResidueInfo myResidueInfoA = myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1).updResidueInfo(myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1).sum( thread.residueStart1 , i));

                ResidueInfo myResidueInfoB = myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).updResidueInfo(myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).sum(thread.residueStart2 , i));
                for (int j = 0; j < (int)myResidueInfoA.getNumAtoms(); j++) {
                    String atomNameA =  myResidueInfoA.getAtomName(ResidueInfo::AtomIndex (j));
                    if (
                        ((
                          (atomNameA.substr(0,1).compare("0") == 0) || 
                          (atomNameA.substr(0,1).compare("1") == 0) || 
                          (atomNameA.substr(0,1).compare("2") == 0) || 
                          (atomNameA.substr(0,1).compare("3") == 0) || 
                          (atomNameA.substr(0,1).compare("4") == 0) || 
                          (atomNameA.substr(0,1).compare("5") == 0) || 
                          (atomNameA.substr(0,1).compare("6") == 0) || 
                          (atomNameA.substr(0,1).compare("7") == 0) || 
                          (atomNameA.substr(0,1).compare("8") == 0) || 
                          (atomNameA.substr(0,1).compare("9") == 0) 
                         )
                         &&
                         (atomNameA.substr(1,1 ).compare("H") == 0)) ||
                        (atomNameA.substr(0,1 ).compare("H") == 0) 
                       ) 
                    { // do nothing; leaving out hydrogens
                    } else {
                        if (myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).hasAtom( myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).sum (thread.residueStart2 , i),atomNameA)) {

                            AtomSpring myAtomSpring1(thread.chainID1, 
                                                     myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1).sum(thread.residueStart1 , i), 
                                                     atomNameA,
                                                     thread.chainID2, 
                                                     myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).sum(thread.residueStart2 , i), 
                                                     atomNameA,
                                                     thread.forceConstant
                                                    );
                            //cout<<__FILE__<<":"<<__LINE__<<" Created atomSpring for proteinThreading: atomNameA, thread.chainID1, residueA, thread.chainID2, residueB : >"<<atomNameA<<"< " <<thread.chainID1<<", "<<thread.residueStart1 + i<<", "<<thread.chainID2<<", "<<thread.residueStart2 + i  <<endl;
                            this->add(myAtomSpring1);

                        }
                    } //of if not H
                } // of for numatoms

            } // of for residues
        }
    }
}

void AtomSpringContainer::clearGappedThreading(){
    gappedThreadingStructVector.clear();
}

// Creates a gapped alignment using only explicitly specified stretches of residues in the two aligned chains.
ThreadingStruct AtomSpringContainer::createGappedThreading(String chain1, ResidueID startResidue1,  ResidueID endResidue1, String chain2,  ResidueID startResidue2,  ResidueID endResidue2,  double forceConstant, bool backboneOnly, BiopolymerClassContainer & myBiopolymerClassContainer)
{
    ThreadingStruct thread;
    thread.chainID1 = chain1;
    thread.chainID2 = chain2;
    ResidueStretch  myResidueStretch1 = ResidueStretch(chain1, startResidue1, endResidue1);
    ResidueStretch  myResidueStretch2 = ResidueStretch(chain2, startResidue2, endResidue2);
    if (!(myBiopolymerClassContainer.updBiopolymerClass(chain1).hasResidueStretch(myResidueStretch1))) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": You have specified an invalid residue stretch: Chain "<<chain1<<" from residue "<<startResidue1.outString()<<" to "<<endResidue1.outString() << endl;
        ErrorManager::instance.treatError();
    }
    if (!(myBiopolymerClassContainer.updBiopolymerClass(chain2).hasResidueStretch(myResidueStretch2))) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": You have specified an invalid residue stretch: Chain "<<chain2<<" from residue "<<startResidue2.outString()<<" to "<<endResidue2.outString() << endl;
        ErrorManager::instance.treatError();
    }
    thread.residueStart1 = startResidue1;//  myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1).getFirstResidueID();
    thread.residueStart2 = startResidue2; //myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).getFirstResidueID();
    thread.residueEnd1 = endResidue1; // myBiopolymerClassContainer.updBiopolymerClass(thread.chainID1).getLastResidueID();
    thread.residueEnd2 = endResidue2; //myBiopolymerClassContainer.updBiopolymerClass(thread.chainID2).getLastResidueID();
    thread.forceConstant = forceConstant;
    thread.backboneOnly = backboneOnly;
    std::cout<<__FILE__<<":"<<__LINE__<<": Created a ThreadingStruct connecting chain "<<thread.chainID1<<" residue "<<thread.residueStart1.outString() <<" to "<<endResidue1.outString()<<" . "<<std::endl;
    std::cout<<__FILE__<<":"<<__LINE__<<":                                   to chain "<<thread.chainID2<<" residue "<<thread.residueStart2.outString() <<" to "<<endResidue2.outString()<<" . "<<std::endl;
    return thread;
}

// Creates a gapped alignment using all residues in the two aligned chains.
ThreadingStruct AtomSpringContainer::createGappedThreading(String chain1, String chain2, double forceConstant, bool backboneOnly, BiopolymerClassContainer & myBiopolymerClassContainer){
    ResidueID myStartResidue1 = myBiopolymerClassContainer.updBiopolymerClass(chain1).getFirstResidueID();
    ResidueID myStartResidue2 = myBiopolymerClassContainer.updBiopolymerClass(chain2).getFirstResidueID();
    ResidueID myEndResidue1   = myBiopolymerClassContainer.updBiopolymerClass(chain1).getLastResidueID();
    ResidueID myEndResidue2   = myBiopolymerClassContainer.updBiopolymerClass(chain2).getLastResidueID();
    return createGappedThreading(chain1, myStartResidue1, myEndResidue1 , chain2, myStartResidue2,  myEndResidue2, forceConstant, backboneOnly, myBiopolymerClassContainer);
}

void AtomSpringContainer::addGappedThreading(const ThreadingStruct & threadingStruct, 
                                       BiopolymerClassContainer & myBiopolymerClassContainer){
    // validateThreading(threadingStruct, myBiopolymerClassContainer);
    gappedThreadingStructVector.push_back(threadingStruct);
}

void AtomSpringContainer::addGappedThreading(String chain1, String chain2, double forceConstant, bool backboneOnly, 
                                       BiopolymerClassContainer & myBiopolymerClassContainer){
    ThreadingStruct thread = createGappedThreading(chain1, chain2, forceConstant, backboneOnly, myBiopolymerClassContainer);
    addGappedThreading(thread, myBiopolymerClassContainer);
}
void AtomSpringContainer::addGappedThreading(String chain1, ResidueID startResidue1,  ResidueID endResidue1,   String chain2, ResidueID startResidue2,  ResidueID endResidue2,  double forceConstant, bool backboneOnly, 
                                       BiopolymerClassContainer & myBiopolymerClassContainer){
    ThreadingStruct thread = createGappedThreading(chain1, startResidue1,endResidue1, chain2,startResidue2,endResidue2, forceConstant, backboneOnly, myBiopolymerClassContainer);
    addGappedThreading(thread, myBiopolymerClassContainer);
}

void AtomSpringContainer::deleteGappedThreading(int id){
    if(id < 0 || id >= gappedThreadingStructVector.size()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": you tried to delete a non existing Threading." << endl;
        ErrorManager::instance.treatError();
    }
    gappedThreadingStructVector.erase(gappedThreadingStructVector.begin()+id);
}

void AtomSpringContainer::updateGappedThreading(int id, const ThreadingStruct & newThread, 
                                          BiopolymerClassContainer & myBiopolymerClassContainer){
    if(id < 0 || id >= gappedThreadingStructVector.size()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": you tried to update a non existing Threading." << endl;
        ErrorManager::instance.treatError();
    }
    // validateThreading(newThread, myBiopolymerClassContainer);
    gappedThreadingStructVector[id] = newThread;
}

void AtomSpringContainer::updateGappedThreading(int id, String chain1, String chain2, double forceConstant, bool backboneOnly, 
                                          BiopolymerClassContainer & myBiopolymerClassContainer){
    if(id < 0 || id >= gappedThreadingStructVector.size()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": you tried to update a non existing Threading." << endl;
        ErrorManager::instance.treatError();
    }
    ThreadingStruct newThread = createGappedThreading(chain1, chain2, forceConstant, backboneOnly, myBiopolymerClassContainer);
    this->updateGappedThreading(id, newThread, myBiopolymerClassContainer);
}

void AtomSpringContainer::createSpringsFromGappedThreading(BiopolymerClassContainer & myBiopolymerClassContainer)
{
    vector<ThreadingStruct>::iterator it;
    for(it = gappedThreadingStructVector.begin(); it != gappedThreadingStructVector.end(); it++)
    {
        ThreadingStruct & thread = *it;
        cout << "ThreadForceConstant " << thread.forceConstant << endl;
        String chainA = thread.chainID1;
        String chainB = thread.chainID2;
        BiopolymerClass & bpA = myBiopolymerClassContainer.updBiopolymerClass(chainA);
        BiopolymerClass & bpB = myBiopolymerClassContainer.updBiopolymerClass(chainB);

        typedef seqan::String<char> TSequence;                 // sequence type
        typedef seqan::Align<TSequence,seqan::ArrayGaps> TAlign;      // align type           
        TSequence seqA = bpA.getSubSequence(thread.residueStart1,thread.residueEnd1).c_str();  // Need a new BiopolymerClass method which retrieves subsequences.!
        //TSequence seqA = bpA.getSequence().c_str();  // Need a new BiopolymerClass method which retrieves subsequences.!
        //exit(1); // force death here, need to rework this method.
        TSequence seqB = bpB.getSubSequence(thread.residueStart2, thread.residueEnd2 ).c_str();
        //TSequence seqB = bpB.getSequence().c_str();
        TAlign align;

        seqan::resize(rows(align), 2);
        assignSource(row(align,0),seqA);
        assignSource(row(align,1),seqB); 
        // simple alignment: 
        int score = globalAlignment(align, seqan::Score<int,seqan::Simple>(0,-1,-1)); // ..signature:Score<TValue, Simple>(match, mismatch, gap [, gap_open])
        //int score = globalAlignment(align, seqan::Score<int,seqan::ScoreMatrix<seqan::AminoAcid, seqan::Blosum62_> >(-1000,-1000 )) ; 
        //"62" means matrix was constructed with max 62% seq. identity alignment. 

        std::cout << "Score: " << score << ::std::endl;
        std::cout << align << ::std::endl;
        cout<<__FILE__<<":"<<__LINE__<<" : "<< seqan::row(align,0)<<endl;
        cout<<__FILE__<<":"<<__LINE__<<" : "<< seqan::row(align,1)<<endl;
        int aIndex = 0; int bIndex = 0; // Indices which count over residues in chains A and B.

        int i  = 0; // counts over columns in alignment
        while ((aIndex < bpA.getSubSequence(thread.residueStart1,thread.residueEnd1 ).length()) && 
               (bIndex < bpB.getSubSequence(thread.residueStart2, thread.residueEnd2 ).length()   )) 
        {
            if ((String(seqan::row (align,0)[i]).compare("-")  != 0  )  &&
                (String(seqan::row (align,1)[i]).compare("-")  != 0  )) 
            { 
                cout<<__FILE__<<":"<<__LINE__<<" Applying threading forces to "<<thread.chainID1<<" : "<<bpA.sum(thread.residueStart1   , aIndex).outString()<<" to "<<thread.chainID2<<" : "<<bpB.sum(thread.residueStart2,bIndex).outString()<<std::endl;
                ResidueInfo myResidueInfoA = bpA.updResidueInfo(bpA.sum(thread.residueStart1   , aIndex  )) ;
                ResidueInfo myResidueInfoB = bpB.updResidueInfo(bpB.sum(thread.residueStart2   , bIndex)) ;
                for (int j = 0; j < (int)myResidueInfoA.getNumAtoms(); j++) 
                {
                    String atomNameA =  myResidueInfoA.getAtomName(ResidueInfo::AtomIndex (j));
                    if (
                            ((
                              (atomNameA.substr(0,1).compare("0") == 0) || 
                              (atomNameA.substr(0,1).compare("1") == 0) || 
                              (atomNameA.substr(0,1).compare("2") == 0) || 
                              (atomNameA.substr(0,1).compare("3") == 0) || 
                              (atomNameA.substr(0,1).compare("4") == 0) || 
                              (atomNameA.substr(0,1).compare("5") == 0) || 
                              (atomNameA.substr(0,1).compare("6") == 0) || 
                              (atomNameA.substr(0,1).compare("7") == 0) || 
                              (atomNameA.substr(0,1).compare("8") == 0) || 
                              (atomNameA.substr(0,1).compare("9") == 0) 
                             )
                             &&
                             (atomNameA.substr(1,1 ).compare("H") == 0)) ||
                            (atomNameA.substr(0,1 ).compare("H") == 0) 
                       ) 
                    { // do nothing; leaving out hydrogens
                    } else 
                    {
                        if (bpB.hasAtom(bpB.sum(thread.residueStart2  , bIndex),atomNameA)) 
                        {

                            AtomSpring myAtomSpring1(chainA, bpA.sum(thread.residueStart1  , aIndex), atomNameA,
                                                     chainB, bpB.sum(thread.residueStart2  , bIndex), atomNameA,
                                                     thread.forceConstant
                                                    );

                            this->add(myAtomSpring1);

                        }
                    } //of if not H
                } // of for numatoms
            } //End if seqan...
            if (String(seqan::row (align,0)[i]).compare("-")  != 0  ) {
                aIndex ++;
            }
            if (String(seqan::row (align,1)[i]).compare("-")  != 0  ) {
                bIndex ++;
            }
            i++;
        } // End While
    } // End for
}
