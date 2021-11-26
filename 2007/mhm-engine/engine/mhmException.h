//==========================================================================================
// Mayhem engine exception class
// Coded:	MozG
// Created:	7 july 2006
//==========================================================================================
#ifndef MHMEXCEPTION_INCLUDED
#define MHMEXCEPTION_INCLUDED

enum enExceptionReason
{
	enExceptUnknown				= 0,
	enExceptOutOfMemory			= 1,
	enExceptHardwareError		= 2,
	enExceptOutOfBounds			= 3,
	enExceptObjectMissed		= 4	
};

class clsException
{
public:
	enExceptionReason	m_reason;
	mmString			m_classname;
	mmString			m_method;
	mmString			m_description;

public:
	clsException(enExceptionReason reason,
				 mmString classname,
				 mmString method,
				 mmString description)
	{
		m_reason		= reason;
		m_classname		= classname;
		m_method		= method;
		m_description	= description;
	}
	
	mmString GetReasonDesc()
	{
		switch(m_reason)
		{
		case enExceptUnknown:
			return "unknown";
		case enExceptOutOfMemory:
			return "out of memory";
		case enExceptHardwareError:
			return "hardware error";
		case enExceptOutOfBounds:
			return "out of bounds";
		case enExceptObjectMissed:
			return "object(s) missed";
		}
		return "unregistered";
	}
};

#define mmThrow(reason, classname, method, description)	throw clsException(reason, \
																		   #classname, \
																		   #method, \
																		   #description);


#endif // MHMEXCPETION_INLCUDED
