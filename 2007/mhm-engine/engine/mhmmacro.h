//==========================================================================================
// Mayhem engine common macro definitions
// Coded:	MozG
// Created:	7 may 2006
//==========================================================================================

#ifndef _MHMMACRO_INCLUDED_
#define _MHMMACRO_INCLUDED_

#ifndef mmCopyMem
#define mmCopyMem(ptr, from, size)	memcpy(ptr, from, size)
#endif

#ifndef mmZeroMem
#define mmZeroMem(ptr, size)		memset(ptr, 0, size)
#endif

#ifndef mmZeroVar
#define mmZeroVar(var)				memset(&var, 0, sizeof(var))
#endif

#ifndef mmSafeDelete
#define mmSafeDelete(ptr)			if (ptr) {delete (ptr); (ptr) = mmnull;}
#endif

#ifndef mmSafeDeleteAr
#define mmSafeDeleteAr(ptr)			if (ptr) {delete [] ptr; ptr = mmnull;}
#endif

#ifndef mmSafeRelease
#define mmSafeRelease(ptr)			if (ptr) {ptr->Release(); ptr = mmnull;}
#endif


#endif // _MHMMACRO_INCLUDED_