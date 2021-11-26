//==========================================================================================
// Mayhem engine INI file class
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================

#ifndef _MHMINI_INCLUDED
#define _MHMINI_INCLUDED

// Error codes
#define MHMINI_FAILED	mmOk + 1

class clsINIFile
{
protected:
	clsString m_name;
	
public:
	clsINIFile(const mmString name);
   ~clsINIFile();
    
    mmRes WriteString(clsString section, clsString key, clsString val);
    mmRes WriteInt	 (clsString section, clsString key, mmInt val);
    mmRes WriteFloat (clsString section, clsString key, mmFloat val);
    mmRes WriteBool  (clsString section, clsString key, mmBool val);
    
    mmRes ReadString (clsString section, clsString key, clsString& val);
    mmRes ReadInt	 (clsString section, clsString key, mmInt* pval);
    mmRes ReadFloat  (clsString section, clsString key, mmFloat* pval);
    mmRes ReadBool   (clsString section, clsString key, mmBool* pval);
    
    clsString GetName();
    mmBool	  IsExist();
};

#endif // _MHMINI_INCLUDED