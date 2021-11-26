//==========================================================================================
// Mayhem engine memory pool class
// Coded:	MozG
// Created:	7 august 2006
//==========================================================================================
#include "mhmbase.h"

//===============================================================
// Memory block
//===============================================================
/*clsMemBlock::clsMemBlock()
{
	m_pNext = mmnull;
	m_pPrev = mmnull;
	m_nSize = 0;
	m_pData = 0;
	m_bFree = mmtrue;
}
clsMemBlock::clsMemBlock(mmVoid *pMem, mmUint nSize, clsMemBlock *pNext, clsMemBlock* pPrev)
{
	m_pData = pMem;
	m_nSize = nSize;
	m_pNext = pNext;
	m_pPrev = pPrev;
	m_bFree = mmfalse;
}
clsMemBlock::~clsMemBlock()
{
	m_pData = mmnull;
	m_nSize = 0;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	m_bFree = mmtrue;
}
mmVoid clsMemBlock::FreeBlock()
{
	m_bFree = mmtrue;
}
mmVoid clsMemBlock::Alloc()
{
	if (m_pData != mmnull)
		mmSafeDelete(m_pData);
	m_pData = new mmByte[m_nSize];
	if (m_pData == mmnull)
		mmThrow(enExceptOutOfMemory, clsMemBlock, Alloc, unable to allocate block);
}*/
//===============================================================
// Memory pool class
//===============================================================
clsMemoryPool::clsMemoryPool()
{
	m_pMemory = new mmByte[MHM_POOL_DEFAULT_SIZE];
	if (m_pMemory == mmnull)
		mmThrow(enExceptOutOfBounds, clsMemoryPool, constructor, unable to alloc pool memory);
	m_nSize = MHM_POOL_DEFAULT_SIZE;
	m_nHeadSize = sizeof(clsMemBlock);	
	m_pBlocks   = mmnull;
	m_pTail     = mmnull;
}
clsMemoryPool::clsMemoryPool(mmUint nMemorySize)
{
	m_pMemory = new mmByte[nMemorySize];
	if (m_pMemory == mmnull)
		mmThrow(enExceptOutOfMemory, clsMemoryPool, constructor, unable to alloc pool memory);
	m_nSize = nMemorySize;	
	m_nHeadSize = sizeof(clsMemBlock);
	m_pBlocks = mmnull;
	m_pTail   = mmnull;
}
clsMemoryPool::~clsMemoryPool()
{
	mmSafeDeleteAr(m_pMemory);
}
mmVoid* clsMemoryPool::Alloc(mmUint nSize)
{
	// 1) first time call
	if (m_pBlocks == mmnull)
	{
		m_pBlocks = (clsMemBlock*)m_pMemory;
		m_pBlocks->m_bFree = mmfalse;
		m_pBlocks->m_nSize = nSize;
		m_pBlocks->m_pNext = mmnull;
		m_pBlocks->m_pPrev = mmnull;
		m_pBlocks->m_pData = m_pMemory + m_nHeadSize;
		m_pTail = m_pBlocks;
		return m_pBlocks->m_pData;
	}
	// 2) check all existing blocks
	clsMemBlock* pBlock = m_pBlocks;
	while(pBlock)
	{
		if ((pBlock->m_bFree) && (pBlock->m_nSize >= nSize))
		{
			if (pBlock->m_nSize == nSize)	// same size block
			{
				pBlock->m_bFree = mmfalse;
				return pBlock->m_pData;
			}
			// TODO
			// Здесь надо сделать разбитие на два блока!
			pBlock->m_nSize = nSize;
			pBlock->m_bFree = mmfalse;
			return pBlock->m_pData;
		}
		pBlock = pBlock->m_pNext;
	}
	
	// 3) Create new block
	mmByte* pFreeMem = m_pTail->m_pData + m_pTail->m_nSize;
	clsMemBlock* pNewBlock = (clsMemBlock*)pFreeMem;
	pNewBlock->m_bFree = mmfalse;
	pNewBlock->m_nSize = nSize;
	pNewBlock->m_pNext = mmnull;
	pNewBlock->m_pPrev = m_pTail;
	pNewBlock->m_pData = pFreeMem + m_nHeadSize;
	m_pTail->m_pNext = pNewBlock;
	m_pTail = pNewBlock;
	return pNewBlock->m_pData;
}
mmVoid clsMemoryPool::Free(mmVoid *pMem)
{
	clsMemBlock* pBlock = m_pBlocks;
	while(pBlock)
	{
		if (pBlock->m_pData == pMem)
		{
			pBlock->m_bFree = mmtrue;
			return;
		}
		pBlock = pBlock->m_pNext;
	}
}
