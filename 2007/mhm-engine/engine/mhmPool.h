//==========================================================================================
// Mayhem engine memory pool class
// Coded:	MozG
// Created:	7 august 2006
//==========================================================================================
#ifndef MHM_POOL_INCLUDED
#define MHM_POOL_INCLUDED

//======================================================
// Pool memory block
//======================================================
class clsMemBlock
{
public:
	clsMemBlock*	m_pNext;
	clsMemBlock*	m_pPrev;
	mmByte*			m_pData;
	mmUint			m_nSize;
	mmBool			m_bFree;
/*	
public:
	clsMemBlock();
	clsMemBlock(mmVoid* pMem, mmUint nSize, clsMemBlock* pNext, clsMemBlock* pPrev);
   ~clsMemBlock();    */
};

//======================================================
// Pool
//======================================================
#define MHM_POOL_DEFAULT_SIZE	1048576 * 100 // (1 MB)

class clsMemoryPool
{
protected:
	mmUint			m_nSize;
	mmByte*			m_pMemory;
	clsMemBlock*	m_pBlocks;
	clsMemBlock*	m_pTail;
	mmUint			m_nHeadSize;
	

public:
	clsMemoryPool();
	clsMemoryPool(mmUint nMemorySize);
   ~clsMemoryPool();
	
	mmVoid* Alloc(mmUint nSize);
	mmVoid  Free(mmVoid* pMem);
};




#endif //MHM_POOL_INCLUDED

