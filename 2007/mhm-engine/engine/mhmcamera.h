//==========================================================================================
// Mayhem engine camera class
// Coded:	Pink
// Created:	31 may 2006
//==========================================================================================

#ifndef _MHMCAMERA_INCLUDED
#define _MHMCAMERA_INCLUDED


//****************************
class clsViewport;
class clsCamera :public clsActor
{
public:

	mmBool m_rh;
	mmBool m_perspective;
	mmBool re_view;
	mmBool re_proj;
	mmBool re_frust;

	mmFloat m_fov;
	mmFloat m_aspect;
	mmFloat m_near;
	mmFloat m_far;
	mmFloat m_width;
	mmFloat m_height;

	clsVector3 m_at;
	clsVector3 m_eye;
	clsVector3 m_up;

	clsMatrix	m_view;
	clsMatrix	m_proj;
	clsMatrix	m_full;

	clsPlane fLeft;
	clsPlane fRight;
	clsPlane fTop;
	clsPlane fBottom;
	clsPlane fFar;
	clsPlane fNear;

	mmVoid Construct();

	mmVoid BuildViewMatrix();
	mmVoid BuildProjectMatrix();
	mmVoid BuildMatrixs();
	mmVoid BuildFrust();

public:

	clsCamera(clsString & name);
	clsCamera();
	~clsCamera();

	mmVoid UseRH();
	mmVoid UseLH();
	mmVoid UseOrto();
	mmVoid UsePerspective();

	clsVector3 Eye();
	mmVoid Eye(clsVector3 & v);
	mmVoid Eye(mmFloat x, mmFloat y, mmFloat z);
	clsVector3 At();
	mmVoid At(clsVector3 & v);
	mmVoid At(mmFloat x, mmFloat y, mmFloat z);
	clsVector3 Up();
	mmVoid Up(clsVector3 & v);
	mmVoid Up(mmFloat x, mmFloat y, mmFloat z);
	mmFloat Fov();
	mmVoid Fov(mmFloat fov_f);
	mmFloat Aspect();
	mmVoid Aspect(mmFloat aspect_f);
	mmFloat Near();
	mmVoid Near(mmFloat near_f);
	mmFloat Far();
	mmVoid Far(mmFloat far_f);
	mmFloat Width();
	mmVoid Width(mmFloat width_f);
	mmFloat Height();
	mmVoid Height(mmFloat height_f);

	mmVoid SetViewMatrix(clsMatrix & m);
	mmVoid SetParentMatrix(clsMatrix & m);

	const clsMatrix * GetView();
	const clsMatrix * GetProjection();
	const clsMatrix * GetFullTransform();

	mmVoid SetPosCamera(clsVector3 & v);
	mmVoid SetPosCamera(mmFloat x, mmFloat y, mmFloat z);
	mmVoid SetPosTarget(clsVector3 & v);
	mmVoid SetPosTarget(mmFloat x, mmFloat y, mmFloat z);
	mmVoid SetPosCameraLook(mmFloat k);
	mmVoid SetPosTargetLook(mmFloat k);

	mmVoid AddPosCamera(clsVector3 & v);
	mmVoid AddPosCamera(mmFloat x, mmFloat y, mmFloat z);
	mmVoid AddPosTarget(clsVector3 & v);
	mmVoid AddPosTarget(mmFloat x, mmFloat y, mmFloat z);
	mmVoid AddPosCameraTarget(clsVector3 & v);
	mmVoid AddPosCameraTarget(mmFloat x, mmFloat y, mmFloat z);

	mmVoid MoovCamera(clsVector3 v, mmFloat k);
	mmVoid MoovTarget(clsVector3 v, mmFloat k);
	mmVoid MoovCameraTarget(clsVector3 v, mmFloat k);

	mmVoid MoovCameraTargetLook(mmFloat k);
	mmVoid MoovTargetLook(mmFloat k);
	mmVoid MoovCameraLook(mmFloat k);

	mmVoid MoovCameraTargetUp(mmFloat k);
	mmVoid MoovTargetUp(mmFloat k);
	mmVoid MoovCameraUp(mmFloat k);

	mmVoid MoovCameraTargetHorizon(mmFloat k);
	mmVoid MoovTargetHorizon(mmFloat k);
	mmVoid MoovCameraHorizon(mmFloat k);

	mmVoid RotationCameraToTargetX(mmFloat a);
	mmVoid RotationCameraToTargetY(mmFloat a);
	mmVoid RotationCameraToTargetZ(mmFloat a);
	mmVoid RotationCameraToTarget(clsVector3 v,mmFloat a);
	mmVoid RotationCameraToTargetUp(mmFloat a);
	mmVoid RotationCameraToTargetHorizon(mmFloat a);


	mmVoid RotationCameraX(mmFloat a);
	mmVoid RotationCameraY(mmFloat a);
	mmVoid RotationCameraZ(mmFloat a);
	mmVoid RotationCamera(clsVector3 v, mmFloat a);
	mmVoid RotationCameraUp(mmFloat a);
	mmVoid RotationCameraHorizon(mmFloat a);
	mmVoid RotationCameraLook(mmFloat a);

	mmBool PointIn(clsVector3 & p);
	mmBool SphereIn(clsVector3 & p, mmFloat r);
	mmBool TriangleIn(clsVector3 p1, clsVector3 p2, clsVector3 p3);

	mmVoid Transform(clsMatrix & m);

	clsVector3 GetLook();
	clsVector3 GetLookNormal();
	clsVector3 GetEye();
	mmFloat GetLookLength();
	
	clsRay UnProject(const clsPoint2D& pt, clsViewport* vpt);
	clsVector3 ProjectHomo(const clsVector3& pt);



};
//****************************


#endif // _MHMCAMERA_INCLUDED