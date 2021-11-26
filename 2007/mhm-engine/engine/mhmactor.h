//==========================================================================================
// Mayhem engine actor class
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================

#ifndef _MHMACTOR_INCLUDED
#define _MHMACTOR_INCLUDED

class clsActor
{
protected:
	clsString	m_name;
	clsMatrix	m_tform;
	
	
public:
	

	clsActor(clsString & name);
	clsActor();
   ~clsActor();
};


#endif // _MHMACTOR_INCLUDED