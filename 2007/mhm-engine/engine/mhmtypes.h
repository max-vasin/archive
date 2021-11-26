//==========================================================================================
// Mayhem engine basic types definitions
// Coded:	MozG
// Created: 7 may 2006
//==========================================================================================

#ifndef _MHMTYPES_INCLUDED
#define _MHMTYPES_INCLUDED

// Integer support
typedef __int8				mmInt8;
typedef __int16				mmInt16;
typedef __int32				mmInt, mmInt32;
typedef __int64				mmInt64, mmLargeInt;
typedef long				mmLong;

// Unsigned integer support
typedef unsigned __int8		mmUint8;
typedef unsigned __int16	mmUint16, mmWord;
typedef unsigned __int32	mmUint32, mmDword, mmUint;
typedef unsigned __int64	mmUint64, mmULargeInt;
typedef unsigned long		mmUlong;

// Boolean support
typedef bool				mmBool;
#define	mmtrue				true
#define mmfalse				false

// Floating point support
typedef float				mmFloat;
typedef double				mmDouble;

// Char, string support
typedef char				mmChar;
typedef unsigned char		mmUchar, mmByte;
typedef char*				mmString;
//#define mmString char*

// Void
typedef void				mmVoid;
#ifdef __cplusplus
#define mmnull	0
#else
#define mmnull  (mmVoid*)0
#endif

// Result type
typedef mmInt				mmRes;
#define	mmOk				0

#define	mmError				-1
#define	mmErr				-1
#define mmFail				-1
#define MHMSUCC(x)			(x==0)
#define MHMFAIL(x)			(x!=0)

// Api
#ifdef MHM_EXPORT
#define mmApi __declspec(dllexport)
#else
#define mmApi __declspec(dllimport)
#endif

#endif // _MHMTYPES_INCLUDED