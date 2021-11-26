//==========================================================================================
// Mayhem engine entity factory class implementation
// Coded:	MozG
// Created:	26 july 2006
//==========================================================================================
#include "mhmbase.h"

// Entity factory construction
//==========================================================================================
clsEntityFactory::clsEntityFactory(clsEngine* pEngine)
{
	m_pEngine = pEngine;
}

// Entity factory destruction
//==========================================================================================
clsEntityFactory::~clsEntityFactory()
{
	m_pEngine = mmnull;
}

// CreateEntity: returns new entity by type
//==========================================================================================
clsEntity* clsEntityFactory::CreateEntity(const clsString& type,
										  const clsString& name)
{
	return new clsEntity(this, type, name);
}

// GetEngine: returns engine
//==========================================================================================
clsEngine* clsEntityFactory::GetEngine()
{
	return m_pEngine;
}