//==========================================================================================
// Mayhem engine actor class implementation
// Coded:	MozG
// Created:	31 may 2006
//==========================================================================================

#include "mhmbase.h"
#include "mhmmath.h"
#include "mhmactor.h"


clsActor::clsActor(clsString & name)
{
	m_name = name;
	m_tform.Identity();
}
clsActor::clsActor()
{

}
clsActor::~clsActor()
{

}
