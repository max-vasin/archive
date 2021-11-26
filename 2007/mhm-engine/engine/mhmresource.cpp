//==========================================================================================
// Mayhem engine resource class implementation
// Coded:	MozG
// Created:	14 may 2006
//------
// Edited	16 may 2006:
// added loadable flag
// added Load method
// base method Create and Load become not pure functions and puts warnings to log
// MozG
//==========================================================================================
#include "mhmbase.h"


// Resource constructor
//==========================================================================================
clsResource::clsResource(clsResourceManager *presman,
						 clsString name,
						 enResType type)
{
	m_presman    = presman;
	m_name	     = name;
	m_type       = type;
	m_refs	     = 0;
	m_invalid    = mmfalse;
	m_eBlendType = enBlendNone;
}

// Resource destructor
//==========================================================================================
clsResource::~clsResource()
{
	if (m_refs != 0)
	{
		g_plog->Out("Resource [" + m_name + "] destroyed", enLCBlue);	
		m_refs = 0;
	}
}

// AddRef: increments refcounter
//==========================================================================================
clsResource* clsResource::AddRef()
{
	clsResource* res = OnAddReference();
	m_refs++;
	return res;
}

// Release: decrements refcounter
//==========================================================================================
mmVoid clsResource::Release()
{
	m_refs--;
	OnRelease();	// <-- if you breaks here something wrong with reference manager
	if (m_refs == 0)
	{
		m_presman->ReleaseResource(this);
		delete this;
	}
}

// GetName: returns name of resource
//==========================================================================================
clsString clsResource::GetName()
{
	return m_name;
}

// GetType: returns type of resource
//==========================================================================================
enResType clsResource::GetType()
{
	return m_type;
}

// GetBlendType: returns type of resource blending
//==========================================================================================
enBlendType clsResource::GetBlendType()
{
	return m_eBlendType;
}

// SetBlendType: sets type of resource blending
//==========================================================================================
mmVoid clsResource::SetBlendType(enBlendType type)
{
	m_eBlendType = type;
}

// Create: dummy base resource method, just log out warning
//==========================================================================================
mmRes clsResource::Create(clsResourceParams *params)
{
	g_plog->Out("clsResource::Create[" + m_name + "] >> base resource method called, something wrong with resource class", enLCYellow);
	return mmOk;
}

// Invalidate: use this to invalidate your resource
//==========================================================================================
mmRes clsResource::Invalidate(clsDeviceInvalidateEvent *pevent)
{
	return mmOk;
}

// Restore: use this to restore your resources
//==========================================================================================
mmRes clsResource::Restore(clsDeviceRestoreEvent *pevent)
{
	return mmOk;
}

// Render: renders the resource
//==========================================================================================
mmRes clsResource::Render()
{
	return mmOk;
}

// Activate: activates resource
//==========================================================================================
mmRes clsResource::Activate(clsResource* powner)
{
	return mmOk;
}

// Deactivate: deactivates resource
//==========================================================================================
mmRes clsResource::Deactivate(clsResource* powner)
{
	return mmOk;
}

// Update: updates resource
//==========================================================================================
mmRes clsResource::Update()
{
	return mmOk;
}


// OnAddReference: event
//==========================================================================================
clsResource* clsResource::OnAddReference()
{
	return this;
}

// OnRelease: event
//==========================================================================================
mmVoid clsResource::OnRelease()
{
}

// SetBlendMode: sets current resource blending mode
//==========================================================================================
mmRes clsResource::SetBlendMode()
{
	MHMDirect3DDevice* pDev = *m_presman->GetDevice();
	if (pDev == mmnull)
		return mmFail;
		
	switch (m_eBlendType)
	{
	case enBlendNone:
		pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		return mmOk;
	case enBlendAdditive:
		pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE );
		pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		return mmOk;
	case enBlendAlpha:
		pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		return mmOk;
	default:
		pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		return mmOk;
	}
	return mmOk;
}


