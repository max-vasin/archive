//==========================================================================================
// Mayhem engine resource class
// Coded:	MozG
// Created:	14 may 2006
//==========================================================================================

#ifndef _MHMRESOURCE_INCLUDED
#define _MHMRESOURCE_INCLUDED

// Error codes
//==========================================================================================
#define MHMRESOURCE_INVALID_ARGS	mmOk + 1
#define MHMRESOURCE_IO_ERROR		mmOk + 2
#define MHMRESOURCE_LIB_ERROR		mmOk + 3
#define MHMRESOURCE_NO_DEVICE		mmOk + 4

// Resources
//==========================================================================================
#define MHMRES_TEXTURE_ID		0x0001
#define MHMRES_MATERIAL_ID		0x0002
#define MHMRES_MESH_ID			0x0003
#define MHMRES_SHADER_ID		0x0004
#define MHMRES_MISC_ID			0x0005


#define mmMakeResourceID(baseclass, id)	(baseclass << 16) | id
#define mmHLID(id)  id >> 16
#define mmLLID(id)  id & 0x0000FFFF


// Resource types
//==========================================================================================
enum enResType
{
	enResNone			= 0x00000000,
// Textures
	enResBaseTexture	= mmMakeResourceID(MHMRES_TEXTURE_ID, 0),
	enResTextureTGA		= mmMakeResourceID(MHMRES_TEXTURE_ID, 1),
	enResFont2D			= mmMakeResourceID(MHMRES_TEXTURE_ID, 2),
	enResTextureJPG		= mmMakeResourceID(MHMRES_TEXTURE_ID, 3),
	enResTextureJP2		= mmMakeResourceID(MHMRES_TEXTURE_ID, 4),
	enResRectangle		= mmMakeResourceID(MHMRES_TEXTURE_ID, 5),


// Materials
	enResBaseMaterial	= mmMakeResourceID(MHMRES_MATERIAL_ID, 0),

// Meshes
	enResBaseMesh		= mmMakeResourceID(MHMRES_MESH_ID, 0),
	enResStaticMesh		= mmMakeResourceID(MHMRES_MESH_ID, 1),
	enResSimpleBox		= mmMakeResourceID(MHMRES_MESH_ID, 2),
	enResStaticMeshMMH  = mmMakeResourceID(MHMRES_MESH_ID, 3),
	enResText			= mmMakeResourceID(MHMRES_MESH_ID, 4),

// Shaders
	enResVertexShader	= mmMakeResourceID(MHMRES_SHADER_ID, 0),
	enResPixelShader	= mmMakeResourceID(MHMRES_SHADER_ID, 1),

// Misc
	enResFont				= mmMakeResourceID(MHMRES_MISC_ID, 0),
	enResBaseLight			= mmMakeResourceID(MHMRES_MISC_ID, 1),
	enResSprite				= mmMakeResourceID(MHMRES_MISC_ID, 2),
	enResAnimKeys			= mmMakeResourceID(MHMRES_MISC_ID, 3),
	enResBaseParticles		= mmMakeResourceID(MHMRES_MISC_ID, 4),
	
	enResDword			= 0xFFFFFFFF
};

// Blending types
//====================================================================
enum enBlendType
{
	enBlendNone			= 0,
	enBlendAdditive		= 1,
	enBlendSubtractive	= 2,
	enBlendAlpha		= 3
};


// Resource class
//==========================================================================================
class clsResourceManager;
class clsDeviceInvalidateEvent;
class clsDeviceRestoreEvent;

class clsResourceParams
{
public:
	// empty
};

class clsResource
{
friend clsResourceManager;
protected:
	clsResourceManager*	m_presman;	// pointer to resource manager that owns this resource
	clsString			m_name;		// resource name (ID)
	clsString			m_fullname;	// resource full name (+path, etc)
	enResType			m_type;		// resource type id
	mmUint				m_refs;		// ref counter
	mmBool				m_invalid;	// invalidation flag
	enBlendType			m_eBlendType;	// blending type
	
public:
	clsResource(clsResourceManager* presman, clsString name, enResType type);
    virtual ~clsResource();
    
    clsResource* AddRef();
    mmVoid Release();
    clsString GetName();
    enResType GetType();
    enBlendType GetBlendType();
    mmVoid  SetBlendType(enBlendType type);
    virtual mmRes Create(clsResourceParams* params);
    virtual mmRes Invalidate(clsDeviceInvalidateEvent* pevent);
    virtual mmRes Restore(clsDeviceRestoreEvent* pevent);
    virtual mmRes Activate(clsResource* powner);
    virtual mmRes Deactivate(clsResource* powner);
    virtual mmRes Render();
    virtual mmRes Update();
    virtual clsResource* OnAddReference();
    virtual mmVoid OnRelease();
    

protected:
	mmRes   SetBlendMode();
};

#endif // _MHMRESOURCE_INCLUDED