#pragma once

enum enVariableType {enVar_Null = 0, enVar_Bool, enVar_Num, enVar_String, enVar_Array, enVar_Struct};

class clsVar
{
private:

	enum enTokenType {enTokNull = 0, enTokVar, enTokNum, enTokBool, enTokString, enTokArray, enTokStruct , enTokKomment};

	clsString m_name;
	
	enVariableType m_type;

	mmDouble m_v_num_bool;
	clsString m_v_str;
	
	tmpList <clsVar> m_v_struct_array;

	class clsToken
	{
		private:
		public:
			clsToken()
			{
				reset();
			};
			~clsToken()
			{
				reset();
			};
			mmUchar * start;
			mmUchar * end;
			mmUint size;
			enTokenType type;
			mmVoid reset()
			{
				start = end = mmnull;
				size = 0;
				type = enTokNull;
			};
		
	} m_token;
	mmRes GetToken(mmUchar * pmem, mmUint size);
	mmRes ParsToken(mmUint size);
	mmRes m_ParseFromMem(mmVoid* pmem, mmUint size);


public:
	clsVar(void);
	clsVar(const mmString name);
	~clsVar(void);

	mmBool log;

	

	mmRes ParseFromFile(const mmString filename);
	mmRes ParseFromMem(mmVoid* pmem, mmUint size);
	clsVar * FindVar(const clsString & v_name);
	clsVar * FindFullVar(clsString v_name , clsString del = "/");

	operator mmFloat();
    operator mmInt();
	operator mmDouble();
    operator mmString();
	operator clsString();
    operator mmBool();

	mmVoid SetName(const mmString name){m_name = name;};
	clsString & GetName(){return m_name;};

	mmDouble GetNun(){return m_v_num_bool;};
	mmBool GetBool(){return (m_v_num_bool != 0);};
	clsString & GetString(){return m_v_str;};
	tmpList<clsVar> & GetStruct(){return m_v_struct_array;};
	enVariableType GetType(){return m_type;};
	mmUint GetCount(){return m_v_struct_array.Count();};

	clsVar * operator[] (mmUint index);
	clsVar * operator[] (mmInt index){return ((*this)[(mmUint)index]);};
	clsVar * GetFromIndex (mmUint index){return (*this)[index];};


	clsString BuildText(mmBool novar = mmtrue, mmUint tab = 0);

};
