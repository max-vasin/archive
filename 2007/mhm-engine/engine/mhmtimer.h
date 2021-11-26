//==========================================================================================
// Mayhem engine timer
// Coded:	Pink
// Created:	17.06.2006
//==========================================================================================

#ifndef _MHMTIMER_INCLUDED
#define _MHMTIMER_INCLUDED

class clsTimer;

enum enTypTime{enTik=0,enSec,enMiSec};

class clsTimeEvent
{
private:
	clsTimer * timer;
	
public:
	clsTimeEvent();
	clsTimeEvent(clsTimer * Timer);
   ~clsTimeEvent();
	mmVoid SetTimer(clsTimer * Timer);

	mmInt SetEvent(clsString Name, mmInt UserID, mmDouble Time, enTypTime TypTim);
	mmInt SetEvent(mmInt UserID, mmDouble Time, enTypTime TypTim);
	mmInt SetEvent(clsString Name, mmInt UserID, mmDouble Time);
	mmInt SetEvent(mmInt UserID, mmDouble Time);

	virtual mmVoid OnTimEvent( mmInt id, mmInt userID, clsString & Name) = mmnull;
	
};
//------------------------------------------------------
class clsEventNode
{
protected:
public:
	clsEventNode();
	~clsEventNode();

	clsTimeEvent * Event;
	mmInt UserID;
	mmInt ID;
	clsString Name;
	mmInt64 SetTim;
	mmInt64 OnTim;
	mmInt64 AbsoluteOnTim;

	mmBool operator == (clsEventNode & n);
	mmBool operator != (clsEventNode & n);
};

#define MHMTIMER_FPSSTACK_FRAMES	100
#define MHMTIMER_FPSSTACK_THRESHOLD	30.0f

//------------------------------------------------------
class clsTimer
{
protected:

	mmFloat k;
	mmFloat TimeElapsed;
	mmInt64 Freq;
	mmInt64 StartTime;
	mmInt64 Time;
	mmInt64 CurrentTime;
	mmFloat	m_fDeltaFactor;
	
	// MozG
	mmFloat m_FrameStack[MHMTIMER_FPSSTACK_FRAMES];
//	mmBool  m_bStackFilled;
	mmUint	m_nStackSize;
	mmFloat m_fAverageDelta;
//	mmFloat m_fLastFPS;
	// MozG

	mmInt LastID;

	mmBool ExecuteEvent(const mmInt64 & tim);

	tmpList<clsEventNode> Events;

public:

	clsTimer();

	mmVoid Initialization();
	mmVoid Update();

	mmInt64 GetFreq();

	mmFloat GetDeltaTicks();
	mmFloat GetDeltaMiSecs() const;
	mmFloat GetDeltaSecs() const;
	mmFloat GetFPS();
	mmFloat GetAverageFPS();

	mmFloat GetTicks() const;
	mmFloat GetMiSecs() const;
	mmFloat GetSecs() const;

	mmFloat GetTicksToEvent(mmInt ID);
	mmFloat GetMiSecsToEvent(mmInt ID);
	mmFloat GetSecsToEvent(mmInt ID);
	mmFloat GetTicksToEvent(clsString & name);
	mmFloat GetMiSecsToEvent(clsString & name);
	mmFloat GetSecsToEvent(clsString & name);

	mmFloat GetTicksAfterEvent(mmInt ID);
	mmFloat GetMiSecsAfterEvent(mmInt ID);
	mmFloat GetSecsAfterEvent(mmInt ID);
	mmFloat GetTicksAfterEvent(clsString & name);
	mmFloat GetMiSecsAfterEvent(clsString & name);
	mmFloat GetSecsAfterEvent(clsString & name);

	mmInt SetEvent(clsTimeEvent * Event,clsString Name, mmInt UserID, mmDouble time, enTypTime TypTim);
	mmInt SetEvent(clsTimeEvent * Event,clsString Name, mmInt UserID, mmDouble time);
	mmVoid DeleteEvent(clsTimeEvent * Event);
	
	mmVoid SetDeltaFactor(mmFloat fFactor);

private:
	mmVoid CalcAverageFPS();
};


#endif // _MHMTIMER_INCLUDED