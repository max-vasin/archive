//==========================================================================================
// Mayhem engine variable class implementation
// Coded:	MozG
// Created:	11 june 2006
//==========================================================================================
#include "mhmbase.h"
//#include "mhmVariable.h"

// Empty variable constructor with unknown type
//==========================================================================================
clsVariable::clsVariable(clsString name)
{
	m_name          = name;
	m_type          = enVarNone;
	m_value.val_int = 0;
}

// Empty variable constructor with specified type
//==========================================================================================
clsVariable::clsVariable(clsString name, enVarType type)
{
	m_name			= name;
	m_type			= type;
	m_value.val_int = 0;
}

// Float variable constructor
//==========================================================================================
clsVariable::clsVariable(clsString name, mmFloat val)
{
	m_name			  = name;
	m_type			  = enVarFloat;
	m_value.val_float = val;
}

// Integer variable constructor
//==========================================================================================
clsVariable::clsVariable(clsString name, mmInt val)
{
	m_name		    = name;
	m_type		    = enVarInt;
	m_value.val_int = val;
}

// Bool variable constructor
//==========================================================================================
clsVariable::clsVariable(clsString name, mmBool val)
{
	m_name		    = name;
	m_type		    = enVarBool;
	m_value.val_int = val;
}

// String variable constructor
//==========================================================================================
clsVariable::clsVariable(clsString name, mmString val)
{
	m_name		       = name;
	m_type		       = enVarString;
	m_value.val_string = new clsString(val);
}

// Array variable constructor
//==========================================================================================
clsVariable::clsVariable(clsString name, clsVarList& val)
{
	m_name		       = name;
	m_type		       = enVarArray;
	m_value.val_array  = new clsVarList;
	for (mmUint i = 0; i < val.Count(); i++)
		m_value.val_array->Add(new clsVariable(*val[i]));
}


// Variable copy constructor
//==========================================================================================
clsVariable::clsVariable(clsVariable &var)
{
	m_type = var.m_type;
	m_name = var.m_name;
	switch(m_type)
	{
	case enVarNone:
		break;
	case enVarFloat:
		m_value.val_float = var.m_value.val_float;
		break;
	case enVarInt:
		m_value.val_int = var.m_value.val_int;
		break;
	case enVarBool:
		m_value.val_bool = var.m_value.val_bool;
		break;
	case enVarString:
		m_value.val_string = new clsString(*var.m_value.val_string);
		break;
	case enVarArray:
		m_value.val_array  = new clsVarList;
		for (mmUint i = 0; i < var.m_value.val_array->Count(); i++)
			m_value.val_array->Add(new clsVariable(*(*var.m_value.val_array)[i]));
		break;
	}
}

// Variable destructor
//==========================================================================================
clsVariable::~clsVariable()
{
	if (m_type == enVarString)
		mmSafeDelete(m_value.val_string);
	if (m_type == enVarArray)
		mmSafeDelete(m_value.val_array);
}

// GetType: returns variable type
//==========================================================================================
enVarType clsVariable::GetType()
{
	return m_type;
}

// GetName: returns variable name
//==========================================================================================
clsString& clsVariable::GetName()
{
	return m_name;
}

// Typecast operators overload 
//==========================================================================================
clsVariable::operator mmFloat()
{
	if (m_type == enVarInt)
		return (mmFloat)(operator mmInt());
	if (m_type != enVarFloat)
		return 0.0f;
	return m_value.val_float;
}
clsVariable::operator mmInt()
{
	if (m_type == enVarFloat)
		return (mmInt)(operator mmFloat());
	if (m_type != enVarInt)
		return 0;
	return m_value.val_int;
}
clsVariable::operator mmBool()
{
	if (m_type != enVarBool)
		return mmfalse;
	return m_value.val_bool;
}
clsVariable::operator mmString()
{
	if (m_type != enVarString)
		return mmnull;
	return *m_value.val_string;
}
clsVariable::operator clsVarList&()
{
#pragma warning(disable:4172)	// disable local var address return warning
	if (m_type != enVarArray)
		return clsVarList();
#pragma warning(default:4172)	// enable local var address return warning
	return *m_value.val_array;
}
// Assignment operators overload
//==========================================================================================
mmFloat clsVariable::operator =(mmFloat val)
{
	if (m_type == enVarString)
		mmSafeDelete(m_value.val_string);
	if (m_type == enVarArray)
		mmSafeDelete(m_value.val_array);
	m_value.val_float = val;
	m_type = enVarFloat;
	return val;
}
mmInt clsVariable::operator =(mmInt val)
{
	if (m_type == enVarString)
		mmSafeDelete(m_value.val_string);
	if (m_type == enVarArray)
		mmSafeDelete(m_value.val_array);
	m_value.val_int = val;
	m_type = enVarInt;
	return val;
}
mmBool clsVariable::operator =(mmBool val)
{
	if (m_type == enVarString)
		mmSafeDelete(m_value.val_string);
	if (m_type == enVarArray)
		mmSafeDelete(m_value.val_array);
	m_value.val_bool = val;
	m_type = enVarBool;
	return val;
}
mmString clsVariable::operator =(mmString val)
{
	if (m_type == enVarString)
		mmSafeDelete(m_value.val_string);
	if (m_type == enVarArray)
		mmSafeDelete(m_value.val_array);
	m_value.val_string = new clsString(val);
	m_type = enVarString;
	return val;
}
clsVarList& clsVariable::operator = (clsVarList& val)
{
	if (m_type == enVarString)
		mmSafeDelete(m_value.val_string);
	if (m_type == enVarArray)
		m_value.val_array->Clear();
	else
		m_value.val_array = new clsVarList;
	for (mmUint i = 0; i < val.Count(); i++)
		m_value.val_array->Add(new clsVariable(*val[i]));
	return *m_value.val_array;
}
clsVariable& clsVariable::operator =(clsVariable &var)
{
	if (m_type == enVarString)
		mmSafeDelete(m_value.val_string);
	m_type = var.m_type;
	m_name = var.m_name;
	switch(m_type)
	{
	case enVarNone:
		break;
	case enVarFloat:
		m_value.val_float = var.m_value.val_float;
		break;
	case enVarInt:
		m_value.val_int = var.m_value.val_int;
		break;
	case enVarBool:
		m_value.val_bool = var.m_value.val_bool;
		break;
	case enVarString:
		m_value.val_string = new clsString(*var.m_value.val_string);
		break;
	}
	return *this;
}

//==========================================================================================
// Variable list class implementation
//==========================================================================================

// Variable list constructor
//==========================================================================================
clsVarList::clsVarList()
{

}

// Variable list copy constructor
//==========================================================================================
clsVarList::clsVarList(clsVarList& list)
{
	for (mmUint i = 0; i < list.Count(); i++)
		Add(new clsVariable(*list[i]));
}

// Variable list destructor
//==========================================================================================
clsVarList::~clsVarList()
{

}

// FindVariable: search variable by name
//==========================================================================================
clsVariable* clsVarList::FindVariable(const clsString& name) const
{
	clsString sUpName = name;
	clsString sUpVarName;
	sUpName.UpperCase();
	clsListNode* pnode = m_head;
	if (pnode == mmnull)
		return mmnull;
	do
	{
		sUpVarName = pnode->m_data->m_name;
		sUpVarName.UpperCase();
		if (sUpVarName == sUpName)
			return pnode->m_data;
		pnode = pnode->m_next;
	}while(pnode);
	return mmnull;
}

// Assignment operator
//==========================================================================================
clsVarList& clsVarList::operator =(clsVarList &list)
{
	Clear();
	for (mmUint i = 0; i < list.Count(); i++)
		Add(new clsVariable(*list[i]));
	return *this;
}





