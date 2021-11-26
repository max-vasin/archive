
#include "mhmbase.h"



//----------------------------------------------------------------------------
clsKey::clsKey()
{
	pos = 0.0f;
}
clsKey::~clsKey()
{
}
//****************************************************************************
clsAnimRange::clsAnimRange()
{
	start = 0;
	end = 0;
}
clsAnimRange::~clsAnimRange()
{
}
//****************************************************************************
clsAnimKeys::clsAnimKeys(clsResourceManager* presman, clsString name):
clsResource(presman, name, enResFont)
{
}
//----------------------------------------------------------------------------
clsAnimKeys::~clsAnimKeys(void)
{
}
//----------------------------------------------------------------------------
mmRes clsAnimKeys::Create(clsResourceParams *params)
{
	clsString filename = (m_fullname.IsEmpty()) ? m_name : m_fullname;
	clsFile file(filename);
	if (file.IsExist())
	{
		if (MHMFAIL(LoadAnim(filename)))
		{
			g_plog->Out("clsAnimKeys::Create[" + m_name + "] >> file parsing error", enLCRed);
			return mmFail;	
		}
	}
	else
	{
		g_plog->Out("clsAnimKeys::Create[" + m_name + "] >> animation file not exist", enLCRed);
		return mmFail;
	}
	g_plog->Out("clsAnimKeys::Create[" + m_name + "] >> animation created", enLCGreen);
	return mmOk;
}
//----------------------------------------------------------------------------
mmRes clsAnimKeys::LoadAnim(clsString &file)
{
	mmFloat fps = 1.0f;
	clsVarParser parser;
	if (MHMFAIL(parser.ParseFile(file)))
	{
		g_plog->Out("clsAnimKeys::LoadAnim[" + m_name + "] >> file parsing failed", enLCRed);
		return mmFail;
	}
	clsVarList* plist = parser.GetVarList();
	clsVariable* var;
	clsVarList arr;

	var = plist->FindVariable("FPS");
	if (var != mmnull)
	{
		fps = *var;
	}

	mmInt KeyCount;
	var = plist->FindVariable("KeyCount");
	if (var == mmnull)
	{
		g_plog->Out("clsAnimKeys::LoadAnim[" + m_name + "] >> KeyCount variable not founded", enLCYellow);
		return mmFail;
	}
	KeyCount = *var;
	Keys.Size(KeyCount);
	

	clsString name;
	for(mmInt z=0 ; z< KeyCount; z++)
	{
		name = "Key"+clsString(z);
		var = plist->FindVariable(name);
		if (var == mmnull)
			Keys[z].name = clsString(z);
		else
			Keys[z].name = (mmString)*var;

		name = "Key"+clsString(z)+"_pos";
		var = plist->FindVariable(name);
		if (var == mmnull)
		{
			g_plog->Out("clsAnimKeys::LoadAnim[" + m_name + "] >>" + name + " variable not founded", enLCYellow);
			return mmFail;
		}
		Keys[z].pos = *var;
		Keys[z].pos = 1000.0f/fps*Keys[z].pos;
	}
	//---------
	mmInt RangeCount;
	var = plist->FindVariable("RangeCount");
	if (var == mmnull)
	{
		g_plog->Out("clsAnimKeys::LoadAnim[" + m_name + "] >> RangeCount variable not founded", enLCYellow);
		return mmFail;
	}
	RangeCount = *var;
	Ranges.Size(RangeCount);

	
	for(mmInt z=0 ; z< RangeCount; z++)
	{
		name = "Range"+clsString(z);
		var = plist->FindVariable(name);
		if (var == mmnull)
			Ranges[z].name = clsString(z);
		else
			Ranges[z].name = (mmString)*var;

		name = "Range"+clsString(z)+"_start";
		var = plist->FindVariable(name);
		if (var == mmnull)
		{
			g_plog->Out("clsAnimKeys::LoadAnim[" + m_name + "] >> "+name+" variable not founded", enLCYellow);
			return mmFail;
		}
		Ranges[z].start = FindKey((mmString)*var);
		if (Ranges[z].start < 0)
		{
			g_plog->Out("clsAnimKeys::LoadAnim[" + m_name + "] >> name key not founded", enLCYellow);
			return mmFail;
		}

		name = "Range"+clsString(z)+"_end";
		var = plist->FindVariable(name);
		if (var == mmnull)
		{
			g_plog->Out("clsAnimKeys::LoadAnim[" + m_name + "] >> "+name+" variable not founded", enLCYellow);
			return mmFail;
		}
		Ranges[z].end = FindKey((mmString)*var);
		if (Ranges[z].end < 0)
		{
			g_plog->Out("clsAnimKeys::LoadAnim[" + m_name + "] >> name key not founded", enLCYellow);
			return mmFail;
		}
	}


	return mmOk;
}
//----------------------------------------------------------------------------
mmInt clsAnimKeys::FindKey(const mmString name)
{
	for(mmUint z=0 ; z < Keys.Size(); z++)
	{
		if(Keys[z].name == name)
			return z;
	}
	return -1;
}
//----------------------------------------------------------------------------
clsKey & clsAnimKeys::GetKey(mmInt i)
{
	return Keys[i];
}
//----------------------------------------------------------------------------
mmBool clsAnimKeys::FindKeys(const mmString name, mmInt & start, mmInt & end)
{
	for(mmUint z=0 ; z < Ranges.Size(); z++)
	{
		if(Ranges[z].name == name)
		{
			start = Ranges[z].start;
			end = Ranges[z].end;
			return mmtrue;
		}
	}
	return mmfalse;
}
//************************************************************************************************
clsAnimationRageController::clsAnimationRageController()
{
	IsPlaying = mmfalse;
	CurrentFrame = 0.0f;
	start = 0;
	end = 0;
	start_t = 0;
	end_t = 0;
	dir = 1.0f;
	CurentNLoop = 0;
	LoopCount = 0;
	Loop = enAnimLoop;
}
clsAnimationRageController::~clsAnimationRageController()
{
}

mmBool clsAnimationRageController::Update(clsEntity * Entity, mmFloat delta)
{
	if( !IsPlaying )
		return mmfalse;


	mmFloat ost;

	CurrentFrame += delta * dir;

	if(dir >0)
	{
		if( CurrentFrame >= end_t )
		{
			ost = CurrentFrame - end_t;
			CurentNLoop++;
			if(  ((CurentNLoop > LoopCount)  && (LoopCount != 0) ) || (Loop == enAnimHold))
			{
				CurrentFrame = end_t;
				IsPlaying = mmfalse;	
			}else{
				if( Loop == enAnimPingPong)
				{
					dir = -dir;
					CurrentFrame = end_t - ost;
					if(CurrentFrame < start_t)
						CurrentFrame = start_t;
				}else
				if( Loop == enAnimLoop)
				{
					CurrentFrame = start_t + ost;
					if(CurrentFrame > end_t)
						CurrentFrame = end_t;
				}
			}
			Entity->OnAnimationKey(end);
		}
	}else
	if(dir < 0)
	{
		if( CurrentFrame <= start_t )
		{
			ost = start_t - CurrentFrame;
			CurentNLoop++;
			if( ((CurentNLoop > LoopCount)  && (LoopCount != 0) ) || (Loop == enAnimHold))
			{
				CurrentFrame = start_t;
				IsPlaying = mmfalse;	
			}else{
				if( Loop == enAnimPingPong)
				{
					dir = -dir;
					CurrentFrame = start_t + ost;
					if(CurrentFrame > end_t)
						CurrentFrame = end_t;
				}else
				if( Loop == enAnimLoop)
				{
					CurrentFrame = end_t - ost;
					if(CurrentFrame < start_t)
						CurrentFrame = start_t;
				}
			}
			Entity->OnAnimationKey(start);
		}
	}

	Entity->m_AnimState.CurrentFrame = CurrentFrame;
	//Entity->m_AnimState.DeltaFrame   = 1;

	return mmtrue;
}