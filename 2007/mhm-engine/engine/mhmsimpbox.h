//==========================================================================================
// Mayhem engine simple box class
// Coded:	MozG
// Created:	29 may 2006
//==========================================================================================

#ifndef _MHMSIMPBOX_INCLUDED
#define _MHMSIMPBOX_INCLUDED

//==============================================================
// Simple box parameters
//==============================================================
class clsSimpleBoxParams: public clsResourceParams
{
public:
	mmFloat	m_size;		// box side lingth
	mmDword m_vclr;		// vertex color
//	mmBool  m_vtxclr;	// enable vertex color channel
//	mmBool  m_normals;	// enable normals channel
//	mmBool  m_mapping;	// enable texture coords channel
};

//==============================================================
// Simple box class
//==============================================================
class clsSimpleBox: public clsBaseMesh
{
protected:
	mmFloat	m_size;
	
public:
	clsSimpleBox(clsResourceManager* presman, clsString name);
   ~clsSimpleBox();
    
    mmRes Create(clsResourceParams* params);
    mmRes FlipNormals();
    mmRes FlipVertexOrder();
};


#endif // _MHMSIMPBOX_INCLUDED