//==========================================================================================
// Mayhem engine string class
// Coded:		MozG
// Created:		7 may 2006
// Info file:	clsString.doc
//==========================================================================================

#ifndef _MHMSTRING_INCLUDED
#define _MHMSTRING_INCLUDED

#define MHM_STRING_INTBUF_SIZE		20

class clsString;
typedef tmpList<clsString> clsStringList;

class clsString
{
private:
	mmString	m_ptr;
	mmUint		m_length;

public:
	clsString();
//	clsString(clsString& str);
	clsString(const clsString& str);
	clsString(mmString ptr);
	clsString(const mmInt intval);
	clsString(mmVoid* ptr, mmUint size);

	~clsString();

	clsString  operator +  (clsString& str);
	clsString  operator +  (mmString ptr);
	clsString  operator +  (mmInt intval);
	clsString& operator =  (const clsString& str);
	clsString& operator =  (mmString ptr);
	clsString& operator = (mmChar ptr);
	clsString& operator =  (mmInt intval);
	mmChar&    operator [] (mmInt index);
	mmChar&    operator [] (mmUint index);

	mmBool     operator != (const clsString& str) const;
	inline mmBool     operator == (const clsString& str) const
	{
		if (str.m_length != m_length)
			return mmfalse;
		for (mmInt i = m_length - 1 ; i >= 0 ; i--)
		{
			if (m_ptr[i] != str.m_ptr[i])
			{
				return mmfalse;
			}
		}
		return mmtrue;
	};
	mmBool     operator != (const mmString   ptr) const;
	inline mmBool     operator == (const mmString   ptr) const
	{
		
		if ((ptr == mmnull) && (m_ptr == mmnull)) return mmtrue;
		if ((ptr == mmnull) || (m_ptr == mmnull)) return mmfalse;

		for (mmUint i = 0; i < m_length; i++)
			if (m_ptr[i] != ptr[i])
				return mmfalse;
		if(ptr[m_length] != 0)
			return mmfalse;
		return mmtrue;
	};

	// clsString::operator ^
	//  compares 2 strings like strcmp does (result is int!)
	//  strcmp((mmString)a1,(mmString)a2) equals to (a1^a2)
	//  NOTE operators precedence!
	mmInt	operator ^ (clsString& str2);

	clsString& operator += (clsString& str);
	clsString& operator += (mmString   ptr);

	operator mmString ();
	operator mmByte*  ();
	operator mmVoid*  ();

	//    mmBool	IsNumeric();
	mmBool	IsInteger() const;	// test string for being either integer value or float
	mmBool	IsFloat() const;
	const mmString Data() const;

	clsString& FromFloat(mmFloat val);
	mmFloat    ToFloat() const;
	mmInt	   ToInt() const;

	mmVoid    Alloc(mmUint lenght);
	clsString Left(mmUint num);
	clsString Right(mmUint num);
	clsString Mid(mmUint pos, mmUint num = 0);
	clsString CutBy(mmChar cut, mmBool bleft = mmtrue);
	mmUint    Length() const;
	mmVoid    Truncate();
	mmBool    IsEmpty() const;
	clsString& TrimLR(mmChar what = ' ');
	clsString& TrimL(mmChar what = ' ');
	clsString& TrimR(mmChar what = ' ');

	mmInt	  FindSubString(const clsString& sub, mmUint pos = 0);
	mmInt	  FindSubString(const mmString sub, mmUint pos = 0);

	mmInt	  GetSubStrings(clsString& delimiter, clsStringList& list);
	mmInt	  GetSubStrings(char* delimiter, clsStringList& list);
	mmInt	FindChar(const mmChar ch) const;
	mmInt	GetSubStringsByDelimiters(const clsString &delimiters, clsStringList &list);

	clsString& UpperCase();
	clsString& LowerCase();
	clsString& ReplaceChar(mmChar what, mmChar with);
	clsString& ReplaceString(mmString what, mmString with);

	friend clsString operator + (mmString ptr, clsString& str);
	friend clsString operator + (mmInt intval, clsString& str);



	clsString & FromFormatNum(mmInt n, const mmString format);
	clsString & FromFormatNum(mmFloat n, const mmString format);
	clsString & FromFormatNum(mmDouble n, const mmString format);
};

#endif // _MHMSTRING_INCLUDED