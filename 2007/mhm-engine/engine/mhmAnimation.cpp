//==========================================================================================
// Mayhem engine animation classes
// Coded:	MozG
// Created:	19 july 2006
//==========================================================================================

#include "mhmbase.h"

//==========================================================================================
// Linear controller class
//==========================================================================================
mmVoid clsLinearController::Animate(clsEntity *pentity)
{
	mmFloat f,cf;
	mmUint	frame,frame2;
	mmUint	len;
	clsQuat	Q;
	clsMatrix MR,MS;
	clsVector3 V;
	
	PathTrack*		track = pentity->GetPathTrack();
	clsAnimState*	state = pentity->GetAnimState();
	
	if((len=track->Size()) > 0)
	{
		cf = state->CurrentFrame;
		if(cf<0) cf=0;

		if( cf >= state->EdgeFrame)
		{
			f = (cf - state->EdgeFrame *
				(mmUint)(cf/state->EdgeFrame))
				/ state->DurationFrame;
		}
		else
		{
			f = cf / state->DurationFrame;
		}
		
		frame = (mmUint)(f);
		f -= (mmFloat)(frame);

		if(frame==(len-1))
		{
			frame2 = frame;
		}
		else
		{
			frame2 = frame+1;
		}
		Q = (*track)[frame].q;
		MR = Q.Slerp((*track)[frame2].q, f);
		
		if((*track)[frame].mi) MR.Inverse(); 
		
		V = (*track)[frame].s;
		V.Lerp((*track)[frame2].s, f);
		MS.Scale(V);

		pentity->m_mxWorld = MS * MR;

		V = (*track)[frame].v;
		V.Lerp((*track)[frame2].v, f);
		pentity->m_mxWorld.SetTranslation(V);
	}
}

//==========================================================================================
// Constant rotation controller class
//==========================================================================================
clsConstantRotationController::clsConstantRotationController()
{
	m_SpeedX = 0.0f;
	m_SpeedY = 0.0f;
	m_SpeedZ = 0.0f;
	m_AmpSinX = 0.0f;
}

mmVoid clsConstantRotationController::Animate(clsEntity *pentity)
{
	clsMatrix mxRotation, mxTemp, mxTrans;
	mxRotation.RotationX(m_SpeedX * pentity->GetAnimState()->DeltaFrame);// / 1000.0f);
	mxTemp.RotationY(m_SpeedY * pentity->GetAnimState()->DeltaFrame);// / 1000.0f);
	mxRotation *= mxTemp;
	mxTemp.RotationZ(m_SpeedZ * pentity->GetAnimState()->DeltaFrame);// / 1000.0f);
	mxRotation *= mxTemp;	
	mxTrans.Translation(sinf(pentity->GetAnimState()->CurrentFrame) * m_AmpSinX, 0.0f, 0.0f);
	pentity->m_mxLocal.SetTranslation(clsVector3(0.0f, 0.0f, sinf(pentity->GetAnimState()->CurrentFrame / 100.0f) * m_AmpSinX));
	pentity->m_mxLocal *= mxRotation;
}

mmVoid clsConstantRotationController::SetSpeedX(mmFloat speed)
{
	m_SpeedX = speed;
}

mmVoid clsConstantRotationController::SetSpeedY(mmFloat speed)
{
	m_SpeedY = speed;
}

mmVoid clsConstantRotationController::SetSpeedZ(mmFloat speed)
{
	m_SpeedZ = speed;
}

mmVoid clsConstantRotationController::SetAmpSinX(mmFloat amp)
{
	m_AmpSinX = amp;
}
