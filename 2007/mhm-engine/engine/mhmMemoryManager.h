//==========================================================================================
// Mayhem engine memry manager object
// Coded:	MozG
// Create:	30 aug 2006
//==========================================================================================
#ifndef MHM_MEMMANAGER_INCLUDED
#define MHM_MEMMANAGER_INCLUDED

class clsMemoryManager
{
public:
	clsMemoryManager();
   ~clsMemoryManager();
    
    mmVoid* Alloc(mmDword dwSize);
    mmVoid  Free(mmVoid* pData);
};

#endif // MHM_MEMMANAGER_INCLUDED
