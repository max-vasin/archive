//==========================================================================================
// Mayhem engine variables parser class implementation
// Coded:	MozG
// Created:	11 june 2006
//==========================================================================================
#include "mhmbase.h"

// Variables parser construction
//==========================================================================================
clsVarParser::clsVarParser()
{

}

// Variables parser destructor
//==========================================================================================
clsVarParser::~clsVarParser()
{

}

mmVoid clsVarParser::Clear()
{
	m_varlist.Clear();
}

// ParseMem: parsing memory block
//==========================================================================================
mmRes clsVarParser::ParseMem(mmVoid *pmem, mmUint size)
{
	if (pmem == mmnull)
	{
		g_plog->Out("clsVarParser::ParseMem >> invalid memory address", enLCRed);
		return mmFail;
	}
	
	if (size == 0)
	{
		g_plog->Out("clsVarParser::ParseMem >> invalid memory size", enLCRed);
		return mmFail;
	}
	
	tmpPtrList<clsString*> strlist;
	if (MHMFAIL(BuildStringList(strlist, (mmString)pmem, size)))
	{
		g_plog->Out("clsVarParser::ParseMem >> cant build string list", enLCRed);
		return mmFail;
	}

	
	
	clsString    statement, varname, varval;
	clsVariable* pvar;

	for (mmUint i = 0; i < strlist.Count(); i++)
	{
		statement = strlist[i]->TrimLR();
		if ((statement.IsEmpty()) || (statement.Length() == 0))
			continue;

		varname   = statement.CutBy('=', mmtrue).TrimLR();

		if(varname.Length() !=  statement.Length())
		{
			varval    = statement.CutBy('=', mmfalse).TrimLR();
			pvar = MakeVariable(varname, varval, i + 1);
		}
		else
		{
			pvar = MakeVariable(varname, "true", i + 1);
		}

		if (pvar != mmnull)
		{
			m_varlist.Add(pvar);
			continue;
		}
		
		// Array variable
		if ((varval[0] == '{') &&
			(varval[varval.Length() - 1] == '}'))
		{
			varval.TrimL('{');
			varval.TrimR('}');
			clsStringList arr;
			varval.GetSubStrings(":", arr);
			if (arr.Count() == 0)
			{
				g_plog->Out("clsVarParser::ParseMem >> empty array in [" + clsString(i + 1) + "] string. Variable skipped.");
				continue;
			}
			clsVarList arraylist;
			for (mmUint z = 0; z < arr.Count(); z++)
			{
				clsVariable* pitem = MakeVariable(varname + "_" + clsString(z), arr[z], i + 1);
				if (pitem == mmnull)
				{
					g_plog->Out("clsVarParser::ParseMem >> unknown variable type in [" + clsString(i + 1) + "] string [" + clsString(z + 1) + "] array element. Variable skipped.");
					continue;
				}
				arraylist.Add(pitem);
			}
			m_varlist.Add(new clsVariable(varname, arraylist));
			//arraylist
			continue;
		}
		g_plog->Out("clsVarParser::ParseMem >> unknown variable type in [" + clsString(i + 1) + "] string. Variable skipped.");
	}
	
	return mmOk;
}

// ParseFile: parsing file
//==========================================================================================
mmRes clsVarParser::ParseFile(mmString filename)
{
	clsFile file(filename);
	if (!file.IsExist())
	{
		g_plog->Out("clsVarParser::ParseFile >> [" + clsString(filename) + 
					"] file not exist or could not be opened now", enLCRed);
		return mmFail;
	}
	if (MHMFAIL(file.Open()))
	{
		g_plog->Out("clsVarParser::ParseFile >> [" + clsString(filename) + 
					"] cant open file", enLCRed);
		return mmFail;
	}
	mmVoid* pmem;
	if (MHMFAIL(file.ReadToMemory(&pmem)))
	{
		g_plog->Out("clsVarParser::ParseFile >> [" + clsString(filename) + 
					"] file reading error", enLCRed);
		return mmFail;
	}
	
	mmRes res = ParseMem(pmem, file.GetSize());

	file.FreeMemory(pmem);

	return res;
}

// GetVarList: returns var list
//==========================================================================================
clsVarList* clsVarParser::GetVarList()
{
	return &m_varlist;
}

// BuildStringList: builds strings list from memory
//==========================================================================================
mmRes clsVarParser::BuildStringList(tmpPtrList<clsString*>& list, mmString pmem, mmUint size)
{
	clsString str;
	str.Alloc(MHMVARPARSER_MAX_STRING);
	mmUint c = 0;
	for (mmUint i = 0; i < size; i++)
	{
		if (pmem[i] == 0x0D)
		{
			str[c] = 32;
			c++;//Pink
			continue;
		}
		if (pmem[i] == 0x0A)
		{
			str[c] = 0;
			c = 0;
			list.Add(new clsString(str));
			continue;
		}
		str[c] = pmem[i];
		c++;
	}	
	//if (str[c] != 0)
	if (c != 0)//Pink
	{
		str[c] = 0;
		list.Add(new clsString(str));
	}
	return mmOk;
}

// MakeVariable: makes new varaible
//==========================================================================================
clsVariable* clsVarParser::MakeVariable(const clsString &varname, clsString varval, mmUint strno)
{
		if ((varname.IsEmpty()) || (varname.Length() == 0))
		{
			g_plog->Out("clsVarParser::ParseMem >> invalid variable name in [" + clsString(strno) + "] string. Variable skipped.");
			return mmnull;
		}
		if ((varval.IsEmpty()) || (varname.Length() == 0))
		{
			g_plog->Out("clsVarParser::ParseMem >> invalid variable value in [" + clsString(strno) + "] string. Variable skipped.");
			return mmnull;
		}
		
		// Float variable
		if (varval.IsFloat() && (!varval.IsInteger()))
			return new clsVariable(varname, varval.ToFloat());
		
		// Integer variable
		if (varval.IsInteger())
			return new clsVariable(varname, varval.ToInt());
		
		// Boolean variable
		if (varval == "true")
			return new clsVariable(varname, mmtrue);
		if (varval == "false")
			return new clsVariable(varname, mmfalse);

		// String variable
		if (varval[0] == '\"')
		{
			varval.TrimLR('\"');
		}

		if(varval[0] != '{')
		{
			return new clsVariable(varname, (mmString)varval);
		}

	return mmnull;
}