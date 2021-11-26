//==========================================================================================
// Mayhem engine string class implementation
// Coded:	MozG
// Created:	7 may 2006
//==========================================================================================
#include "mhmbase.h"

//#define mmAllocString(Size) (mmString)g_pEnvironment->m_pStringPool->Alloc(Size)
//#define mmFreeString(Ptr)   g_pEnvironment->m_pStringPool->Free(Ptr)
#define mmAllocString(Size) new mmChar[Size]
#define mmFreeString(Ptr)	mmSafeDeleteAr(Ptr)
// String default constructor
//======================================================================================
clsString::clsString()
{
	m_ptr    = mmnull;
	m_length = 0;
}

// String memory constructor
//======================================================================================
clsString::clsString(mmString ptr)
{
	if (ptr == mmnull)
	{
		m_ptr    = mmnull;
		m_length = 0;
	}
	else
	{
		m_ptr = mmAllocString(strlen(ptr) + 1);
		if (m_ptr == mmnull)
			mmThrow(enExceptOutOfMemory, clsString, constructor, String alloc error);
		strcpy(m_ptr, ptr);
		m_length = strlen(m_ptr);
	}
}
//======================================================================================
clsString::clsString(mmVoid* ptr, mmUint size)
{
	if (ptr == mmnull)
	{
		m_ptr    = mmnull;
		m_length = 0;
	}
	else
	{
		m_ptr = mmAllocString(size + 1);
		if (m_ptr == mmnull)
			mmThrow(enExceptOutOfMemory, clsString, constructor, String alloc error);
		memcpy(m_ptr, ptr, size);
		m_ptr[size]=0;
		m_length = size;//strlen(m_ptr);
	}
}

// String copy constructor
//======================================================================================
/*clsString::clsString(clsString& str)
{
	if (str.IsEmpty())
	{
		m_ptr    = mmnull;
		m_length = 0;
		return;
	}
	m_ptr = new mmChar[strlen(str.m_ptr) + 1];
	if (m_ptr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, copy constructor, String alloc error);
	strcpy(m_ptr, str.m_ptr);
	m_length = strlen(m_ptr);
}*/

// String copy constructor
//======================================================================================
clsString::clsString(const clsString& str)
{
	if (str.IsEmpty())
	{
		m_ptr    = mmnull;
		m_length = 0;
		return;
	}
	m_ptr = mmAllocString(str.Length() + 1);
	if (m_ptr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, copy constructor, String alloc error);
	strcpy(m_ptr, str.m_ptr);
	m_length = strlen(m_ptr);
}

// String from int value constructor
//======================================================================================
clsString::clsString(mmInt intval)
{
	m_ptr = mmAllocString(MHM_STRING_INTBUF_SIZE);
	if (m_ptr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, int constructor, String alloc error);
	_itoa(intval, m_ptr, 10);
	m_length = strlen(m_ptr);
	Truncate();
}

// String destructor
//======================================================================================
clsString::~clsString()
{
	mmFreeString(m_ptr);
}

// Operator + overload
//======================================================================================
clsString clsString::operator + (clsString& str)
{
	mmString newstr = mmAllocString(Length() + str.Length() + 1);
	
	if (newstr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, operator +, String alloc error);
	if (m_ptr != mmnull)
		strcpy(newstr, m_ptr);
	if (str.m_ptr != mmnull)
		if (m_ptr == mmnull)
			strcpy(newstr, str.m_ptr);
		else
			strcat(newstr, str.m_ptr);

	clsString NewStr(newstr);
	mmFreeString(newstr);
	return NewStr;
}

// Operator + overload
//======================================================================================
clsString clsString::operator + (mmString ptr)
{
	mmUint nLength = Length() + ((ptr == mmnull) ? 0 : strlen(ptr)) + 1;
	mmString newstr = mmAllocString(nLength);
	if (newstr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, operator +, String alloc error);

	if (m_ptr != mmnull)
		strcpy(newstr, m_ptr);
	if (ptr != mmnull)
		if (m_ptr == mmnull)
			strcpy(newstr, ptr);
		else
			strcat(newstr, ptr);

	clsString NewStr(newstr);
	mmFreeString(newstr);
	return NewStr;
}

// Operator + overload
//======================================================================================
clsString clsString::operator + (mmInt intval)
{
	mmString newstr = mmAllocString(Length() + MHM_STRING_INTBUF_SIZE + 1);
	mmString intstr = mmAllocString(MHM_STRING_INTBUF_SIZE);

	if ((newstr == mmnull) || (intstr == mmnull))
		mmThrow(enExceptOutOfMemory, clsString, operator +, String alloc error);

	_itoa(intval, intstr, 10);

	if (m_ptr != mmnull)
	{
		strcpy(newstr, m_ptr);
		strcat(newstr, intstr);
	}
	else
		strcpy(newstr, intstr);
	clsString NewStr(newstr);
	//NewStr.m_ptr = newstr;
	mmFreeString(newstr);
	mmFreeString(intstr);
	return NewStr;
}

// Operator = overload
//======================================================================================
clsString& clsString::operator = (const clsString& str)
{
	if(str.m_ptr == m_ptr)
		return *this;
	mmFreeString(m_ptr);
	
	if (str.m_ptr == mmnull) {m_ptr = mmnull; m_length=0; return *this;}

	m_ptr = mmAllocString(strlen(str.m_ptr) + 1);

	if (m_ptr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, operator =, String alloc error);
	strcpy(m_ptr, str.m_ptr);
	m_length = strlen(m_ptr);
	return *this;
}

// Operator = overload
//======================================================================================
clsString& clsString::operator = (mmString ptr)
{
	mmFreeString(m_ptr);
	if (ptr == mmnull) 
		return *this;
	m_ptr = mmAllocString(strlen(ptr) + 1);

	if (m_ptr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, operator =, String alloc error);

	strcpy(m_ptr, ptr);
	m_length = strlen(m_ptr);
	return *this;
}

clsString& clsString::operator = (mmChar ptr)
{
	mmChar str[2];
	str[0] = ptr;
	str[1]=0;
	(*this) = str;
	return *this;
}
// Operator = overload
//======================================================================================
clsString& clsString::operator = (mmInt intval)
{
	mmFreeString(m_ptr);
	m_ptr = mmAllocString(MHM_STRING_INTBUF_SIZE);

	if (m_ptr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, operator =, String alloc error);
	_itoa(intval, m_ptr, 10);
	m_length = strlen(m_ptr);
	Truncate();
	return *this;
}

// Operator += overload
//======================================================================================
clsString& clsString::operator += (clsString& str)
{
	mmString newstr = mmAllocString(Length() + str.Length() + 1);
	if (newstr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, operator +=, String alloc error);

	if (m_ptr != mmnull)
		strcpy(newstr, m_ptr);

	if (str.m_ptr != mmnull)
		if (m_ptr == mmnull)
			strcpy(newstr, str.m_ptr);
		else
			strcat(newstr, str.m_ptr);
	mmFreeString(m_ptr);
	m_ptr = newstr;
	m_length = strlen(m_ptr);
	return *this;
}


// Operator += overload
//======================================================================================
clsString& clsString::operator += (mmString ptr)
{
	mmString newstr = mmAllocString(Length() + ((ptr == mmnull) ? 0 : strlen(ptr)) + 1);

	if (newstr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, operator +=, String alloc error);

	if (m_ptr != mmnull)
		strcpy(newstr, m_ptr);
	if (ptr != mmnull)
		if (m_ptr == mmnull)
			strcpy(newstr, ptr);
		else
			strcat(newstr, ptr);
	mmFreeString(m_ptr);
	m_ptr = newstr;
	m_length = strlen(m_ptr);
	return *this;
}

// Operator [] overload
//======================================================================================
mmChar& clsString::operator [] (mmInt index)
{
	
	if (index < 0)
		mmThrow(enExceptOutOfBounds, clsString, operator [], Index less than zero);
	if (index > (mmInt)m_length)
		mmThrow(enExceptOutOfBounds, clsString, operator [], Index more than string length);
	return m_ptr[index];
}

// Operator [] overload
//======================================================================================
mmChar& clsString::operator [] (mmUint index)
{
	if (index > m_length)
		mmThrow(enExceptOutOfBounds, clsString, operator [], Index more than string length);
	return m_ptr[index];
}

// Typecast operators
//======================================================================================
clsString::operator mmString ()
{
	return m_ptr;
}
//======================================================================================
clsString::operator mmVoid*  ()
{
	return (mmVoid*)m_ptr;
}
//======================================================================================
clsString::operator mmByte * ()
{
	return (mmByte*)m_ptr;
}


// Boolean operators overload
//======================================================================================
mmBool clsString::operator != (const clsString& str) const
{
	if ((str.m_ptr == mmnull) && (m_ptr == mmnull)) return mmfalse;
	if ((str.m_ptr == mmnull) && (m_ptr != mmnull)) return mmtrue;
	if ((str.m_ptr != mmnull) && (m_ptr == mmnull)) return mmtrue;
	if (str.m_length != m_length)
		return mmtrue;
	
	for (mmUint i = 0; i < m_length; i++)
		if (m_ptr[i] != str.m_ptr[i])
			return mmtrue;

	return mmfalse;
}
//======================================================================================
mmBool clsString::operator != (const mmString ptr) const
{
	if ((ptr == mmnull) && (m_ptr == mmnull)) return mmfalse;
	if ((ptr == mmnull) && (m_ptr != mmnull)) return mmtrue;
	if ((ptr != mmnull) && (m_ptr == mmnull)) return mmtrue;
	mmUint nLength = strlen(ptr);
	if (nLength != m_length)
		return mmtrue;
	
	for (mmUint i = 0; i < m_length; i++)
		if (m_ptr[i] != ptr[i])
			return mmtrue;
	return mmfalse;
}
//======================================================================================
/*
mmBool clsString::operator == (const clsString& str) const
{
	if ((str.m_ptr == mmnull) && (m_ptr == mmnull)) return mmtrue;
	if ((str.m_ptr == mmnull) && (m_ptr != mmnull)) return mmfalse;
	if ((str.m_ptr != mmnull) && (m_ptr == mmnull)) return mmfalse;
	if (str.m_length != m_length)
		return mmfalse;
	for (mmUint i = 0; i < m_length; i++)
		if (m_ptr[i] != str.m_ptr[i])
			return mmfalse;
	return mmtrue;
}
*/
//======================================================================================
mmInt clsString::operator ^ (clsString& str2)
{
	mmString left,right;
	mmString empty="";

	left  = (this->m_ptr==mmnull) ? empty : this->m_ptr; // for comparison, NULL string equals to ""
	right = (str2.m_ptr==mmnull)  ? empty : str2.m_ptr;

	return strcmp(left,right);
}

//======================================================================================
/*
mmBool clsString::operator == (const mmString ptr) const
{
	if ((ptr == mmnull) && (m_ptr == mmnull)) return mmtrue;
	if ((ptr == mmnull) || (m_ptr == mmnull)) return mmfalse;
	
	//if ((ptr == mmnull) && (m_ptr != mmnull)) return mmfalse;
	//if ((ptr != mmnull) && (m_ptr == mmnull)) return mmfalse;
	
	

	//mmUint nLength = strlen(ptr);
	//if (nLength != m_length)
	//	return mmfalse;
		
	
	for (mmUint i = 0; i < m_length; i++)
		if (m_ptr[i] != ptr[i])
			return mmfalse;
	return mmtrue;
}
*/

// IsFloat: returns true if string is float (0, 0.0, -0.0, 1e5, etc...)
//======================================================================================
mmBool clsString::IsFloat() const
{
	mmChar a;

	if (m_ptr == mmnull)
		return mmfalse;
	mmUint i = 0;

	mmUint wasdot,wasE,wasminus,wasdigit;
	wasdot=wasE=wasminus=wasdigit=0;

	while( (a=m_ptr[i++]) )
	{
		switch( a )
		{
			case ' ':
				continue;
			// digits
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				wasdigit=1;
				continue;

			// decimal dot
			case '.':
				if( wasdot ) return mmfalse; // no more than 1 dot
				if( wasE   ) return mmfalse; // no dot in exponent
				wasdot = 1;
				wasdigit=0;
				continue;

			// minus sign
			case '-':
				if( wasminus )
				{
					if( wasE )
					{ // allow second minus in exponent
						if( wasdigit ) return mmfalse;
                                                if( wasminus>1 ) return mmfalse;
                                                wasminus++;
					}
					else return mmfalse;
				}
				else
				{ // still was no minus
					if( wasdigit ) return mmfalse;
					if( wasdot ) return mmfalse;
					if( wasE )
					{
						if( wasdigit ) return mmfalse;
						wasminus++; // to prevent 2 minuses in exponent
					}
					wasminus++;
				}
				wasdigit=0;
				continue;

			//exponent mark
			case 'e':
			case 'E':
				if( wasE ) return mmfalse;
				if( !wasdigit ) return mmfalse;
				wasdigit=0;
				wasE=1;
				continue;
			default:
				return mmfalse;
		}
	}

	if( wasdigit ) return mmtrue;
	else return mmfalse;
}


// IsInteger: returns true if string is integer (1, -1, etc...)
//======================================================================================
mmBool clsString::IsInteger() const
{
	mmChar a;
	mmUint i;
	mmUint wasminus,wasdigit;

	if( m_ptr==mmnull ) return mmfalse;

	i = 0;
	wasminus=wasdigit=0;

	while( (a=m_ptr[i++]) )
	{
		switch( a )
		{
			// digits
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				wasdigit=1;
				continue;

			// minus sign
			case '-':
				if( wasdigit ) return mmfalse;
				if( wasminus ) return mmfalse;
				wasminus++;
				continue;
			default:
				return mmfalse;
		}
	}

	if( wasdigit ) return mmtrue;
	else return mmfalse;
}

// Data: returns string data
//======================================================================================
const mmString clsString::Data() const
{
	return m_ptr;
}



// FromFloat: builds string from float value
//======================================================================================
clsString& clsString::FromFloat(mmFloat val)
{
	mmFreeString(m_ptr);
	Alloc(20);
	sprintf(m_ptr, "%5.5f", val);	// cut last one
	Truncate();
	return *this;
}

// ToFloat: returns floating point value from string
//======================================================================================
mmFloat clsString::ToFloat() const
{
	if (m_ptr == mmnull)
		return 0.0f;
	mmFloat fbuf;
	sscanf(m_ptr, "%f", &fbuf);
	return fbuf;
}

// ToInt: returns integer string value
//==========================================================================================
mmInt clsString::ToInt() const
{
	if (m_ptr == mmnull)
		return 0;
	if (!IsInteger())
		return 0;
	return atoi(m_ptr);
}

// Alloc: allocates the string
//======================================================================================
mmVoid clsString::Alloc(mmUint length)
{
	mmFreeString(m_ptr);
	m_ptr = mmAllocString(length + 1);
	if (m_ptr == mmnull)
		mmThrow(enExceptOutOfMemory, clsString, Alloc, String memory alloc failed);
	m_ptr[length] = 0;
	m_length = length;
}

// Left: returns substring that contains number of left symbols of current string
//======================================================================================
clsString clsString::Left(mmUint num)
{
	clsString newstr;
	if (m_ptr == mmnull)
		return newstr;
	else if (num > m_length) num = m_length;
	if(num == 0)
		return clsString("");
	newstr.Alloc(num);
	memcpy(newstr, m_ptr, sizeof(mmChar) * num);
	newstr.m_ptr[num] = 0;
	return newstr;
}

// Right: returns substring that contains number of right symbols of current string
//======================================================================================
clsString clsString::Right(mmUint num)
{
	clsString newstr;
	if (m_ptr == mmnull)
		return newstr;
	else if (num > m_length) num = m_length;
	newstr.Alloc(num);
	memcpy(newstr.m_ptr, &m_ptr[m_length - num], sizeof(mmChar) * num);
	newstr.m_ptr[num] = 0;
	return newstr;
}

// Mid: returns substring that contains number of middle symbols of current string
//======================================================================================
clsString clsString::Mid(mmUint pos, mmUint num)
{
	clsString newstr;
	if ( (m_ptr == mmnull) || (pos >= m_length))
		return newstr;
	if (num==0)	num = m_length-pos;

	newstr.Alloc(num);
	memcpy(newstr.m_ptr, &m_ptr[pos], sizeof(mmChar) * num);
	newstr.m_ptr[num] = 0;
	return newstr;
}

// Cut: returns cutted string by given charater. Useful to get filename without
//      extension
//======================================================================================
clsString clsString::CutBy(mmChar cut, mmBool bleft)
{
	clsString empty;
	if (m_ptr == mmnull)
		return empty;

	clsString newstr;
	newstr.Alloc(Length() + 1);
	mmUint i;
	if (bleft)
	{
		i = 0;
		while (i != (Length() + 1))
		{
			if (m_ptr[i] == cut) break;
			newstr.m_ptr[i] = m_ptr[i];
			i++;
		}
		newstr.m_ptr[i] = 0;
	}
	else
	{
		i = Length();
		while (i != 0)
		{
			if (m_ptr[i] == cut) break;
			newstr.m_ptr[i] = m_ptr[i];
			i--;
		}
		if (i > 0) i++;
		mmUint offs = i;
		while (i != (Length() + 1))
		{
			newstr.m_ptr[i - offs] = m_ptr[i];
			i++;
		}
	}
	return newstr;
}
// Length: returns string length
//======================================================================================
mmUint clsString::Length() const
{
	return m_length;
}

// Truncate: frees unused string memory (beyond zero)
//======================================================================================
mmVoid clsString::Truncate()
{
	clsString buf(*this);
	*this = buf;
}

// IsEmpty: returns true if no string exist
//======================================================================================
mmBool clsString::IsEmpty() const
{
	if (m_ptr == mmnull)
		return mmtrue;
	if (m_length == 0)
		return mmtrue;
	return mmfalse;
}

// TrimLR: returns trimmed (spaces deleted) string from left and right
//======================================================================================
clsString& clsString::TrimLR(mmChar what)
{
	TrimL(what);
	TrimR(what);
	return *this;
}

// TrimL: returns trimmed (spaces deleted) string from left
//======================================================================================
clsString& clsString::TrimL(mmChar what)
{
	if ((m_ptr != mmnull)&&(m_length != 0))
	{
		mmUint i;
		for (i = 0; i < m_length; i++)
			if (m_ptr[i] != what)
				break;
		// BUG FIXED
		*this = Right(m_length - i);
		/* for test
		if(i>0)
		{
			mmUint j=0;
			for (; i < m_length; j++,i++)
				m_ptr[j] = m_ptr[i];
			m_length = j;
			m_ptr[j]='\0';
		}
		*/

	}
	return *this;
}

// TrimR: returns trimmed (spaces deleted) string from right
//======================================================================================
clsString& clsString::TrimR(mmChar what)
{
	if (m_ptr == mmnull)
		return *this;
	if (m_length == 0)
		return *this;
	for (mmUint i = m_length - 1; i != 0; i--)
		if (m_ptr[i] != what)
			break;
	return *this = Left(i + 1);
}

// GetSubStrings: returns list of substrings
//======================================================================================
mmInt clsString::GetSubStrings(clsString &delimiter, clsStringList &list)
{
	mmInt pos, pos0;
	pos0 = 0;
	while ((pos = FindSubString(delimiter, pos0)) != -1)
	{
		if((pos - pos0)<1) list.Add("");
		else list.Add(Mid(pos0, pos - pos0));
		pos0 = pos + delimiter.Length();
	}
	list.Add(Right(Length() - pos0));
	return list.Count();
}

// GetSubStrings: returns list of substrings
//======================================================================================
mmInt clsString::GetSubStrings(char* delimiter, clsStringList &list)
{
	clsString str(delimiter);
	return GetSubStrings(str, list);
}

// FindSubString: returns position of substring if its founded, -1 otherwise
//======================================================================================
mmInt clsString::FindSubString(const clsString &substr, mmUint pos)
{
	//if (m_ptr == mmnull)
	//	return -1;
	//if (substr.IsEmpty())
	//	return -1;
	if (substr.Length() > Length())
		return -1;
	//if (pos > Length())
	//	return -1;
/*	
	clsString cursub;
	for (mmUint i = pos; i < (Length() - substr.Length()); i++)
	{
		cursub = Mid(i, substr.Length());
		if (cursub == substr)
			return i;
	}
	return -1;
	*/
	return FindSubString(substr.Data(), pos);
}
// FindSubString: returns position of substring if its founded, -1 otherwise
//======================================================================================
mmInt clsString::FindSubString(const mmString substr, mmUint pos)
{
	if (m_ptr == mmnull)
		return -1;
	if (substr == mmnull)
		return -1;
	if (pos > Length())
		return -1;
	

	for (mmUint i = pos; i < Length() ; i++)
	{
		for(mmUint n = i;;n++)
		{
			if(m_ptr[n] != substr[n-i])
				break;
			if(substr[n-i+1] == 0)
				return i;
			if(m_ptr[n] == 0)
				return -1;
		}
	}
	return -1;
}

// UpperCase: make all letters upper case
//==========================================================================================
clsString& clsString::UpperCase()
{
	if (m_ptr == mmnull)
		return *this;
	_strupr(m_ptr);
	return *this;
}

// LowerCase: make all letters lowercase
//==========================================================================================
clsString& clsString::LowerCase()
{
	if (m_ptr == mmnull)
		return *this;
	_strlwr(m_ptr);
	return *this;
}
// ReplaceChar: replaces one char by another
//======================================================================================
clsString& clsString::ReplaceChar(mmChar what, mmChar with)
{
	if (m_ptr == mmnull)
		return *this;
	for (mmUint i = 0; i < m_length; i++)
		if (m_ptr[i] == what)
			m_ptr[i] = with;
	return *this;
}
//======================================================================================
clsString& clsString::ReplaceString(mmString what, mmString with)
{
	clsString s1(what);
	clsString s2(with);
	clsString s3;

	mmInt pos, pos0;
	pos0 = 0;
	while ((pos = FindSubString(s1, pos0)) != -1)
	{
		s3 += Mid(pos0, pos - pos0) + s2;
		pos0 = pos + s1.Length();
	}
	if(pos0 > 0)
	{
		s3 += s2 + Right(Length() - pos0);
		*this = s3;
	}
	return *this;
}
// String global operators overload
//======================================================================================
clsString operator + (mmString ptr, clsString& str)
{
	if (ptr == mmnull)
		return clsString(str);
	if (str.IsEmpty())
		return clsString(ptr);
	mmUint nLength = ((ptr == mmnull) ? 0 : strlen(ptr)) + str.Length() + 1;
	mmString newstr = mmAllocString(nLength);
	if (newstr == mmnull)
		mmThrow(enExceptOutOfMemory, external function, operator +, String memory alloc failed);

	strcpy(newstr, ptr);
	strcat(newstr, str);
	clsString NewStr(newstr);
	mmFreeString(newstr);
	return NewStr;
}
//======================================================================================
clsString operator + (mmInt intval, clsString& str)
{
	if (str.IsEmpty())
		return clsString(intval);
	mmString newstr = mmAllocString(str.Length() + MHM_STRING_INTBUF_SIZE + 1);
	mmString intstr = mmAllocString(MHM_STRING_INTBUF_SIZE);
	if ((newstr == mmnull) || (intstr == mmnull))
		mmThrow(enExceptOutOfMemory, external function, operator +, String memory alloc failed);
	_itoa(intval, intstr, 10);
	strcpy(newstr, intstr);
	strcat(newstr, str.m_ptr);
	clsString NewStr(newstr);
	mmFreeString(newstr);
	mmFreeString(intstr);
	return NewStr;
}

// FindChar: returns list of pure substrings without delimiter chars
//======================================================================================
mmInt clsString::FindChar(const mmChar ch) const
{
	mmUint pos=0;
	if(m_length>0)
	{
		while(pos<m_length)
		{
			if(m_ptr[pos] == ch) return pos;
			pos++;
		}
	}
	return -1;
}

// GetSubStringsByDelimiters: returns list of pure substrings without delimiter chars
//======================================================================================
mmInt clsString::GetSubStringsByDelimiters(const clsString &delimiters, clsStringList &list)
{
	mmUint pos, pos0, len;
	mmBool supress = mmfalse;
	
	if(m_length>0)
	{
		pos0 = pos = 0;
		while(pos <= m_length)
		{
			if((pos==m_length)||((!supress) && (delimiters.FindChar(m_ptr[pos])>=0)))
			{
				if(pos0!=pos)
				{
					if(m_ptr[pos0] == '|') pos0++;
					len = pos - pos0;
					if(m_ptr[pos-1] == '|') len--;
					list.Add(Mid(pos0, len));
				}
				if(pos==m_length) break; // str over
				pos0 = pos+1;
			}
			if(m_ptr[pos] == '|')
			{
				supress = !supress;
			}
			else if(m_ptr[pos] == '\n')
			{
				supress = mmfalse;
			}

			pos++;
		}
//		if(pos0!=pos) list.Add(Mid(pos0, pos - pos0)); 
	}
	return list.Count();
}

//******* Pink
clsString & clsString::FromFormatNum(mmInt n, const mmString format)
{
	mmChar sf[128]="%";
	mmChar s[128];
	strcat(sf,format);
	strcat(sf,"d");
	sprintf(s,sf,n);
	*this = s;
	return *this;
}
clsString & clsString::FromFormatNum(mmFloat n, const mmString format)
{
	mmChar sf[128]="%";
	mmChar s[128];
	strcat(sf,format);
	strcat(sf,"f");
	sprintf(s,sf,n);
	*this = s;
	return *this;
}
clsString & clsString::FromFormatNum(mmDouble n, const mmString format)
{
	mmChar sf[128]="%";
	mmChar s[128];
	strcat(sf,format);
	strcat(sf,"f");
	sprintf(s,sf,n);
	*this = s;
	return *this;
}
