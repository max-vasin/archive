//==========================================================================================
// Mayhem 3DS MAX custom attributes plugin header file
// Coded:	MozG
// Created:	20 june 2006
//==========================================================================================
#include <stdio.h>
#include <shlobj.h>
#include "max.h"
#include "resource.h"
#include "iparamm.h"
#include "stdmat.h"
#include "bmmlib.h"
#include "utilapi.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "CustAttrib.h"
#include <guplib.h>
#include "ICustAttribContainer.h"
#include "IMtlEdit.h"
#include "Notify.h"

class clsMayhemMapCA;
TCHAR* GetString(int id);

//============================================================
// Map custom attributes class ID
//============================================================
#define MHM_MAP_CUSTOM_ATTRIB_CLASS_ID		Class_ID(0x6a014fe0, 0x279d670c)


