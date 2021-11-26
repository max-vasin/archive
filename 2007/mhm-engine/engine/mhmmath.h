//==========================================================================================
// Mayhem engine math matrix,vector,plane class
// Coded:	Pink
// Created:	31 may 2006
//==========================================================================================

#ifndef _MHMMATH_INCLUDED
#define _MHMMATH_INCLUDED

#define MHM_PI		3.14159265359f
#define MHM_1BYPI   0.31830988618f
#define mmToRadian(degree) ((degree) * (MHM_PI / 180.0f))
#define mmToDegree(radian) ((radian) * (180.0f / MHM_PI))
#define mmAbs(value)	    (((value) < 0.0f) ? -(value) : value)
#define mmAlmostZero(value) (mmAbs(value) < 0.00000001f)


inline double mmSqrt(double v)
{
	volatile double res;
	__asm
	{
		fld v
		fsqrt
		fstp res
	}
	return res;
}


class clsPlane;
class clsMatrix;
class clsVector3;
class clsVector2;
class clsVector4;

//----------------------------
class clsVector2
{
protected:

public:

	union
	{
		struct
		{
			mmFloat x,y;
		};
		mmFloat v[2];
	};

	clsVector2();
	clsVector2(mmFloat X,mmFloat Y);
	clsVector2(const clsVector2 & v);
	clsVector2( mmFloat* ptr);
	~clsVector2();

	mmBool		operator>(const clsVector2 & v) const;
	mmBool		operator>=(const clsVector2 & v) const;
	mmBool		operator<(const clsVector2 & v) const;
	mmBool		operator<=(const clsVector2 & v) const;
	mmBool		operator==(const clsVector2 & v) const;
	mmBool		operator!=(const clsVector2 & v) const;
	clsVector2	operator+(const clsVector2 & v) const;
	clsVector2	operator-(const clsVector2 & v) const;
	clsVector2	operator*(const mmFloat & s) const;
	clsVector2	operator*(const mmInt & s) const;
	clsVector2	operator*(const mmDouble & s) const;
	clsVector2			operator*(const clsMatrix & m) const;
	clsVector2 &	operator+=(const clsVector2 & v);
	clsVector2 &	operator-=(const clsVector2 & v);
	clsVector2 &	operator*=(const mmFloat & s);
	clsVector2 &	operator*=(const mmInt & s);
	clsVector2 &	operator*=(const mmDouble & s);
	clsVector2 &		operator*=(const clsMatrix & m);
	clsVector2	operator - () const;
	clsVector2 & Normalize();
	mmFloat Length() const;
	mmFloat LengthSq() const;
	clsVector2 & Scale(const mmFloat s);
	clsVector2 & Lerp(const clsVector2 & v, const mmFloat k);
	clsVector2 & Transform(const clsMatrix & m);
	clsVector2 & TransformNormal(const clsMatrix & m);
	mmFloat Dot(const clsVector2 & v) const;
};
//----------------------------
class clsVector3
{
protected:

public:

	union
	{
		struct
		{
			mmFloat x,y,z;
		};
		mmFloat v[3];
	};

	clsVector3();
	clsVector3(mmFloat X,mmFloat Y,mmFloat Z);
	clsVector3(const clsVector3 & v);
	clsVector3( mmFloat* ptr);
	~clsVector3();

	clsVector3 &	operator=(mmFloat f);
	clsVector3 &	operator=(mmInt f);
	clsVector3 &	operator=(mmDouble f);
	clsVector3 &	operator=(const clsVector3 & v);
	mmBool		operator>(const clsVector3 & v) const;
	mmBool		operator>=(const clsVector3 & v) const;
	mmBool		operator<(const clsVector3 & v) const;
	mmBool		operator<=(const clsVector3 & v) const;
	mmBool		operator==(const clsVector3 & v) const;
	mmBool		operator!=(const clsVector3 & v) const;
	clsVector3	operator+(const clsVector3 & v) const
	{
		return clsVector3(x+v.x,y+v.y,z+v.z);
	};
	clsVector3	operator-(const clsVector3 & v) const
	{
		return clsVector3(x-v.x,y-v.y,z-v.z);
	};
	clsVector3	operator*(const mmFloat & s) const;
	clsVector3	operator*(const mmInt & s) const;
	clsVector3	operator*(const mmDouble & s) const;
	clsVector3			operator*(const clsMatrix & m) const;
	clsVector3	operator*(const clsVector3 & v) const
	{
		return clsVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	};
	clsVector3 &	operator+=(const clsVector3 & v)
	{
		x+=v.x;y+=v.y;z+=v.z;
		return *this;
	};
	clsVector3 &	operator-=(const clsVector3 & v);
	clsVector3 &	operator*=(const mmFloat & s);
	clsVector3 &	operator*=(const mmInt & s);
	clsVector3 &	operator*=(const mmDouble & s);
	clsVector3 &		operator*=(const clsMatrix & m);
	clsVector3 &	operator*=(const clsVector3 & v);
	clsVector3	operator - () const;
	clsVector3 & Normalize();
	clsVector3 & NormalizeFast();
	mmFloat Length() const;
	mmFloat LengthSq() const;
	clsVector3 & Scale(const mmFloat s);
	clsVector3 & Lerp(const clsVector3 & v, const mmFloat k)
	{
		(*this) += ((v-(*this))*k);
		return *this;	
	};
	clsVector3 & Transform(const clsMatrix & m);
	clsVector3 & TransformNormal(const clsMatrix & m);
	clsVector3 & Set(mmFloat val);//?
	clsVector3 & Negate();//?
	mmFloat Dot(const clsVector3 & v) const;
	mmBool		 AlmostZero() const;

	clsVector3 & MoveToVector(mmFloat x, mmFloat y, mmFloat z, mmFloat k);
	clsVector3 & MoveToVector(const clsVector3 & v, mmFloat k);
	mmFloat DistSq(const clsVector3 & v1) const;

	clsVector3 & CatmullRom(const clsVector3 & v1, const clsVector3 & v2, const clsVector3 & v3, const clsVector3 & v4, mmFloat k);
	clsVector3 & Hermite(const clsVector3 & v1, const clsVector3 & t1, const clsVector3 & v2, const clsVector3 & t2, mmFloat k);
};
//----------------------------
class clsVector4
{
public:

	union
	{
		struct
		{
			mmFloat x,y,z,w;
		};
		mmFloat v[4];
	};

	clsVector4();
	clsVector4(mmFloat Fill);
	clsVector4(mmFloat X,mmFloat Y,mmFloat Z,mmFloat W);
	~clsVector4();
	
	clsVector4 & Transform(const clsMatrix & m);
	
	
	mmBool operator==(const clsVector4 & v) const
	{
		return ((v.x == x) && (v.y == y) && (v.z == z) && (v.w == w)) ? true : false;
	};
	mmBool operator!=(const clsVector4 & v) const
	{
		return !operator==(v);//((v.x == x) && (v.y == y) && (v.z == z) && (v.w == w)) ? false : true;
	};

__inline clsVector4	operator*(const mmFloat & s) const
{
	return clsVector4(x*s, y*s, z*s, w*s);
};

__inline clsVector4	operator*=(const mmFloat & s)
{
	x*=s;y*=s;z*=s;w*=s;
	return *this;
};

__inline clsVector4 & Normalize()
{
	register mmFloat norm = 1.0f / sqrtf(x*x + y*y + z*z + w*w);
	*this *= norm;
	return *this;
};

};


//----------------------------
class clsQuat : public clsVector4
{
public:
	clsQuat() : clsVector4(){};
// void AQuatAxisAngle(QUAT *q, const VECTOR *v);
__inline clsQuat(const clsVector4 & v);
clsQuat &	operator=(const clsVector4 & v);
clsQuat &	Slerp(clsQuat & p, mmFloat t);

};




//----------------------------
class clsMatrix
{
protected:

public:

	union
	{
		struct
		{
			mmFloat        _11, _12, _13, _14;
			mmFloat        _21, _22, _23, _24;
			mmFloat        _31, _32, _33, _34;
			mmFloat        _41, _42, _43, _44;
		};
		mmFloat m[4][4];
	};

	clsMatrix();
	clsMatrix(const clsMatrix & m);
	clsMatrix(mmFloat* ptr);
	~clsMatrix();

	mmBool				operator==(const clsMatrix & m) const;
	mmBool		operator!=(const clsMatrix & m) const;
	clsMatrix			operator+(const clsMatrix & m) const;
	clsMatrix			operator-(const clsMatrix & m) const;
	clsMatrix			operator*(const clsMatrix & m) const;
	clsMatrix			operator*(const mmFloat & s) const;
	clsMatrix	operator*(const mmInt & s) const;
	clsMatrix	operator*(const mmDouble & s) const;
	clsMatrix &			operator+=(const clsMatrix & m);
	clsMatrix &			operator-=(const clsMatrix & m);
	clsMatrix &			operator*=(const mmFloat & s);
	clsMatrix &			operator*=(const mmInt & s);
	clsMatrix &			operator*=(const mmDouble & s);
	clsMatrix &			operator*=(const clsMatrix & m);
	clsMatrix			operator - () const;

	clsMatrix &			operator=(const clsQuat & quat);

	clsMatrix &			Identity();
	clsMatrix &			Inverse();
	mmBool				Inverse(clsMatrix& mxOut);
	clsMatrix &			Transpose();
	clsMatrix &			Translation(const mmFloat x,const mmFloat y,const mmFloat z);
	clsMatrix &			Translation(const clsVector3 & v);
	clsMatrix &			Scale(const mmFloat x,const mmFloat y,const mmFloat z);
	clsMatrix &			Scale(const clsVector3 & v);
	clsMatrix &			RotationX(const mmFloat a);
	clsMatrix &			RotationY(const mmFloat a);
	clsMatrix &			RotationZ(const mmFloat a);
	clsMatrix &			RotationVectorX(const clsVector3& v);
	clsMatrix &			RotationVectorY(const clsVector3& v);
	clsMatrix &			RotationVectorZ(const clsVector3& v);
	clsMatrix &			RotationAxis(const clsVector3 & v, const mmFloat a);

	clsMatrix &			LookAtLH(const clsVector3 & Eye,const clsVector3 & At,const clsVector3 & Up);
	clsMatrix &			LookAtRH(const clsVector3 & Eye,const clsVector3 & At,const clsVector3 & Up);
	clsMatrix &			OrthoLH(const mmFloat w,const mmFloat h,const mmFloat zn,const mmFloat zf);
	clsMatrix &			OrthoRH(const mmFloat w,const mmFloat h,const mmFloat zn,const mmFloat zf);
	clsMatrix &			OrthoOffCenterLH(const mmFloat l,const mmFloat r,const mmFloat b,const mmFloat t,const mmFloat zn,const mmFloat zf);
	clsMatrix &			OrthoOffCenterRH(const mmFloat l,const mmFloat r,const mmFloat b,const mmFloat t,const mmFloat zn,const mmFloat zf);
	clsMatrix &			PerspectiveFovLH(const mmFloat fovy,const mmFloat Aspect,const mmFloat zn,const mmFloat zf);
	clsMatrix &			PerspectiveFovRH(const mmFloat fovy,const mmFloat Aspect,const mmFloat zn,const mmFloat zf);
	clsMatrix &			PerspectiveLH(const mmFloat w,const mmFloat h,const mmFloat zn,const mmFloat zf);
	clsMatrix &			PerspectiveRH(const mmFloat w,const mmFloat h,const mmFloat zn,const mmFloat zf);
	clsMatrix &			PerspectiveOffCenterLH(const mmFloat l,const mmFloat r,const mmFloat b,const mmFloat t,const mmFloat zn,const mmFloat zf);
	clsMatrix &			PerspectiveOffCenterRH(const mmFloat l,const mmFloat r,const mmFloat b,const mmFloat t,const mmFloat zn,const mmFloat zf);
	clsMatrix &			Reflect(clsPlane P);
	clsMatrix &			Shadow(clsVector3 & Light, clsPlane P, mmBool DirectionalLight);
	// MozG
	clsVector3			GetTranslation();
	// fyrex
	clsMatrix &				SetTranslation(clsVector3 & v);
};
//----------------------------
class clsRay;
class clsPlane
{
protected:

public:

	union
	{
		struct
		{
			mmFloat a;
			mmFloat b;
			mmFloat c;
			mmFloat d;
		};
	};

	clsPlane();
	clsPlane(mmFloat A,mmFloat B,mmFloat C,mmFloat D);
	clsPlane(clsPlane & p);
	clsPlane( mmFloat* ptr);
	~clsPlane();

	mmBool		operator==(const clsPlane & p)const;
	mmBool		operator!=(const clsPlane & p)const;
	clsPlane &	operator=(const clsVector3 & v);
	clsPlane &	operator=(const clsVector4 & v);

	clsVector3	Vector3();
	clsVector4	Vector4();
	clsPlane &			FromPointNormal(const clsVector3 & Point,const clsVector3 & Normal);
	clsPlane &			FromPoints(const clsVector3 & v1,const clsVector3 & v2,const clsVector3 & v3);
	clsPlane &			Normalize();
	mmFloat		Dot(const clsVector3 & v);
	mmFloat		DotNormal(const clsVector3 & v);

	mmBool				IntersectedLine(const clsVector3 & v1,const clsVector3 & v2);
	mmBool				IntersectLine(clsVector3 & v_out, const clsVector3 & v1,const clsVector3 & v2);

	mmBool				IntersectedRay(const clsVector3 & p, clsVector3 v);
	mmBool				IntersectRay(clsVector3 & v_out, const clsVector3 & p,const clsVector3 & v);
	mmBool				IntersectRay(clsVector3 & v_out, const clsRay& ray);


	clsPlane &			FromMatrixLeft(const clsMatrix & m);
	clsPlane &			FromMatrixRight(const clsMatrix & m);
	clsPlane &			FromMatrixTop(const clsMatrix & m);
	clsPlane &			FromMatrixBottom(const clsMatrix & m);
	clsPlane &			FromMatrixFar(const clsMatrix & m);
	clsPlane &			FromMatrixNear(const clsMatrix & m);
	
	mmVoid MakeRotationMatrix(clsMatrix& mx);


};
//****************************

class clsRect
{
protected:

public:
	union
	{
		struct
		{
			mmFloat left;
			mmFloat top;
			mmFloat right;
			mmFloat bottom;
		};
		struct
		{
			mmFloat x1;
			mmFloat y1;
			mmFloat x2;
			mmFloat y2;
		};
	};
	clsRect();
	clsRect(mmFloat Left,mmFloat Top,mmFloat Right,mmFloat Bottom);

	mmBool		operator==(const clsRect & r) const;
	mmBool		operator!=(const clsRect & r) const;

	mmFloat Width();
	mmFloat Height();
};

//****************************
class clsColorRGB
{
public:
	mmByte	r, g, b;
	
public:
	clsColorRGB();
    clsColorRGB(mmByte r, mmByte g, mmByte b);
    operator mmDword();
};

//****************************
class clsColorRGBA
{
public:
	union
	{
		mmByte	r, g, b, a;
		mmFloat fr, fg, fb, fa;
	};
	
public:
	clsColorRGBA();
	clsColorRGBA(mmInt i);
    clsColorRGBA(mmByte r, mmByte g, mmByte b, mmByte a);
    clsColorRGBA(mmFloat r, mmFloat g, mmFloat b, mmFloat a);
    operator mmDword();
    operator D3DCOLORVALUE() const;
};
//****************************
class clsPoint2D
{
public:
	mmInt	x, y;

public:
	clsPoint2D();
    clsPoint2D(mmInt _x, mmInt _y);
};
//****************************
class clsRectInt
{
public:
	mmInt left, right, top, bottom;

public:
	clsRectInt();
	clsRectInt(mmInt _left, mmInt _right, mmInt _top, mmInt _bottom);
	mmBool IsInside(mmInt x, mmInt y);
	mmBool IsInside(clsPoint2D pt);

};

//============================================
// Ray class
//============================================
class clsRay
{
public:
	clsVector3	m_vOrigin;
	clsVector3	m_vDirection;

public:
	clsRay();
	clsRay(const clsVector3& origin, const clsVector3& direction);
	mmBool IntersectTriangle(const clsVector3& va,
							 const clsVector3& vb,
							 const clsVector3& vc,
							 mmFloat& fDistance,
							 clsVector3& vPoint) const;
};

//============================================
// Bounding box class
//============================================
class clsBoundBox
{
public:
	clsVector3	m_cMin, m_cMax;

public:
	clsBoundBox();
	clsBoundBox(const clsVector3& min, const clsVector3& max);
};

//============================================
// Bounding sphere class
//============================================
class clsBoundSphere
{
public:
	clsVector3	m_cCenter;
	mmFloat		m_fRadius;
	clsVector3	m_vSpeed;

public:
	clsBoundSphere();
	clsBoundSphere(const clsVector3& center, mmFloat radius);
	// ray intersection test
	mmBool RayIntersects(const clsRay& ray,
						 clsVector3& vNear,
						 clsVector3& vFar);
	// line intersection test
	mmBool LineIntersects(const clsVector3& a, const clsVector3& b);
	// bounding sphere intersection test
	mmBool SphereIntersects(const clsBoundSphere& sphere, 
							const clsVector3& vSelfSpeed,
							const clsVector3& vSphereSpeed,
							mmFloat& fFirstTime,
							mmFloat& fSecondTime);
	// point in sphere
	mmBool PointInSphere(const clsVector3& pt);
	// sliding vector
	mmVoid SlidingVector(const clsVector3& vSpeed,
						 const clsVector3& pt);
//	mmBool SphereIntersects(const clsBoundSphere& sphere);
};


//****************************


//****************************

//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
class clsRand
{
protected:
	mmUint m_seed;
public:
	clsRand();
	clsRand(mmUint seed);
	mmVoid init(mmUint seed);
	mmFloat frand(mmFloat range);
	mmInt rand();

	mmFloat lerpfrand(mmFloat k, mmBool cubic = mmtrue);
};

//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
enum enInterpolation {enInterLinear = 0, enInterSin, enInterUp, enInterDoun};
enum enLooping {enHold = 0, enLoop, enPingPong};
class clsInterpolation
{
protected:
	mmVoid init(mmFloat V1, mmFloat V2, enInterpolation Inter);
	mmFloat t;
public:
	clsInterpolation();
	clsInterpolation(mmFloat V);
	clsInterpolation(mmFloat V1, mmFloat V2);
	clsInterpolation(mmFloat V1, mmFloat V2, enInterpolation Inter);

	enInterpolation Interpolation;
	enLooping Looping;
	mmFloat v;
	mmFloat v1;
	mmFloat v2;
	mmFloat Time_K;

	mmVoid SetT(mmFloat T);
	mmVoid SetDeltaTime(mmFloat Tim);
	inline mmFloat GetT(){return t;};

};

//**************************************************************************************************
class clsRandRange
{
protected:
	//clsIntArr Probability;
	mmFloat deltaMiSec;
	class clsRandRangeItem
	{
		protected:			
		public:
			clsRandRangeItem()
			{
			//	deltaMiSec = 0.0f;
				lastMiSec = 0.0f;
				Probability = 1;
				MinMiSec = 1000.0f;
				ok = mmfalse;
			};
			//	mmFloat deltaMiSec;
				mmFloat lastMiSec;
				mmUint Probability;
				mmFloat MinMiSec;
				mmBool ok;
	};
	tmpRefArr <clsRandRangeItem> Items;
//	clsRand rand;

	mmInt all;
public:
	clsRandRange();
	mmVoid Update(const clsTimer * Timer, mmBool reset = mmtrue, mmFloat MSe = 0);
	mmVoid SetCount(mmUint count);
	mmVoid SetItem(mmUint id, mmUint probability, mmFloat min_MiSec);
	mmVoid Reset();
	mmInt GetMin(mmBool tim = mmtrue);
	mmInt GetMax(mmBool tim = mmtrue);
};
//**************************************************************************************************

#endif // _MHMMATH_INCLUDED