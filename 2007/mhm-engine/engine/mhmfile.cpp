//==========================================================================================
// Mayhem file class implementation
// Coded:	MozG
// Created:	7 may 2006
//==========================================================================================
#include "mhmbase.h"


//==========================================================================================
// ZIP section
// Coded:	fyrex
// Created:	11 june 2006
//==========================================================================================
#pragma region ZIP section

static mmBool ZipEnvironment = mmfalse;
static mmInt ZipEnvironmentCount;

#ifdef MHM_ZIPUSE

extern "C"
{
	void WINAPI MZIP_Init();
	void WINAPI MZIP_Pak(int, char*);
	void WINAPI MZIP_Free();
	int WINAPI MZIP_Load(char*, char **);
	int WINAPI MZIP_Exist(char*);
};

#else

//extern "C"
//{
	__inline void WINAPI MZIP_Init(){};
	__inline void WINAPI MZIP_Pak(int, char*){};
	__inline void WINAPI MZIP_Free(){};
	__inline int WINAPI MZIP_Load(char*, char **){return 0;};
	__inline int WINAPI MZIP_Exist(char*){return 0;};
//};


#endif


mmBool OpenZipEnvironment(const mmString zip_name)
{
	MZIP_Init();
	MZIP_Pak(0, zip_name);
	ZipEnvironmentCount = 1;
	
	// add own zips here (1..6)
	//
	//

	// look on OpenZipEnvironment(...
//	MZIP_Pak(0, "firedata.dat");
	
	ZipEnvironment = mmtrue;

	return mmtrue;
};

mmVoid AddZipEnvironment(mmChar * ZipFile)
{
	if(ZipEnvironmentCount<8)
	{
		MZIP_Pak(ZipEnvironmentCount++, ZipFile);
	}
}

mmVoid CloseZipEnvironment()
{
	MZIP_Free();
	ZipEnvironment = mmfalse;
}

#pragma endregion 

// File constructor with name as argument
//==========================================================================================
clsFile::clsFile(const mmString strname)
{
	m_strname = strname;
	m_hfile   = mmnull;
	m_curpos  = 0;


	m_zipmemory = mmnull;
	m_zipsize = 0;

}

// File copy constructor
//==========================================================================================
clsFile::clsFile(clsFile& file)
{
	m_strname = file.m_strname;
	m_hfile   = file.m_hfile;
	m_curpos  = file.m_curpos;


	m_zipmemory = file.m_zipmemory; // ???
	m_zipsize = file.m_zipsize;

}

// File destructor
//==========================================================================================
clsFile::~clsFile()
{
	Close();
}

void clsFile::NewName(const mmString strname)
{
	Close();
	m_strname = strname;
}

// Close: closes file
//==========================================================================================
mmRes clsFile::Close()
{	
	if (m_hfile != mmnull)
	{
		if (CloseHandle(m_hfile) == 0)
			return MHMFILE_FAILED;
//		if (g_plog && g_plog->GetFileName() != m_strname)
//		{
//			g_plog->Out("File: [" + m_strname + "] closed", enLCWhite);
//		}
	}
	else if(m_zipsize) // test for zip
	{
		free(m_zipmemory); m_zipmemory = mmnull; m_zipsize = 0;
	}

	m_hfile  = mmnull;
	m_curpos = 0;
	return mmOk;
}

// Open: opens file
//==========================================================================================
mmRes clsFile::Open()
{
	m_hfile = CreateFile(m_strname,
						 GENERIC_WRITE|GENERIC_READ, 0,
						 mmnull, OPEN_EXISTING,
						 mmnull, mmnull);
	
	if (m_hfile == INVALID_HANDLE_VALUE)
	{
		// try unzip
		m_hfile = mmnull;

		m_zipsize = 0; // NO ZIP
		if(ZipEnvironment)
		{

		if((m_zipsize = MZIP_Load(m_strname, &m_zipmemory)) == 0)
		{
			return MHMFILE_FAILED;
		}
		}

	}
//	if (g_plog && g_plog->GetFileName() != m_strname)
//	{
//		g_plog->Out("File: [" + m_strname + "] opened", enLCWhite);
//	}
	m_curpos = 0;
	return mmOk;
}

// Create: creates new file
//==========================================================================================
mmRes clsFile::Create()
{
	m_hfile = CreateFile(m_strname, 
						 GENERIC_WRITE,
					 	 0, mmnull,
						 CREATE_ALWAYS,
						 FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, 
						 mmnull);

	if (m_hfile == INVALID_HANDLE_VALUE)
	{
		m_hfile = mmnull;
		return MHMFILE_FAILED;
	}
	Close();
	return mmOk;
}

// IsExist: returns true if file with specified name already exist
//==========================================================================================
mmBool clsFile::IsExist()
{
	m_hfile = CreateFile(m_strname, GENERIC_READ, 0, mmnull, OPEN_EXISTING, mmnull, mmnull);
	if (m_hfile == INVALID_HANDLE_VALUE)
	{
		m_hfile = mmnull;
	#pragma region try zip test

		// --- zip test
		if(ZipEnvironment)
		{
		if(MZIP_Exist(m_strname))
		{
			return mmtrue;
		}
		}

	#pragma endregion 
		return mmfalse;
	}
	else
	{
		CloseHandle(m_hfile);
		m_hfile = mmnull;
		return mmtrue;
	}
}

// Read: reads given number of bytes from file to memory buffer
//==========================================================================================
mmRes clsFile::Read(mmVoid* pdata, mmDword size)
{

	#pragma region try zip first

	if(m_zipsize)
	{
		if( (m_curpos + size) > m_zipsize )
		{
			size = m_zipsize - m_curpos;
			if(size == 0)
			{
				return MHMFILE_EOF;
			}
		}
		
		memcpy(pdata, m_zipmemory + m_curpos, size);
		
		m_curpos += size;
		return mmOk;
	}

	#pragma endregion 

	DWORD readed;
	
	if (ReadFile(m_hfile, pdata, size, &readed, NULL) == 0)
	{
		if (GetLastError() == ERROR_HANDLE_EOF)
			return MHMFILE_EOF;
		else
			return MHMFILE_FAILED;
	}
	if (readed != size)
		if (readed == 0)
			return MHMFILE_EOF;
		else
			return MHMFILE_FAILED;
	m_curpos += size;
	return mmOk;
}

// Read: overload for string-class reading
//==========================================================================================
mmRes clsFile::Read(clsString& str, mmBool bzeroterm)
{
//	DWORD readed;
	str.Alloc(MHM_MAX_FILE_STRING);
	mmInt index = 0;
	mmRes res;

	do
	{
		/*
		if (ReadFile(m_hfile, &str[index], sizeof(mmChar), &readed, NULL) == 0)
		{
			if (GetLastError() == ERROR_HANDLE_EOF)
				return MHMFILE_EOF;
			else
				return MHMFILE_FAILED;
		}
		if (readed != sizeof(mmChar))
			if (readed == 0)
				return MHMFILE_EOF;
			else
				return MHMFILE_FAILED;
		*/
		
		res  = Read(&str[index],1);

		if( res != mmOk )
		{
			break;
		}

		if (bzeroterm)
		{
			if (str[index] == 0) break;
		}
		else
		{
			if (str[index] == 0x0A) break;
		}
		index++;
	}while (1);
	
	//m_curpos += (index + 1);
	//if (str[index] == 0x0A)
	
	str[index] = 0;
		
	str.Truncate();

	return res;
}

// Write: writes bytes from memory to file
//==========================================================================================
mmRes clsFile::Write(mmVoid* pdata, mmDword size)
{
	DWORD written;
	if (WriteFile(m_hfile, pdata, size, &written, NULL) == 0)
		return MHMFILE_FAILED;
	if (written != size)
		return MHMFILE_FAILED;
	m_curpos += size;
	return mmOk;
}

// Write: writes string-class object to memory
//==========================================================================================
mmRes clsFile::Write(clsString& str, mmBool bzeroterm)
{
	DWORD written;
	mmChar buf = 0;
	if (str.IsEmpty()) 
		return MHMFILE_FAILED;
	if (WriteFile(m_hfile, str, str.Length() + (bzeroterm ? 1 : 0), &written, NULL) == 0)
		return MHMFILE_FAILED;
	if (written != (str.Length() + (bzeroterm ? 1 : 0)))
		return MHMFILE_FAILED;
	m_curpos += (str.Length() + (bzeroterm ? 1 : 0));
	return mmOk;
}

// Seek: sets file pointer
//==========================================================================================
mmRes clsFile::Seek(mmLong seek, enSeekType type)
{
	#pragma region try zip first

	if(m_zipsize)
	{
		mmDword m_curpos2 = m_curpos;

		switch (type)
		{
		case enSeekBegin:
			m_curpos2 = seek;
			break;
		case enSeekCurrent:
			m_curpos2 += seek;
			break;
		case enSeekEnd:
			m_curpos2 = GetSize() - seek;
			break;
		}
		if( (m_curpos2 >= 0) && (m_curpos2 <= m_zipsize))
		{
			m_curpos = m_curpos2;
			return mmOk;
		}
		else
		{
			return MHMFILE_FAILED;
		}
	}

	#pragma endregion 

	mmDword movemethod;

	switch (type)
	{
		case enSeekBegin:
			movemethod = FILE_BEGIN;
			m_curpos = seek;
			break;
		case enSeekCurrent:
			movemethod = FILE_CURRENT;
			m_curpos += seek;
			break;
		case enSeekEnd:
			movemethod = FILE_END;
			m_curpos = GetSize() - seek;
			break;
	}
	if (SetFilePointer(m_hfile, seek, NULL, movemethod) == INVALID_SET_FILE_POINTER)
		return MHMFILE_FAILED;
	return mmOk;
}

// Delete: deletes file
//==========================================================================================
mmRes clsFile::Delete()
{
	if (DeleteFile(m_strname) != 0)
		return MHMFILE_FAILED;
	m_curpos = 0;
	return mmOk;
}

// GetFileName: returns file name
//==========================================================================================
const clsString& clsFile::GetFileName()
{
	return m_strname;
}


// GetSize: returns file size
//==========================================================================================
mmDword clsFile::GetSize()
{

	if(m_zipsize) return m_zipsize;

	mmDword size = GetFileSize(m_hfile, mmnull);
	if (size == INVALID_FILE_SIZE)
		return MHMFILE_INVALID_SIZE;
	return size;
}

// IsOpened: returns true if file currently opened
//==========================================================================================
mmBool clsFile::IsOpened()
{

	if(m_zipsize)
	{
		return mmtrue;
	}

	return (m_hfile != mmnull);
}

// CopyContents: copy contents of one file to another
//==========================================================================================
mmRes clsFile::CopyContents(clsFile* pfile)
{
	mmDword fsize;
	mmByte* pbuf;
	mmDword processed;
	mmDword block;
	
	mmRes res = pfile->Seek(0, enSeekBegin);
	if (res != mmOk)
		return res;
		
	fsize = pfile->GetSize();

#define COPY_HEAP	(64*1024)
	
	pbuf  = new mmByte[COPY_HEAP];
	
	processed = 0;
	while (processed < fsize)
	{
		block = ((fsize - processed) > COPY_HEAP) ? COPY_HEAP : fsize - processed;
		res = pfile->Read(pbuf, block);
		if (res != mmOk)
			return res;
		res = Write(pbuf, block);
		if (res != mmOk)
			return res;
		processed += block;
	}
	m_curpos += processed;
	mmSafeDeleteAr(pbuf);
	return mmOk;
}

// GetPointer: returns current file pointer position
//==========================================================================================
mmDword clsFile::GetPointer()
{
	return m_curpos;
}


// ReadToMemory: just read all to temp memory
//==========================================================================================
mmRes clsFile::ReadToMemory(mmVoid** pdata)
{

	#pragma region try zip 

	if(m_zipsize)
	{
		*pdata = m_zipmemory;
		if(*pdata)
		{
			return mmOk;
		}
		else
		{
			return mmErr;
		}
	}

	#pragma endregion 

	register mmDword Size = GetSize();
	
	if(Size == MHMFILE_INVALID_SIZE) *pdata = mmnull;
		else *pdata = new mmByte[Size];
	
	if(*pdata)
	{
		Read(*pdata, Size);
		return mmOk;
	}
	else
	{
		 return mmErr;
	}
}

// FreeMemory: free temp memory
//==========================================================================================
mmVoid clsFile::FreeMemory(mmVoid* pdata)
{

	if(m_zipsize)
	{
	#pragma region try zip 
	//	free(pdata);
	}
	else
	{
		mmSafeDeleteAr(pdata);
	}


}

clsString clsFile::ReadToTemp()
{
	clsString TF;
	char temppath[MAX_PATH];
	char mp_tempfile[MAX_PATH*2];
	HANDLE hfile;
	mmVoid *filedata;
	mmUlong i;


	ReadToMemory(&filedata);

	GetTempPath(MAX_PATH,temppath);
	GetTempFileName(temppath,"mhm",0,mp_tempfile);

	if ((hfile = CreateFile(mp_tempfile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_TEMPORARY,NULL)))
	{
		WriteFile(hfile,filedata,GetSize(),&i,NULL);
		CloseHandle(hfile);
		TF = mp_tempfile;
	}
	else TF= "";

	FreeMemory(filedata);
	return TF;
}

