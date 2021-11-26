#include "mhmbase.h"

#include "iff_lib\TrackFile.h"

clsEntity::clsEntity(clsEntityFactory* pfact,
					 const clsString& type,
					 const clsString& name)
{
	if(!pfact)
		pfact = g_pEnvironment->m_pMyApp->m_pEFactory;

	Type = enUndefined;

	Owner = mmnull;
//	Res = mmnull;
	Next = mmnull;
	List = mmnull;
	Child = mmnull;
	Renderer = mmnull;
	Res		= mmnull;
	m_pAnimController = mmnull;
	m_mxLocal.Identity();
	m_mxWorld.Identity();
	m_mxFull.Identity();
	m_pEFactory    = pfact;
	m_sType        = type;
	Renderer       = m_pEFactory->GetEngine()->GetRenderer();
	m_pTformHolder = mmnull;
	
	m_bEnableSphereTest = mmtrue;
	Name = name;
	
	// events flags
	m_bNeedBufferedMouseEvent   = mmfalse;
	m_bNeedMouseEvent		    = mmfalse;
	m_bNeedSpeedCalc			= mmfalse;
	m_bNeedUpdateEvent		    = mmfalse;
	m_bNeedBufferedKeyboardEvent = mmfalse;
	m_bDrawBoundSphere			= mmfalse;
	m_bNeedLoad					= mmfalse;
	
	// update events depencies
	m_pNextDepency		  = mmnull;
	m_pDepency			  = mmnull;
	m_pDepencyMaster      = mmnull;
	m_pNextDepencyMaster  = mmnull;
	
	m_bKillMe			  = mmfalse;
	m_bInitialized		  = mmfalse;
	m_bKillProtection	  = mmfalse;
	m_bOutOfView		  = mmfalse;
	m_bNeedCollide		  = mmfalse;
	m_bFinishUpdate		  = mmfalse;
	m_bDisableHiddenHierarchy = mmfalse;
	m_bRayCollide		  = mmtrue;
	m_bBlockRender		  = mmfalse;
	
	// test
	m_bs = mmnull;

	m_bRenderable = mmtrue;
	m_eBoundType  = enBoundNone;
	m_vSpeed.Set(0.0f);
	m_fBoundScale = 1.0f;


	//Animation
	m_AnimKeys = mmnull;
	m_AnimationRageController = mmnull;
	mmZeroVar(m_AnimState);
	
	
}

clsEntity::~clsEntity(void)
{
	SetOwner(mmnull);

while( this->Child != mmnull) // Pink
{
	//mmSafeDelete(this->Child);
	delete this->Child;
}

	//mmSafeDelete(Next); <-- Ôóðåõ ìàæîð áëèà %))
	mmSafeRelease(Res);
	mmSafeRelease(m_AnimKeys);
	mmSafeDelete(m_AnimationRageController);

	m_pAnimController = mmnull;
	
}



clsEntity * clsEntity::FindNext(const mmString name)
{
	if(Name == name) return this;
	if(Next==mmnull) return mmnull;
	return Next->FindNext(name);
}

clsEntity * clsEntity::FindList(const mmString name)
{
	if(Name == name) return this;
	if(List==mmnull) return mmnull;
	return List->FindList(name);
}

clsEntity * clsEntity::FindRoot()
{
	clsEntity* Current = this;
	clsEntity* Root    = this;
	while (Current)
	{
		Root = Current;
		Current = Current->Owner;
	}
	return Root;
}

clsEntity * clsEntity::FindChild(const clsString& Name)
{
	clsEntity* pChildrens = Child;
	while(pChildrens)
	{
		if (pChildrens->Name == Name)
			return pChildrens;
		clsEntity* pFound = pChildrens->FindChild(Name);
		if (pFound != mmnull)
			return pFound;
		pChildrens = pChildrens->Next;
	}
	return mmnull;
}

clsEntity * clsEntity::FindChild(const mmString Name)
{
	clsEntity* pChildrens = Child;
	while(pChildrens)
	{
		if (pChildrens->Name == Name)
			return pChildrens;
		clsEntity* pFound = pChildrens->FindChild(Name);
		if (pFound != mmnull)
			return pFound;
		pChildrens = pChildrens->Next;
	}
	return mmnull;
}

mmVoid clsEntity::SetOwner(clsEntity *O)
{
	// bugfix : (MozG)
	// first remove from old parent list
	if (Owner != mmnull)
	{
		clsEntity* pChilds = Owner->Child;
		if (pChilds == this)
		{
			Owner->Child = Next;		
		}
		else
		{
			while(pChilds)
			{
				if (pChilds->Next == this)
				{
					pChilds->Next = Next;
					break;
				}
				pChilds = pChilds->Next;
			}
		}
		Next = mmnull;//pink
	}
	// end bugfix
	if (O != mmnull)
	{
		Owner = O;
		Next = O->Child;
		O->Child = this;
	}
	else
	{

		Owner = mmnull;	// <---- ß ÌÓÄÎ×ÈÙÙÅÅÅ ÁËÈÀÀÀÀÀÀÀÀÀÀÀÀÀÀÀÀÀÀ
						// ^---- ÀÁÑÀËÞÒÍÀß ÁËÈÀÀÀÀÀÀÀÀÀÀÀÀÀÀÀÀÀÀ
	}
}

mmVoid clsEntity::Animate(clsCamera* pCam)
{
	clsEntity	*Childs = Child;

	if(Owner)
	{
		m_mxFull = m_mxLocal;
		m_mxFull *= m_mxWorld;
		m_mxFull *= Owner->m_mxFull;
	}
	else
	{
		m_mxFull = m_mxLocal;
		m_mxFull *= m_mxWorld;
	}

	
	if ((pCam != mmnull) && m_bDisableHiddenHierarchy)
	{
		clsBoundSphere bs = m_cBoundSphere;
		clsVector3 vRadius(1.0f, 0.0f, 0.0f);
		vRadius.TransformNormal(m_mxFull);
		bs.m_fRadius *= vRadius.Length();
		bs.m_cCenter.Transform(m_mxFull);
		if (!pCam->SphereIn(bs.m_cCenter, bs.m_fRadius))
			return;
	}

	while(Childs)
	{
		Childs->Animate(pCam);
		Childs = Childs->Next;
	}

}

mmVoid clsEntity::AnimationFrame(mmFloat ms, mmFloat delta)
{
	if(m_AnimationRageController != mmnull)
		m_AnimationRageController->CurrentFrame = ms;

	m_AnimState.CurrentFrame = ms;
	m_AnimState.DeltaFrame   = delta;
	AnimationBuild();

	clsEntity	*Childs = Child;
	while(Childs)
	{
		Childs->AnimationFrame(ms, delta);
		Childs = Childs->Next;
	}
}


mmVoid clsEntity::AnimationBuild()
{
	if (m_pAnimController != mmnull)
		m_pAnimController->Animate(this);
}

// Pink \/

mmVoid clsEntity::AnimationRangeBuildChild(clsAnimState * AnimState)
{
			clsEntity	*Childs = Child;
			while(Childs)
			{
				if( (Childs->m_AnimationRageController != mmnull) && (Childs->m_AnimationRageController->IsPlaying )  )
					break;
				Childs->m_AnimState.CurrentFrame = AnimState->CurrentFrame; 
				Childs->m_AnimState.DeltaFrame   = AnimState->DeltaFrame;
				if (Childs->m_pAnimController != mmnull)
					Childs->m_pAnimController->Animate(Childs);
				Childs->AnimationRangeBuildChild(AnimState);
				Childs = Childs->Next;
			}
}
mmVoid clsEntity::AnimationRangeStop(mmBool childs)
{
	if(m_AnimationRageController != mmnull)
		m_AnimationRageController->IsPlaying = mmfalse;
	clsEntity	*Childs = Child;
	if(childs)
		while(Childs)
		{
			Childs->AnimationRangeStop(childs);
			Childs = Childs->Next;
		}
}
mmVoid clsEntity::AnimationRangeBuild(mmFloat delta)
{
	if (m_AnimationRageController != mmnull)
	{
		if( m_AnimationRageController->Update(this, delta) )
		{
			if (m_pAnimController != mmnull)
				m_pAnimController->Animate(this);
	
			AnimationRangeBuildChild(&m_AnimState);
		}
	}

	clsEntity	*Childs = Child;
	while(Childs)
	{
		Childs->AnimationRangeBuild(delta);
		Childs = Childs->Next;
	}
}

mmRes clsEntity::PlayAnimationRange(mmInt start, mmInt end, enAnimLooping Loop, mmInt LoopCount)
{
	if(m_AnimKeys == mmnull)
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys file not exist", enLCRed);
		return mmFail;
	}
	OnAnimationPlay();
	if(m_AnimationRageController == mmnull)
		m_AnimationRageController = new clsAnimationRageController;

	

	
	m_AnimationRageController->start = start;
	m_AnimationRageController->end = end;
	m_AnimationRageController->start_t = m_AnimKeys->GetKey(start).pos;
	m_AnimationRageController->end_t = m_AnimKeys->GetKey(end).pos;
	m_AnimationRageController->CurrentFrame = m_AnimState.CurrentFrame;

	if(!m_AnimationRageController->IsPlaying )
	{
		if( m_AnimState.CurrentFrame < m_AnimationRageController->start_t)
			m_AnimationRageController->dir = 1.0f;
		else
		if( m_AnimState.CurrentFrame > m_AnimationRageController->end_t)
			m_AnimationRageController->dir = -1.0f;
		else
			m_AnimationRageController->dir = 1.0f;
	}
	

	m_AnimationRageController->CurentNLoop = 0;
	m_AnimationRageController->LoopCount = LoopCount;
	m_AnimationRageController->Loop = Loop;
	m_AnimationRageController->IsPlaying = mmtrue;

	return mmOk;
}
mmRes clsEntity::PlayAnimationRange(const mmString name, enAnimLooping Loop , mmInt LoopCount)
{
	mmInt s,e;
	if(m_AnimKeys == mmnull)
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys file not exist", enLCRed);
		return mmFail;
	}
	if( !m_AnimKeys->FindKeys(name, s, e))
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys not exist", enLCRed);
		return mmFail;
	}
	return PlayAnimationRange( s, e, Loop, LoopCount);	
}
mmRes clsEntity::PlayAnimationRange(const mmString start, const mmString end, enAnimLooping Loop , mmInt LoopCount )
{
	if(m_AnimKeys == mmnull)
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys file not exist", enLCRed);
		return mmFail;
	}
	return PlayAnimationRange( m_AnimKeys->FindKey(start), m_AnimKeys->FindKey(end), Loop, LoopCount);
}

mmRes clsEntity::PlayAnimationRangeMiSec(mmFloat start, mmFloat end, enAnimLooping Loop , mmInt LoopCount )
{

	if(m_AnimationRageController == mmnull)
		m_AnimationRageController = new clsAnimationRageController;
	
	
	m_AnimationRageController->start = 0;
	m_AnimationRageController->end = 0;
	m_AnimationRageController->start_t = start;
	m_AnimationRageController->end_t = end;
	m_AnimationRageController->CurrentFrame = m_AnimState.CurrentFrame;

	if(!m_AnimationRageController->IsPlaying )
	{
		if( m_AnimState.CurrentFrame < m_AnimationRageController->start_t)
			m_AnimationRageController->dir = 1.0f;
		else
		if( m_AnimState.CurrentFrame > m_AnimationRageController->end_t)
			m_AnimationRageController->dir = -1.0f;
		else
			m_AnimationRageController->dir = 1.0f;
	}

	m_AnimationRageController->CurentNLoop = 0;
	m_AnimationRageController->LoopCount = LoopCount;
	m_AnimationRageController->Loop = Loop;
	m_AnimationRageController->IsPlaying = mmtrue;

	return mmOk;
}
mmRes clsEntity::PlayAnimationRangeStart(const mmString name, enAnimLooping Loop, mmInt LoopCount)
{
	mmInt s,e;
	if(m_AnimKeys == mmnull)
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys file not exist", enLCRed);
		return mmFail;
	}
	if( !m_AnimKeys->FindKeys(name, s, e))
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys not exist", enLCRed);
		return mmFail;
	}
	SetAnimationPos(s);
	return PlayAnimationRange(name, Loop, LoopCount);	
}
mmRes clsEntity::PlayAnimationRangeStart(const mmString start, const mmString end, enAnimLooping Loop , mmInt LoopCount)
{
	SetAnimationPos(start);
	return PlayAnimationRange( start, end, Loop, LoopCount);	
}
mmRes clsEntity::PlayAnimationRangeStart(mmInt start, mmInt end, enAnimLooping Loop, mmInt LoopCount)
{
	SetAnimationPos(start);
	return PlayAnimationRange(start, end, Loop , LoopCount );	
}
mmRes clsEntity::PlayAnimationRangeMiSecStart(mmFloat start, mmFloat end, enAnimLooping Loop, mmInt LoopCount)
{
	SetAnimationPosMiSec(start);
	return PlayAnimationRangeMiSec( start,  end,  Loop ,  LoopCount );	
}

mmRes clsEntity::PlayAnimationToKey(const mmString key)
{
	if(m_AnimKeys == mmnull)
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys file not exist", enLCRed);
		return mmFail;
	}
	return PlayAnimationRange( m_AnimKeys->FindKey(key), m_AnimKeys->FindKey(key), enAnimHold, 0);
}

mmRes clsEntity::PlayAnimationToKey(mmInt key)
{
	return PlayAnimationRange( key, key, enAnimHold, 0);
}

mmRes clsEntity::PlayAnimationToMiSec(mmFloat time)
{
	return PlayAnimationRangeMiSec(time, time, enAnimHold, 0);
}


mmRes clsEntity::SetAnimationPos(const mmString key)
{
	if(m_AnimKeys == mmnull)
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys file not exist", enLCRed);
		return mmFail;
	}
	return SetAnimationPos(m_AnimKeys->FindKey(key));
}

mmRes clsEntity::SetAnimationPos(mmInt key)
{
	if(m_AnimKeys == mmnull)
	{
		g_plog->Out("clsEntity::PlayAnimation[" + Name+ "] >> AnimKeys file not exist", enLCRed);
		return mmFail;
	}
	if(m_AnimationRageController == mmnull)
		m_AnimationRageController = new clsAnimationRageController;
	m_AnimState.CurrentFrame = m_AnimKeys->GetKey(key).pos;
	m_AnimationRageController->CurrentFrame = m_AnimState.CurrentFrame;
	return mmOk;
}
mmRes clsEntity::SetAnimationPosMiSec(mmFloat time)
{
	if(m_AnimationRageController == mmnull)
		m_AnimationRageController = new clsAnimationRageController;
	m_AnimState.CurrentFrame = time;
	m_AnimationRageController->CurrentFrame = m_AnimState.CurrentFrame;
	return mmOk;
}

mmRes clsEntity::LoadAnimationKeys(const mmString name , mmBool childs)
{
	m_AnimKeys = (clsAnimKeys*)Renderer->GetResourceManager()->GetResource(name, enResAnimKeys);
	if(m_AnimKeys == mmnull)
	{
		g_plog->Out("clsEntity::LoadAnimationKeys[" + clsString(name) + "] >> AnimKeys file not exist", enLCRed);
		return mmFail;
	}
	if(childs)
	{
		clsEntity	*Childs = Child;
		while(Childs)
		{
			if( Childs->LoadAnimationKeys(name, childs) != mmOk)
			{
				g_plog->Out("clsEntity::LoadAnimationKeys[" + clsString(name) + "] >> AnimKeys file not loaded", enLCRed);
				return mmFail;
			}
			Childs = Childs->Next;
		}
	}
	return mmOk;
}
//pink /\

mmVoid clsEntity::AnimationReset()
{
	m_AnimState.Speed = 1;
	AnimationFrame(0.0f, 0.0f);
}

PathTrack* clsEntity::GetPathTrack()
{
	return &Path;
}

clsAnimState* clsEntity::GetAnimState()
{
	return &m_AnimState;
}

mmVoid clsEntity::AssignController(IAnimController* pctrl, mmBool recursive)
{
	m_pAnimController = pctrl;
	if (recursive)
	{
		clsEntity	*Childs = Child;
		while(Childs)
		{
			Childs->AssignController(pctrl, recursive);
			Childs = Childs->Next;
		}
	}
}


mmVoid clsEntity::LoadPath(const mmString filename)
{
	mmVoid *fmem;
	IFILE iff;
	IFileTrack *ift;
	mmUint fir;
	mmUint j,i,keys;	
	mmUint fps,frames;
	mmFloat *f;
	clsPathKey Last,*p;

	clsFile TrackFile(clsString("meshes\\") + filename);
	if(MHMFAIL(TrackFile.Open())) return;
	if(MHMFAIL(TrackFile.ReadToMemory((mmVoid**)&fmem))) return;

	IF_Init();
	iff = IF_OpenMem(fmem);
	if(iff)
		{
			if(IF_Find(iff,ID_HEAD))
			{
				ift = (IFileTrack*)IF_GetFound(iff);
				if((ift->type == 4)||(ift->type == 5)||(ift->type == 6))
				{
					if(IF_Find(iff,ID_TRAC))
					{
						f = (float*)IF_GetFound(iff);
						frames = ift->frames;
						if((ift->type == 5)||(ift->type == 6))
						{
						 fps = ift->fps;
						 keys = ift->keys;
						}
						else
						{
						 fps = ift->keys;
						 keys = frames;
						}
						
						//CreatePoints();
						m_AnimState.DurationFrame = 1000.0f / fps;
						m_AnimState.EdgeFrame = m_AnimState.DurationFrame * frames;
						m_AnimState.CurrentFrame = 0;
						Path.Size(frames);

						Last.t = 0; Last.v.x = 0; Last.v.y = 0; Last.v.z = 0;
						Last.qx = 1;Last.qy = 0;Last.qz = 0;Last.qw = 0;
						Last.s.x = 1;Last.s.y = 1;Last.s.z = 1;Last.mi = 0;
						Last.q.x = 0;Last.q.y = 0;Last.q.z = 0;Last.q.w = 1;

						for(j=0;j<keys;j++)
						{
							if(ift->type == 6)
							{
								i=j;
								fir = *((mmUchar*)(f));
								f = (float*)(((mmUchar*)f)+1);
							}
							else
							{
								fir = *(unsigned int*)(f); f++;
								i = fir>>8;
							}
							
							p = &Path[i];

							p->t = (mmFloat)(i) / (mmFloat)fps;

							if(fir&1)
							{
								p->v.x = *f++;
								p->v.y = *f++;
								p->v.z = *f++;
							}
							else
							{
								p->v = Last.v;
							}

							if(fir&2) // rotate component
							{
								/*
								p->qx = *f++;
								p->qy = *f++;
								p->qz = *f++;
								p->qw = *f++;
								*/
								
								{
								// pre convertion to Quatertnion 
									clsVector4 v;
									
									v.x = *f++;
									v.y = *f++;
									v.z = *f++;
									v.w = *f++;
									if(v.w >= 0 ) {p->mi = 0; v.w = -v.w;}
											else p->mi = 1;
									p->q = v;
									//AQuatAxisAngle(&p->q,&v);

								}
							}
							else
							{
								p->qx = Last.qx;
								p->qy = Last.qy;
								p->qz = Last.qz;
								p->qw = Last.qw;

								p->q.x = Last.q.x;
								p->q.y = Last.q.y;
								p->q.z = Last.q.z;
								p->q.w = Last.q.w;
								p->mi = Last.mi;
							}

							if(fir&4)
							{
								p->s.x = *f++;
								p->s.y = *f++;
								p->s.z = *f++;
							}
							else
							{
								p->s = Last.s;
							}
							Last = *p;
						}
						//m = 2;
						//Fill();
						//GenRotate();
						//AnimationReset();
					}
				}
			}
		IF_Close(iff);
	}
	IF_Release();
	TrackFile.FreeMemory(fmem);
	TrackFile.Close();
}


mmRes clsEntity::LoadMJ(const mmString FileName)
{
	tmpList <clsEntity*> PropList_ent;
	tmpList <clsVarParser> PropList_var;



	clsResource * R;
	clsEntity * Ent2;
	clsEntity * Ent;
	clsString str, sProps;
	clsStringList list;
	enType t;
	mmBool	isPath;
	mmFloat	fBoundScale;

	clsVarParser vp;
	mmInt	nProps,nLightPars;


	List = mmnull;
	clsFile f(FileName);
	if(MHMSUCC(f.Open()))
	{
		while(MHMSUCC(f.Read(str)))
		{
			list.Clear();
			str.GetSubStringsByDelimiters(" \t", list);
			clsString sType;
			if(list.Count() > 1)
			{
				R = mmnull;
				fBoundScale = 1.0f;
				str = list[0].Left(3);
				nProps = list.Count();

				if( (nProps == 3)||(nProps == 10)) // path, no props
				{
					isPath = mmtrue;
					nProps = 0;
				}
				else
				if((nProps == 4)||(nProps == 11)) // path, props
				{
					isPath = mmtrue;
					nProps = 3;
				}
				else
				if((nProps == 14)||(nProps == 21)) // matrix, no props
				{
					isPath = mmfalse;
					nProps = 0;
				}
				else // matrix, props
				{
					isPath = mmfalse;
					nProps = 14;
				}

				if(str==";he")	// HELPER
				{
					str = list[1];
					t=enHelper;
					sType = "Helper";
				}
				else
				if(str==";ca")	// CAMERA
				{
					str = list[1];
					t=enCamera;
					sType = "Camera";
				}
				else
				if(str==";li")	// LIGHT
				{
					str	=	list[1];
					t	=	enLight;
					sType = "Light";
					R = Renderer->GetResourceManager()->GetResource("", enResBaseLight);
					// light's pars
					nLightPars = (nProps?nProps+1:(list.Count()==10?3:14));
					((clsBaseLight*) R)->SetType((enLightType)(list[nLightPars].ToInt()+1));

					mmFloat Range = list[nLightPars+4].ToFloat();
					Range = Range<1?100000:Range;

					((clsBaseLight*) R)->SetDiffuse(clsColorRGBA(list[nLightPars+1].ToFloat(),list[nLightPars+2].ToFloat(),list[nLightPars+3].ToFloat(),1));
					//((clsBaseLight*) R)->SetAmbient(clsColorRGBA(list[nLightPars+1].ToFloat(),list[nLightPars+2].ToFloat(),list[nLightPars+3].ToFloat(),1));
					((clsBaseLight*) R)->SetAmbient(clsColorRGBA(0.0f, 0.0f, 0.0f, 0.0f));
					((clsBaseLight*) R)->SetSpecular(clsColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
					((clsBaseLight*) R)->SetFalloff(1.0f);
					((clsBaseLight*) R)->SetRange(Range);
					//((clsBaseLight*) R)->SetTheta(list[nLightPars+5].ToFloat());
					//((clsBaseLight*) R)->SetPhi(list[nLightPars+6].ToFloat());
					((clsBaseLight*) R)->SetTheta(1.0f);
					((clsBaseLight*) R)->SetPhi(2.2f);
					

					

					((clsBaseLight*) R)->SetAttenuation((enAttenuationIndex)1,1);
					((clsBaseLight*) R)->SetAttenuation((enAttenuationIndex)2,0);
					((clsBaseLight*) R)->SetAttenuation((enAttenuationIndex)3,0);
				}
				else
				if(str==";ps")	// PARTICLES
				{
					str = list[1];
					t=enParticles;
					sType = "Particles";
					R = Renderer->GetResourceManager()->GetResource(str + ".apd", enResBaseParticles);
				}
				else
				if(str==";pa") // BINDING
				{
					if(Ent = FindList(list[2]))
					{
						Ent2 = FindList(list[1]);
						if (Ent2 == mmnull)
							continue;
						Ent2->SetOwner(Ent);
					}
					continue;
				}
				else
				{				// MESH
					if(str[0] == ';') continue; // skip unsupported
					t=enMesh;
					R = Renderer->GetResourceManager()->GetResource(list[1], enResStaticMesh);
					str = list[0];	
					sType = "Mesh";
				}

				// get props !
				if(nProps)
				{
					vp.Clear();
					sProps = list[nProps];
					sProps.ReplaceString("\\n","\n");
					if(MHMSUCC(vp.ParseMem(sProps,sProps.Length())))
					{	
						clsVariable *var = vp.GetVarList()->FindVariable("type");
						if(var && (var->GetType() == enVarString))
						{
							sType = (mmString)*var;
						}
						else
						{
							clsVariable *var = vp.GetVarList()->FindVariable("objclass");
							if(var && (var->GetType() == enVarString))
							{
								sType = (mmString)*var;
							}
						}
						var = vp.GetVarList()->FindVariable("boundscale");
						if(var && (var->GetType() == enVarFloat))
						{
							fBoundScale = (mmFloat)*var;
						}
					}
					else
					{
						nProps=0;
						g_plog->Out("Props for object " + str + " is damaged !", enLCRed);
					}
				}

				Ent = m_pEFactory->CreateEntity(sType, str);
				Ent->Type = t;
			//	Ent->Renderer = Renderer;
				Ent->Name = str;
				Ent->List = List;
				if (R != mmnull)
					Ent->Res = R;
				
				List = Ent;
				Ent->m_fBoundScale = fBoundScale;
				
				// matrix
				Ent->m_mxWorld.Identity();
				Ent->Path.Size(0);
				if(isPath) // path
				{
					Ent->LoadPath(list[2]);
					clsQuat	Q;
					clsMatrix MR,MS;
					clsVector3 V;
					if (Ent->Path.Size() > 0)
					{
						Q = Ent->Path[0].q;
						MR = Q;
						if (Ent->Path[0].mi)
							MR.Inverse(); 
						V = Ent->Path[0].s;
						MS.Scale(V);
						Ent->m_mxWorld = MS * MR;
						V = Ent->Path[0].v;
						Ent->m_mxWorld.SetTranslation(V);
					}
					Ent->AssignController(&g_pEnvironment->m_cLinearController);
				}
				else
				if((list.Count() >= 3) && (list[2].IsFloat()))//matrix
				{
					Ent->m_mxWorld._11 = list[2].ToFloat();
					Ent->m_mxWorld._12 = list[3].ToFloat();
					Ent->m_mxWorld._13 = list[4].ToFloat();
					Ent->m_mxWorld._21 = list[5].ToFloat();
					Ent->m_mxWorld._22 = list[6].ToFloat();
					Ent->m_mxWorld._23 = list[7].ToFloat();
					Ent->m_mxWorld._31 = list[8].ToFloat();
					Ent->m_mxWorld._32 = list[9].ToFloat();
					Ent->m_mxWorld._33 = list[10].ToFloat();
					Ent->m_mxWorld._41 = list[11].ToFloat();
					Ent->m_mxWorld._42 = list[12].ToFloat();
					Ent->m_mxWorld._43 = list[13].ToFloat();
				}
				// Test 
				Ent->m_bs = Renderer->GetResourceManager()->GetResource("Boundsphere.mmh", MHMRES_MESH_ID);		

				// Props
				if(nProps) 
				{
					//Pink

					Ent->OnLoadProps(vp.GetVarList());

					PropList_ent.Add(Ent);
					PropList_var.Add(vp);
				}
			}
		}


		Ent2 = List;
		while(Ent2)
		{
			// collect free nodes
			if(Ent2->Owner == mmnull)
			{	
				Ent2->SetOwner(this); // for scene !
			}

			Ent2 = Ent2->List;
		}

		// BINDING !!!

		clsMatrix	Identity;
		Identity.Identity();

		//AnimationFrame(0);
		Animate();

		Ent2 = List;
		while(Ent2)
		{
			
			// init bones meshes
			if( (Ent2->Type == enMesh)&& (Ent2->Res))
			{
				clsStaticMesh *mesh = (clsStaticMesh *)Ent2->Res;
				mmUint		bones = mesh->getBones();
				mmUint		i;
				mmString	BoneName;
				clsMatrix	MI,MO;

				MO = m_mxFull;
				MO.Inverse();
				//MO.Identity();

				if(bones>0) // bones !!!
				{
					for(i = 0; i<bones ;i++)
					{
						BoneName = mesh->getBoneName(i);
						Ent = FindChild(BoneName);

						mesh->setBoneHandle(i, mmnull);
						mesh->setBoneMatrix(i, &Identity);

						if(Ent)
						{
							MI = Ent->m_mxFull * MO;
							mesh->setBoneInit(i, &MI.Inverse());
						}
						else
						{
							mesh->setBoneInit(i, &Identity);
						}
					}
				}
			}

			Ent2 = Ent2->List;
		}
		Ent2 = List;
		while(Ent2)
		{
			Ent2->m_cBoundSphere.m_fRadius = 1;
			Ent2->m_fBoundScale = 1;

			// get mesh bounds
			if ((Ent2->Res != mmnull) && (mmHLID(Ent2->Res->GetType()) == MHMRES_MESH_ID))
			{
				if (Ent2->Res->GetType() == enResStaticMesh)
					((clsStaticMesh*)Ent2->Res)->Animate(Ent2->Owner);
				((clsBaseMesh*)Ent2->Res)->ComputeBounds(&Ent2->m_cBoundBox,
												 &Ent2->m_cBoundSphere,
												 mmnull);
				Ent2->m_cBoundSphere.m_fRadius *= Ent2->m_fBoundScale;
				Ent2->m_eBoundType = enBoundSphere;
			}	
			Ent2 = Ent2->List;
		}

		Animate(false);
		
		ProcessLoading();
		
		Ent2 = Child;
		while(Ent2)
		{
			Ent2->ProcessInitialize(mmfalse);
			Ent2 = Ent2->Next;
		}


				//Pink
		for(mmUint z = 0; z < PropList_ent.Count(); z++)
		{
			PropList_ent[z]->OnLoadPropsPost( PropList_var[z].GetVarList() );
		}
		
		return f.Close();
	}
	return mmErr;
}

// RayIntersection: 
//==========================================================================================
mmVoid clsEntity::RayBoundsIntersection(const clsRay &ray, 
										clsEntity* pEntity,
										clsVector3* pPosSelect,
										mmFloat fScale)

{
	
	clsVector3 vNear, vFar;
	if (m_bRayCollide)
		if (m_eBoundType == enBoundSphere)
		{
			clsBoundSphere bs = m_cBoundSphere;
			clsVector3 vRadius( fScale, 0.0f, 0.0f);
			vRadius.TransformNormal(m_mxFull);
			bs.m_fRadius *= vRadius.Length();
			bs.m_cCenter.Transform(m_mxFull);
			if (pPosSelect != mmnull)
			{
				bs.m_cCenter.x *= pPosSelect->x;
				bs.m_cCenter.y *= pPosSelect->y;
				bs.m_cCenter.z *= pPosSelect->z;
			}
			if (bs.RayIntersects(ray, vNear, vFar))
				pEntity->OnSphereRayIntersect(this, vNear, vFar);
		}	

	clsEntity* pChilds = Child;
	while(pChilds)
	{
		pChilds->RayBoundsIntersection(ray, pEntity, pPosSelect,  fScale);
		pChilds = pChilds->Next;	
	}
}

// SetTransformHolder: sets one matrix, which will recieve transformation every time
//==========================================================================================
mmVoid clsEntity::SetTransformHolder(clsMatrix *pMx)
{
	m_pTformHolder = pMx;
}

// AddChildNode: adds new child node
//==========================================================================================
clsEntity* clsEntity::AddChildNode(const clsString& type,
								   const clsString& name,
								   mmBool bInitialize,
								   mmBool bPostLoad)
{
	clsEntity* pEntity = m_pEFactory->CreateEntity(type, name);
	pEntity->List = List;
	List = pEntity;
	pEntity->SetOwner(this);
	
	if (bInitialize)
		pEntity->ProcessLoading();
	if (bPostLoad)
		pEntity->ProcessInitialize(mmfalse);
	return pEntity;
}
clsEntity* clsEntity::AddChildNode(clsEntity* pEntity)
{
	if (pEntity == mmnull)
		return mmnull;
	pEntity->SetOwner(this);
	return pEntity;
}

// EnableBoundSphereTest: enables camera bound sphere test
//==========================================================================================
mmVoid clsEntity::EnableBoundSphereTest(mmBool bEnable, mmBool bRecursive)
{
	m_bEnableSphereTest = bEnable;
	
	if (bRecursive)
	{
		clsEntity *Childs = Child;
		while(Childs)
		{
			Childs->EnableBoundSphereTest(bEnable, bRecursive);
			Childs = Childs->Next;
		}
	}
}

// EnableRendering: enables rendering
//==========================================================================================
mmVoid clsEntity::EnableRendering(mmBool bEnable, mmBool bRecursive)
{
	m_bRenderable = bEnable;
	
	if (bRecursive)
	{
		clsEntity *Childs = Child;
		while(Childs)
		{
			Childs->EnableRendering(bEnable, bRecursive);
			Childs = Childs->Next;
		}
	}
}

// EnableRendering: enables rendering
//==========================================================================================
mmVoid clsEntity::OutOfView(mmBool bOut, mmBool bRecursive)
{
	m_bOutOfView = bOut;
	
	if (bRecursive)
	{
		clsEntity *Childs = Child;
		while(Childs)
		{
			Childs->OutOfView(bOut, bRecursive);
			Childs = Childs->Next;
		}
	}
}

// EnableBoundSphereDraw: enables bound sphere drawing
//==========================================================================================
mmVoid clsEntity::EnableBoundSphereDraw(mmBool bEnable, mmBool bRecursive)
{
	m_bDrawBoundSphere = bEnable;
	
	if (bRecursive)
	{
		clsEntity *Childs = Child;
		while(Childs)
		{
			Childs->EnableBoundSphereDraw(bEnable, bRecursive);
			Childs = Childs->Next;
		}
	}
}

// CloneBoundSphere: clones bound sphere via transform
//==========================================================================================
mmVoid clsEntity::CheckCollision(clsEntity *pEntity)
{
	if ((m_eBoundType == enBoundNone) ||
		(pEntity->m_eBoundType == enBoundNone))
			return;
	
	if ((m_eBoundType == enBoundSphere) &&
		(pEntity->m_eBoundType == enBoundSphere))
	{
		clsBoundSphere SphereA = m_cBoundSphere;
		clsBoundSphere SphereB = pEntity->m_cBoundSphere;
		clsVector3 vRadius(1.0f, 0.0f, 0.0f);
		vRadius.TransformNormal(m_mxFull);
		SphereA.m_fRadius *= vRadius.Length();
		SphereA.m_cCenter.Transform(m_mxFull);
		vRadius.Set(0.0f);
		vRadius.x = 1.0f;
		vRadius.TransformNormal(pEntity->m_mxFull);
		SphereB.m_fRadius *= vRadius.Length();
		SphereB.m_cCenter.Transform(pEntity->m_mxFull);
		SphereA.m_vSpeed = m_vSpeed;
		SphereB.m_vSpeed = pEntity->m_vSpeed;
		mmFloat fFirstTime, fSecondTime;
		if (SphereA.SphereIntersects(SphereB, m_vSpeed, pEntity->m_vSpeed, fFirstTime, fSecondTime))
		{
			OnCollide(pEntity, fFirstTime, fSecondTime);
			pEntity->OnCollide(this,  fFirstTime, fSecondTime);
		}
		
	}
}

// GetSuperType: returns entity supertype
//==========================================================================================
const clsString& clsEntity::GetSuperType()
{
	return m_sSuperType;
}


//==========================================================================================
// Update event depencies
//==========================================================================================
mmVoid clsEntity::LinkUpdateDepency(clsEntity* pEntity)
{
	clsEntity* pDepencies = m_pDepency;
	if (pDepencies == mmnull)
	{
		m_pDepency = pEntity;
		return;
	}
	clsEntity* pLastDepency;// = m_pDepency;
	do
	{
		if (pDepencies == pEntity)
			break;
		pLastDepency = pDepencies;
		pDepencies = pDepencies->m_pNextDepency; 
	}while(pDepencies);
	if (pDepencies == mmnull)
		pLastDepency->m_pNextDepency = pEntity;
	
	clsEntity *pMaster = pEntity->m_pDepencyMaster;
	if (pMaster == mmnull)
	{
		pEntity->m_pDepencyMaster = this;
		return;
	}
	
	clsEntity* pLastMaster;
	do
	{
		pLastMaster = pMaster;
		if (pMaster == this)
			return;
		pMaster = pMaster->m_pNextDepencyMaster;
	}while(pMaster);
	pLastMaster->m_pNextDepencyMaster = this;
}
//==========================================================================================
mmVoid clsEntity::UnlinkUpdateDepency(clsEntity* pEntity, mmBool bRemoveMaster)
{
	clsEntity* pPrevDepency = m_pDepency;
	if (pPrevDepency != mmnull)
	{
		if (pPrevDepency == pEntity)
			m_pDepency = m_pDepency->m_pNextDepency;
		else
		{
			clsEntity* pNextDepency = m_pDepency->m_pNextDepency;
			do
			{
				if (pNextDepency == pEntity)
				{
					pPrevDepency->m_pNextDepency = pNextDepency->m_pNextDepency;
					break;
				}
			pPrevDepency = pNextDepency;
			pNextDepency = pNextDepency->m_pNextDepency;
			}while(pNextDepency);
			if (pNextDepency == mmnull)
				pPrevDepency->m_pNextDepency = mmnull;
		}
	}
		
	if (bRemoveMaster)
	{
		clsEntity *pLastMaster = pEntity->m_pDepencyMaster;
		if (pLastMaster != mmnull)
		{
			if (pLastMaster == this)
				pLastMaster = pLastMaster->m_pNextDepencyMaster;
			else
			{
				clsEntity* pMaster = pLastMaster->m_pNextDepencyMaster;
				do
				{
					if (pMaster == this)
					{
						pLastMaster->m_pNextDepencyMaster = pMaster->m_pNextDepencyMaster;
						break;
					}
					pLastMaster = pMaster;
					pMaster = pMaster->m_pNextDepencyMaster;
				}while(pMaster);
				if (pMaster == mmnull)
					pLastMaster->m_pNextDepencyMaster = mmnull;
			}
		}
	}
}

// Render: clones entity
//==========================================================================================
clsEntity* clsEntity::Clone(const clsString& sNewName,
							const clsString& sNewType,
							mmBool bCloneChildrens,
							clsMatrix* pTForm,
							mmBool bFirst)
{
	clsEntity* pEntity;
	if (Owner == mmnull)
	{
		pEntity = m_pEFactory->CreateEntity(sNewType, sNewName);	// <-- ß ÌÓÄÎ×ÈÙÙÅ ÂÒÎÐÎÉ ÐÀÇ
	}
	else
	{
		pEntity = Owner->AddChildNode(sNewType, sNewName);
	}
	if (pEntity == mmnull)
		return mmnull;
	
	pEntity->m_AnimState					= m_AnimState;
	pEntity->m_bEnableSphereTest			= m_bEnableSphereTest;
/*	pEntity->m_bNeedBufferedKeyboardEvent   = m_bNeedBufferedKeyboardEvent;
	pEntity->m_bNeedBufferedMouseEvent      = m_bNeedBufferedMouseEvent;
	pEntity->m_bNeedMouseEvent              = m_bNeedMouseEvent;
	pEntity->m_bNeedUpdateEvent             = m_bNeedUpdateEvent;*/
	pEntity->m_bRenderable					= m_bRenderable;
	pEntity->m_cBoundBox					= m_cBoundBox;
	pEntity->m_cBoundSphere					= m_cBoundSphere;
	pEntity->m_pAnimController				= m_pAnimController;
	pEntity->m_mxFull						= m_mxFull;
	pEntity->m_mxLocal						= m_mxLocal;
	pEntity->m_mxWorld						= (pTForm == mmnull) ? m_mxWorld : *pTForm;
	pEntity->m_pDepency						= m_pDepency;
	pEntity->m_pDepencyMaster               = m_pDepencyMaster;
	pEntity->m_pTformHolder					= m_pTformHolder;
	pEntity->m_sType						= sNewType;
	pEntity->m_bDrawBoundSphere				= m_bDrawBoundSphere;
	if (Res != mmnull)
		pEntity->Res = Renderer->GetResourceManager()->GetResource(Res->GetName(), Res->GetType());
	pEntity->Type							= Type;
	pEntity->Path							= Path;
	pEntity->m_eBoundType					= m_eBoundType;
	pEntity->m_bRayCollide					= m_bRayCollide;
	
	pEntity->OnClone(this);
	
	if (bCloneChildrens)
	{
		clsEntity* pChildrens = Child;
		while(pChildrens)
		{
			clsEntity* pClone = pChildrens->Clone(pChildrens->Name, pChildrens->m_sType, bCloneChildrens, mmnull, mmfalse);
			if (pClone != mmnull)
				pClone->SetOwner(pEntity);
			pChildrens = pChildrens->Next;
		}
	}
	// test
	pEntity->m_bs = Renderer->GetResourceManager()->GetResource("Boundsphere.mmh", MHMRES_MESH_ID);
	// test


	if (m_AnimKeys != mmnull)
		pEntity->m_AnimKeys = (clsAnimKeys*)Renderer->GetResourceManager()->GetResource(m_AnimKeys->GetName(), m_AnimKeys->GetType());
	if (m_AnimationRageController != mmnull)
	{
		pEntity->m_AnimationRageController = new clsAnimationRageController;
		*pEntity->m_AnimationRageController = *m_AnimationRageController;
	}else{
		pEntity->m_AnimationRageController = mmnull;
	}
	if (bFirst && Owner)					// <-- ÌÓÄÀÊ ÒÐÅÒÈÉ ÐÀÑ
		pEntity->ProcessInitialize(mmtrue);
	return pEntity;
}

// Clone: clones same-type entity
//========================================================================================
clsEntity* clsEntity::Clone(const clsString &sNewName, 
							mmBool bCloneChildrens,
							clsMatrix *pTForm)
{
	return Clone(sNewName, m_sType, bCloneChildrens, pTForm);
}

// Render: renders entity
//==========================================================================================
mmRes clsEntity::Render()
{
	mmRes r= mmFail;
	Renderer->GetDevice()->SetModelTM(&m_mxFull);
	mmRes RenRes = OnRender();
	if (Res && MHMSUCC(RenRes))
	{
		if (Res->GetType() == enResStaticMesh)
			((clsStaticMesh*)Res)->Animate(Owner);
		r = Res->Render();
	}
	
	if (m_bDrawBoundSphere)
		if (m_cBoundSphere.m_fRadius > 0.0f)
		{
			/*clsMatrix mxScale, mxTrans;
			mxScale.Scale(m_cBoundSphere.m_fRadius, m_cBoundSphere.m_fRadius, m_cBoundSphere.m_fRadius);
			clsVector3 vPosition = m_cBoundSphere.m_cCenter;
			vPosition.Transform(m_mxFull);
			mxTrans.Translation(vPosition);
			//mxScale.SetTranslation(vPosition);
			mxScale = mxScale * Owner->m_mxFull;
			mxScale.SetTranslation(vPosition);
			Renderer->GetDevice()->SetModelTM(&mxScale);*/
			clsMatrix mxScale;
			clsVector3 vPosition = m_cBoundSphere.m_cCenter;
			vPosition.Transform(m_mxFull);
			clsMatrix mxTrans;
			clsVector3 vScale(1.0f, 0.0f, 0.0f);
			vScale.TransformNormal(m_mxFull);
			mxScale.Scale(vScale.Length() * m_cBoundSphere.m_fRadius,
				vScale.Length() * m_cBoundSphere.m_fRadius,
				vScale.Length() * m_cBoundSphere.m_fRadius);
			mxTrans.Translation(vPosition);
			mxScale = mxScale * mxTrans;
			Renderer->GetDevice()->SetModelTM(&mxScale);
			if (m_bs)
				m_bs->Render();
		}
	return r;
}

// ProcessBufferedMouseEvent: processing mouse event
//==========================================================================================
mmVoid clsEntity::ProcessBufferedMouseEvent(const clsMouseEvent* pEvent, mmBool bFirst)
{
	if (m_bNeedBufferedMouseEvent)
		OnBufferedMouseEvent(pEvent, bFirst);
	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->ProcessBufferedMouseEvent(pEvent, bFirst);
		Childs = Childs->Next;
	}
}

// ProcessBufferedKeyboardEvent: processing keyboard event
//==========================================================================================
mmVoid clsEntity::ProcessBufferedKeyboardEvent(const clsKeyState* pEvent, mmBool bFirst)
{
	if (m_bNeedBufferedKeyboardEvent)
		OnBufferedKeyboardEvent(pEvent, bFirst);
	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->ProcessBufferedKeyboardEvent(pEvent, bFirst);
		Childs = Childs->Next;
	}
}


// ProcessMouseEvent: processing mouse event
//==========================================================================================
mmVoid clsEntity::ProcessMouseEvent(const clsMouseEvent* pEvent)
{
	if (m_bNeedMouseEvent)
		OnMouseEvent(pEvent);
	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->ProcessMouseEvent(pEvent);
		Childs = Childs->Next;
	}
}

// ProcessUpdate: processing update event
//==========================================================================================
mmVoid clsEntity::ProcessUpdate(const clsTimer* pTimer)
{
	if (m_bNeedUpdateEvent)
		OnUpdate(pTimer);
	// processing depencies
	clsEntity* pDepencies = m_pDepency;
	while(pDepencies)
	{
		pDepencies->OnDepencyUpdate(this);
		pDepencies = pDepencies->m_pNextDepency;
	}
	if (m_bFinishUpdate)
		return;
	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->ProcessUpdate(pTimer);
		Childs = Childs->Next;
	}
}

// ProcessEndBufferesEvetns: processing end of buffered data event
//==========================================================================================
mmVoid clsEntity::ProcessEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents)
{
	if ((m_bNeedBufferedMouseEvent)||(m_bNeedBufferedKeyboardEvent))
		OnEndBufferedEvents(nNumMouseEvents, nNumKeyboardEvents);
	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->ProcessEndBufferedEvents(nNumMouseEvents, nNumKeyboardEvents);
		Childs = Childs->Next;
	}
}

// ProcessInitialize: processing initialization event
//==========================================================================================
mmVoid clsEntity::ProcessInitialize(mmBool bClone)
{
	if (!m_bInitialized)
	{
		m_bInitialized = mmtrue;
		OnInitialize(bClone);
	}
	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->ProcessInitialize(bClone);
		Childs = Childs->Next;
	}
}

// ProcessLoading: processing post-load
//==========================================================================================
mmVoid clsEntity::ProcessLoading()
{
	if (m_bNeedLoad)
	{
		m_bNeedLoad = mmfalse;
		OnPostLoading();
	}
	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->ProcessLoading();
		Childs = Childs->Next;
	}
}

// ProcessLoading: processing post-load
//==========================================================================================
mmVoid clsEntity::ProcessSpeedCalc(const clsTimer* pTimer)
{
	if (m_bNeedSpeedCalc)
		OnCalcSpeed(pTimer);

	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->ProcessSpeedCalc(pTimer);
		Childs = Childs->Next;
	}
}


// CollectSuicides: collects all want to death objects
//==========================================================================================
mmVoid clsEntity::Collect(clsEntityArray &cSuicides, clsEntityArray& cCollides)
{
	if (m_bKillMe && !m_bKillProtection)
	{
		cSuicides.Add(this);
	}
	else
	{
		if (m_bNeedCollide)
			cCollides.Add(this);
	}
	
	
	clsEntity *Childs = Child;
	while(Childs)
	{
		Childs->Collect(cSuicides, cCollides);
		Childs = Childs->Next;
	}
}

// UnbindAllLists: remove this entity from all lists
//==========================================================================================
mmVoid clsEntity::ProcessUnbindDepencies()
{
	// processing depencies about DEATH
	clsEntity* pDepencies = m_pDepency;
	while(pDepencies)
	{
		pDepencies->OnDepencyUpdate(this, mmtrue);
		pDepencies = pDepencies->m_pNextDepency;
	}

	clsEntity* pMaster = m_pDepencyMaster;
	while(pMaster)
	{
		pMaster->UnlinkUpdateDepency(this, mmfalse);
		pMaster = pMaster->m_pNextDepencyMaster;
	}
	clsEntity* pChildrens = Child;
	while(pChildrens)
	{
		pChildrens->ProcessUnbindDepencies();
		pChildrens = pChildrens->Next;
	}
	
	// finally remove from global list
	// DONT USE FINDLIST
/*	clsEntity* pLastEntity = FindRoot()->List;
	if (pLastEntity == mmnull)
		return;
	clsEntity* pEntity = pLastEntity->List;
	while(pEntity)
	{
		if (pEntity == this)
		{
			pLastEntity->List = List;
			return;
		}
		pLastEntity = pEntity;
		pEntity = pEntity->List;
	}
	pLastEntity->List = mmnull;*/
}

mmVoid clsEntity::EnumChilds(const clsString& sType, clsEntity* pCallback)
{
	if (pCallback == mmnull)
		return;
	
	if (sType.IsEmpty())
	{
		pCallback->OnEnumChild(this);
	}
	else
	{
		if (sType == m_sType)
			pCallback->OnEnumChild(this);
	}
	
	clsEntity* pChildrens = Child;
	while(pChildrens)
	{
		pChildrens->EnumChilds(sType, pCallback);
		pChildrens = pChildrens->Next;
	}
}



mmVoid clsEntity::DumpHierarchy(clsString sSpace)
{
	g_plog->Out(sSpace + Name, enLCBlue);
	clsEntity* pChilds = Child;
	while(pChilds)
	{
		pChilds->DumpHierarchy(sSpace + "  ");		
		pChilds = pChilds->Next;
	}
}


clsVector3	clsEntity::GetPosition()
{
	return clsVector3(m_mxFull._41,m_mxFull._42,m_mxFull._43);
}

clsVector3	clsEntity::GetWorldPosition()
{
	return clsVector3(m_mxWorld._41,m_mxWorld._42,m_mxWorld._43);
}

mmVoid clsEntity::ProcessCount(mmUint& nTotal)
{
	nTotal++;
	clsEntity* pChilds = Child;
	while(pChilds)
	{
		pChilds->ProcessCount(nTotal);
		pChilds = pChilds->Next;
	}
}

