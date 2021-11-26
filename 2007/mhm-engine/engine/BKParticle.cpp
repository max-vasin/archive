//==========================================================================================
// BIOKRIZ mesh particle
//==========================================================================================
#include "mhmbase.h"
#include "BKParticle.h"

// Particle constructor
//==========================================================================================
clsBKParticle::clsBKParticle(clsEntityFactory *pFactory,
							 const clsString &sType,
							 const clsString &sName):
			   clsEntity(pFactory, sType, sName),
			   m_vRotationAxis(0.0f, 0.0f, 0.0f),
			   m_vSpeed(0.0f, 0.0f, 0.0f),
			   m_vGravity(0.0f, 0.0f, 0.0f),
			   m_cReflectPlane(0.0f, 0.0f, 0.0f, 0.0f),
			   m_fBouncing(0.0f),
			   m_vPosition(0.0f, 0.0f, 0.0f),
			   m_fLifeTime(0.0f),
			   m_fCurrentLife(0.0f)
{
	m_pParticles = mmnull;
}

// Particle destructor
//==========================================================================================
clsBKParticle::~clsBKParticle()
{
	mmSafeRelease(m_pParticles);
}

// OnRender
//==========================================================================================
mmRes clsBKParticle::OnRender()
{
	if (Res)
	{
		clsBaseMaterial* pMtl = (clsBaseMaterial*)((clsBaseMesh*)Res)->GetMaterial(0);
		pMtl->m_dxmtl.Diffuse.a = m_fAlpha;
	}
	if (m_pParticles)
	{
		m_pParticles->SetEmitterScale(1.5f);
		m_pParticles->Render();
	}
	Renderer->GetDevice()->SetModelTM(&m_mxFull);
	return mmOk;
}

mmVoid clsBKParticle::OnLoadProps(clsVarList *pvlist)
{
	clsVariable* pVar = pvlist->FindVariable("ParticlesSource");
	if (pVar != mmnull)
	{
//		m_pParticles = (clsBaseParticles*)Renderer->GetResourceManager()->GetResource((mmString)*pVar, enResBaseParticles);
	}
}

mmVoid clsBKParticle::OnInitialize(mmBool bClone)
{
	if (!bClone && Res)
	{
		clsBaseMaterial* pMtl = (clsBaseMaterial*)((clsBaseMesh*)Res)->GetMaterial(0);
		pMtl->SetBlendType(enBlendAlpha);
		Res->SetBlendType(enBlendAlpha);
	}
	m_bEnableSphereTest = mmfalse;
}

// Particle initialization
//==========================================================================================
mmVoid clsBKParticle::OnUpdate(const clsTimer* pTimer)
{
	m_fCurrentLife += pTimer->GetDeltaSecs();
	if (m_fCurrentLife >= m_fLifeTime)
	{
		m_bKillMe = mmtrue;
		return;
	}
	m_fAlpha = 1.0f - (mmFloat)m_fCurrentLife / (mmFloat)m_fLifeTime;
	m_vSpeed += m_vGravity;
	clsVector3 vOffset = m_vSpeed * pTimer->GetDeltaSecs();
	clsVector3 vHit;
	m_vPosition += vOffset;
	if (m_vPosition.z - m_cBoundSphere.m_fRadius < 0.0f)
	{
		m_vPosition.z = m_cBoundSphere.m_fRadius;
		m_vSpeed.z    = -m_vSpeed.z;
		m_vSpeed     *= m_fBouncing;
	}
	clsVector3 vRotOffs = m_vRotationAxis * pTimer->GetDeltaSecs();
	clsMatrix mxRotX, mxRotY, mxRotZ, mxRotFull;
	mxRotX.RotationX(vRotOffs.x);
	mxRotY.RotationY(vRotOffs.y);
	mxRotZ.RotationZ(vRotOffs.z);
	mxRotFull = mxRotX * mxRotY * mxRotZ;
	m_mxWorld *= mxRotFull;
	m_mxWorld.SetTranslation(m_vPosition);
}

// OnClone
//==========================================================================================
mmVoid clsBKParticle::OnClone(clsEntity* pInstance)
{
	m_pInstance = pInstance;
}


// Particle initialztion from emitter
//==========================================================================================
mmVoid clsBKParticle::InitParticle()
{
	m_vPosition = m_mxWorld.GetTranslation();
	m_mxInitial = m_mxWorld;
	m_fAlpha = 1.0f;
	if (((clsBKParticle*)m_pInstance)->m_pParticles)
		m_pParticles = (clsBaseParticles*)Renderer->GetResourceManager()->GetResource(((clsBKParticle*)m_pInstance)->m_pParticles->GetName(), enResBaseParticles);
	
}


