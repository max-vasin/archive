//==========================================================================================
// Mayhem engine file class
// Coded:	MozG
// Created:	7 may 2006
// !!! === !!!
// ¡Àﬂ“‹, Õ≈ ”¡»–¿“‹ »‘ƒ≈‘€ œŒ «»œ”
//==========================================================================================

#ifndef _MHMFILE_INCLUDED
#define _MHMFILE_INCLUDED

#define MHM_MAX_FILE_STRING		1023

//===========================================================
// File seeking type
//===========================================================
enum enSeekType
{
	enSeekBegin,
	enSeekEnd,
	enSeekCurrent
};

//===========================================================
// Error codes
//===========================================================
#define MHMFILE_FAILED			mmOk + 1
#define MHMFILE_EOF				mmOk + 2
#define MHMFILE_NOT_OPENED		mmOk + 3
#define MHMFILE_INVALID_SIZE	0xFFFFFFFF

//===========================================================
// File class
//===========================================================
class clsFile
{
protected:
	HANDLE		m_hfile;		// current file handle
	clsString	m_strname;		// current file name
	mmDword		m_curpos;		// current file position

	// fyrex 10 june 2006
	mmChar		*m_zipmemory;	// memory after unzip

public:
	mmUint		m_zipsize;
	
	clsFile(const mmString strname);					// <-- file constructor
	clsFile(const mmString strname, mmUint zipindex);	// <-- zip file constructor
	clsFile(clsFile& file);
   ~clsFile();
    mmRes   Close();
    mmRes   Open();
    mmRes   Create();	
    mmBool  IsExist();
    mmBool  IsOpened();
    mmRes   Write(mmVoid* pdata, mmDword size);
    mmRes   Read(mmVoid* pdata,  mmDword size);
    mmRes   Delete();
	mmDword GetSize();
    mmDword GetPointer();
    mmRes   Seek(mmLong seek, enSeekType type);
    mmRes   CopyContents(clsFile* pifsrc);
    mmRes   Write(clsString& str, mmBool bzeroterm = mmfalse);
    mmRes   Read(clsString& str, mmBool bzeroterm = mmfalse);
    
	mmRes   ReadToMemory(mmVoid** pdata); // need to "FreeMemory(pdata)" !!!
	mmVoid  FreeMemory(mmVoid* pdata);
	clsString	ReadToTemp();

    const clsString& GetFileName();
	void	NewName(const mmString strname);

};


mmBool OpenZipEnvironment(const mmString zip_name = "startup.dat");
mmVoid CloseZipEnvironment();
mmVoid AddZipEnvironment(mmChar *);




#endif // _MHMFILE_INCLUDED