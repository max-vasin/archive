//==========================================================================================
// Mayhem custom attributes MAX plugin GUP class
//==========================================================================================
#include "mhmca.h"

extern HINSTANCE g_hinst;

//===========================================================
// GUP class interface
//===========================================================
class clsMayhemGUP: public GUP, public ReferenceMaker
{
protected:
	bool     m_same;
	MtlBase *m_mbase;
	
public:
	clsMayhemGUP();
   ~clsMayhemGUP();
    
    // GUP
    DWORD Start();
    void  Stop();
    DWORD Control(DWORD prm);
    void  DeleteThis();
    
    // ReferenceMaker
    RefResult NotifyRefChanged(Interval ival,
			  				   RefTargetHandle htrg,
							   PartID& partID,
							   RefMessage message);
							   
	void SetReference(int i, ReferenceTarget* rtarg);
	ReferenceTarget* GetReference(int i);
	int NumRefs();
	
	// MozG
	bool IsValidObject(MtlBase* mtl);
	bool IsAttributesExist(MtlBase* mtl);
	void LinkAttributes(MtlBase* mtl);
	//void RemoveAttributes();
	void OnPostFileOpen(void *param, NotifyInfo *info);
};



//===========================================================
// GUP class description
//===========================================================
#define MHM_CUSTOM_ATTRIB_GUP_CLASS_ID		Class_ID(0x1bb35d0c, 0x62b031ec)

class clsMayhemGUPDesc: public ClassDesc2
{
	int 			IsPublic()				{return 1;}
	void*			Create(BOOL loading)	{return new clsMayhemGUP;}
	const TCHAR*	ClassName()				{return GetString(IDS_MAYHEM_GUP_CLASS_NAME);}
	SClass_ID		SuperClassID()			{return GUP_CLASS_ID;}
	Class_ID 		ClassID()				{return MHM_CUSTOM_ATTRIB_GUP_CLASS_ID;}
	const TCHAR* 	Category()				{return _T("");}
	const TCHAR*	InternalName()			{return _T("MayhemCustomAttribGUP");}
	HINSTANCE		HInstance()				{return g_hinst;}
};

static clsMayhemGUPDesc cdesc;

ClassDesc2* GetMayhemGUPDesc()		{ return &cdesc; }

//===========================================================
// GUP class implementation
//===========================================================

// Constructor
//==========================================================================================
clsMayhemGUP::clsMayhemGUP()
{
	m_same = false;
	m_mbase = NULL;
}

// Destructor
//==========================================================================================
clsMayhemGUP::~clsMayhemGUP()
{
	DeleteAllRefsFromMe();
}

// OnFilePostOpen: event, used to create additional attributes on old files
//==========================================================================================
void clsMayhemGUP::OnPostFileOpen(void *param, NotifyInfo *info)
{
/*	IMtlEditInterface *mtlEdit = (IMtlEditInterface *)GetCOREInterface(MTLEDIT_INTERFACE);
	MtlBase *mtl = mtlEdit->GetCurMtl();
	int defaultVis = 0;
    DependentIterator di(mtl); 
    RefMakerHandle rm; 
    while (NULL!=(rm=di.Next())) 
    { 
		if ((rm->SuperClassID() == REF_MAKER_CLASS_ID) &&  (rm->ClassID() == Class_ID(MEDIT_CLASS_ID,0))) 
		{
			if (IsValidObject((MtlBase*)rm))
				if (!IsAttributesExist((MtlBase*)rm))
					LinkAttributes((MtlBase*)rm);
		}
    }*/
}

// Start: called then GUP plugin loaded
//==========================================================================================
DWORD clsMayhemGUP::Start()
{
	// link all materials
	IMtlEditInterface *mtlEdit = (IMtlEditInterface *)GetCOREInterface(MTLEDIT_INTERFACE);
	MtlBase *mtl = mtlEdit->GetCurMtl();
	int defaultVis = 0;
    DependentIterator di(mtl); 
    RefMakerHandle rm; 
    while (NULL!=(rm=di.Next())) 
    { 
		if ((rm->SuperClassID() == REF_MAKER_CLASS_ID) &&  (rm->ClassID() == Class_ID(MEDIT_CLASS_ID,0))) {
			ReplaceReference(0,(ReferenceTarget*)rm,TRUE);
		}
    }
    
//    RegisterNotification(&clsMayhemGUP::OnPostFileOpen, this, NOTIFY_FILE_POST_OPEN);

	return GUPRESULT_KEEP;
}

// Stop: called then GUP plugin unloaded or deffered
//==========================================================================================
void clsMayhemGUP::Stop()
{
//	UnRegisterNotification(OnPostFileOpen, this, NOTIFY_FILE_POST_OPEN);
	DeleteAllRefsFromMe();	
}

// Control: ??? 
//==========================================================================================
DWORD clsMayhemGUP::Control(DWORD parameter)
{
	return 0;
}

// DeleteThis: self-delete
//==========================================================================================
void clsMayhemGUP::DeleteThis()
{
	delete this;
}

// NumRefs: ???
//==========================================================================================
int clsMayhemGUP::NumRefs()
{
	return 1;
}

// SetReference: ???
//==========================================================================================
void clsMayhemGUP::SetReference(int i, ReferenceTarget* rtarg)
{
	m_mbase = (MtlBase*)rtarg;
}

// GetReference: ???
//==========================================================================================
ReferenceTarget* clsMayhemGUP::GetReference(int i)
{
	return m_mbase;
}

// NotifyRefChanged: reference change hook
//==========================================================================================
RefResult clsMayhemGUP::NotifyRefChanged(Interval ival, 
										 RefTargetHandle htrg,
										 PartID &partID,
										 RefMessage message)
{
	IMtlEditInterface *mtlEdit;
	MtlBase *mtl;
	switch (message)
	{
		case REFMSG_TARGET_DELETED:
//			MessageBox(NULL, "Target deleted", "Notify", MB_OK);
			break;
		case REFMSG_TARGET_SELECTIONCHANGE:
//			MessageBox(NULL, "Target selection changed", "Notify", MB_OK);
			break;
		case REFMSG_SUBANIM_STRUCTURE_CHANGED:
//			MessageBox(NULL, "Target subanim structure changed", "Notify", MB_OK);
			mtlEdit = (IMtlEditInterface *)GetCOREInterface(MTLEDIT_INTERFACE);
			mtl     = mtlEdit->GetCurMtl();

			if (IsValidObject(mtl))
			{
				m_same = true;
				if (!IsAttributesExist(mtl))
					LinkAttributes(mtl);
			}
			if (m_same)
				return REF_STOP;
				
			break;

	}
	return REF_SUCCEED;
}

// IsValidObject: check reference target object
//==========================================================================================
bool clsMayhemGUP::IsValidObject(MtlBase* mtl)
{
	if(!mtl)
		return false;	
	
	if(mtl->SuperClassID() != TEXMAP_CLASS_ID)
		return false;
	
	// ok, its texture map
	//MessageBox(NULL, "This is texmap", "Notify", MB_OK);
	return true;
}

// IsAttributesExist: checks if attributes exists for texmap
//==========================================================================================
bool clsMayhemGUP::IsAttributesExist(MtlBase* mtl)
{
	if(!mtl)
		return false;
	
	
	ICustAttribContainer* cc = mtl->GetCustAttribContainer();
	if(!cc)
	{
		return false;
	}
	
	for(int i=0; i<cc->GetNumCustAttribs();i++)
	{
		CustAttrib * ca = cc->GetCustAttrib(i);
		if(ca->ClassID() == MHM_MAP_CUSTOM_ATTRIB_CLASS_ID)
			return true;
	}
	
	m_same = false;
	return true;
}

// LinkAttributes: links attributes to texmap
//==========================================================================================
void clsMayhemGUP::LinkAttributes(MtlBase* mtl)
{
	if(!mtl)
		return;		//only in very BAD cases or early ref messages
	if(IsAttributesExist(mtl))
		return;
		
	ICustAttribContainer* cc = mtl->GetCustAttribContainer();
	if(!cc)
	{
		mtl->AllocCustAttribContainer();
		cc = mtl->GetCustAttribContainer();
	}
	if(cc->GetNumCustAttribs()>0)
		cc->InsertCustAttrib(0,(CustAttrib *)CreateInstance(CUST_ATTRIB_CLASS_ID,MHM_MAP_CUSTOM_ATTRIB_CLASS_ID));
	else
		cc->AppendCustAttrib((CustAttrib *)CreateInstance(CUST_ATTRIB_CLASS_ID,MHM_MAP_CUSTOM_ATTRIB_CLASS_ID));
}

