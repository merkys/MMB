/* -------------------------------------------------------------------------- *
 *                           MMB (MacroMoleculeBuilder)                       *
 * -------------------------------------------------------------------------- *
 *                                                                            *
 * Copyright (c) 2011-12 by the Author.                                       *
 * Author: Samuel Flores                                                      *
 *                                                                            *
 * See RNABuilder.cpp for the copyright and usage agreement.                  *
 * -------------------------------------------------------------------------- */

#include "Utils.h"

//#include <boost/lexical_cast.hpp>


#include  <sstream> 
using namespace std;
using namespace SimTK;


ConstraintClass::ConstraintClass(){
        chain1 = ""; residueID1 = ResidueID(); atomName1 = ""; 
        chain2 = ""; residueID2 = ResidueID(); atomName2 = ""; 
        constraintType = WeldToGround ; 
        };  
ConstraintClass::ConstraintClass(String myChain, ResidueID inputResidueID,String myAtomName) {
        residueID1 = (inputResidueID);
        atomName1 = myAtomName;
        chain1 = myChain;
        residueID2 = ResidueID();
        atomName2 = "" ;    
        chain2 = ""; 
        constraintType = ( WeldToGround );
        //toGround = true ;
    }; 

ConstraintClass::ConstraintClass(String myChain, ResidueID inputResidueID,String myAtomName,String myChain2, ResidueID inputResidueID2,String myAtomName2, ConstraintType myConstraintType) {
        residueID1 = (inputResidueID);
        //residueID1.setInsertionCode ( residueID1.getInsertionCode());
        atomName1 = myAtomName;
        chain1 = myChain;
        residueID2 = (inputResidueID2);
        atomName2 = myAtomName2;
        chain2 = myChain2;
        setConstraintType(myConstraintType);
        //toGround = false;
    };
/*
Array_<MobilizedBodyIndex> ConstraintClass::fetchMobilizedBodyIndexArray_(BiopolymerClassContainer myBiopolymerClassContainer,SimbodyMatterSubsystem & matter ) {
        Array_< MobilizedBodyIndex >    coordMobod(2);
        coordMobod[0] =  myBiopolymerClassContainer.updBiopolymerClass(chain1).getAtomMobilizedBodyIndex(matter,residueID1    ,atomName1    );
        coordMobod[1] =  myBiopolymerClassContainer.updBiopolymerClass(chain2).getAtomMobilizedBodyIndex(matter,residueID2    ,atomName2    );
        return coordMobod;
    };
Array_<MobilizerQIndex> ConstraintClass::fetchMobilizerQIndexArray_(BiopolymerClassContainer myBiopolymerClassContainer,SimbodyMatterSubsystem & matter, State & state) {
        Array_< MobilizerQIndex >       coordQIndex(2);
        coordQIndex[0] =  MobilizerQIndex(0); //mobilizedBody1.getMobilizerQIndex(state);
        coordQIndex[1] =  MobilizerQIndex(0); // // mobilizedBody2.getFirstQIndex(state);
        return coordQIndex;
    }
*/

void ConstraintClass::setConstraintType (ConstraintType myConstraintType) 
{
    constraintType = myConstraintType;
}

ConstraintType ConstraintClass::getConstraintType () const
{
    return constraintType ;
}

String ConstraintClass::constraintTypeString () const  
{  // const promises not to change the object, i.e. ConstraintClass
        if (constraintType == WeldToAtom) { return  "WeldToAtom" ;}
        else if (constraintType == WeldToGround) {  return  "WeldToGround" ;}
        else if (constraintType == CoupledCoordinate) {  return  "CoupledCoordinate" ;}
        else if (constraintType == Undefined) { return "Undefined" ;}
        else return " ERROR! ";
}


void  ConstraintClass::print() const {
        std::cout<<__FILE__<<":"<<__LINE__   // to here is fine
          <<" : Chain ID : "      <<getChain1()
          <<" Residue    ID: "    <<getResidueID1().outString()
          <<" atom name : "       <<getAtomName1()
          <<" : Chain ID2 : "     <<getChain2()
          <<" Residue    ID2: "   <<getResidueID2().outString()
          <<" atom name2 : "      <<getAtomName2()
          //<<" to Ground: "        <<getToGround()
          <<" constraintType : " << constraintTypeString()
          <<endl;
    };  




int IntLen(const char* cstr)
{
  int    k, n = 0;
  if (cstr)
  {
    n = strspn(cstr,spaces);
    cstr += n;
    if (*cstr == '-' || *cstr == '+')
        ++cstr, ++n;
    k = strspn(cstr,digits);
    n = k?n+k:0;
  }
  return n;
}
/// <int>::[spaces][+|-]<digits>[garbage]

bool isNumber(const char* cstr)
{
  int    n = IntLen(cstr);
  if (n)
  {
    cstr += n;
    if (*cstr == 'e' || *cstr == 'E')
       n = strspn(++cstr,digits);
  }
  return n > 0;
}

bool isFixed (const String putativeFixedFloat) { // This checks that the string represents a floating point number in fixed format .. no scientific notation or other stray characters.
	int dotCount = 0;
	for (size_t i = 0 ; i < putativeFixedFloat.length() ; i ++) {
                //std::cout<<__FILE__<<":"<<__LINE__<<" putativeFixedFloat[i] = >"<<putativeFixedFloat[i]<<"< "<<std::endl;
		if (!((String(putativeFixedFloat[i]).compare("0") == 0) || 
		   (String(putativeFixedFloat[i]).compare("1") == 0) || 
		   (String(putativeFixedFloat[i]).compare("2") == 0) || 
		   (String(putativeFixedFloat[i]).compare("3") == 0) || 
		   (String(putativeFixedFloat[i]).compare("4") == 0) || 
		   (String(putativeFixedFloat[i]).compare("5") == 0) || 
		   (String(putativeFixedFloat[i]).compare("6") == 0) || 
		   (String(putativeFixedFloat[i]).compare("7") == 0) || 
		   (String(putativeFixedFloat[i]).compare("8") == 0) || 
		   (String(putativeFixedFloat[i]).compare("9") == 0) || 
		   (String(putativeFixedFloat[i]).compare(".") == 0) || 
		   (String(putativeFixedFloat[i]).compare("+") == 0) || 
		   (String(putativeFixedFloat[i]).compare("-") == 0)))  {
	 	        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<"Found a character : >"<<putativeFixedFloat[i]<< "< which is not of [0-9].+- .. this is not a fixed/float!"<<endl;
			ErrorManager::instance.treatError();
			return false; // actually we shouldn't get to this line
		}
		if (((String(putativeFixedFloat[i]).compare("+") == 0) || (String(putativeFixedFloat[i]).compare("-") == 0)) && (i > 0)) {
	 	        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<"You have tried to use '+' or '-' somewhere other than at the beginning of the number string.  This is not allowed!"<<endl;
			ErrorManager::instance.treatError();
			return false; // actually we shouldn't get to this line
		}
		if  (String(putativeFixedFloat[i]).compare(".") == 0) {
			dotCount++;
			if (dotCount > 1) {
			    ErrorManager::instance <<__FILE__<<":"<<__LINE__<<"You have tried to use more than one '.' .. This is not allowed!"<<endl;
			    ErrorManager::instance.treatError();
			    return false; // actually we shouldn't get to this line
			}
		} 
	} // of for
	return true;
}



    // a recursive algorithm for reading an integer from a String.  This String may contain ints, user variables (begin with @), +, and -.  No whitespaces or additional characters should be in the String.
    int   myAtoI(  map<const String,double> myUserVariables,  const char* value){
        size_t plusPosition  = String(value).find_last_of('+');
        size_t minusPosition = String(value).find_last_of('-');
        if ((plusPosition > minusPosition) && (plusPosition  != String::npos) )  minusPosition = String::npos;
        if ((plusPosition < minusPosition) && (minusPosition != String::npos) )  plusPosition  = String::npos;
        String baseIntegerString ;
        int          increment = -1111;
        //int          decrement = -1111;
        size_t lastPlusOrMinus = min(plusPosition,minusPosition);
        if ((lastPlusOrMinus != String::npos) && (lastPlusOrMinus != 0)) {
            baseIntegerString = String(value).substr(0, (lastPlusOrMinus + 0) ); // Put everything to the left of the last +/- into baseIntegerString
            String incrementString = (String(value).substr(lastPlusOrMinus+0,1000)); //  NOT adding 1 to lastPlusOrMinus means that the sign at lastPlusOrMinus goes with incrementString.
            cout<<__FILE__<<":"<<__LINE__<<" At this stage, we are adding >"<<baseIntegerString<<"< and >"<<incrementString<<"<"<<endl;           
            increment = myAtoI(myUserVariables, incrementString.c_str() );
            cout<<__FILE__<<":"<<__LINE__<<" "<<incrementString<<" was interpreted as >"<<increment<<"<"<<endl;
        } else if (lastPlusOrMinus == 0) { // There is a leading + or -, and  this is the only +/- in the whole expression.
            baseIntegerString = String(value).substr(1, 1000); // Put everything to from position 1 onwards into baseIntegerString
            if (plusPosition == 0) {
                cout<<__FILE__<<":"<<__LINE__<<" Detected that the base string : >"<<String(value) <<"< has a leading \'+\'. "<<endl;
                // Trim the leading '+' and return the rest    
                return myAtoI(myUserVariables, baseIntegerString.c_str() );
                //cout<<__FILE__<<":"<<__LINE__<<" Interpreted >"<<baseIntegerString<<"< as "<<increment<<endl; 
	    } else if (minusPosition == 0) {
                cout<<__FILE__<<":"<<__LINE__<<" Detected that the base integer string : >"<<baseIntegerString<<"< has a leading \'-\'.  Inverting sign."<<endl;
                // Trim the leading '-' and return the negative
                return -myAtoI(myUserVariables, baseIntegerString.c_str() );
                //cout<<__FILE__<<":"<<__LINE__<<" Interpreted >"<<baseIntegerString<<"< as "<<increment<<endl; 
            }
        }
        else { // no + or - found.
            if (!((increment == -1111 ))){// && (decrement == -1111 )  )) {
                ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" Unexplained error!"<<endl;
                ErrorManager::instance.treatError();
            }   
            baseIntegerString = String(value);
            increment = 0;
            //decrement = 0;
            int baseInteger;
            {   
                if ((baseIntegerString.substr(0,1)).compare("@") ==0) {
                    if (myUserVariables.find(baseIntegerString.c_str()) == myUserVariables.end())
                        {   
                        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": Undefined user variable "<<value<<endl;
                        ErrorManager::instance.treatError();
                        }   

                    double  intCast   = double(int(myUserVariables[baseIntegerString.c_str()]));
                    double  doubleCast = double(myUserVariables[baseIntegerString.c_str()]);
                    cout<<__FILE__<<":"<<__LINE__<<" Read user variable "<<baseIntegerString.c_str()<<"  which is set to : "<<myUserVariables[baseIntegerString.c_str()]<<endl;
                    SimTK_ERRCHK_ALWAYS(( (intCast) == doubleCast  ) ,"[ParameterReader.cpp]","Expected an int and got a non-integer");
                    baseInteger = int(myUserVariables[baseIntegerString.c_str()]);
                }   
                else if (isNumber(baseIntegerString.c_str()))
                {
                    double  intCast   = double(int(atof(baseIntegerString.c_str())));
                    double  doubleCast = double(atof(baseIntegerString.c_str()));
                    SimTK_ERRCHK_ALWAYS(( (intCast) == doubleCast  ) ,"[ParameterReader.cpp]","Expected an int and got a non-integer");
                    baseInteger = (atoi(baseIntegerString.c_str()));
                } else {
                    ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" : What you have entered: >"<<baseIntegerString<<"< is neither a variable (starting with @) nor an explicit number."<<endl;
                    ErrorManager::instance.treatError();
                }  
            }   
            //cout<<__FILE__<<":"<<__LINE__<<" : Result of "<<value<<" is : " <<  baseInteger <<endl;
            return baseInteger;
        }   

        int baseInteger = myAtoI(myUserVariables,baseIntegerString.c_str() ) ; 

        int finalInteger = baseInteger + increment ;//- decrement;
        cout<<__FILE__<<":"<<__LINE__<<" : Result of "<< value  <<" is : " << finalInteger <<endl;
        return finalInteger;
    }   




/*void printBiopolymerSequenceInfo(const Biopolymer & myBiopolymer) {
    for (int i = 0; i < myBiopolymer.getNumResidues(); i++) {
        cout<<__FILE__<<":"<<__LINE__<<" Residue type, number, and insertion code: "<<myBiopolymer.getResidue(ResidueInfo::Index(i)).getOneLetterCode() <<", "<<myBiopolymer.getResidue(ResidueInfo::Index(i)).getPdbResidueNumber()<<", "<<myBiopolymer.getResidue(ResidueInfo::Index(i)).getPdbInsertionCode()<<endl;
    }    
};*/


         String intToString(int i) {
		/*
                // if boost doesn't work, go back to the stringstream method:
                String s ; 
                std::stringstream out;
                out << i;
                s = out.str();
                return s;
		*/
		//return  boost::lexical_cast<String>(i);      
                return SimTK::String(i);
            };  

Vec3 ValidateVec3(Vec3 myVec3){

    if (! myVec3.isFinite()){
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" This Vec3 vector is infinite or not a number : "<<myVec3<<endl;
        ErrorManager::instance.treatError();
    }
    return myVec3;
    /*if      (std::isnan(myVec3[0])) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" x-component "<<myVec3[0] <<" of Vec3 "<<myVec3 <<" is invalid. "<<endl; ErrorManager::instance.treatError();}
    else if (std::isnan(myVec3[1])) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" y-component "<<myVec3[1] <<" of Vec3 "<<myVec3 <<" is invalid. "<<endl; ErrorManager::instance.treatError();}
    else if (std::isnan(myVec3[2])) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" z-component "<<myVec3[2] <<" of Vec3 "<<myVec3 <<" is invalid. "<<endl; ErrorManager::instance.treatError();}
    else return myVec3;
*/

}



int ValidateInt (const int myInt) {
	
                /*if      (std::isnan(myInt))      {
			ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" The integer is not valid : "<<myInt<<endl;
			ErrorManager::instance.treatError();
		}
                else if      (std::isinf(myInt)) {
			ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" The integer is not valid : "<<myInt<<endl;
			ErrorManager::instance.treatError();
		}
		else*/ return myInt;
}



int ValidateNonNegativeInt (const int myInt) {
	
                if (!(myInt >= 0)) {
			ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" Expected a nonnegative integer. "<<endl; 
			ErrorManager::instance.treatError();
		}
		else return myInt;
}




Real ValidateNonNegativeReal(const Real myReal) {
	
    if (isnan(myReal)) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" Not a number! "<<endl; 
        ErrorManager::instance.treatError();
    }
    if (isinf(myReal)) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" Not a number! "<<endl; 
        ErrorManager::instance.treatError();
    }
    if (!(myReal>= 0)) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" Expected a nonnegative Real   . "<<endl; 
        ErrorManager::instance.treatError();
    }
    return myReal;
}

Real ValidateReal(const Real myReal) {
	
    if (isnan(myReal)) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" Not a number! "<<endl; 
        ErrorManager::instance.treatError();
    }
    if (isinf(myReal)) {
        ErrorManager::instance <<__FILE__<<":"<<__LINE__<<" Not a number! "<<endl; 
        ErrorManager::instance.treatError();
    }
    return myReal;
}


Real DotProduct(const Vec3 vecA, const Vec3 vecB) {
	ValidateVec3(vecA);
	ValidateVec3(vecB);
	return (vecA[0]*vecB[0] + vecA[1]*vecB[1] + vecA[2]*vecB[2] );
}


vector<String> readAndParseLine   (ifstream & inFile) {
        stringstream u;
	String inString;
 	getline (inFile,inString);
	//u.str(inString);
	//cout<<__FILE__<<":"<<__LINE__<<" read: "<<inString<<endl;
	vector<String> mystring;
	
        istringstream iss(inString);

    	do
    	{
        	String sub;
        	iss >> sub;
        	//cout << "Substring: " << sub << endl;
		mystring.push_back(sub);
    	} while (iss);

	return mystring;
}

String removeAllWhite (String &str)
{
    String temp;
    for (int i = 0; i < str.length(); i++)
        if (std::string(str)[i] != ' ') temp += std::string(str)[i];
    str = temp;
    return str;
}

vector<String> readAndParseOnColWidth   (ifstream & inFile, int columnWidth) {
        stringstream u;
	String inString;
 	getline (inFile,inString);
	//u.str(inString);
	//cout<<__FILE__<<":"<<__LINE__<<" read: "<<inString<<endl;
	vector<String> mystring;
	
        istringstream iss(inString);
    	//for (int j = 0; j < numColumns; j++)
        	String sub;
	int j = 0;
	do
    	{
		sub = inString.substr(j * columnWidth,columnWidth);
		removeAllWhite(sub);
        	//cout << "Substring: >" << sub <<"<"<< endl;
		if (sub.length()>0)  mystring.push_back(sub);			
		j++;
    	} while (sub.length()>0);

	return mystring;
}

    void ParameterStringClass::validateNumFields(int correctNumFields) const{ // make sure we have the right number of parameters
        //exit(1);
        if (stringVector[correctNumFields-1].length()==0){
            ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": You have not specified enough parameters for this command."<<endl;
            ErrorManager::instance.treatError();
        } else if (stringVector[correctNumFields].length() >0) {
            ErrorManager::instance <<__FILE__<<":"<<__LINE__<<": You have specified too many parameters for this command."<<endl;
            ErrorManager::instance.treatError();
        };  
    };  

