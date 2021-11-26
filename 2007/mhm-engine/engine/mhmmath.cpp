#include "mhmbase.h"

#pragma region PLANE

//***************************************************************************
// Buffers
//***************************************************************************
static clsMatrix	g_mxBuffer;



//***************************************************************************
// Mozg inlines
//***************************************************************************
inline mmBool mhmQuadraticFormula(mmFloat a,
								  mmFloat b,
								  mmFloat c,
								  mmFloat& r1,
								  mmFloat& r2)
{
	mmFloat q = b*b - 4*a*c;
	if (q >= 0)
	{
		mmFloat sq = (mmFloat)mmSqrt(q);
		mmFloat d  = 1.0f / (2*a);
		r1 = (-b + sq) * d;
		r2 = (-b - sq) * d;
		return mmtrue; // real roots (настоящие корни)
	}
	else
	{
		return mmfalse; // complex roots (комплексные корни == нафиг)
	}
}
inline mmVoid mhmSwap(mmFloat& a, mmFloat& b)
{
	mmFloat temp = a;
	a = b;
	b = temp;
}

//***************************************************************************
//	clsPlane
//---------------------------------------------------------------------------
clsPlane::clsPlane(mmFloat A,mmFloat B,mmFloat C,mmFloat D)
{
	a=A; b=B; c=C; d=D;
}
//---------------------------------------------------------------------------
clsPlane::clsPlane(clsPlane & p)
{
	a=p.a; b=p.b; c=p.c; d=p.d;
}
//---------------------------------------------------------------------------
clsPlane::clsPlane( mmFloat* ptr)
{
	memcpy(&a, ptr, sizeof(mmFloat) * 4);
}
//---------------------------------------------------------------------------
clsPlane::clsPlane()
{
}
//---------------------------------------------------------------------------
clsPlane::~clsPlane()
{
}
//---------------------------------------------------------------------------
clsPlane &	clsPlane::FromPointNormal(const clsVector3 & Point,const clsVector3 & Normal)
{
	a = Normal.x; b = Normal.y; c = Normal.z;
	d = - a*Point.x - b*Point.y - c*Point.z;
	return *this;
}
//---------------------------------------------------------------------------
clsPlane &	clsPlane::FromPoints(const clsVector3 & v1,const clsVector3 & v2,const clsVector3 & v3)
{
	clsVector3 n;
	n = ((v1-v2)*(v3-v2)).Normalize();
	a = n.x; b = n.y; c = n.z;
	d = - a*v2.x - b*v2.y - c*v2.z;
	return *this;
}
//---------------------------------------------------------------------------
clsPlane &	clsPlane::Normalize()
{
#define ONE_AS_INTEGER ((mmDword)(0x3F800000))
	mmFloat L_squared, one_over_L;
	L_squared = (a * a) + (b * b)+ (c * c);
	mmDword   tmp = ((ONE_AS_INTEGER << 1) + ONE_AS_INTEGER - *(mmDword*)&L_squared) >> 1;
	mmFloat _y = *(mmFloat*)&tmp;
	one_over_L = _y * (1.47f - 0.47f * L_squared * _y * _y);
	a *= one_over_L;
	b *= one_over_L;
	c *= one_over_L;
	d *= one_over_L;
	return *this;
}
//---------------------------------------------------------------------------
mmVoid clsPlane::MakeRotationMatrix(clsMatrix& mx)
{
	// find system basis
	clsVector3 vNormal(a, b, c);
	clsVector3 vZAxis(0.0f, 0.0f, 1.0f);
	//clsVector
	vNormal.Normalize();
	clsVector3 vRotAxis = vZAxis * vNormal;
	if (vRotAxis.LengthSq() == 0.0f)
	{
		mx.Identity();
		return;
	}
	vRotAxis.Normalize();
	clsVector3 vBasis   = vNormal * vRotAxis;
	vBasis.Normalize();
	// find cos - sin
	mmFloat fSin = vZAxis.Dot(vBasis);
	mmFloat fCos = vZAxis.Dot(vNormal);
	
	mx._11 = fCos + (1 - fCos) * vRotAxis.x * vRotAxis.x;
	mx._12 = (1 - fCos) * vRotAxis.x * vRotAxis.y + fSin * vRotAxis.z;
	mx._13 = (1 - fCos) * vRotAxis.x * vRotAxis.z - fSin * vRotAxis.y;
	mx._14 = 0;
	mx._21 = (1 - fCos) * vRotAxis.x * vRotAxis.y - vRotAxis.z * fSin;
	mx._22 = fCos + (1 - fCos) * vRotAxis.y * vRotAxis.y;
	mx._23 = (1 - fCos) * vRotAxis.y * vRotAxis.z + vRotAxis.x * fSin;
	mx._24 = 0;
	mx._31 = (1 - fCos) * vRotAxis.x * vRotAxis.z + fSin * vRotAxis.y;
	mx._32 = (1 - fCos) * vRotAxis.y * vRotAxis.z - fSin * vRotAxis.x;
	mx._33 = fCos + (1 - fCos) * vRotAxis.z * vRotAxis.z;
	mx._34 = 0;
	mx._41 = 0;
	mx._42 = 0;
	mx._43 = 0;
	mx._44 = 1;
}
//---------------------------------------------------------------------------
clsVector3 clsPlane::Vector3()
{
	return clsVector3(a,b,c);
}
//---------------------------------------------------------------------------
clsVector4 clsPlane::Vector4()
{
	return clsVector4(a,b,c,d);
}
//---------------------------------------------------------------------------
clsPlane & clsPlane::operator=(const clsVector3 & v)
{
	a=v.x; b=v.y; c=v.z;
	return *this;
}
//---------------------------------------------------------------------------
clsPlane & clsPlane::operator=(const clsVector4 & v)
{
	a=v.x; b=v.y; c=v.z; d=v.w;
	return *this;
}
//---------------------------------------------------------------------------
mmFloat clsPlane::Dot(const clsVector3 & v)
{
	return a*v.x + b*v.y + c*v.z + d;
}
//---------------------------------------------------------------------------
mmFloat clsPlane::DotNormal(const clsVector3 & v)
{
	return a*v.x + b*v.y + c*v.z;
}
//---------------------------------------------------------------------------
mmBool clsPlane::IntersectedLine(const clsVector3 & v1,const clsVector3 & v2)
{
	return (this->Dot(v1) * this->Dot(v2)) < 0.0f;
}
//---------------------------------------------------------------------------
mmBool clsPlane::IntersectLine(clsVector3 & v_out, const clsVector3 & v1,const clsVector3 & v2)
{
	if(!IntersectedLine(v1,v2))
		return mmfalse;
	mmFloat Numerator,Denominator,dist;
	clsVector3 vPoint, vLineDir;
	vLineDir = (v2 - v1).Normalize();  
	Numerator = -(this->Dot(v1));
	Denominator = this->Vector3().Dot(vLineDir);
	if(Denominator == 0.0f)
		return mmfalse;          
	dist = Numerator / Denominator;          
	v_out.x = (v1.x + (vLineDir.x * dist));
	v_out.y = (v1.y + (vLineDir.y * dist));
	v_out.z = (v1.z + (vLineDir.z * dist));
	return mmtrue;
}
//---------------------------------------------------------------------------
mmBool clsPlane::IntersectedRay(const clsVector3 & p, clsVector3 v)
{
	v += p;
	if(IntersectedLine(p,v))
		return mmtrue;
	return mmAbs(this->Dot(p)) >  mmAbs(this->Dot(v));
}
//---------------------------------------------------------------------------
mmBool clsPlane::IntersectRay(clsVector3 & v_out, const clsVector3 & p,const clsVector3 & v)
{
//	if(!IntersectedRay(p,v))
//		return mmfalse;
	mmFloat Numerator,Denominator,dist;
	clsVector3 vPoint, vLineDir;
	vLineDir = v;
	vLineDir.Normalize();  
	Numerator = -(this->Dot(p));
	Denominator = DotNormal(vLineDir);
	if(Denominator == 0.0f)
		return mmfalse;          
	dist = Numerator / Denominator;          
	v_out.x = (p.x + (vLineDir.x * dist));
	v_out.y = (p.y + (vLineDir.y * dist));
	v_out.z = (p.z + (vLineDir.z * dist));
	return mmtrue;
}
//---------------------------------------------------------------------------
mmBool clsPlane::IntersectRay(clsVector3 & v_out, const clsRay& ray)
{
	mmFloat Numerator,Denominator,dist;
	Denominator = DotNormal(ray.m_vDirection);
	if (mmAlmostZero(Denominator))	// ray is parallel to plane
		return mmfalse;          
	Numerator = -Dot(ray.m_vOrigin);
	dist = Numerator / Denominator;
	if (dist < 0.0f)
		return mmfalse;	// ray intersects behind origin
	v_out = ray.m_vOrigin + (ray.m_vDirection * dist);
	return mmtrue;
}
//---------------------------------------------------------------------------
clsPlane & clsPlane::FromMatrixLeft(const clsMatrix & m)
{
	a = m._14 + m._11; 
	b = m._24 + m._21; 
	c = m._34 + m._31; 
	d = m._44 + m._41;
	this->Normalize();
	return *this;
}
//---------------------------------------------------------------------------
clsPlane & clsPlane::FromMatrixRight(const clsMatrix & m)
{
	a = m._14 - m._11; 
	b = m._24 - m._21; 
	c = m._34 - m._31; 
	d = m._44 - m._41;
	this->Normalize();
	return *this;
}
//---------------------------------------------------------------------------
clsPlane & clsPlane::FromMatrixTop(const clsMatrix & m)
{
	a = m._14 - m._12; 
	b = m._24 - m._22; 
	c = m._34 - m._32; 
	d = m._44 - m._42;
	this->Normalize();
	return *this;
}
//---------------------------------------------------------------------------
clsPlane & clsPlane::FromMatrixBottom(const clsMatrix & m)
{
	a = m._14 + m._12; 
	b = m._24 + m._22; 
	c = m._34 + m._32; 
	d = m._44 + m._42;
	this->Normalize();
	return *this;
}
//---------------------------------------------------------------------------
clsPlane & clsPlane::FromMatrixFar(const clsMatrix & m)
{
	a = m._14 - m._13; 
	b = m._24 - m._23; 
	c = m._34 - m._33; 
	d = m._44 - m._43;
	this->Normalize();
	return *this;
}
//---------------------------------------------------------------------------
clsPlane & clsPlane::FromMatrixNear(const clsMatrix & m)
{
	a = m._14 + m._13; 
	b = m._24 + m._23; 
	c = m._34 + m._33; 
	d = m._44 + m._43;
	this->Normalize();
	return *this;
}
//---------------------------------------------------------------------------
mmBool clsPlane::operator==(const clsPlane & p)const
{
	return (a==p.a)&&(b==p.b)&&(c==p.c)&&(d==p.d);
}
//---------------------------------------------------------------------------
mmBool clsPlane::operator!=(const clsPlane & p)const
{
	return (a==p.a)&&(b==p.b)&&(c==p.c)&&(d==p.d);
}


#pragma endregion

#pragma region VECTOR2

//***************************************************************************
//	clsVector2
//---------------------------------------------------------------------------
clsVector2::clsVector2(mmFloat X,mmFloat Y)
{
	x = X; y = Y;
}
//---------------------------------------------------------------------------
clsVector2::clsVector2(const clsVector2 & v)
{
	x = v.x; y = v.y;
}
//---------------------------------------------------------------------------
clsVector2::clsVector2( mmFloat* ptr)
{
	memcpy(&x, ptr, sizeof(mmFloat) * 2);
}
//---------------------------------------------------------------------------
clsVector2::clsVector2()
{
}
//---------------------------------------------------------------------------
clsVector2::~clsVector2()
{
}
//---------------------------------------------------------------------------
mmBool clsVector2::operator>(const clsVector2 & v) const
{
	return (this->Length() > v.Length());
}
//---------------------------------------------------------------------------
mmBool clsVector2::operator>=(const clsVector2 & v) const
{
	return (this->Length() >= v.Length());
}
//---------------------------------------------------------------------------
mmBool clsVector2::operator<(const clsVector2 & v) const
{
	return (this->Length() < v.Length());
}
//---------------------------------------------------------------------------
mmBool clsVector2::operator<=(const clsVector2 & v) const
{
	return (this->Length() <= v.Length());
}
//---------------------------------------------------------------------------
mmBool clsVector2::operator==(const clsVector2 & v) const
{
	return ((v.x == x) && (v.y == y)) ? mmtrue : mmfalse;
}
//---------------------------------------------------------------------------
mmBool clsVector2::operator!=(const clsVector2 & v) const
{
	return ((v.x == x) && (v.y == y)) ? mmfalse : mmtrue;
}
//---------------------------------------------------------------------------
clsVector2 clsVector2::operator+(const clsVector2 & v) const
{
	return clsVector2(x+v.x,y+v.y);
}
//---------------------------------------------------------------------------
clsVector2 clsVector2::operator-(const clsVector2 & v) const
{
	return clsVector2(x-v.x,y-v.y);
}
//---------------------------------------------------------------------------
clsVector2 clsVector2::operator*(const mmFloat & s) const
{
	return clsVector2(x*s,y*s);
}
//---------------------------------------------------------------------------
clsVector2 clsVector2::operator*(const mmInt & s) const
{
	return clsVector2(x*(mmFloat)s,y*(mmFloat)s);
}
//---------------------------------------------------------------------------
clsVector2 clsVector2::operator*(const mmDouble & s) const
{
	return clsVector2(x*(mmFloat)s,y*(mmFloat)s);
}
//---------------------------------------------------------------------------
clsVector2 clsVector2::operator*(const clsMatrix & m) const
{
	clsVector2 v_out;
	v_out.x = x*m._11 + y*m._21 + m._31 + m._41;
	v_out.y = x*m._12 + y*m._22 + m._32 + m._42;
	return v_out;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::operator+=(const clsVector2 & v)
{
	x+=v.x;y+=v.y;
	return *this;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::operator-=(const clsVector2 & v)
{
	x-=v.x;y-=v.y;
	return *this;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::operator*=(const mmFloat & s)
{
	x*=s;y*=s;
	return *this;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::operator*=(const mmInt & s)
{
	x*=(mmFloat)s;y*=(mmFloat)s;
	return *this;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::operator*=(const mmDouble & s)
{
	x*=(mmFloat)s;y*=(mmFloat)s;
	return *this;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::operator*=(const clsMatrix & m)
{
	clsVector2 v_out;
	v_out.x = x*m._11 + y*m._21 + m._31 + m._41;
	v_out.y = x*m._12 + y*m._22 + m._32 + m._42;
	x = v_out.x; y = v_out.y;
	return *this;
}
//---------------------------------------------------------------------------
clsVector2 clsVector2::operator - () const
{
	return clsVector2(-x,-y);
}
//---------------------------------------------------------------------------
mmFloat clsVector2::Dot(const clsVector2 & v) const
{
	return x*v.x + y*v.y;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::Normalize()
{
#define ONE_AS_INTEGER ((mmDword)(0x3F800000))
	mmFloat L_squared, one_over_L;
	L_squared = (x * x) + (y * y);
	mmDword   tmp = ((ONE_AS_INTEGER << 1) + ONE_AS_INTEGER - *(mmDword*)&L_squared) >> 1;
	mmFloat _y = *(mmFloat*)&tmp;
	one_over_L = _y * (1.47f - 0.47f * L_squared * _y * _y);
	x *= one_over_L;
	y *= one_over_L;
	return *this;
}
//---------------------------------------------------------------------------
mmFloat clsVector2::Length() const
{
	return (mmFloat)mmSqrt( x*x + y*y );
}
//---------------------------------------------------------------------------
mmFloat clsVector2::LengthSq() const
{
	return ( x*x + y*y);
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::Scale(const mmFloat s)
{
	this->Normalize();
	x *= s; y*=s;
	return *this;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::Lerp(const clsVector2 & v, const mmFloat k)
{
	(*this) += ((v-(*this))*k);
	return *this;	
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::Transform(const clsMatrix & m)
{
	clsVector2 v_out;
	v_out.x = x*m._11 + y*m._21 + m._31 + m._41;
	v_out.y = x*m._12 + y*m._22 + m._32 + m._42;
	x = v_out.x; y = v_out.y;
	return *this;
}
//---------------------------------------------------------------------------
clsVector2 & clsVector2::TransformNormal(const clsMatrix & m)
{
	clsVector2 v_out;
	v_out.x = x*m._11 + y*m._21 + m._31;
	v_out.y = x*m._12 + y*m._22 + m._32;
	x = v_out.x; y = v_out.y;
	return *this;
}
#pragma endregion

#pragma region VECTOR3

//***************************************************************************
//	clsVector3
//---------------------------------------------------------------------------
clsVector3::clsVector3(mmFloat X,mmFloat Y,mmFloat Z)
{
	x = X; y = Y; z = Z;
}
//---------------------------------------------------------------------------
clsVector3::clsVector3(const clsVector3 & v)
{
	x = v.x; y = v.y; z = v.z;
}
//---------------------------------------------------------------------------
clsVector3::clsVector3( mmFloat* ptr)
{
	memcpy(&x, ptr, sizeof(mmFloat) * 3);
}
//---------------------------------------------------------------------------
clsVector3::clsVector3()
{
}
//---------------------------------------------------------------------------
clsVector3::~clsVector3()
{
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator=(const clsVector3 & v)
{
	x = v.x; y = v.y; z = v.z;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator=(mmFloat f)
{
	x = y = z = f;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator=(mmInt f)
{
	x = y = z = (mmFloat)f;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator=(mmDouble f)
{
	x = y = z = (mmFloat)f;
	return *this;
}
//---------------------------------------------------------------------------
mmBool clsVector3::operator>(const clsVector3 & v) const
{
	return (this->Length() > v.Length());
}
//---------------------------------------------------------------------------
mmBool clsVector3::operator>=(const clsVector3 & v) const
{
	return (this->Length() >= v.Length());
}
//---------------------------------------------------------------------------
mmBool clsVector3::operator<(const clsVector3 & v) const
{
	return (this->Length() < v.Length());
}
//---------------------------------------------------------------------------
mmBool clsVector3::operator<=(const clsVector3 & v) const
{
	return (this->Length() <= v.Length());
}
//---------------------------------------------------------------------------
mmBool clsVector3::operator==(const clsVector3 & v) const
{
	return ((v.x == x) && (v.y == y) && (v.z == z)) ? mmtrue : mmfalse;
}
//---------------------------------------------------------------------------
mmBool clsVector3::operator!=(const clsVector3 & v) const
{
	return ((v.x == x) && (v.y == y) && (v.z == z)) ? mmfalse : mmtrue;
}
//---------------------------------------------------------------------------
/*
clsVector3 clsVector3::operator+(const clsVector3 & v) const
{
	return clsVector3(x+v.x,y+v.y,z+v.z);
}
*/
//---------------------------------------------------------------------------
/*
clsVector3 clsVector3::operator-(const clsVector3 & v) const
{
	return clsVector3(x-v.x,y-v.y,z-v.z);
}
*/
//---------------------------------------------------------------------------
clsVector3 clsVector3::operator*(const mmFloat & s) const
{
	return clsVector3(x*s,y*s,z*s);
}
//---------------------------------------------------------------------------
clsVector3 clsVector3::operator*(const mmInt & s) const
{
	return clsVector3(x*(mmFloat)s,y*(mmFloat)s,z*(mmFloat)s);
}
//---------------------------------------------------------------------------
clsVector3 clsVector3::operator*(const mmDouble & s) const
{
	return clsVector3(x*(mmFloat)s,y*(mmFloat)s,z*(mmFloat)s);
}
//---------------------------------------------------------------------------
clsVector3 clsVector3::operator*(const clsMatrix & m) const
{
	clsVector3 v_out;
	v_out.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	v_out.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	v_out.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	return v_out;
}
//---------------------------------------------------------------------------
/*
clsVector3 clsVector3::operator*(const clsVector3 & v) const
{
	return clsVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
*/
//---------------------------------------------------------------------------
/*
clsVector3 & clsVector3::operator+=(const clsVector3 & v)
{
	x+=v.x;y+=v.y;z+=v.z;
	return *this;
}*/
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator-=(const clsVector3 & v)
{
	x-=v.x;y-=v.y;z-=v.z;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator*=(const mmFloat & s)
{
	x*=s;y*=s;z*=s;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator*=(const mmInt & s)
{
	x*=(mmFloat)s;y*=(mmFloat)s;z*=(mmFloat)s;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator*=(const mmDouble & s)
{
	x*=(mmFloat)s;y*=(mmFloat)s;z*=(mmFloat)s;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator*=(const clsMatrix & m)
{
	clsVector3 v_out;
	v_out.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	v_out.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	v_out.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	x = v_out.x; y = v_out.y; z = v_out.z;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::operator*=(const clsVector3 & v)
{
	mmFloat _x,_y,_z;
	_x = y * v.z - z * v.y;
	_y = z * v.x - x * v.z;
	_z = x * v.y - y * v.x;
	x = _x; y = _y; z = _z;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 clsVector3::operator - () const
{
	return clsVector3(-x,-y,-z);
}
//---------------------------------------------------------------------------
mmFloat clsVector3::Dot(const clsVector3 & v) const
{
	return x*v.x + y*v.y + z*v.z;
}
//---------------------------------------------------------------------------
mmBool clsVector3::AlmostZero() const
{
	return (mmAlmostZero(x)) && (mmAlmostZero(y)) && (mmAlmostZero(z));
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::Normalize()
{
	mmFloat l = (mmFloat)mmSqrt(x*x + y*y + z*z);
	if (l == 0.0f)
		return *this;
	l = 1.0f / l;
	x *= l;
	y *= l;
	z *= l;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::NormalizeFast()
{
#define ONE_AS_INTEGER ((mmDword)(0x3F800000))
	mmFloat L_squared, one_over_L;
	L_squared = (x * x) + (y * y)+ (z * z);
	mmDword   tmp = ((ONE_AS_INTEGER << 1) + ONE_AS_INTEGER - *(mmDword*)&L_squared) >> 1;
	mmFloat _y = *(mmFloat*)&tmp;
	one_over_L = _y * (1.47f - 0.47f * L_squared * _y * _y);
	x *= one_over_L;
	y *= one_over_L;
	z *= one_over_L;
	return *this;
}
//---------------------------------------------------------------------------
mmFloat clsVector3::Length() const
{
	return (mmFloat)mmSqrt(x*x + y*y + z*z);
}
//---------------------------------------------------------------------------
mmFloat clsVector3::LengthSq() const
{
	return ( x*x + y*y + z*z );
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::Scale(const mmFloat s)
{
	this->Normalize();
	x *= s; y*=s; z *= s;
	return *this;
}
//---------------------------------------------------------------------------
/*
clsVector3 & clsVector3::Lerp(const clsVector3 & v, const mmFloat k)
{
	(*this) += ((v-(*this))*k);
	return *this;	
}
*/
//---------------------------------------------------------------------------
clsVector3 & clsVector3::Transform(const clsMatrix & m)
{
	clsVector3 v_out;
	v_out.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	v_out.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	v_out.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	x = v_out.x; y = v_out.y; z = v_out.z;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::TransformNormal(const clsMatrix & m)
{
	clsVector3 v_out;
	v_out.x = x*m._11 + y*m._21 + z*m._31;
	v_out.y = x*m._12 + y*m._22 + z*m._32;
	v_out.z = x*m._13 + y*m._23 + z*m._33;
	x = v_out.x; y = v_out.y; z = v_out.z;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3& clsVector3::Set(mmFloat val)
{
	x = y = z = val;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3& clsVector3::Negate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::CatmullRom(const clsVector3 & v1, const clsVector3 & v2, const clsVector3 & v3, const clsVector3 & v4, mmFloat k)
{
	mmFloat k2 = k*k, k3 = k*k*k;
	x = ((-k3 + 2*k2 - k)*v1.x + (3*k3- 5*k2 + 2)*v2.x + (-3*k3 + 4*k2 + k)*v3.x + (k3 - k2)*v4.x) * 0.5f;
	y = ((-k3 + 2*k2 - k)*v1.y + (3*k3- 5*k2 + 2)*v2.y + (-3*k3 + 4*k2 + k)*v3.y + (k3 - k2)*v4.y) * 0.5f;
	z = ((-k3 + 2*k2 - k)*v1.z + (3*k3- 5*k2 + 2)*v2.z + (-3*k3 + 4*k2 + k)*v3.z + (k3 - k2)*v4.z) * 0.5f;	
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::Hermite(const clsVector3 & v1, const clsVector3 & t1, const clsVector3 & v2, const clsVector3 & t2, mmFloat k)
{
	mmFloat k2 = k*k, k3 = k*k*k, _k = 1.0f - k;
	x = ((_k*v1.x+3*k*t1.x) * _k +3*k2*v2.x) * _k +k3*t2.x;
	y = ((_k*v1.y+3*k*t1.y) * _k +3*k2*v2.y) * _k +k3*t2.y;
	z = ((_k*v1.z+3*k*t1.z) * _k +3*k2*v2.z) * _k +k3*t2.z;
	return *this;
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::MoveToVector(mmFloat x, mmFloat y, mmFloat z, mmFloat k)
{
	return MoveToVector(clsVector3(x,y,z),k);
}
//---------------------------------------------------------------------------
clsVector3 & clsVector3::MoveToVector(const clsVector3 & v, mmFloat k)
{
	clsVector3 n = v - *this;
	mmFloat dist = n.LengthSq();
	n.NormalizeFast();
	n *= k;
	if( n.LengthSq() >= dist)
	{
		*this = v;
		return *this;
	}
	*this += n;
	return *this;
}

mmFloat	clsVector3::DistSq(const clsVector3 & v1) const
{
	clsVector3 v = v1 - *this;
	return (v.x*v.x + v.y*v.y + v.z*v.z);
}
//---------------------------------------------------------------------------

#pragma endregion

#pragma region VECTOR4

//***************************************************************************
//	clsVector4
//---------------------------------------------------------------------------
clsVector4::clsVector4()
{
}
//---------------------------------------------------------------------------
clsVector4::clsVector4(mmFloat Fill)
{
	x = y = z = w = Fill;
}
//---------------------------------------------------------------------------
clsVector4::clsVector4(mmFloat X,mmFloat Y,mmFloat Z,mmFloat W)
{
	x = X; y = Y; z = Z; w = W;
}
//---------------------------------------------------------------------------
clsVector4::~clsVector4()
{
}
//---------------------------------------------------------------------------
clsVector4 & clsVector4::Transform(const clsMatrix & m)
{
	clsVector4 v_out;
	v_out.x = x*m._11 + y*m._21 + z*m._31 + w*m._41;
	v_out.y = x*m._12 + y*m._22 + z*m._32 + w*m._42;
	v_out.z = x*m._13 + y*m._23 + z*m._33 + w*m._43;
	v_out.w = x*m._14 + y*m._24 + z*m._34 + w*m._44;
	x = v_out.x; y = v_out.y; z = v_out.z; w = v_out.w;
	return *this;
}

#pragma endregion

#pragma region QUAT

clsQuat &	clsQuat::operator=(const clsVector4 & v)
{

	register mmFloat ang = v.w*0.5f;
	register mmFloat sin_a = sinf(ang);

    x = v.x * sin_a;
    y = v.y * sin_a;
    z = v.z * sin_a;
    w = cosf(ang);

	Normalize();
	return *this;
};

clsQuat::clsQuat(const clsVector4 & v)
{
	operator=(v);
};


clsQuat & clsQuat::Slerp(clsQuat & p, mmFloat t)
{
  double omega, cosom, sinom, scale0, scale1;

  // COS
  cosom = x*p.x + y*p.y + z*p.z + w*p.w;

  if ( cosom < 0.0 )
  { 
    cosom = -cosom;
    scale1 = -1.0;
  }
  else
  {
    scale1 = 1.0;
  }

  // 0 < cosom < 1
  if ( cosom < 0.9 )
  {
    omega = acos(cosom);
    sinom = sin(omega);
    scale0 = sin((1.0 - t) * omega) / sinom;
    scale1 *= sin(t * omega) / sinom;
  }
  else
  {        
    scale0 = 1.0 - t;
    scale1 *= t;
  }

  x = (mmFloat) (scale0 * x + scale1 * p.x);
  y = (mmFloat) (scale0 * y + scale1 * p.y);
  z = (mmFloat) (scale0 * z + scale1 * p.z);
  w = (mmFloat) (scale0 * w + scale1 * p.w);
  
  return *this;
}

/*

void AMatrixQuat(MATRIX *m, const QUAT * quat);
void AQuatMatrix(QUAT * quat, const MATRIX *mo);
void AQuatSlerp(QUAT * res, QUAT * q, QUAT * p, float t);
*/



#pragma endregion

#pragma region MATRIX

//***************************************************************************
//	clsMatrix
//---------------------------------------------------------------------------
clsMatrix::clsMatrix()
{	
}
//---------------------------------------------------------------------------
clsMatrix::clsMatrix(const clsMatrix & m)
{
	*this = m;
}
//---------------------------------------------------------------------------
clsMatrix::clsMatrix(mmFloat* ptr)
{
	memcpy(this->m, ptr, sizeof(mmFloat) * 16);
}
//---------------------------------------------------------------------------
clsMatrix::~clsMatrix()
{
}
//---------------------------------------------------------------------------
mmBool clsMatrix::operator==(const clsMatrix & m) const
{
	mmInt z,x;
	for(z=0;z<4;z++)
		for(x=0;x<4;x++)
			if(this->m[z][x] != m.m[z][x])
				return mmfalse;
	return mmtrue;
}
//---------------------------------------------------------------------------
mmBool clsMatrix::operator!=(const clsMatrix & m) const
{
	return !(*this == m);
}
//---------------------------------------------------------------------------
clsMatrix clsMatrix::operator+(const clsMatrix & m) const
{
	clsMatrix m_out;
	mmInt z,x;
	for(z=0;z<4;z++)
		for(x=0;x<4;x++)
			m_out.m[z][x] = this->m[z][x] + m.m[z][x];
	return m_out;
}
//---------------------------------------------------------------------------
clsMatrix clsMatrix::operator-(const clsMatrix & m) const
{
	clsMatrix m_out;
	mmInt z,x;
	for(z=0;z<4;z++)
		for(x=0;x<4;x++)
			m_out.m[z][x] = this->m[z][x] - m.m[z][x];
	return m_out;
}
//---------------------------------------------------------------------------
clsMatrix clsMatrix::operator*(const clsMatrix & mx) const
{
//	(clsMatrix*)D3DXMatrixMultiply((D3DXMATRIX*)(&g_mxBuffer), (D3DXMATRIX*)(this) , ((D3DXMATRIX*)(&mx)));
	// return g_mxBuffer;


	int i,j;
	//clsMatrix m_out;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			g_mxBuffer.m[i][j] = this->m[i][0]*mx.m[0][j]+this->m[i][1]*mx.m[1][j]+this->m[i][2]*mx.m[2][j]+this->m[i][3]*mx.m[3][j];
//			for(int l = 0; l < 4; l++)
//			{
//				g_mxBuffer.m[i][j] = g_mxBuffer.m[i][j]+this->m[i][l]*mx.m[l][j];
//			}
		}
	}
	return g_mxBuffer;
	//m_out.m[0][0] = m[
/*	g_mxBuffer._11 = (mx._11 * _11) + (mx._12 * _21) + (mx._13 * _31) + (mx._14 * _41);
	g_mxBuffer._12 = (mx._11 * _12) + (mx._12 * _22) + (mx._13 * _32) + (mx._14 * _42);
	g_mxBuffer._13 = (mx._11 * _13) + (mx._12 * _23) + (mx._13 * _33) + (mx._14 * _43);
	g_mxBuffer._14 = (mx._11 * _14) + (mx._12 * _24) + (mx._13 * _34) + (mx._14 * _44);
	
	g_mxBuffer._21 = (mx._21 * _11) + (mx._22 * _21) + (mx._23 * _31) + (mx._24 * _41);
	g_mxBuffer._22 = (mx._21 * _12) + (mx._22 * _22) + (mx._23 * _32) + (mx._24 * _42);
	g_mxBuffer._23 = (mx._21 * _13) + (mx._22 * _23) + (mx._23 * _33) + (mx._24 * _43);
	g_mxBuffer._24 = (mx._21 * _14) + (mx._22 * _24) + (mx._23 * _34) + (mx._24 * _44);
	
	g_mxBuffer._31 = (mx._31 * _11) + (mx._32 * _21) + (mx._33 * _31) + (mx._34 * _41);
	g_mxBuffer._32 = (mx._31 * _12) + (mx._32 * _22) + (mx._33 * _32) + (mx._34 * _42);
	g_mxBuffer._33 = (mx._31 * _13) + (mx._32 * _23) + (mx._33 * _33) + (mx._34 * _43);
	g_mxBuffer._34 = (mx._31 * _14) + (mx._32 * _24) + (mx._33 * _34) + (mx._34 * _44);
	
	g_mxBuffer._41 = (mx._41 * _11) + (mx._42 * _21) + (mx._43 * _31) + (mx._44 * _41);
	g_mxBuffer._42 = (mx._41 * _12) + (mx._42 * _22) + (mx._43 * _32) + (mx._44 * _42);
	g_mxBuffer._43 = (mx._41 * _13) + (mx._42 * _23) + (mx._43 * _33) + (mx._44 * _43);
	g_mxBuffer._44 = (mx._41 * _14) + (mx._42 * _24) + (mx._43 * _34) + (mx._44 * _44);
	return g_mxBuffer; */
}
//---------------------------------------------------------------------------
clsMatrix clsMatrix::operator*(const mmFloat & s) const
{
	clsMatrix m_out;
	mmInt z,x;
	for(z=0;z<4;z++)
		for(x=0;x<4;x++)
			m_out.m[z][x] = this->m[z][x] * s;
	return m_out;
}
//---------------------------------------------------------------------------
clsMatrix clsMatrix::operator*(const mmInt & s) const
{
	return (*this) * ((mmFloat)s);
}
//---------------------------------------------------------------------------
clsMatrix clsMatrix::operator*(const mmDouble & s) const
{
	return (*this) * ((mmFloat)s);
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::operator+=(const clsMatrix & m)
{
	mmInt z,x;
	for(z=0;z<4;z++)
		for(x=0;x<4;x++)
			this->m[z][x] += m.m[z][x];
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::operator-=(const clsMatrix & m)
{
	mmInt z,x;
	for(z=0;z<4;z++)
		for(x=0;x<4;x++)
			this->m[z][x] -= m.m[z][x];
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::operator*=(const clsMatrix & m)
{
	/*
	 (clsMatrix*)D3DXMatrixMultiply((D3DXMATRIX*)this, (D3DXMATRIX*)this , ((D3DXMATRIX*)(&m)));
	 return *this;

	
	int i,j,l;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			g_mxBuffer.m[i][j] = 0;
			for(l = 0; l < 4; l++)
			{
				g_mxBuffer.m[i][j] = g_mxBuffer.m[i][j]+this->m[i][l]*m.m[l][j];
			}
		}
	}*/
	operator*(m);
	*this = g_mxBuffer;
	return *this;
	
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::operator*=(const mmFloat & s)
{
	mmInt z,x;
	for(z=0;z<4;z++)
		for(x=0;x<4;x++)
			this->m[z][x] *= s;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::operator*=(const mmInt & s)
{
	return (*this) *= ((mmFloat)s);
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::operator*=(const mmDouble & s)
{
	return (*this) *= ((mmFloat)s);
}
//---------------------------------------------------------------------------
clsMatrix clsMatrix::operator - () const
{
	clsMatrix m_out;
	mmInt z,x;
	for(z=0;z<4;z++)
		for(x=0;x<4;x++)
			m_out.m[z][x] = -this->m[z][x];
	return m_out;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Identity()
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = _22 = _33 = _44 = 1.0f;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Inverse()
{
	clsMatrix m_out,mm;
	Inverse(m_out);
	*this = m_out;
	return *this;


/*
	mmInt i,j,k;
	mmFloat sk, sz;
	clsMatrix m_out,mm;
	m_out.Identity();
	mm=*this;
	for( i=0;i<4;i++)
	{
		if(m[i][i] == 0)
		{
			*this = mm;
			_41 = -_41;	
			_42 = -_42;	
			_43 = -_43;	
			_44 = -_44;
			return *this;
		}
		sk = 1.0f/m[i][i];
		for( j=0;j<4;j++)
		{
			if(i == j)
				continue;
			sz = sk*m[j][i];
			for( k=0;k<4;k++)
			{
				m[j][k] -= sz*m[i][k];
				m_out.m[j][k] -= sz*m_out.m[i][k];
			}
		}
		for( k=0;k<4;k++)
		{
			m[i][k] = sk*m[i][k];
			m_out.m[i][k] = sk*m_out.m[i][k];
		}
	}
	*this = m_out;
	return *this;*/
	
}
//---------------------------------------------------------------------------
mmBool clsMatrix::Inverse(clsMatrix& mxOut)
{
	// 84+4+16 = 104 multiplications
	//         1 division
	mmFloat det, invDet;

	// 2x2 sub-determinants required to calculate 4x4 determinant
	mmFloat det2_01_01 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
	mmFloat det2_01_02 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
	mmFloat det2_01_03 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
	mmFloat det2_01_12 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
	mmFloat det2_01_13 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
	mmFloat det2_01_23 = m[0][2] * m[1][3] - m[0][3] * m[1][2];

	// 3x3 sub-determinants required to calculate 4x4 determinant
	mmFloat det3_201_012 = m[2][0] * det2_01_12 - m[2][1] * det2_01_02 + m[2][2] * det2_01_01;
	mmFloat det3_201_013 = m[2][0] * det2_01_13 - m[2][1] * det2_01_03 + m[2][3] * det2_01_01;
	mmFloat det3_201_023 = m[2][0] * det2_01_23 - m[2][2] * det2_01_03 + m[2][3] * det2_01_02;
	mmFloat det3_201_123 = m[2][1] * det2_01_23 - m[2][2] * det2_01_13 + m[2][3] * det2_01_12;

	det = ( - det3_201_123 * m[3][0] + det3_201_023 * m[3][1] - det3_201_013 * m[3][2] + det3_201_012 * m[3][3] );

	if ( mmAlmostZero(det) ) {
		return mmfalse;
	}

	invDet = 1.0f / det;

	// remaining 2x2 sub-determinants
	mmFloat det2_03_01 = m[0][0] * m[3][1] - m[0][1] * m[3][0];
	mmFloat det2_03_02 = m[0][0] * m[3][2] - m[0][2] * m[3][0];
	mmFloat det2_03_03 = m[0][0] * m[3][3] - m[0][3] * m[3][0];
	mmFloat det2_03_12 = m[0][1] * m[3][2] - m[0][2] * m[3][1];
	mmFloat det2_03_13 = m[0][1] * m[3][3] - m[0][3] * m[3][1];
	mmFloat det2_03_23 = m[0][2] * m[3][3] - m[0][3] * m[3][2];

	mmFloat det2_13_01 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
	mmFloat det2_13_02 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
	mmFloat det2_13_03 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
	mmFloat det2_13_12 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
	mmFloat det2_13_13 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
	mmFloat det2_13_23 = m[1][2] * m[3][3] - m[1][3] * m[3][2];

	// remaining 3x3 sub-determinants
	mmFloat det3_203_012 = m[2][0] * det2_03_12 - m[2][1] * det2_03_02 + m[2][2] * det2_03_01;
	mmFloat det3_203_013 = m[2][0] * det2_03_13 - m[2][1] * det2_03_03 + m[2][3] * det2_03_01;
	mmFloat det3_203_023 = m[2][0] * det2_03_23 - m[2][2] * det2_03_03 + m[2][3] * det2_03_02;
	mmFloat det3_203_123 = m[2][1] * det2_03_23 - m[2][2] * det2_03_13 + m[2][3] * det2_03_12;

	mmFloat det3_213_012 = m[2][0] * det2_13_12 - m[2][1] * det2_13_02 + m[2][2] * det2_13_01;
	mmFloat det3_213_013 = m[2][0] * det2_13_13 - m[2][1] * det2_13_03 + m[2][3] * det2_13_01;
	mmFloat det3_213_023 = m[2][0] * det2_13_23 - m[2][2] * det2_13_03 + m[2][3] * det2_13_02;
	mmFloat det3_213_123 = m[2][1] * det2_13_23 - m[2][2] * det2_13_13 + m[2][3] * det2_13_12;

	mmFloat det3_301_012 = m[3][0] * det2_01_12 - m[3][1] * det2_01_02 + m[3][2] * det2_01_01;
	mmFloat det3_301_013 = m[3][0] * det2_01_13 - m[3][1] * det2_01_03 + m[3][3] * det2_01_01;
	mmFloat det3_301_023 = m[3][0] * det2_01_23 - m[3][2] * det2_01_03 + m[3][3] * det2_01_02;
	mmFloat det3_301_123 = m[3][1] * det2_01_23 - m[3][2] * det2_01_13 + m[3][3] * det2_01_12;

	mxOut.m[0][0] = - det3_213_123 * invDet;
	mxOut.m[1][0] = + det3_213_023 * invDet;
	mxOut.m[2][0] = - det3_213_013 * invDet;
	mxOut.m[3][0] = + det3_213_012 * invDet;

	mxOut.m[0][1] = + det3_203_123 * invDet;
	mxOut.m[1][1] = - det3_203_023 * invDet;
	mxOut.m[2][1] = + det3_203_013 * invDet;
	mxOut.m[3][1] = - det3_203_012 * invDet;

	mxOut.m[0][2] = + det3_301_123 * invDet;
	mxOut.m[1][2] = - det3_301_023 * invDet;
	mxOut.m[2][2] = + det3_301_013 * invDet;
	mxOut.m[3][2] = - det3_301_012 * invDet;

	mxOut.m[0][3] = - det3_201_123 * invDet;
	mxOut.m[1][3] = + det3_201_023 * invDet;
	mxOut.m[2][3] = - det3_201_013 * invDet;
	mxOut.m[3][3] = + det3_201_012 * invDet;
	return mmtrue;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Transpose()
{
	clsMatrix mxtr;
	mxtr._11 = _11; mxtr._12 = _21; mxtr._13 = _31; mxtr._14 = _41;
	mxtr._21 = _12; mxtr._22 = _22; mxtr._23 = _32; mxtr._24 = _42;
	mxtr._31 = _13; mxtr._32 = _23; mxtr._33 = _33; mxtr._34 = _43;
	mxtr._41 = _14; mxtr._42 = _24; mxtr._43 = _34; mxtr._44 = _44;
	*this = mxtr;	
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Translation(const mmFloat x,const mmFloat y,const mmFloat z)
{
	this->Identity();
	_41 = x;
	_42 = y;
	_43 = z;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Translation(const clsVector3 & v)
{
	this->Identity();
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Scale(const mmFloat x,const mmFloat y,const mmFloat z)
{
	this->Identity();
	_11 = x;
	_22 = y;
	_33 = z;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Scale(const clsVector3 & v)
{
	this->Identity();
	_11 = v.x;
	_22 = v.y;
	_33 = v.z;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::RotationX(const mmFloat a)
{
	this->Identity();
	mmFloat s = sinf(a);
	mmFloat c = cosf(a);
	_22 = _33 = c;
	_32 = -s;
	_23 = s;
	return *this;
};
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::RotationY(const mmFloat a)
{
	this->Identity();
	mmFloat s = sinf(a);
	mmFloat c = cosf(a);
	_11 = _33 = c;
	_13 = -s;
	_31 = s;
	return *this;
};
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::RotationZ(const mmFloat a)
{
	this->Identity();
	mmFloat s = sinf(a);
	mmFloat c = cosf(a);
	_11 = _22 = c;
	_21 = -s;
	_12 = s;
	return *this;
};
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::RotationVectorX(const clsVector3& v)
{
	
	this->Identity();
	mmFloat s = v.Dot(clsVector3(0.0f, 0.0f, 0.0f));
	mmFloat c = v.Dot(clsVector3(0.0f, 0.0f, 0.0f));
	_22 = _33 = c;
	_32 = -s;
	_23 = s;
	return *this;
};
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::RotationVectorY(const clsVector3& v)
{
	this->Identity();
	mmFloat c = v.Dot(clsVector3(0.0f, 0.0f, 0.0f));
	mmFloat s = v.Dot(clsVector3(0.0f, 0.0f, 0.0f));
	_11 = _33 = c;
	_13 = -s;
	_31 = s;
	return *this;
};
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::RotationVectorZ(const clsVector3& v)
{
	this->Identity();
	mmFloat c = v.Dot(clsVector3(1.0f, 0.0f, 0.0f));
	mmFloat s = v.Dot(clsVector3(0.0f, 1.0f, 0.0f));
	_11 = _22 = c;
	_21 = -s;
	_12 = s;
	return *this;
};
//---------------------------------------------------------------------------
clsMatrix &	clsMatrix::RotationAxis(const clsVector3 & v, const mmFloat a)
{
	mmFloat cosa = cos(a);
	mmFloat sina = sin(a);
	clsVector3 _v = v;
	_v.Normalize();
	_11 = cosa + (1 - cosa) * _v.x * _v.x;
	_12 = (1 - cosa) * _v.x * _v.y + sina * _v.z;
	_13 = (1 - cosa) * _v.x * _v.z - sina * _v.y;
	_14 = 0;
	_21 = (1 - cosa) * _v.x * _v.y - _v.z * sina;
	_22 = cosa + (1 - cosa) * _v.y * _v.y;
	_23 = (1 - cosa) * _v.y * _v.z + _v.x * sina;
	_24 = 0;
	_31 = (1 - cosa) * _v.x * _v.z + sina * _v.y;
	_32 = (1 - cosa) * _v.y * _v.z - sina * _v.x;
	_33 = cosa + (1 - cosa) * _v.z * _v.z;
	_34 = 0;
	_41 = 0;
	_42 = 0;
	_43 = 0;
	_44 = 1;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::LookAtLH(const clsVector3 & Eye,const clsVector3 & At,const clsVector3 & Up)
{
	clsVector3 zaxis,xaxis,yaxis;
	zaxis = (At - Eye).Normalize();
	xaxis = (Up * zaxis).Normalize();
	yaxis = zaxis * xaxis;

	_11 = xaxis.x;	_12 = yaxis.x;	_13 = zaxis.x;	_14 = 0;
	_21 = xaxis.y;	_22 = yaxis.y;	_23 = zaxis.y;	_24 = 0;
	_31 = xaxis.z;	_32 = yaxis.z;	_33 = zaxis.z;	_34 = 0;
	_41 = -(xaxis.Dot(Eye));	_42 =   -(yaxis.Dot(Eye));	_43 = -(zaxis.Dot(Eye));	_44 = 1;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::LookAtRH(const clsVector3 & Eye,const clsVector3 & At,const clsVector3 & Up)
{
	clsVector3 zaxis,xaxis,yaxis;
	zaxis = (Eye - At).Normalize();
	xaxis = (Up * zaxis).Normalize();
	yaxis = zaxis * xaxis;

	_11 = xaxis.x;	_12 = yaxis.x;	_13 = zaxis.x;	_14 = 0;
	_21 = xaxis.y;	_22 = yaxis.y;	_23 = zaxis.y;	_24 = 0;
	_31 = xaxis.z;	_32 = yaxis.z;	_33 = zaxis.z;	_34 = 0;
	_41 = -(xaxis.Dot(Eye));	_42 =   -(yaxis.Dot(Eye));	_43 = -(zaxis.Dot(Eye));	_44 = 1;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::OrthoLH(const mmFloat w,const mmFloat h,const mmFloat zn,const mmFloat zf)
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = 2.0f/w;
	_22 = 2.0f/-h;
	_33 = 1.0f/(zf-zn);
	_43 = zn/(zn-zf);
	_44 = 1.0f;

	_41 = -1;
	_42 = 1;
	return *this;	
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::OrthoRH(const mmFloat w,const mmFloat h,const mmFloat zn,const mmFloat zf)
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = 2.0f/w;
	_22 = 2.0f/-h;
	_33 = 1.0f/(zn-zf);
	_43 = zn/(zn-zf);
	_44 = 1.0f;

	_41 = -1;
	_42 = 1;
	return *this;	
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::OrthoOffCenterLH(const mmFloat l,const mmFloat r,const mmFloat b,const mmFloat t,const mmFloat zn,const mmFloat zf)
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = 2.0f/(r-l);
	_22 = 2.0f/(t-b);
	_33 = 1.0f/(zf-zn);
	_41 = (l+r)/(l-r);
	_42 = (t+b)/(b-t);
	_43 = zn/(zn-zf);
	_44 = 1.0f;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::OrthoOffCenterRH(const mmFloat l,const mmFloat r,const mmFloat b,const mmFloat t,const mmFloat zn,const mmFloat zf)
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = 2.0f/(r-l);
	_22 = 2.0f/(t-b);
	_33 = 1.0f/(zn-zf);
	_41 = (l+r)/(l-r);
	_42 = (t+b)/(b-t);
	_43 = zn/(zn-zf);
	_44 = 1.0f;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::PerspectiveFovLH(const mmFloat fovy,const mmFloat Aspect,const mmFloat zn,const mmFloat zf)
{
	mmFloat h = 1.0f / tanf(fovy * 0.5f);
	mmFloat w = h / Aspect;
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = w;
	_22 = h;
	_33 = zf/(zf-zn);
	_34 = 1.0f;
	_43 = -zn*zf/(zf-zn);
	_44 = 0.0f;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::PerspectiveFovRH(const mmFloat fovy,const mmFloat Aspect,const mmFloat zn,const mmFloat zf)
{
	mmFloat h = 1.0f / tanf(fovy * 0.5f);
	mmFloat w = h / Aspect;
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = w;
	_22 = h;
	_33 = zf/(zn-zf);
	_34 = -1.0f;
	_43 = zn*zf/(zn-zf);
	_44 = 0.0f;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::PerspectiveLH(const mmFloat w,const mmFloat h,const mmFloat zn,const mmFloat zf)
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = 2*zn/w;
	_22 = 2*zn/h;
	_33 = zf/(zf-zn);
	_34 = 1.0f;
	_43 = zn*zf/(zn-zf);
	_44 = 0.0f;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::PerspectiveRH(const mmFloat w,const mmFloat h,const mmFloat zn,const mmFloat zf)
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = 2*zn/w;
	_22 = 2*zn/h;
	_33 = zf/(zn-zf);
	_34 = -1.0f;
	_43 = zn*zf/(zn-zf);
	_44 = 0.0f;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::PerspectiveOffCenterLH(const mmFloat l,const mmFloat r,const mmFloat b,const mmFloat t,const mmFloat zn,const mmFloat zf)
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = 2.0f*zn/(r-l);
	_22 = 2.0f*zn/(t-b);
	_31 = (l+r)/(l-r);
	_32 = (t+b)/(b-t);
	_33 = zf/(zf-zn);
	_34 = 1.0f;
	_44 = zn*zf/(zn-zf);
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::PerspectiveOffCenterRH(const mmFloat l,const mmFloat r,const mmFloat b,const mmFloat t,const mmFloat zn,const mmFloat zf)
{
	memset(this->m,0,sizeof(mmFloat)*16);
	_11 = 2.0f*zn/(r-l);
	_22 = 2.0f*zn/(t-b);
	_31 = (l+r)/(r-l);
	_32 = (t+b)/(t-b);
	_33 = zf/(zn-zf);
	_34 = -1.0f;
	_44 = zn*zf/(zn-zf);
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Reflect(clsPlane P)
{
	P.Normalize();
	_11=-2 * P.a * P.a + 1;  _12=-2 * P.b * P.a ;     _13=-2 * P.c * P.a;        _14=0;
	_21=-2 * P.a * P.b;      _22=-2 * P.b * P.b + 1;  _23=-2 * P.c * P.b;        _24=0;
	_31=-2 * P.a * P.c;      _32=-2 * P.b * P.c;      _33=-2 * P.c * P.c + 1;    _34=0;
	_41=-2 * P.a * P.d;      _42=-2 * P.b * P.d;      _43=-2 * P.c * P.d;        _44=1;
	return *this;
}
//---------------------------------------------------------------------------
clsMatrix & clsMatrix::Shadow(clsVector3 & Light, clsPlane P, mmBool DirectionalLight)
{
	mmFloat d,w;
	P.Normalize();
	if(DirectionalLight)
	{
		w = 0.0f;
		d = P.DotNormal(Light);
	}
	else
	{
		w = 1.0f;
		d = P.Dot(Light);
	}
	_11=P.a * Light.x + d;  _12=P.a * Light.y ;     _13=P.a * Light.z;      _14=P.a * w;  
	_21=P.b * Light.x;      _22=P.b * Light.y + d;  _23=P.b * Light.z;      _24=P.b * w;  
	_31=P.c * Light.x;      _32=P.c * Light.y;      _33=P.c * Light.z + d;  _34=P.c * w;  
	_41=P.d * Light.x;      _42=P.d * Light.y;      _43=P.d * Light.z;      _44=P.d * w + d;
	return *this;
}

clsVector3 clsMatrix::GetTranslation()
{
	return clsVector3(_41, _42, _43);
}

clsMatrix &	clsMatrix::SetTranslation(clsVector3 & v)
{
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
	_44 = 1;
	return *this;
}

clsMatrix &	clsMatrix::operator=(const clsQuat & quat)
{
	mmFloat wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

  x2 = quat.x + quat.x;
  y2 = quat.y + quat.y;
  z2 = quat.z + quat.z;

  xx = quat.x * x2;   xy = quat.x * y2;   xz = quat.x * z2;
  yy = quat.y * y2;   yz = quat.y * z2;   zz = quat.z * z2;
  wx = quat.w * x2;   wy = quat.w * y2;   wz = quat.w * z2;

  _11 = 1.0f-(yy+zz); _21 = xy-wz;        _31 = xz+wy;
  _12 = xy+wz;        _22 = 1.0f-(xx+zz); _32 = yz-wx;
  _13 = xz-wy;        _23 = yz+wx;        _33 = 1.0f-(xx+yy);

  _14 = _24 = _34 = 0;
  _41 = _42 = _43 = 0;
  _44 = 1;
  
  return *this;
}


#pragma endregion

#pragma region RECT


//***************************************************************************
clsRect::clsRect()
{

}
//---------------------------------------------------------
clsRect::clsRect(mmFloat Left,mmFloat Top,mmFloat Right,mmFloat Bottom)
{
	left=Left;
	top = Top;
	right = Right;
	bottom = Bottom;
}
//---------------------------------------------------------
mmFloat clsRect::Width()
{
	return right - left;
}
//---------------------------------------------------------
mmFloat clsRect::Height()
{
	return bottom - top;
}
//---------------------------------------------------------
mmBool clsRect::operator==(const clsRect & r) const
{
	return (left == r.left)&&(right == r.right)&&(top == r.top)&&(bottom == r.bottom);
}//---------------------------------------------------------
mmBool clsRect::operator!=(const clsRect & r) const
{
	return !((left == r.left)&&(right == r.right)&&(top == r.top)&&(bottom == r.bottom));
}
//***************************************************************************
clsColorRGB::clsColorRGB()
{
	r = g = b = 0;
}
//---------------------------------------------------------
clsColorRGB::clsColorRGB(mmByte _r, mmByte _g, mmByte _b)
{
	r = _r; g = _g; b = _b;
}
//***************************************************************************
clsColorRGBA::clsColorRGBA()
{
	r = g = b = 0;
	a = 255;
}
//---------------------------------------------------------
clsColorRGBA::clsColorRGBA(mmByte _r, mmByte _g, mmByte _b, mmByte _a)
{
	r = _r; g = _g; b = _b; a = _a;
}
//---------------------------------------------------------
clsColorRGBA::clsColorRGBA(mmFloat _r, mmFloat _g, mmFloat _b, mmFloat _a)
{
	fr = _r; fg = _g; fb = _b; fa = _a;
}
//---------------------------------------------------------
clsColorRGBA::clsColorRGBA(mmInt i)
{
	fr = fg = fb = fa = (mmFloat)(((mmFloat)(i)) * (mmFloat)(1.0/ 255.0) );
}
//---------------------------------------------------------
clsColorRGBA::operator D3DCOLORVALUE() const
{
	D3DCOLORVALUE val;
	val.r = fr;
	val.g = fg;
	val.b = fb;
	val.a = fa;
	return val;
}
//---------------------------------------------------------
clsPoint2D::clsPoint2D()
{
	x = y = 0;
}
//---------------------------------------------------------
clsPoint2D::clsPoint2D(mmInt _x, mmInt _y)
{
	x = _x;
	y = _y;
}
//---------------------------------------------------------
clsRectInt::clsRectInt()
{
	left = right = top = bottom = 0;
}
//---------------------------------------------------------
clsRectInt::clsRectInt(mmInt _left, mmInt _right, mmInt _top, mmInt _bottom)
{
	left   = _left;
	right  = _right;
	top    = _top;
	bottom = _bottom;
}
//---------------------------------------------------------
mmBool clsRectInt::IsInside(mmInt x, mmInt y)
{
	return mmfalse;//return (((x > 
}
//====================================================================================
// Ray class implementation
//====================================================================================
clsRay::clsRay():m_vOrigin(0.0f, 0.0f, 0.0f),
				 m_vDirection(0.0f, 0.0f, 0.0f)
{
}
//====================================================================================
clsRay::clsRay(const clsVector3& origin, const clsVector3& direction):
			     m_vOrigin(origin), m_vDirection(direction)
{
}
//====================================================================================
mmBool clsRay::IntersectTriangle(const clsVector3 &va,
								 const clsVector3 &vb,
								 const clsVector3 &vc,
								 mmFloat& fDistance,
								 clsVector3 &vPoint) const
{
	clsVector3 vEdge1 = vb - va;
	clsVector3 vEdge2 = vc - va;
	clsVector3 vCross = m_vDirection * vEdge2;
	mmFloat fDet = vEdge1.Dot(vCross);
	clsVector3 tvec;
	if (fDet > 0.0f)
	{
		tvec = m_vOrigin - va;
	}
	else
	{
		tvec = va - m_vOrigin;
		fDet = -fDet;
	}
	if (fDet < 0.00001f)
		return mmfalse;
	
	mmFloat fBaryCentricU = tvec.Dot(vCross);
	if (fBaryCentricU < 0.0f || fBaryCentricU > fDet)
		return mmfalse;
	
	clsVector3 qvec = tvec * vEdge1;
	mmFloat fBaryCentricV = m_vDirection.Dot(qvec);
	if (fBaryCentricV < 0.0f || fBaryCentricU + fBaryCentricV > fDet)
		return mmfalse;
		
	fDistance = vEdge2.Dot(qvec);
	mmFloat fInvDet = 1.0f / fDet;
	fDistance		 *= fInvDet;
	fBaryCentricU    *= fInvDet;
	fBaryCentricV    *= fInvDet;
	vPoint = va + (vb - va) * fBaryCentricU + (vc - va) * fBaryCentricV;
	return mmtrue;
}
//====================================================================================

//---------------------------------------------------------
// Bounding box class implementation
//---------------------------------------------------------
clsBoundBox::clsBoundBox():m_cMax(0.0f, 0.0f, 0.0f),
						   m_cMin(0.0f, 0.0f, 0.0f)
{
}
//---------------------------------------------------------
clsBoundBox::clsBoundBox(const clsVector3 &min, const clsVector3 &max):
			 m_cMin(min), m_cMax(max)
{
}
//====================================================================================
// Bounding sphere class implementation
//====================================================================================
clsBoundSphere::clsBoundSphere():m_cCenter(0.0f, 0.0f, 0.0f),
								 m_fRadius(0.0f),
								 m_vSpeed(0.0f, 0.0f, 0.0f)
{
}
//====================================================================================
clsBoundSphere::clsBoundSphere(const clsVector3 &center, mmFloat radius):
				m_cCenter(center), m_fRadius(radius), m_vSpeed(0.0f, 0.0f, 0.0f)
{
}
//====================================================================================
mmBool clsBoundSphere::RayIntersects(const clsRay& ray,
									 clsVector3& vNear,
									 clsVector3& vFar)
{
/*  clsVector3 ray = e - s;
	clsVector3 org = m_cCenter - s;
	if (ray.Dot(org) < 0.0f)
		return mmfalse;
	mmFloat a = ray.LengthSq();
	mmFloat b = 2.0f * ray.Dot(org);
	mmFloat c = org.LengthSq() - m_fRadius*m_fRadius;
	mmFloat d = (b*b) - (4.0f*a*c);
	return d >= 0.0f;*/
	//clsVector3 ray(e.x - s.x, e.y - s.y, e.z - s.z);
	clsVector3 vCO = m_cCenter - ray.m_vOrigin;
	mmFloat	rsq = m_fRadius * m_fRadius;
	
	if(vCO.LengthSq() <= rsq) return mmtrue;

	clsVector3 vNearest = ray.m_vDirection;
	vNearest.NormalizeFast();
	mmFloat proj = vNearest.Dot(vCO);

	if (proj < 0.0f)
		return mmfalse;
	
	clsVector3 vMinPoint = vNearest * proj;
	vMinPoint = ray.m_vOrigin + vMinPoint;
	clsVector3 diff = m_cCenter - vMinPoint;

	
	mmFloat fA = diff.LengthSq();
	if (fA <= rsq)
	{
		mmFloat fN = (mmFloat)mmSqrt(rsq - fA);
		
		if ((proj - fN)*(proj - fN) > ray.m_vDirection.LengthSq())
			return mmfalse;
		vNear = vMinPoint - (vNearest * fN);
		vFar  = vMinPoint + (vNearest * fN);
		return mmtrue;
	}
	return mmfalse;
}
//====================================================================================
mmBool clsBoundSphere::LineIntersects(const clsVector3 &a, const clsVector3 &b)
{
	return mmfalse;
}
//====================================================================================
mmBool clsBoundSphere::SphereIntersects(const clsBoundSphere& sphere, 
										const clsVector3& vSelfSpeed,
										const clsVector3& vSphereSpeed,
										mmFloat& fFirstTime,
										mmFloat& fSecondTime)
{
/*	// Relative speed and position
	clsVector3 RelSpeed = vSphereSpeed  - vSelfSpeed;
	clsVector3 RelPos   = sphere.m_cCenter - m_cCenter;
	// Minimal distance
	mmFloat minDist = m_fRadius + sphere.m_fRadius;
	mmFloat pp = RelPos.LengthSq() - minDist*minDist;
	// 1) check if spheres already intersected
	if (pp < 0.0f) 
		return mmtrue;
	
/*	mmFloat pv = RelPos.Dot(RelSpeed);
	// 2)Check if the spheres are moving away from each other
	if (pv >= 0.0f)
		return mmfalse;
	
	mmFloat vv = RelSpeed.LengthSq();
	// 3)Check if the spheres can intersect within 1 frame
	if (((pv + vv) <= 0) && ((vv + 2*pv + pp) >= 0.0f)) 
		return mmfalse;
	
	mmFloat tmin = -pv/vv;
	return (pp + pv*tmin > 0.0f);*/
	clsVector3	vRelSpeed = vSphereSpeed - vSelfSpeed;
	clsVector3  vRelPos   = sphere.m_cCenter - m_cCenter;
	mmFloat		fMinDist  = m_fRadius + sphere.m_fRadius;
	mmFloat		a = vRelSpeed.LengthSq();
	mmFloat		b = 2 * vRelSpeed.Dot(vRelPos);
	mmFloat		c = vRelPos.LengthSq() - fMinDist*fMinDist;
	if (vRelPos.LengthSq() <= (fMinDist*fMinDist))
	{
		fFirstTime = 0.0f;
		fSecondTime = 0.0f;
		return mmtrue;
	}
	if (mhmQuadraticFormula(a, b, c, fFirstTime, fSecondTime))
	{
		if (fFirstTime > fSecondTime)
			mhmSwap(fFirstTime, fSecondTime);
		if (fFirstTime > 0.0f)
			return mmtrue;
	}
	return mmfalse;
}
//====================================================================================
mmBool clsBoundSphere::PointInSphere(const clsVector3& pt)
{
	clsVector3 RelPos = pt - m_cCenter;
	return (RelPos.LengthSq() <= m_fRadius * m_fRadius);
}
//====================================================================================
mmVoid clsBoundSphere::SlidingVector(const clsVector3& vSpeed,
									 const clsVector3 &pt)
{
	clsVector3 vDir = pt - m_cCenter;
//	vDir
}
//**************************************************************************************
#pragma endregion
//**************************************************************************************
clsRand::clsRand()
{
	m_seed = 0;
}
//---------------------------------------------------------
clsRand::clsRand(mmUint seed)
{
	m_seed = seed;
}
//---------------------------------------------------------
mmVoid clsRand::init(mmUint seed)
{
	m_seed = seed;
}
//---------------------------------------------------------
mmFloat clsRand::frand(mmFloat range)
{
	m_seed = m_seed * 2650845021L + 7;
	return (mmFloat)((mmDouble)(m_seed)/((65536.0 * 65536.0)-1.0))*range;
}
//---------------------------------------------------------
mmInt clsRand::rand()
{
	return (mmInt)(frand(1.0f)*65536.0f);
}
//---------------------------------------------------------
mmFloat clsRand::lerpfrand(mmFloat k, mmBool cubic)
{
	mmFloat y1,y2,_k = k;
	m_seed = (mmUint)k;
	k -= (mmFloat)m_seed;
	y1 = frand(1.0f);
	m_seed = ((mmUint)_k)+1;
	y2 = frand(1.0f);
	_k = 1.0f - k;
	if(cubic)
		return (k*k*(y2-y1)+y1) * _k + (_k*_k*(y1-y2)+y2) * k;
	return _k*y1 + k*y2;
}
//---------------------------------------------------------
//**************************************************************************************
//**************************************************************************************
mmVoid clsInterpolation::init(mmFloat V1, mmFloat V2, enInterpolation Inter)
{
	t = 0.0f;
	Time_K = 1.0f;
	v = v1;
	v1 = V1;
	v2 = V2;
	Interpolation = Inter;
	Looping = enHold;
}
//---------------------------------------------------------
clsInterpolation::clsInterpolation()
{
	init(0.0f,0.0f,enInterLinear);
}
//---------------------------------------------------------
clsInterpolation::clsInterpolation(mmFloat V)
{
	init(V,V,enInterLinear);
}
//---------------------------------------------------------
clsInterpolation::clsInterpolation(mmFloat V1, mmFloat V2)
{
	init(V1,V2,enInterLinear);
}
//---------------------------------------------------------
clsInterpolation::clsInterpolation(mmFloat V1, mmFloat V2, enInterpolation Inter)
{
	init(V1,V2,Inter);
}
//---------------------------------------------------------
mmVoid clsInterpolation::SetT(mmFloat T)
{
	t = T;

	if( t > 1.0f )
	{
		if(Looping == enHold)
		{
			t = 1.0f;
		}
		else
		if(Looping == enLoop)
		{
			t -= ((mmFloat)((mmInt)t));
		}
		else
		if(Looping == enPingPong)
		{
			t = 1.0f - (t - ((mmFloat)((mmInt)t)));
			Time_K = -Time_K;
		}
	}else
	if( t < 0.0f )
	{
		if(Looping == enHold)
		{
			t = 0.0f;
		}
		else
		if(Looping == enLoop)
		{
			t = 1.0f + (t - ((mmFloat)((mmInt)t)));
		}
		else
		if(Looping == enPingPong)
		{
			t = -(t - ((mmFloat)((mmInt)t)));
			Time_K = -Time_K;
		}
	}

	if(Interpolation == enInterLinear)
	{
	 v = v1 + t*(v2-v1);	
	}else
	if(Interpolation == enInterSin)
	{
		v = v1 +   (    1.0f - ((cosf(t * MHM_PI)+1.0f) * 0.5f)     )   *(v2-v1);	
	}else
	if(Interpolation == enInterUp)
	{
		v = v1 +  (  t*t  ) *(v2-v1);	
	}else
	if(Interpolation == enInterDoun)
	{
		v = v1 +  (  1.0f - ((t - 1.0f)*(t - 1.0f))  ) *(v2-v1);	
	}
}
//---------------------------------------------------------
mmVoid clsInterpolation::SetDeltaTime(mmFloat Tim)
{
		SetT( t + (Tim * Time_K) );
}
//---------------------------------------------------------
//************************************************************************************
clsRandRange::clsRandRange()
{
	Items.AutoTruncate = mmfalse;
	deltaMiSec = 0.0f;
}
//---------------------------------------------------------
mmVoid clsRandRange::Update(const clsTimer * Timer, mmBool reset, mmFloat MSe)
{
	mmInt r;
	//r = rand.rand()%all;
	r = rand()%all;

	mmInt p1 = 0;
	mmInt p2 = 0;

	for(mmUint z = 0; z<Items.Size();z++)
	{
		if(reset)
			Items[z].ok = mmfalse;

		p2 = p1+(mmInt)Items[z].MinMiSec;
		if( (r >= p1)&&(r < p2) )
			Items[z].ok = mmtrue;
		p1 = p2;
	}






	/*
	static mmUint n = 0;
	n++;


	for(mmUint z = 0; z<Items.Size();z++)
	{
		if(reset)
			Items[z].ok = mmfalse;
		if((n%Items[z].Probability) == 0)
		{
		//	Items[z].lastMiSec = 0;
			if( (rand() % 3)!=0)
				Items[z].ok = mmtrue;
		}
	}
	*/


	
	/*
	static mmFloat delta = 0; 
	if(MSe == 0)
	{
		MSe = Timer->GetMiSecs()-delta;

		delta = Timer->GetMiSecs();
	}
	mmInt K,r;
	r = rand.rand();
	for(mmUint z = 0; z<Items.Size();z++)
	{
		if(reset)
			Items[z].ok = mmfalse;
		Items[z].lastMiSec += MSe * 0.00002 ;//* MSe;
		K = (mmInt)  Items[z].MinMiSec - (mmInt) Items[z].lastMiSec;
		K = max(K,1);
	//	if((rand()%K) == 0)
	//	if((rand()%Items[z].Probability) == 0)
		if((r%K) == 0)
		{
			Items[z].lastMiSec = 0;
			Items[z].ok = mmtrue;
		}

	}
*/

	/*


	//mmFloat MSe = Timer->GetMiSecs()
	for(mmUint z = 0; z<Items.Size();z++)
	{
		if(reset)
			Items[z].ok = mmfalse;
		//Items[z].deltaMiSec += Timer->GetDeltaMiSecs();
		//Items[z].deltaMiSec = Timer->GetMiSecs() ;
		//if(Items[z].deltaMiSec < Items[z].MinMiSec)
		//	continue;	

		if(Items[z].lastMiSec == 0)
			Items[z].lastMiSec = MSe;
		if((MSe - Items[z].lastMiSec) < Items[z].MinMiSec)
			continue;
		Items[z].lastMiSec = MSe;

		//Items[z].deltaMiSec = 0.0f;
	//	if((rand.rand()%Items[z].Probability) == 0)
		if((rand()%Items[z].Probability) == 0)
			Items[z].ok = mmtrue;
	}
	*/
}
//---------------------------------------------------------
mmVoid clsRandRange::SetCount(mmUint count)
{
	Items.Size(count);
}
//---------------------------------------------------------
mmVoid clsRandRange::SetItem(mmUint id, mmUint probability, mmFloat min_MiSec)
{
	Items[id].Probability = probability;
	Items[id].MinMiSec = min_MiSec;
	Items[id].lastMiSec = 0.0f;
	Items[id].ok = mmfalse;

	all = 0;
	for(mmUint z = 0; z<Items.Size();z++)
	{
		all += (mmInt)Items[z].MinMiSec;
	}
	all += all/2;
}
//---------------------------------------------------------
mmVoid clsRandRange::Reset()
{
	for(mmUint z = 0; z<Items.Size();z++)
	{
	//Items[z].deltaMiSec = 0.0f;
	Items[z].ok = mmfalse;
	}
}
//---------------------------------------------------------
mmInt clsRandRange::GetMin(mmBool tim)
{
	if(Items.Size() == 0)
		return -1;

	mmUint i = 0;
	for(mmUint z = 0; z<Items.Size();z++)
	{
		if(tim)
		{
			if(((Items[i].MinMiSec > Items[z].MinMiSec)&&(Items[z].ok)) || ( (!Items[i].ok)&&(Items[z].ok) ) )
				i = z;
		}else{
			if(((Items[i].Probability > Items[z].Probability)&&(Items[z].ok)) || ( (!Items[i].ok)&&(Items[z].ok) ) )
				i = z;
		}
	}
	if(!Items[i].ok)
		return -1;
	return i;
}
//---------------------------------------------------------
mmInt clsRandRange::GetMax(mmBool tim)
{
	if(Items.Size() == 0)
		return -1;

	mmUint i = 0;
	for(mmUint z = 0; z<Items.Size();z++)
	{
		if(tim)
		{
			if(((Items[i].MinMiSec < Items[z].MinMiSec)&&(Items[z].ok)) || ( (!Items[i].ok)&&(Items[z].ok) ) )
				i = z;
		}else{
			if(((Items[i].Probability < Items[z].Probability)&&(Items[z].ok)) || ( (!Items[i].ok)&&(Items[z].ok) ) )
				i = z;
		}
	}
	if(!Items[i].ok)
		return -1;
	return i;
}
//************************************************************************************

