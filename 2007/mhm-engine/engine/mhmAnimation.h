//==========================================================================================
// Mayhem engine animation classes
// Coded:	MozG
// Created:	19 july 2006
//==========================================================================================

#ifndef MHMANIMATION_INCLUDED
#define MHMANIMATION_INCLUDED

class clsEntity;

//===============================================
// Path animation track
//===============================================
class clsPathKey
{
public:
	mmFloat		t;		 // time
	clsVector3	v;		 // coords
	mmInt		mi;
	clsVector3	s;
	clsQuat		q;
	mmFloat tx,ty,tz,tw; // tangent vector
	mmFloat qx,qy,qz,qw; // rotate quaternion
	clsPathKey()	{}
};
typedef tmpRefArr<clsPathKey> PathTrack;

//===============================================
// Entity animation state
//===============================================
class clsAnimState
{
public:
	clsAnimState()
	{
		CurrentFrame = 0;
		DeltaFrame = 0;
	};
	mmFloat		DurationFrame;
	mmFloat		EdgeFrame;
	mmFloat		CurrentFrame;
	mmFloat		DeltaFrame;
	mmFloat		Speed;
};

//===============================================
// Path animation controller
//===============================================
class IAnimController
{
public:
	virtual mmVoid Animate(clsEntity* pentity) = 0;
};

//===============================================
// Linear path animation controller
//===============================================
class clsLinearController: public IAnimController
{
public:
	mmVoid Animate(clsEntity* pentity);
};

//===============================================
// Linear constant rotation controller
//===============================================
class clsConstantRotationController: public IAnimController
{
protected:
	mmFloat m_SpeedX, m_SpeedY, m_SpeedZ;
	mmFloat m_AmpSinX;

public:
	clsConstantRotationController();
	mmVoid Animate(clsEntity* pentity);
	mmVoid SetSpeedX(mmFloat speed);
	mmVoid SetSpeedY(mmFloat speed);
	mmVoid SetSpeedZ(mmFloat speed);
	mmVoid SetAmpSinX(mmFloat amp);
};




#endif // MHMANIMATION_INCLUDED