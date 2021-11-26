//==========================================================================================
// Mayhem engine variable class interface
// Coded:	MozG
// Created:	11 june 2006
//==========================================================================================

#ifndef _MHMVARIABLE_INCLUDED
#define _MHMVARIABLE_INCLUDED

//====================================================
// Variable types
//====================================================
enum enVarType
{
	enVarNone		= 0,
	enVarFloat		= 1,
	enVarInt		= 2,
	enVarString		= 3,
	enVarBool		= 4,
	enVarArray		= 5,
	enVarForceDword = 0xFFFFFFFF
};

//====================================================
// Variable class
//====================================================
class clsVarList;
class clsVariable
{
friend clsVarList;
protected:
	clsString	m_name;
	enVarType	m_type;
	union
	{
		mmFloat		val_float;
		mmInt		val_int;
		clsString*	val_string;
		mmBool		val_bool;
		clsVarList*	val_array;
	}m_value;

public:
	clsVariable(clsString name);
	clsVariable(clsString name, enVarType type);
    clsVariable(clsString name, mmFloat   val);
    clsVariable(clsString name, mmInt     val);
    clsVariable(clsString name, mmBool    val);
    clsVariable(clsString name, mmString  val);
    clsVariable(clsString name, clsVarList& val);
    clsVariable(clsVariable& var);
   ~clsVariable();
    enVarType  GetType();
    clsString& GetName();
    
    operator mmFloat();
    operator mmInt();
    operator mmString();
    operator mmBool();
    operator clsVarList&();
	
	mmFloat      operator = (mmFloat      val);
	mmInt        operator = (mmInt        val);
	mmString     operator = (mmString     val);
	mmBool       operator = (mmBool	      val);
	clsVarList&	 operator = (clsVarList&   val);
	clsVariable& operator = (clsVariable& var);
};

//====================================================
// Variables list class
//====================================================
class clsVarList: public tmpPtrList<clsVariable*>
{
public:
	clsVarList();
	clsVarList(clsVarList& list);
   ~clsVarList();
    
    clsVariable* FindVariable(const clsString& name) const;
    clsVarList& operator = (clsVarList& list);
};


#endif // _MHMVARIABLE_INCLUDED