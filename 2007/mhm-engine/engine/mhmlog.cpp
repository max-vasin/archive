//==========================================================================================
// Mayhem engine log implementation
// Coded:	MozG
// Created:	7 may 2006
//==========================================================================================
#include <windows.h>
#include "mhmbase.h"
#include "mhmlog.h"

clsLog* g_plog = mmnull;

// Log constructor
//==========================================================================================
clsLog::clsLog(const mmString strfile):m_file(strfile)
{
	m_defclr = enLCWhite;
	m_opened = mmfalse;
	if (g_plog == mmnull)
		g_plog = this;
}

// Log destructor
//==========================================================================================
clsLog::~clsLog()
{
Close();
}

// Open: opens log without file clearing
//==========================================================================================
mmRes clsLog::Open()
{
	if (!m_file.IsExist())
		return MHMLOG_FILE_NOT_EXIST;
	m_opened = mmtrue;
	return mmOk;
}

mmRes clsLog::Message(clsString str)
{
	if (m_file.Open() != mmOk)
		return MHMLOG_FAILED;
	if (m_file.Seek(0, enSeekEnd) != mmOk)
		return MHMLOG_FAILED;
	if (m_file.Write(str) != mmOk)
		return MHMLOG_FAILED;
	if (m_file.Close() != mmOk)
		return MHMLOG_FAILED;
	return mmOk;
}

// Create: flushes log file and open it
//==========================================================================================
mmRes clsLog::Create()
{
	if (m_file.Create() != mmOk)
		return MHMLOG_FAILED;

	Message("<HTML><BODY bgcolor = black><PRE>");
	
	m_opened = mmtrue;
	return mmOk;
}

// SetDefaultColor: sets text default color
//==========================================================================================
mmVoid clsLog::SetDefaultColor(enLC color)
{
	if (color != enLCDefault)
		m_defclr = color;
}

// Out: writes string to log
//==========================================================================================
mmVoid clsLog::Out(mmString str, enLC color, mmBool nextline)
{
	clsString s(str);
	
	Out(s,color,nextline);
}
// Out: writes string to log
//==========================================================================================
mmVoid clsLog::Out(clsString & str, enLC color, mmBool nextline)
{
	if (!m_opened)
		return;
	
	if (m_file.Open() != mmOk)
		return;
	
	if (m_file.Seek(0, enSeekEnd) != mmOk)
		return;
	
	if (str.IsEmpty())
		return;
	
	if (color == enLCDefault)
		color = m_defclr;
		
	if (m_file.Write(clsString("<FONT color = white> >> </FONT><FONT color = ")) != mmOk)
		return;
	switch(color)
	{
	case enLCRed:
		if (m_file.Write(clsString("red>")) != mmOk)
			return;
		break;
	case enLCGreen:
		if (m_file.Write(clsString("lightgreen>")) != mmOk)
			return;
		break;
	case enLCYellow:
		if (m_file.Write(clsString("yellow>")) != mmOk)
			return;
		break;
	case enLCWhite:
		if (m_file.Write(clsString("white>")) != mmOk)
			return;
		break;
	case enLCBlue:
		if (m_file.Write(clsString("blue>")) != mmOk)
			return;
		break;
	}
	if (m_file.Write(str) != mmOk)
		return;
	if (m_file.Write(clsString("</FONT>")) != mmOk)
		return;
	if (nextline)
		m_file.Write(clsString("\n"));
	
	m_file.Close();
}

// OutBreak: puts break line into HTML code
//==========================================================================================
mmVoid clsLog::OutBreakLine()
{
	if (!m_opened)
		return;
	if (m_file.Open() != mmOk)
		return;
	if (m_file.Seek(0, enSeekEnd) != mmOk)
		return;
	m_file.Write(clsString("<HR/>"));
	m_file.Close();
}

// Close: closes log file
//==========================================================================================
mmVoid clsLog::Close()
{
	//Message("</PRE></BODY</HTML>");
	m_opened = mmfalse;
}

// SetAsCommon: sets this log object as common for all engine objects
//==========================================================================================
mmVoid clsLog::SetAsCommon()
{
	g_plog = this;
}

// GetFileName: returns log filename
//==========================================================================================
const clsString& clsLog::GetFileName()
{
	return m_file.GetFileName();
}
