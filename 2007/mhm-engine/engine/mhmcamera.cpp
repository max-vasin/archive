#include "mhmbase.h"
#include "mhmmath.h"
#include "mhmactor.h"
#include "mhmcamera.h"

//***************************************************************************
//***************************************************************************

clsCamera::clsCamera(clsString & name)
{
	Construct();
}
//---------------------------------------------------------
clsCamera::clsCamera()
{
	Construct();
}
//---------------------------------------------------------
clsCamera::~clsCamera()
{
}
//---------------------------------------------------------
mmVoid clsCamera::Construct()
{
	re_view = mmtrue;
	re_proj = mmtrue;
	re_frust = mmtrue;
	re_frust = mmtrue;

	m_fov = MHM_PI/3.0f;
	m_aspect = 4.0f/3.0f;
	m_near = 1.0f;
	m_far = 10000.0f;

	m_width = 40.0f;
	m_height = 30.0f;



	m_eye = 0;

	m_at.x=m_at.y = 0;
	m_at.z = -1;
	m_up.x=m_up.z = 0;
	m_up.y = 1;

	m_rh = true;
	m_perspective = true;

	BuildMatrixs();
}
//---------------------------------------------------------
mmVoid clsCamera::BuildMatrixs()
{
	mmBool re_full = mmfalse;
	
	if(re_view)
	{
		BuildViewMatrix();
		re_full = mmtrue;
	}
	if(re_proj)
	{
		BuildProjectMatrix();
		re_full = mmtrue;
	}
	if (re_full)
		m_full = m_view * m_proj;
}
//---------------------------------------------------------
mmVoid clsCamera::BuildViewMatrix()
{
	if(!re_view)
		return;
	if(m_rh)
		m_view.LookAtRH(Eye(),At(),m_up);
	else
		m_view.LookAtLH(Eye(),At(),m_up);
	re_view = mmfalse;
	re_frust = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::BuildProjectMatrix()
{
	if(!re_proj)
		return;
	if(m_perspective)
		if(m_rh)
			m_proj.PerspectiveFovRH(m_fov, m_aspect, m_near, m_far);
		else
			m_proj.PerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
	else
		if(m_rh)
			m_proj.OrthoRH(m_width, m_height, m_near, m_far);
		else
			m_proj.OrthoLH(m_width, m_height, m_near, m_far);
	re_proj = mmfalse;
	re_frust = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::BuildFrust()
{
	if(!re_frust)
		return;
	BuildViewMatrix();
	BuildProjectMatrix();

	clsMatrix m;
	m = m_view * m_proj;

	fLeft.FromMatrixLeft(m);
	fRight.FromMatrixRight(m);
	fTop.FromMatrixTop(m);
	fBottom.FromMatrixBottom(m);
	fFar.FromMatrixFar(m);
	fNear.FromMatrixNear(m);

	re_frust = mmfalse;
}
//---------------------------------------------------------
mmBool clsCamera::PointIn(clsVector3 & p)
{
	BuildFrust();

	if(fLeft.Dot(p) < 0.0f)
		return mmfalse;
	if(fRight.Dot(p) < 0.0f)
		return mmfalse;
	if(fTop.Dot(p) < 0.0f)
		return mmfalse;
	if(fBottom.Dot(p) < 0.0f)
		return mmfalse;
	if(fFar.Dot(p) < 0.0f)
		return mmfalse;
	if(fNear.Dot(p) < 0.0f)
		return mmfalse;
	return mmtrue;
}
//---------------------------------------------------------
mmBool clsCamera::SphereIn(clsVector3 & p, mmFloat r)
{
	BuildFrust();
	r = -r;
	if(fLeft.Dot(p) < r)
		return mmfalse;
	if(fRight.Dot(p) < r)
		return mmfalse;
	if(fTop.Dot(p) < r)
		return mmfalse;
	if(fBottom.Dot(p) < r)
		return mmfalse;
	if(fFar.Dot(p) < r)
		return mmfalse;
	if(fNear.Dot(p) < r)
		return mmfalse;
	return mmtrue;
}
//---------------------------------------------------------
mmBool clsCamera::TriangleIn(clsVector3 p1, clsVector3 p2, clsVector3 p3)
{
	/*
	clsMatrix m;
	mmBool tmp_f = re_frust;
	BuildMatrixs();
	re_frust = tmp_f;

	m = m_view * m_proj;
	p1 *= m;
	p2 *= m;
	p3 *= m;
	*/

	return mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::Transform(clsMatrix & m)
{
	m_eye *= m;
	m_at *= m;
	m_up.TransformNormal(m);
	re_view = mmtrue;
}
//---------------------------------------------------------
clsVector3 clsCamera::Eye()
{
	return m_eye;
}
//---------------------------------------------------------
mmVoid clsCamera::Eye(clsVector3 & v)
{
	clsVector3 n;
	n = m_up*(m_at - m_eye);
	m_eye = v; 
	m_up = n * ( v - m_at );
	m_up.Normalize();
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::Eye(mmFloat x, mmFloat y, mmFloat z)
{
	clsVector3 n;
	n = m_up*(m_at - m_eye);
	m_eye.x = x; m_eye.y = y; m_eye.z = z; 
	m_up = n * ( m_eye - m_at );
	m_up.Normalize();
	re_view = mmtrue;
}
//---------------------------------------------------------
clsVector3 clsCamera::At()
{
		return m_at;
}
//---------------------------------------------------------
mmVoid clsCamera::At(clsVector3 & v)
{
	clsVector3 n;
	n = m_up*(m_at - m_eye);
	m_at = v; 
	m_up = n * ( m_eye - m_at );
	m_up.Normalize();
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::At(mmFloat x, mmFloat y, mmFloat z)
{
	clsVector3 n;
	n = m_up*(m_at - m_eye);
	m_at.x = x; m_at.y = y; m_at.z = z;
	m_up = n * ( m_eye-m_at );
	m_up.Normalize();
	re_view = mmtrue;
}
//---------------------------------------------------------
clsVector3 clsCamera::Up()
{
	return m_up;
}
//---------------------------------------------------------
mmVoid clsCamera::Up(clsVector3 & v)
{
	m_up = v;
	clsVector3 n;
	n = m_up*(m_at - m_eye);
	m_up = n * ( m_eye-m_at ); 
	m_up.Normalize();
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::Up(mmFloat x, mmFloat y, mmFloat z)
{
	m_up.x = x; m_up.y = y; m_up.z = z; 
 	clsVector3 n;
	n = m_up*(m_at - m_eye);
	m_up = n * (m_eye - m_at); 
	m_up.Normalize();
	re_view = mmtrue;
}

//---------------------------------------------------------
mmFloat clsCamera::Fov()
{
	return m_fov;
}
//---------------------------------------------------------
mmVoid clsCamera::Fov(mmFloat fov_f)
{
	m_fov = fov_f;
	re_proj = mmtrue;
}
//---------------------------------------------------------
mmFloat clsCamera::Aspect()
{
	return m_aspect;
}
//---------------------------------------------------------
mmVoid clsCamera::Aspect(mmFloat aspect_f)
{
	m_aspect = aspect_f;
	re_proj = mmtrue;
}
//---------------------------------------------------------
mmFloat clsCamera::Near()
{
	return m_near;
}
//---------------------------------------------------------
mmVoid clsCamera::Near(mmFloat near_f)
{
	m_near = near_f;
	re_proj = mmtrue;
}
//---------------------------------------------------------
mmFloat clsCamera::Far()
{
	return m_far;
}
//---------------------------------------------------------
mmVoid clsCamera::Far(mmFloat far_f)
{
	m_far = far_f;
	re_proj = mmtrue;
}
//---------------------------------------------------------
mmFloat clsCamera::Width()
{
	return m_width;
}
//---------------------------------------------------------
mmVoid clsCamera::Width(mmFloat width_f)
{
	m_width = width_f;
	m_aspect = m_width / m_height;
	re_proj = mmtrue;
}
//---------------------------------------------------------
mmFloat clsCamera::Height()
{
	return m_height;
}
//---------------------------------------------------------
mmVoid clsCamera::Height(mmFloat height_f)
{
	m_height = height_f;
	m_aspect = m_width / m_height;
	re_proj = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::UseRH()
{
	m_rh = mmtrue;
	re_proj = mmtrue;
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::UseLH()
{
	m_rh = mmfalse;
	re_proj = mmtrue;
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::UseOrto()
{
	m_perspective = mmfalse;
	re_proj = mmtrue;
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::UsePerspective()
{
	m_perspective = mmtrue;
	re_proj = mmtrue;
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::SetViewMatrix(clsMatrix & m)
{
	m_eye.x = m._41; m_eye.y = m._42; m_eye.z = m._43;
	m_at = clsVector3(0.0f,0.0f,-m_at.Length()).Transform(m);// * m_at.Length();
	m_up = clsVector3(0.0f,1.0f,0.0f).TransformNormal(m);
	re_view = mmtrue;
}
//---------------------------------------------------------
mmFloat clsCamera::GetLookLength()
{
	return GetLook().Length();
}
//---------------------------------------------------------
mmVoid clsCamera::SetParentMatrix(clsMatrix & m)
{
	clsVector3 v;
	v = (-GetLookNormal())*GetLook().Length();
	v.TransformNormal(m);
	m_at.x = m._41; m_at.y = m._42; m_at.z = m._43;
	m_eye = m_at + v;
	m_up = clsVector3(0,1,0).TransformNormal(m);
	re_view = mmtrue;
}
//---------------------------------------------------------
const clsMatrix * clsCamera::GetView()
{
	BuildViewMatrix();
	return &m_view;
}
//---------------------------------------------------------
const clsMatrix * clsCamera::GetProjection()
{
	BuildProjectMatrix();
	return &m_proj;
}
//---------------------------------------------------------
const clsMatrix* clsCamera::GetFullTransform()
{
	//BuildMatrixs();
	m_full = m_view * m_proj;
	return &m_full;
}
//---------------------------------------------------------
mmVoid clsCamera::SetPosCamera(clsVector3 & v)
{
	Eye(v);
}
//---------------------------------------------------------
mmVoid clsCamera::SetPosCamera(mmFloat x, mmFloat y, mmFloat z)
{
	Eye(x, y, z);
}
//---------------------------------------------------------
mmVoid clsCamera::SetPosTarget(clsVector3 & v)
{
	At(v);
}
//---------------------------------------------------------
mmVoid clsCamera::SetPosTarget(mmFloat x, mmFloat y, mmFloat z)
{
	At(x,y,z);
}
//---------------------------------------------------------
mmVoid clsCamera::AddPosCamera(clsVector3 & v)
{
	AddPosCamera(v.x,v.y,v.z);
}
//---------------------------------------------------------
mmVoid clsCamera::AddPosCamera(mmFloat x, mmFloat y, mmFloat z)
{
	Eye(m_eye.x + x , m_eye.y + y , m_eye.z + z);	
}
//---------------------------------------------------------
mmVoid clsCamera::AddPosTarget(clsVector3 & v)
{
	At(m_at + v);
}
//---------------------------------------------------------
mmVoid clsCamera::AddPosTarget(mmFloat x, mmFloat y, mmFloat z)
{
	At(m_at.x + x, m_at.y + y, m_at.z + z);
}
//---------------------------------------------------------
mmVoid clsCamera::AddPosCameraTarget(clsVector3 & v)
{
	AddPosCameraTarget(v.x, v.y, v.z);
}
//---------------------------------------------------------
mmVoid clsCamera::AddPosCameraTarget(mmFloat x, mmFloat y, mmFloat z)
{
	m_eye.x += x; m_eye.y += y; m_eye.z += z;
	m_at.x += x; m_at.y += y; m_at.z += z;
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::MoovCamera(clsVector3 v, mmFloat k)
{
	v.Normalize() *= k;
	AddPosCamera(v);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovTarget(clsVector3 v, mmFloat k)
{
	v.Normalize() *= k;
	AddPosTarget(v);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovCameraTarget(clsVector3 v, mmFloat k)
{
	v.Normalize() *= k;
	AddPosCameraTarget(v);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovCameraTargetLook(mmFloat k)
{
	clsVector3 v;
	v = m_at - m_eye;
	MoovCameraTarget(v,k);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovTargetLook(mmFloat k)
{
	m_at += ((m_at - m_eye).Normalize() * k);
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::MoovCameraLook(mmFloat k)
{
	m_eye += ((m_at - m_eye).Normalize() * k);
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::MoovCameraTargetUp(mmFloat k)
{
	MoovCameraTarget(m_up,k);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovTargetUp(mmFloat k)
{
	AddPosTarget(m_up*k);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovCameraUp(mmFloat k)
{
	AddPosCamera(m_up*k);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovCameraTargetHorizon(mmFloat k)
{
	MoovCameraTarget( (m_up*(m_at-m_eye)).Normalize() ,k);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovTargetHorizon(mmFloat k)
{
	AddPosTarget((m_up*(m_at-m_eye)).Normalize()*k);
}
//---------------------------------------------------------
mmVoid clsCamera::MoovCameraHorizon(mmFloat k)
{
	AddPosCamera((m_up*(m_at-m_eye)).Normalize()*k);
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraToTargetX(mmFloat a)
{
	clsMatrix m;
	m.RotationX(a);
	Eye((m_eye - m_at)*m + m_at);
	m_up.TransformNormal(m);
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraToTargetY(mmFloat a)
{
	clsMatrix m;
	m.RotationY(a);
	Eye((m_eye - m_at)*m + m_at);
	m_up.TransformNormal(m);
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraToTargetZ(mmFloat a)
{
	clsMatrix m;
	m.RotationZ(a);
	Eye((m_eye - m_at)*m + m_at);
	m_up.TransformNormal(m);
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraToTargetUp(mmFloat a)
{
	clsMatrix m;
	m.RotationAxis(m_up,a);
	m_eye = (m_eye - m_at)*m + m_at;
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraToTarget(clsVector3 v,mmFloat a)
{
	clsMatrix m;
	m.RotationAxis(v,a);
	m_eye = (m_eye - m_at)*m + m_at;
	m_up.TransformNormal(m); 
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraToTargetHorizon(mmFloat a)
{
	clsMatrix m;
	m.RotationAxis(m_up * (m_at - Eye()),a);
	m_eye = (m_eye - m_at)*m + m_at;
	m_up.TransformNormal(m); 
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraLook(mmFloat a)
{
	clsMatrix m;
	m.RotationAxis(m_at - m_eye, a);
	m_up.TransformNormal(m); 
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraX(mmFloat a)
{
	clsMatrix m;
	m.RotationX(a);
	At( (m_at-m_eye)*m+m_eye );	
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraY(mmFloat a)
{
	clsMatrix m;
	m.RotationY(a);
	At( (m_at-m_eye)*m+m_eye );	
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraZ(mmFloat a)
{
	clsMatrix m;
	m.RotationZ(a);
	At( (m_at-m_eye)*m+m_eye );		
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraUp(mmFloat a)
{
	clsVector3 v;
	clsMatrix m;
	m.RotationAxis(m_up,a);
	m_at = ( (m_at-m_eye)*m+m_eye );	
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCameraHorizon(mmFloat a)
{
	clsVector3 v;
	clsMatrix m;
	m.RotationAxis(m_up * (m_at - Eye()),a);
	m_at = ( (m_at-m_eye)*m+m_eye );
	m_up.TransformNormal(m); 
	re_view = mmtrue;
}
//---------------------------------------------------------
mmVoid clsCamera::RotationCamera(clsVector3 v, mmFloat a)
{
	clsVector3 _v;
	clsMatrix m;
	m.RotationAxis(v,a);
	m_at = ( (m_at-m_eye)*m+m_eye );
	m_up.TransformNormal(m); 
	re_view = mmtrue;
}
//---------------------------------------------------------
clsVector3 clsCamera::GetLook()
{
	return ( m_at - m_eye );
}
//---------------------------------------------------------
clsVector3 clsCamera::GetLookNormal()
{
	return ( (m_at - m_eye).Normalize() );
}
//---------------------------------------------------------
clsVector3 clsCamera::GetEye()
{
	return m_eye;
}
//---------------------------------------------------------
mmVoid clsCamera::SetPosCameraLook(mmFloat k)
{
	Eye( (-GetLookNormal())*k );
}
//---------------------------------------------------------
mmVoid clsCamera::SetPosTargetLook(mmFloat k)
{
	At( (GetLookNormal())*k );
}
//---------------------------------------------------------
clsRay clsCamera::UnProject(const clsPoint2D& pt, clsViewport* vp)
{
	clsMatrix mxInv = m_view;
	mxInv.Inverse();
	clsVector4 vIn((mmFloat)pt.x, (mmFloat)pt.y, -1.0f, 0.0f);
	vIn.x = (vIn.x - (mmFloat)vp->m_nX) / (mmFloat)vp->m_nWidth;
	vIn.y = (vIn.y - (mmFloat)vp->m_nY) / (mmFloat)vp->m_nHeight;
	vIn.x = (vIn.x * 2.0f - 1.0f) / m_proj._11;
	vIn.y = -(vIn.y * 2.0f - 1.0f) / m_proj._22;
	vIn.Transform(mxInv);
	clsVector3 origin(mxInv._41, mxInv._42, mxInv._43);
	return clsRay(origin, clsVector3(vIn.x, vIn.y, vIn.z).Normalize());
}
//---------------------------------------------------------
clsVector3 clsCamera::ProjectHomo(const clsVector3 &pt)
{
	clsVector3 vPt(pt);
	vPt.Transform(m_view);
	clsVector4 vProj(vPt.x, vPt.y, vPt.z, 1.0f);
	vProj.Transform(m_proj);
	mmFloat fOneByW = 1.0f / vProj.w;
	return clsVector3(vProj.x * fOneByW, vProj.y * fOneByW, vProj.z * fOneByW);
}
//**************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************