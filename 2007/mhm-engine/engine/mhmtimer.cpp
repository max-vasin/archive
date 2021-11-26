#include "mhmbase.h"
#include "mhmtimer.h"

//******************************************************
clsEventNode::clsEventNode()
{
	Event = mmnull;
	ID = -1;
	Name = "";
	SetTim = 0;
	OnTim = 0;
	AbsoluteOnTim = 0;
}
//--------------------------------------------------------
clsEventNode::~clsEventNode()
{
}
//--------------------------------------------------------
mmBool clsEventNode::operator == (clsEventNode & n)
{
	return (Event==n.Event)&&(ID==n.ID)&&(Name==n.Name)&&(SetTim==n.SetTim)&&(OnTim==n.OnTim)&&(AbsoluteOnTim==n.AbsoluteOnTim)&&(UserID==n.UserID);	
}
//--------------------------------------------------------
mmBool clsEventNode::operator != (clsEventNode & n)
{
	return !(*this == n);	
}
//******************************************************
clsTimeEvent::clsTimeEvent()
{
	timer = mmnull;
}
//--------------------------------------------------------
clsTimeEvent::clsTimeEvent(clsTimer * Timer)
{
	timer = Timer;
}
//--------------------------------------------------------
clsTimeEvent::~clsTimeEvent()
{
	if(timer == mmnull)
		return;
	timer->DeleteEvent(this);
}
//--------------------------------------------------------
mmVoid clsTimeEvent::SetTimer(clsTimer * Timer)
{
	timer = Timer;
}
//--------------------------------------------------------
mmInt clsTimeEvent::SetEvent(mmInt UserID, mmDouble Time, enTypTime TypTim)
{
	return SetEvent(clsString(""),UserID,Time,TypTim);
}
//--------------------------------------------------------
mmInt clsTimeEvent::SetEvent(clsString Name, mmInt UserID, mmDouble Time, enTypTime TypTim)
{
	if(timer == mmnull)
		return -1;
	return timer->SetEvent(this,Name,UserID,Time,TypTim);	
}
//--------------------------------------------------------
mmInt clsTimeEvent::SetEvent(mmInt UserID, mmDouble Time)
{
	return SetEvent(clsString(""),UserID,Time,enMiSec);
}
//--------------------------------------------------------
mmInt clsTimeEvent::SetEvent(clsString Name, mmInt UserID, mmDouble Time)
{
	return SetEvent(Name,UserID,Time,enMiSec);	
}
//******************************************************
mmVoid clsTimer::DeleteEvent(clsTimeEvent * Event)
{
	Events.ResetSelect();
	while(Events.SelectNext())
		if(Events.Selected().Event == Event)
		{
			Events.Remove(Events.SelectedID());
			Events.ResetSelect();
		}
}
//--------------------------------------------------------
mmInt clsTimer::SetEvent(clsTimeEvent * Event,clsString Name, mmInt UserID, mmDouble time)
{
	return SetEvent(Event,Name,UserID,time, enMiSec);
}
//--------------------------------------------------------
mmInt clsTimer::SetEvent(clsTimeEvent * Event,clsString Name, mmInt UserID, mmDouble time, enTypTime TypTim)
{
	clsEventNode node;
	if(TypTim == enTik)
		node.OnTim = (mmInt64)time;
	else if(TypTim == enSec)
		node.OnTim = (mmInt64)(time * (mmDouble)Freq);
	else if(TypTim == enMiSec)
		node.OnTim = (mmInt64)(time * 0.001 * (mmDouble)Freq);
	else
		return -1;
	node.Event = Event;
	node.Name = Name;
	node.UserID = UserID;
	node.SetTim = CurrentTime;
	node.AbsoluteOnTim = node.SetTim + node.OnTim;
	LastID++;
	node.ID = LastID;

	if(Events.Count() == 0)
	{
		Events.Add(node);
		return LastID;
	}
	Events.ResetSelect();
	while(Events.SelectPrev())
		if(Events.Selected().AbsoluteOnTim <= node.AbsoluteOnTim)
		{
			Events.Insert(node,Events.SelectedID()+1);
			break;
		}
	return LastID;
}
//--------------------------------------------------------
clsTimer::clsTimer()
{
	Initialization();
}
//--------------------------------------------------------
mmVoid clsTimer::Initialization()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&Freq);  
	k=(mmFloat)(1.0/((mmDouble)Freq));                          
	QueryPerformanceCounter((LARGE_INTEGER *) &StartTime);  
	Time = StartTime;
	CurrentTime = StartTime;
	Events.Clear();
	LastID = -1;
	TimeElapsed = 0.0f;
	
//	m_bStackFilled = mmfalse;
	mmZeroVar(m_FrameStack);
	m_nStackSize    = 0;
	m_fAverageDelta  = 0.0f;
//	m_fLastFPS     = 0.0f;
	m_fDeltaFactor = 1.0f;
}
//--------------------------------------------------------
mmVoid clsTimer::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER *) &CurrentTime);
	TimeElapsed=((mmFloat)(CurrentTime-Time));

	//TimeElapsed = (TimeElapsed + ((mmFloat)(CurrentTime-Time)) ) / 2;

	Time = CurrentTime;
//	CalcAverageFPS();




	if(Events.Count()>0)
		while(ExecuteEvent(CurrentTime));

//	m_fAverageDelta = Freq / 60;

//	m_fAverageDelta = Freq / 50;

	m_fAverageDelta = TimeElapsed;
	return;

	static mmFloat _fAverageDelta=0;

	if( (_fAverageDelta * m_fAverageDelta * 0.01f ) < 50.0f )
	{

		mmZeroVar(m_FrameStack);
		m_nStackSize    = 0;
		//m_FrameStack[0] = _fAverageDelta;
		_fAverageDelta = m_fAverageDelta;
		m_fAverageDelta  = _fAverageDelta;
		//	m_fLastFPS     = 0.0f;
		m_fDeltaFactor = 1.0f;

	}else{
		_fAverageDelta = m_fAverageDelta;
	}
}
//--------------------------------------------------------
mmBool clsTimer::ExecuteEvent(const mmInt64 & tim)
{
	if(Events.Count()==0)
		return mmfalse;
	clsEventNode ev;
	ev = Events[0];
	if(ev.AbsoluteOnTim > tim)
		return mmfalse;
	if(ev.Event != mmnull)
		ev.Event->OnTimEvent(ev.ID, ev.UserID, ev.Name);
	Events.Remove(0);
	return mmtrue;
}
//--------------------------------------------------------
mmFloat clsTimer::GetDeltaTicks()
{
	return TimeElapsed;
}
//--------------------------------------------------------
mmFloat clsTimer::GetDeltaMiSecs() const
{
	return m_fAverageDelta*k*1000.0f*m_fDeltaFactor;//TimeElapsed*k*1000.0f*m_fDeltaFactor;
}
//--------------------------------------------------------
mmFloat clsTimer::GetDeltaSecs() const
{
	return m_fAverageDelta*k*m_fDeltaFactor;//TimeElapsed*k*m_fDeltaFactor;
}
//--------------------------------------------------------
mmFloat clsTimer::GetFPS()
{
	return ((mmFloat)Freq) / m_fAverageDelta;//TimeElapsed;
}
//--------------------------------------------------------
mmFloat clsTimer::GetAverageFPS()
{
	return m_fAverageDelta;
}
//--------------------------------------------------------
mmFloat clsTimer::GetTicks() const
{
	return ((mmFloat)(CurrentTime - StartTime));
}
//--------------------------------------------------------
mmFloat clsTimer::GetMiSecs() const
{
	return ((mmFloat)(CurrentTime - StartTime))*k*1000.0f;
}
//--------------------------------------------------------
mmFloat clsTimer::GetSecs() const
{
	return ((mmFloat)(CurrentTime - StartTime))*k;
}
//--------------------------------------------------------
mmInt64 clsTimer::GetFreq()
{
	return Freq;
}
//--------------------------------------------------------
mmFloat clsTimer::GetTicksToEvent(mmInt ID)
{
	Events.ResetSelect();
	while(Events.SelectNext())
		if(Events.Selected().ID == ID)
		{
			return (mmFloat)(Events.Selected().AbsoluteOnTim - CurrentTime);
		}
	return 0;
}
//--------------------------------------------------------
mmFloat clsTimer::GetMiSecsToEvent(mmInt ID)
{
	Events.ResetSelect();
	while(Events.SelectNext())
		if(Events.Selected().ID == ID)
		{
			return ((mmFloat)(Events.Selected().AbsoluteOnTim - CurrentTime))*k*1000.0f;
		}
	return 0;
}
//--------------------------------------------------------
mmFloat clsTimer::GetSecsToEvent(mmInt ID)
{
	Events.ResetSelect();
	while(Events.SelectNext())
		if(Events.Selected().ID == ID)
		{
			return ((mmFloat)(Events.Selected().AbsoluteOnTim - CurrentTime))*k;
		}
	return 0;
}
//--------------------------------------------------------
mmFloat clsTimer::GetTicksToEvent(clsString & name)
{
	Events.ResetSelect();
	while(Events.SelectNext())
		if(Events.Selected().Name == name)
		{
			return (mmFloat)(Events.Selected().AbsoluteOnTim - CurrentTime);
		}
	return 0;
}
//--------------------------------------------------------
mmFloat clsTimer::GetMiSecsToEvent(clsString & name)
{
	Events.ResetSelect();
	while(Events.SelectNext())
		if(Events.Selected().Name == name)
		{
			return ((mmFloat)(Events.Selected().AbsoluteOnTim - CurrentTime))*k*1000.0f;
		}
	return 0;
}
//--------------------------------------------------------
mmFloat clsTimer::GetSecsToEvent(clsString & name)
{
	Events.ResetSelect();
	while(Events.SelectNext())
		if(Events.Selected().Name == name)
		{
			return ((mmFloat)(Events.Selected().AbsoluteOnTim - CurrentTime))*k;
		}
	return 0;
}
//--------------------------------------------------------
mmFloat clsTimer::GetTicksAfterEvent(mmInt ID)
{
	return -GetTicksToEvent(ID);
}
//--------------------------------------------------------
mmFloat clsTimer::GetMiSecsAfterEvent(mmInt ID)
{
	return -GetMiSecsToEvent(ID);
}
//--------------------------------------------------------
mmFloat clsTimer::GetSecsAfterEvent(mmInt ID)
{
	return -GetSecsToEvent(ID);
}
//--------------------------------------------------------
mmFloat clsTimer::GetTicksAfterEvent(clsString & name)
{
	return -GetTicksToEvent(name);
}
//--------------------------------------------------------
mmFloat clsTimer::GetMiSecsAfterEvent(clsString & name)
{
	return -GetMiSecsToEvent(name);
}
//--------------------------------------------------------
mmFloat clsTimer::GetSecsAfterEvent(clsString & name)
{
	return -GetSecsToEvent(name);
}
//--------------------------------------------------------
mmVoid clsTimer::CalcAverageFPS()
{
	//if (m_nStackPos == MHMTIMER_FPSSTACK_FRAMES)
	//{
	//	m_nStackPos = 0;
	//	m_bStackFilled = mmtrue;
	//}
	//mmFloat curfps = GetFPS();
	//mmFloat delta = curfps - m_fLastFPS;
	//if (delta < 0.0f)
	//	delta = -delta;
	//if (delta < MHMTIMER_FPSSTACK_THRESHOLD) // skip frame
	//	return;
	//m_aFramesStack[m_nStackPos] = curfps;
	//mmUint count = (m_bStackFilled == mmtrue) ? MHMTIMER_FPSSTACK_FRAMES : m_nStackPos + 1;
	//m_fAverageFPS = 0.0f;
	//for (mmUint i = 0; i < count; i++)
	//	m_fAverageFPS += m_aFramesStack[i];
	//m_fAverageFPS /= count;
	//m_fLastFPS = curfps;
	//m_aFramesStack[m_nStackPos] = m_fAverageFPS;
	//m_nStackPos++;


	if (m_nStackSize < MHMTIMER_FPSSTACK_FRAMES)
	{
		//m_FrameStack[m_nStackSize].m_fDelta = fDelta;
		m_FrameStack[m_nStackSize] = TimeElapsed;
		m_nStackSize++;
	}
	else
	{
		for (mmUint i = MHMTIMER_FPSSTACK_FRAMES - 1; i != 0; i--)
			m_FrameStack[i] = m_FrameStack[i - 1];
		//m_FrameStack[0].m_fDelta = fDelta;
		m_FrameStack[0] = TimeElapsed;
	}
	
	m_fAverageDelta = 0.0f;
	for (mmUint i = 0; i < m_nStackSize; i++)
	{
		//m_fAverageDelta += m_FrameStack[i].m_fDelta;
		m_fAverageDelta += m_FrameStack[i];
	}
	m_fAverageDelta /= m_nStackSize;

}
//--------------------------------------------------------
mmVoid clsTimer::SetDeltaFactor(mmFloat fFactor)
{
	m_fDeltaFactor = fFactor;
}


//******************************************************