//---------------------------------------------------------------------------

#ifndef SpriteEditH
#define SpriteEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSpriteRect
{
public:
 	TSpriteRect*	m_pNext;
    int				m_iZoom;
    int				m__iX;
    int				m__iY;
    int				m__iWidth;
    int				m__iHeight;
    int				m_iPivotX;
    int				m_iPivotY;
    bool			m_bSelected;
    bool			m_bLocked;

public:
	__fastcall TSpriteRect();
	__fastcall TSpriteRect(TSpriteRect* pNext, int iX, int iY, int iWidth,
    						int iHeight, int iPivotX, int iPivotY);
    void __fastcall Invert(bool& x, bool& y);
    void __fastcall SetZoom(int iZoom);

	void __fastcall SetX(int X);
    void __fastcall SetY(int Y);
    void __fastcall SetWidth(int Width);
    void __fastcall SetHeight(int Height);

    int __fastcall GetX();
    int __fastcall GetY();
    int __fastcall GetWidth();
    int __fastcall GetHeight();

	__property int m_iX = {read=GetX, write=SetX};
  	__property int m_iY = {read=GetY, write=SetY};
   	__property int m_iWidth  = {read=GetWidth, write=SetWidth};
   	__property int m_iHeight = {read=GetHeight, write=SetHeight};
};
//---------------------------------------------------------------------------
enum enMouseIntersect
{
	enNone,
	enTop,
    enBottom,
    enLeft,
    enRight,
    enLeftTop,
    enRightTop,
    enLeftBottom,
    enRightBottom
};
//---------------------------------------------------------------------------
enum enEditMode
{
	emMove		 = 0,
    emTransform  = 1,
    emSelect	 = 2
};
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TLockEvent)(TSpriteRect* pLockedRect);
typedef void __fastcall (__closure *TRectChangeEvent)(TSpriteRect* pLockedRect);
//---------------------------------------------------------------------------
class PACKAGE TSpriteEdit : public TGraphicControl
{
private:
	TSpriteRect*	m_pHead;
    int				m_iRectCount;
    int				m_iPointSize;
    TColor			m_SelectedPointColor;
    TColor			m_UnselectedPointColor;
    TColor			m_SelectedLineColor;
    TColor			m_UnselectedLineColor;
    TColor			m_LockedLineColor;
    TColor			m_LockedPointColor;
    TColor			m_FrameNumberColor;
    TSpriteRect*	m_pSelected;
    TSpriteRect*	m_pLocked;
    TSpriteRect*	m_pCurrent;
    enMouseIntersect m_eIntersect;
	int				m_iInterX;
   	int				m_iInterY;
    enEditMode		m_eEditMode;
    int				m_MouseLineSens;
    int				m_iZoom;
    TLockEvent		m_pLockEvent;
    TRectChangeEvent m_pChangeEvent;
    Graphics::TBitmap*		m_Bitmap;


protected:
	void __fastcall DrawRectangles();
    void __fastcall GetPointRect(int iX, int iY, TRect& Rect, bool bUseZoom = false);
    enMouseIntersect __fastcall CheckMouseRect(int iX, int iY, TSpriteRect* pRect);
    void __fastcall TransformRect(TSpriteRect* pRect, int iX, int iY);
    void __fastcall MoveRect(TSpriteRect* pRect, int iX, int iY);

public:
	__fastcall TSpriteEdit(TComponent* Owner);
	TSpriteRect* __fastcall AddRect();
    TSpriteRect* __fastcall AddRect(int iX, int iY, int iWidth, int iHeight,
    												int iPivotX, int iPivotY);
    void __fastcall RemoveRect(TSpriteRect* pRect);
    void __fastcall RemoveAllRects();
    int  __fastcall GetRectCount();
    void __fastcall Paint();
    void __fastcall MouseMoves(TObject* pSender, TShiftState Shift, int X, int Y);
    void __fastcall MouseButtonDown(TObject* pSender, TMouseButton Button, TShiftState Shift,
    											int X, int Y);
	void __fastcall SelectRects();
    TSpriteRect* __fastcall NextRect();

    // point size property
    void __fastcall SetPointSize(int iSize);
    int  __fastcall GetPointSize();

    // selected point color property
    void __fastcall SetSelectedPointColor(TColor Color);
    TColor __fastcall GetSelectedPointColor();

    // unselected point color property
    void __fastcall SetUnselectedPointColor(TColor Color);
    TColor __fastcall GetUnselectedPointColor();

    // selected line color property
    void __fastcall SetSelectedLineColor(TColor Color);
    TColor __fastcall GetSelectedLineColor();

    // unselected line color property
    void __fastcall SetUnselectedLineColor(TColor Color);
    TColor __fastcall GetUnselectedLineColor();

    // locked line color property
    void __fastcall SetLockedLineColor(TColor Color);
    TColor __fastcall GetLockedLineColor();

    // locked point color property
    void __fastcall SetLockedPointColor(TColor Color);
    TColor __fastcall GetLockedPointColor();

    // frame numbers property
    void __fastcall SetFrameNumberColor(TColor Color);
    TColor __fastcall GetFrameNumberColor();

    // edit mode prop
    void __fastcall SetEditMode(enEditMode eMode);
    enEditMode __fastcall GetEditMode();

    // edit mode prop
    void __fastcall SetLineSense(int iSense);
    int __fastcall GetLineSense();

    // zoom prop
    void __fastcall SetZoom(int iZoom);
    int __fastcall GetZoom();

    // lock event prop
    void __fastcall SetLockEvent(TLockEvent pEvent);
    TLockEvent __fastcall GetLockEvent();

    // rectangle change event prop
    void __fastcall SetChangeEvent(TRectChangeEvent pEvent);
    TRectChangeEvent __fastcall GetChangeEvent();


__published:
	__property Anchors;
    __property int PointSize = {read=GetPointSize,write=SetPointSize};
    __property TColor SelectedPointColor = {read=GetSelectedPointColor,write=SetSelectedPointColor};
    __property TColor UnselectedPointColor = {read=GetUnselectedPointColor,write=SetUnselectedPointColor};
    __property TColor SelectedLineColor = {read=GetSelectedLineColor,write=SetSelectedLineColor};
    __property TColor UnselectedLineColor = {read=GetUnselectedLineColor,write=SetUnselectedLineColor};
    __property enEditMode EditMode = {read=GetEditMode, write=SetEditMode};
    __property int LineSense = {read=GetLineSense, write=SetLineSense};
    __property int Zoom = {read=GetZoom, write=SetZoom};
    __property TColor LockedLineColor = {read=GetLockedLineColor,write=SetLockedLineColor};
    __property TColor LockedPointColor = {read=GetLockedPointColor,write=SetLockedPointColor};
    __property TLockEvent OnLockRectangle = {read=GetLockEvent,write=SetLockEvent};
    __property TRectChangeEvent OnChangeRectangle = {read=GetChangeEvent, write=SetChangeEvent};
    __property TColor FrameNumberColor = {read=GetFrameNumberColor, write=SetFrameNumberColor};
};
//---------------------------------------------------------------------------
#endif
