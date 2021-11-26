//==========================================================================================
// Mayhem engine log
// Coded:	MozG
// Created:	7 may 2006
//==========================================================================================

#ifndef _MHMLOG_INCLUDED
#define _MHMLOG_INCLUDED

// Log colors enumerator
enum enLC
{
	enLCDefault = 0,
	enLCRed	    = 1,
	enLCGreen	= 2,
	enLCYellow  = 3,
	enLCWhite	= 4,
	enLCBlue	= 5
};

// Log error codes
#define MHMLOG_FAILED			mmOk + 1
#define MHMLOG_FILE_NOT_EXIST	mmOk + 2

class clsLog
{
protected:
	clsFile m_file;
	enLC	m_defclr;
	mmBool  m_opened;

public:
	clsLog(const mmString file);
   ~clsLog();
    

    mmRes  Message(clsString str);
    mmRes  Create();
    mmRes  Open();
    mmVoid SetDefaultColor(enLC color);
    mmVoid Out(clsString & str, enLC color = enLCDefault, mmBool nextline = mmtrue);
    mmVoid Out(mmString str, enLC color = enLCDefault, mmBool nextline = mmtrue);
    mmVoid OutBreakLine();
    mmVoid Close();
    mmVoid SetAsCommon();
    const clsString& GetFileName();
};

extern clsLog* g_plog;

#endif // _MHMLOG_INCLUDED