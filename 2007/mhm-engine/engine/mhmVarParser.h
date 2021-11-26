//==========================================================================================
// Mayhem engine variable parser class interface
// Coded:	MozG
// Created:	11 june 2006
//==========================================================================================

#ifndef _MHMVARPARSER_INCLUDED
#define _MHMVARPARSER_INCLUDED

#define MHMVARPARSER_MAX_STRING		255

class clsVarParser
{
protected:
	clsVarList	m_varlist;

public:
	clsVarParser();
   ~clsVarParser();

   mmVoid Clear();
    
    mmRes      ParseMem(mmVoid* pmem, mmUint size);
    mmRes      ParseFile(mmString filename);
    clsVarList* GetVarList();

private:
	mmRes         BuildStringList(tmpPtrList<clsString*>& list, mmString pmem, mmUint size);
	clsVariable*  MakeVariable(const clsString& varname, clsString varval, mmUint strno);
};


#endif //_MHMVARPARSER_INCLUDED