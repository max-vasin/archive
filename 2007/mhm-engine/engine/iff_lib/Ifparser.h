// AIfparser.h: interface for the AIfparser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIFPARSER_H__92519C4F_04FC_4297_B96D_400A15679C1D__INCLUDED_)
#define AFX_AIFPARSER_H__92519C4F_04FC_4297_B96D_400A15679C1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !(defined(ULONG))
typedef unsigned long ULONG;
typedef unsigned char UBYTE;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int IFILE;

#ifdef _WIN32
#define GET_SIZE(a)         ( (ULONG) (( *((UBYTE*)a) )<<24) + ( (*(((UBYTE*)a)+1)) <<16) + ( (*(((UBYTE*)a)+2))<<8) + ( (*(((UBYTE*)a)+3) ) ) )
#define MAKE_ID(a,b,c,d)	((ULONG) (d)<<24 | (ULONG) (c)<<16 | (ULONG) (b)<<8 | (ULONG) (a))
#define SET_SIZE(a,b)		{ *( ((UBYTE*)(a)) + 0) = b>>24; *((UBYTE*)a + 1) = b>>16; *((UBYTE*)a + 2) = b>>8; *((UBYTE*)a + 3) = b;}

#if !defined(WINAPI)
#define WINAPI __stdcall
#endif

#else
#define GET_SIZE(a)         (*(ULONG*)a)
#define MAKE_ID(a,b,c,d)	((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#define SET_SIZE(a,b)		{ ((ULONG*)a) = b;}

#if !defined(WINAPI)
#define WINAPI
#endif

#endif
#define GET_ID(a)         (*(ULONG*)a)
//#define GET_ID(a)			GET_SIZE(a)

#define MAX_IFILE 32

typedef struct
IFileFORM
{
	ULONG	id;
	ULONG	start;
	ULONG	size;
	void *Next;
	void *Own;

} IFileFORM;

typedef struct
{
	ULONG	status;
	ULONG	pos;
	void * Select;
	IFileFORM First;
	union
	{
		void *mem;
		char name[256];
	};

} IFileStruct;

//extern "C"

 void WINAPI IF_Init();
//class Ifparser  
//{
//public:
	void * WINAPI IF_GetFound(IFILE f);
	int WINAPI IF_Next(IFILE f, ULONG *id);
	int WINAPI IF_LoadFound(IFILE f, void *target, int size);
	int WINAPI IF_SelectFound(IFILE f);
	int WINAPI IF_Find(IFILE f, ULONG id);
 	int WINAPI IF_Parent(IFILE f);
 	int WINAPI IF_WriteChunk(IFILE f, ULONG id, ULONG size, void *mem);
	IFILE WINAPI IF_FindSlot();
 	IFILE WINAPI IF_Create(char *name);
 	void WINAPI IF_Close(IFILE f);
	IFILE WINAPI IF_OpenMem(void *p);
 	void WINAPI IF_Release();
//	Ifparser();
//	virtual ~Ifparser();
//private:
//};

int WINAPI Normal2Index(float x,float y,float z);
void WINAPI Index2Normal(int i, float *x,float *y,float *z);

//Ifparser * CreateIFParser();

/* Universal IFF identifiers */
#define ID_FORM		MAKE_ID('F','O','R','M')
#define ID_LIST		MAKE_ID('L','I','S','T')
#define ID_CAT		MAKE_ID('C','A','T',' ')
#define ID_PROP		MAKE_ID('P','R','O','P')
#define ID_NULL		MAKE_ID(' ',' ',' ',' ')

// mayhem ids
#define ID_MHM2		MAKE_ID('M','H','M','2')
#define ID_TYPE		MAKE_ID('T','Y','P','E')
#define ID_OWNR		MAKE_ID('O','W','N','R')
#define ID_NAME		MAKE_ID('N','A','M','E')
#define ID_COMM		MAKE_ID('C','O','M','M')
#define ID_HEAD		MAKE_ID('H','E','A','D')


#ifdef __cplusplus
}
#endif

#endif // !defined(AFX_AIFPARSER_H__92519C4F_04FC_4297_B96D_400A15679C1D__INCLUDED_)
