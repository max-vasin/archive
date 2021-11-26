//==========================================================================================
// Mayhem engine ini file class implementation
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================
#include "mhmbase.h"
#include "mhmini.h"

// INI file constructor
//==========================================================================================
clsINIFile::clsINIFile(const mmString name)
{
	m_name = name;
}

// INI file destructor
//==========================================================================================
clsINIFile::~clsINIFile()
{

}

// INI writing methods
//==========================================================================================
mmRes clsINIFile::WriteString(clsString section, clsString key, clsString val)
{
	if (WritePrivateProfileString(section, key, val, m_name) == 0)
		return MHMINI_FAILED;
	return mmOk;
}
//==========================================================================================
mmRes clsINIFile::WriteInt(clsString section, clsString key, mmInt val)
{
	clsString strval = val;
	if (WritePrivateProfileString(section, key, strval, m_name) == 0)
		return MHMINI_FAILED;
	return mmOk;
}
//==========================================================================================
mmRes clsINIFile::WriteFloat(clsString section, clsString key, mmFloat val)
{
	clsString strval;
	strval.FromFloat(val);
	if (WritePrivateProfileString(section, key, strval, m_name) == 0)
		return MHMINI_FAILED;
	return mmOk;
}
//==========================================================================================
mmRes clsINIFile::WriteBool(clsString section, clsString key, mmBool val)
{
	if (val)
		return WriteString(section, key, "true");
	return WriteString(section, key, "false");
}

//==========================================================================================
// INI file read methods
//==========================================================================================
mmRes clsINIFile::ReadString(clsString section, clsString key, clsString& val)
{
	val.Alloc(255);
	if (GetPrivateProfileString(section, key, "fail", val, 255, m_name) == 0)
		return MHMINI_FAILED;
	val.Truncate();
	if (val == "fail")
		return MHMINI_FAILED;
	return mmOk;
}
//==========================================================================================
mmRes clsINIFile::ReadInt(clsString section, clsString key, mmInt* pval)
{
	if (pval == mmnull)
		return mmfalse;
	clsString buf;
	buf.Alloc(30);
	if (GetPrivateProfileString(section, key, "fail", buf, 30, m_name) == 0)
		return MHMINI_FAILED;
	buf.Truncate();
	if (buf == "fail")
		return MHMINI_FAILED;
	if (!buf.IsInteger())
		return MHMINI_FAILED;
	*pval = atoi(buf);
	return mmOk;
}
//==========================================================================================
mmRes clsINIFile::ReadFloat(clsString section, clsString key, mmFloat* pval)
{
	if (pval == mmnull)
		return 1;
	clsString buf;
	buf.Alloc(30);
	if (GetPrivateProfileString(section, key, "fail", buf, 30, m_name) == 0)
		return MHMINI_FAILED;
	buf.Truncate();
	if (buf == "fail")
		return MHMINI_FAILED;
	if (!buf.IsFloat())
		return MHMINI_FAILED;
	*pval = buf.ToFloat();
	return mmOk;
}
//==========================================================================================
mmRes clsINIFile::ReadBool(clsString section, clsString key, mmBool* pval)
{
	if (pval == mmnull)
		return 1;
	clsString buf;
	buf.Alloc(30);
	if (GetPrivateProfileString(section, key, "fail", buf, 30, m_name) == 0)
		return MHMINI_FAILED;
	buf.Truncate();
	if (buf == "fail")
		return MHMINI_FAILED;
	if (buf == "true")
	{
		*pval = mmtrue;
		return mmOk;
	}
	*pval = mmfalse;
	return mmOk;
}

//==========================================================================================
// Miscellanous methods
//==========================================================================================
clsString clsINIFile::GetName()
{
	return m_name;
}
mmBool clsINIFile::IsExist()
{
	clsFile file(m_name);
	return file.IsExist();
}