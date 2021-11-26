#include "mhmbase.h"
#include "mhmVar.h"

mmBool IsNum(mmUchar ch)
{
	if( (ch >= '0') && (ch <= '9') )
	{
		return mmtrue;
	}
	return mmfalse;
}
mmBool IsChar(mmUchar ch)
{
	if( ((ch >= ' ') && (ch <= '~'))||((ch >= 'À') && (ch <= 'ÿ')) )
	{
		return mmtrue;
	}
	return mmfalse;
}
mmBool IsLit(mmUchar ch)
{
	if( 
		((ch >= 'A') && (ch <= 'Z'))||((ch >= 'a') && (ch <= 'z')) 
		||
		((ch >= 'À') && (ch <= 'ß'))||((ch >= 'à') && (ch <= 'ÿ'))
		||
		(ch == '_')
		||
		(ch == '$')
		||
		(ch == '-')
		||
		(ch == '@')
		||
		(ch == '!')
		||
		(ch == '#')
		||
		(ch == '~')
		)
	{
		return mmtrue;
	}
	return mmfalse;
}


//***************************************************************************************************************
clsVar::clsVar(void)
{
	m_name = "";
	m_type = enVar_Null;
	m_v_num_bool = 0;
	m_v_str = "";
	m_v_struct_array.Clear();
	m_token.reset();
	log = mmfalse;
}
clsVar::clsVar(const mmString name)
{
	m_name = name;
	m_type = enVar_Null;
	m_v_num_bool = 0;
	m_v_str = "";
	m_v_struct_array.Clear();
	m_token.reset();
	log = mmfalse;
}

clsVar::~clsVar(void)
{
}

mmRes clsVar::GetToken(mmUchar * pmem, mmUint size)
{
	mmInt countD;
	mmUint z;
	m_token.reset();
	for(z = 0 ; z < size; z++)
	{
		if( (pmem[z]=='/')&&((size-z)>1) )
			if(pmem[z+1]=='/')
				for(; z < size; z++)
					if(pmem[z]=='\n')
						break;

		if((IsChar(pmem[z])) && (pmem[z] != '=')&& (pmem[z] != ' ')&& (pmem[z] != ',')&&(pmem[z] != ']')&&(pmem[z] != '}'))
			break;
	}
	if(z == size)
	{
		return mmFail;
	}
	m_token.start = pmem + z;
	m_token.end = m_token.start;


	if(IsNum(pmem[z]) || (pmem[z] == '-'))
	{
		m_token.type = enTokNum;

		for(;z<size;z++)
		{
			if((IsNum(pmem[z])) || (pmem[z] == '.')|| (pmem[z] == '-'))
				m_token.end++;
			else
				break;
		}

	}else if(IsLit(pmem[z]))
	{
		m_token.type = enTokVar;
		for(;z<size;z++)
		{
			if(IsLit(pmem[z])||( (pmem[z] >= '0') && (pmem[z] <= '9')))
				m_token.end++;
			else
				break;
		}
		if( ((m_token.end - m_token.start)==4) || ((m_token.end - m_token.start)==5) )
		{
			if( (strcmp(clsString(m_token.start,m_token.end-m_token.start).LowerCase(),"true") == 0)||(strcmp(clsString(m_token.start,m_token.end-m_token.start).LowerCase(),"false") == 0) )
				m_token.type = enTokBool;
		}
	}
	else if(pmem[z] == '"')
	{
		z++;
		m_token.start++;
		m_token.end++;
		m_token.type = enTokString;
		for(;z<size;z++)
		{
			if(pmem[z] != '"')
				m_token.end++;
			else
				break;
		}
	}
	else if(pmem[z] == '[')
	{
		m_token.type = enTokArray;
		countD = 0;
		for(;z<size;z++)
		{
			if(pmem[z] == '[')
			{
				countD++;
			}
			else if(pmem[z] == ']')
			{
				countD--;
			}
			if(countD == 0)
				break;
			m_token.end++;
		}
		m_token.start++;
	}
	else if(pmem[z] == '{')
	{
		m_token.type = enTokStruct;

		countD = 0;
		for(;z<size;z++)
		{
			if(pmem[z] == '{')
			{
				countD++;
			}
			else if(pmem[z] == '}')
			{
				countD--;
			}
			if(countD == 0)
				break;
			m_token.end++;
		}
		m_token.start++;
	}

	

	if(m_token.type == enTokNull)
		return mmFail;

	if(log)g_plog->Out("Get Token: "+clsString(m_token.start,m_token.end-m_token.start), enLCYellow);
	return mmOk;
}

mmRes clsVar::ParsToken(mmUint size)
{
	if     (m_token.type == enTokNull)
	{
		m_type = enVar_Null;
	}
	else if(m_token.type == enTokVar)
	{
		m_v_struct_array.Add();
		m_v_struct_array.Last().log = log;
		m_v_struct_array.Last().m_name = clsString(m_token.start,m_token.end-m_token.start);
		GetToken(m_token.end, size);
		while(m_token.type == enTokVar)
		{
			m_v_struct_array.Add();
			m_v_struct_array.Last().log = log;
			m_v_struct_array.Last().m_name = clsString(m_token.start,m_token.end-m_token.start);
			GetToken(m_token.end, size);
		}
		m_v_struct_array.Last().m_token = m_token;
		m_v_struct_array.Last().ParsToken(size);
		m_token = m_v_struct_array.Last().m_token;
	//	m_token.end = m_v_struct_array.Last().m_token.end;
	//	m_v_struct_array.Last().m_ParseFromMem(m_token.end, size);
	//	m_token.end = m_v_struct_array.Last().m_token.end;
	
	}
	else if(m_token.type == enTokNum)
	{
		m_type = enVar_Num;
		m_v_num_bool = atof(clsString(m_token.start,m_token.end-m_token.start));
	}
	else if(m_token.type == enTokBool)
	{
		m_type = enVar_Bool;
		if(m_token.start[0] == 't')
			m_v_num_bool = 1;
		else
			m_v_num_bool = 0;
	}
	else if(m_token.type == enTokString)
	{
		m_type = enVar_String;
		m_v_str = clsString(m_token.start,m_token.end-m_token.start);
		m_token.end++;
	}
	else if(m_token.type == enTokArray)
	{
		m_type = enVar_Array;

		mmUchar * pmem = m_token.start;
		mmUint size = m_token.end - m_token.start;

		clsToken tmpt;
		tmpt = m_token;
		mmInt ar_n = 0;
		while(GetToken(pmem, size) == mmOk)
		{
			size -= m_token.end - (pmem);
			pmem = m_token.end;

			m_v_struct_array.Add();
			m_v_struct_array.Last().log = log;
			m_v_struct_array.Last().m_name = clsString(ar_n);
			m_v_struct_array.Last().m_token = m_token;
			m_v_struct_array.Last().ParsToken(size);
			m_token = m_v_struct_array.Last().m_token;

			size -= m_token.end - ((mmUchar *)pmem);
			pmem = m_token.end;

			ar_n++;
		}
		m_token = tmpt;
	//	m_token.end++;

	}
	else if(m_token.type == enTokStruct)
	{
		m_type = enVar_Struct;
		clsToken tmpt;
		tmpt = m_token;
		m_ParseFromMem(m_token.start , m_token.end - m_token.start );
		m_token = tmpt;
//		m_token.end++;
	}
//	if(log)g_plog->Out("Pars Token: "+clsString(m_type), enLCYellow);
	return mmOk;
}

mmRes clsVar::ParseFromFile(const mmString filename)
{
	clsFile file(filename);
	if (!file.IsExist())
	{
		g_plog->Out("clsVar::ParseFromFile >> [" + clsString(filename) + 
					"] file not exist or could not be opened now", enLCRed);
		return mmFail;
	}
	if (MHMFAIL(file.Open()))
	{
		g_plog->Out("clsVar::ParseFromFile >> [" + clsString(filename) + 
					"] cant open file", enLCRed);
		return mmFail;
	}
	mmVoid* pmem;
	if (MHMFAIL(file.ReadToMemory(&pmem)))
	{
		g_plog->Out("clsVar::ParseFromFile >> [" + clsString(filename) + 
					"] file reading error", enLCRed);
		return mmFail;
	}
	
	mmRes res = ParseFromMem(pmem, file.GetSize());

	file.FreeMemory(pmem);

	return res;
}

mmRes clsVar::ParseFromMem(mmVoid* pmem, mmUint size)
{
	if(log)g_plog->Out("Start VarParser ***************************************************************", enLCGreen);


	m_type = enVar_Struct;
	m_token.reset();
	m_ParseFromMem( pmem, size);
	m_token.reset();


	if(log)g_plog->Out("VarParser ***************************************************************", enLCGreen);
	if(log)g_plog->Out(BuildText(), enLCWhite);
	if(log)g_plog->Out("End VarParser ***************************************************************", enLCGreen);

	return mmOk;
}

mmRes clsVar::m_ParseFromMem(mmVoid* pmem, mmUint size)
{
	while(GetToken((mmUchar*)pmem, size) == mmOk)
	{
		size -= m_token.end - ((mmUchar *)pmem);
		pmem = m_token.end;

		ParsToken(size);

		size -= m_token.end - ((mmUchar *)pmem);
		pmem = m_token.end;
	}
	return mmOk;
}
clsVar * clsVar::FindVar(const clsString & v_name)
{
	m_v_struct_array.ResetSelect();
	while(m_v_struct_array.SelectNext())
	{
		if( m_v_struct_array.Selected().m_name == v_name )
			return & m_v_struct_array.Selected();
	}
	return mmnull;
}

clsVar * clsVar::FindFullVar(clsString v_name , clsString del)
{
	clsVar * v = this;
	clsStringList list;
	v_name.GetSubStringsByDelimiters(del,list);
	list.ResetSelect();
	while(list.SelectNext())
	{
		v = v->FindVar(list.Selected());
	}
	return v;
}



//==========================================================================================
clsVar::operator mmFloat()
{
	if ((m_type == enVar_Num)||(m_type == enVar_Bool))
		return (mmFloat)(m_v_num_bool);
	return 0.0f;
}
clsVar::operator mmInt()
{
	if ((m_type == enVar_Num)||(m_type == enVar_Bool))
		return (mmInt)(m_v_num_bool);
	return 0;
}
clsVar::operator mmDouble()
{
	if ((m_type == enVar_Num)||(m_type == enVar_Bool))
		return (m_v_num_bool);
	return 0;
}
clsVar::operator mmString()
{
	if ((m_type == enVar_String))
		return (m_v_str.Data());
	return mmnull;
}
clsVar::operator clsString()
{
	if ((m_type == enVar_String))
		return (m_v_str);
	return clsString("");
}
clsVar::operator mmBool()
{
	if ((m_type == enVar_Num)||(m_type == enVar_Bool))
		return (m_v_num_bool != 0);
	return mmfalse;
}
clsVar * clsVar::operator[] (mmUint index)
{
	if ((m_type != enVar_Array)&&(m_type != enVar_Struct))
		return mmnull;
	if( index >= (m_v_struct_array.Count()) )
		return mmnull;
	return &m_v_struct_array[index];
}


//----
clsString clsVar::BuildText(mmBool novar, mmUint tab)
{
	clsString str;
	clsString t;
	str = "";
	t = "";

	for(mmUint z=0;z<tab;z++)
		t += "\t";
	str = t + str;

	if(!novar)
		str += m_name + " = ";

	if(m_type == enVar_Null)
	{
		str += "-0\n";
	}else if(m_type == enVar_Bool)
	{
		if(m_v_num_bool == 0)
			str += "false";
		else
			str += "true";
	}else if(m_type == enVar_Num)
	{
		if(m_v_num_bool == ((mmDouble)((mmUint)m_v_num_bool)) )
			str += clsString(((mmInt)m_v_num_bool));//+ "\n";
		else
			str += clsString().FromFloat((mmFloat)m_v_num_bool);//+ "\n";
	}else if(m_type == enVar_String)
	{
		str += "\""+m_v_str+"\"";
	}else if(m_type == enVar_Array)
	{
		if(!novar)
			str += "[";
		m_v_struct_array.ResetSelect();
		while(m_v_struct_array.SelectNext())
		{
			if(m_v_struct_array.Selected().GetType() == enVar_Struct)
				str += "\n{\n";

			if(m_v_struct_array.Selected().GetType() == enVar_Array)
				str += "[";

			str += m_v_struct_array.Selected().BuildText(mmtrue);

			if(m_v_struct_array.Selected().GetType() == enVar_Struct)
				str += "}\n";

			if(m_v_struct_array.Selected().GetType() == enVar_Array)
				str += "]";

			str += ", ";
		}
		str = str.Left(str.Length()-2);
		if(!novar)
			str += "]";
	}else if(m_type == enVar_Struct)
	{
		if(!novar)
		{
			str += "\n"+t+"{\n";
			tab++;
		}
		m_v_struct_array.ResetSelect();
		while(m_v_struct_array.SelectNext())
		{
			str += m_v_struct_array.Selected().BuildText(mmfalse,tab);
			str += "\n";
		}
		if(!novar)
		{
			str += t+"}";
			tab--;
		}
	}

	return str;
}