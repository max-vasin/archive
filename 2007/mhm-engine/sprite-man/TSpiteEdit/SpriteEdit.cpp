//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SpriteEdit.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// Sprite rectangle class
//---------------------------------------------------------------------------
__fastcall TSpriteRect::TSpriteRect()
{
    m_iZoom     = 1;
 	m_iX		= 0;
    m_iY 		= 0;
    m_iWidth 	= 0;
    m_iHeight   = 0;
    m_iPivotX   = 0;
    m_iPivotY   = 0;
    m_pNext		= NULL;
    m_bSelected = false;
    m_bLocked   = false;
}
//---------------------------------------------------------------------------
__fastcall TSpriteRect::TSpriteRect(TSpriteRect* pNext, int iX, int iY,
									int iWidth, int iHeight,
                                    int iPivotX, int iPivotY)
{
    m_iZoom     = 1;
	m_iX 	    = iX;
    m_iY 	    = iY;
    m_iWidth    = iWidth;
    m_iHeight   = iHeight;
    m_iPivotX   = iPivotX;
    m_iPivotY   = iPivotY;
    m_pNext     = pNext;
    m_bSelected = false;
    m_bLocked   = false;
}
//---------------------------------------------------------------------------
void __fastcall TSpriteRect::Invert(bool& x, bool& y)
{
	x = y = false;
	if (m_iWidth < 0)
	{
    	m_iX += m_iWidth;
    	m_iWidth = -m_iWidth;
        x = true;
    }
	if (m_iHeight < 0)
	{
    	m_iY += m_iHeight;
    	m_iHeight = -m_iHeight;
        y = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TSpriteRect::SetZoom(int iZoom)
{
	m_iZoom = iZoom;
}
//---------------------------------------------------------------------------
void __fastcall TSpriteRect::SetX(int X)
{
 	m__iX = (int)(X / m_iZoom);
}
//---------------------------------------------------------------------------
void __fastcall TSpriteRect::SetY(int Y)
{
 	m__iY = (int)(Y / m_iZoom);
}
//---------------------------------------------------------------------------
void __fastcall TSpriteRect::SetWidth(int Width)
{
 	m__iWidth = (int)(Width / m_iZoom);
}
//---------------------------------------------------------------------------
void __fastcall TSpriteRect::SetHeight(int Height)
{
 	m__iHeight = (int)(Height / m_iZoom);
}
//---------------------------------------------------------------------------
int __fastcall TSpriteRect::GetX()
{
 	return m__iX * m_iZoom;
}
//---------------------------------------------------------------------------
int __fastcall TSpriteRect::GetY()
{
 	return m__iY * m_iZoom;
}
//---------------------------------------------------------------------------
int __fastcall TSpriteRect::GetWidth()
{
 	return m__iWidth * m_iZoom;
}
//---------------------------------------------------------------------------
int __fastcall TSpriteRect::GetHeight()
{
 	return m__iHeight * m_iZoom;
}
//---------------------------------------------------------------------------
// Sprite edit component
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TSpriteEdit *)
{
	new TSpriteEdit(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSpriteEdit::TSpriteEdit(TComponent* Owner)
	: TGraphicControl(Owner)
{
	m_iRectCount   = 0;
    PointSize      = 2;
    SelectedPointColor 		= clRed;
    UnselectedPointColor 	= clGray;
    SelectedLineColor 		= clRed;
    UnselectedLineColor 	= clGray;
    LockedLineColor		 	= clBlue;
    LockedPointColor	 	= clBlue;
    FrameNumberColor		= clWhite;
    OnMouseMove   = this->MouseMoves;
    OnMouseDown   = this->MouseButtonDown;
    m_pSelected = NULL;
    m_iInterX	= 0;
    m_iInterY	= 0;
    EditMode    = emTransform;
    LineSense   = 2;
    Zoom        = 1;
    m_pLocked   = NULL;
    m_pCurrent  = NULL;
    m_Bitmap    = new Graphics::TBitmap;
}
//---------------------------------------------------------------------------
namespace Spriteedit
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSpriteEdit)};
		RegisterComponents("Mayhem", classes, 0);
	}
}
//---------------------------------------------------------------------------
TSpriteRect* __fastcall TSpriteEdit::AddRect()
{
	return AddRect(0,0,0,0,0,0);
}
//---------------------------------------------------------------------------
TSpriteRect* __fastcall TSpriteEdit::AddRect(int iX, int iY, int iWidth,
  											 int iHeight, int iPivotX,
                                             int iPivotY)
{
	if (m_pHead == NULL)
    {
     	m_pHead = new TSpriteRect(NULL, iX, iY, iWidth, iHeight, iPivotX, iPivotY);
		m_iRectCount++;
		m_pHead->SetZoom(Zoom);
        return m_pHead;
    }
    TSpriteRect* pRect = m_pHead;
    while(pRect)
    {
    	if (pRect->m_pNext == NULL)
        {
        	pRect->m_pNext = new TSpriteRect(NULL, iX, iY, iWidth, iHeight, iPivotX, iPivotY);
   			pRect->m_pNext->SetZoom(Zoom);
			m_iRectCount++;
        	return pRect->m_pNext;
        }
        pRect = pRect->m_pNext;
    }
    return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::RemoveRect(TSpriteRect* pRect)
{
 	if (pRect == NULL)
    	return;

    if (m_pHead == pRect)
    {
    	TSpriteRect* pNext = m_pHead->m_pNext;
    	delete m_pHead;
        m_pHead = pNext;
		m_iRectCount--;
        return;
    }
    TSpriteRect* pCurRect = m_pHead;
    TSpriteRect* pPrev = NULL;
    while(pCurRect)
    {
        pPrev = pCurRect;
    	pCurRect = pCurRect->m_pNext;
        if (pCurRect == pRect)
        {
        	pPrev->m_pNext = pCurRect->m_pNext;
            delete pCurRect;
            break;
        }
    }
	m_iRectCount--;
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::RemoveAllRects()
{
	TSpriteRect* pRect = m_pHead;
    TSpriteRect* pPrev;
    while(pRect)
    {
    	pPrev = pRect;
        pRect = pRect->m_pNext;
        delete pPrev;
    }
    m_pHead 	 = NULL;
	m_iRectCount = 0;
}
//---------------------------------------------------------------------------
int __fastcall TSpriteEdit::GetRectCount()
{
 	return m_iRectCount;
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SelectRects()
{
	m_pCurrent = m_pHead;
}
//---------------------------------------------------------------------------
TSpriteRect* __fastcall TSpriteEdit::NextRect()
{
	if (m_pCurrent)
    {
		TSpriteRect* pCur = m_pCurrent;
        m_pCurrent = m_pCurrent->m_pNext;
        return pCur;
    }
    return NULL;
}

//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::Paint()
{
 	DrawRectangles();
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::MouseButtonDown(TObject* pSender, TMouseButton Button, TShiftState Shift,
    											int X, int Y)
{
	if (Button == mbRight)
    {
        if (m_pLocked != NULL)
        {
        	m_pLocked->m_bLocked   = false;
			m_pLocked->m_bSelected = false;
        }
    	if (m_pSelected != NULL)
        {
			m_pLocked = m_pSelected;
            m_pLocked->m_bLocked = true;
        }
		(m_pLockEvent)(m_pLocked);
        Invalidate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::MouseMoves(TObject* pSender, TShiftState Shift, int X, int Y)
{
	X = X - X%m_iZoom;
    Y = Y - Y%m_iZoom;
	if (Shift.Contains(ssLeft) && (m_pSelected != NULL))
    {
		switch(m_eEditMode)
        {
		case emTransform:
	    	TransformRect(m_pSelected, X, Y);
            if (m_pChangeEvent)
            	(m_pChangeEvent)(m_pSelected);
            break;
        case emMove:
			MoveRect(m_pSelected, X, Y);
            if (m_pChangeEvent)
            	(m_pChangeEvent)(m_pSelected);
            break;
        }
        Invalidate();
        return;
    }
 	TSpriteRect* pCurRect = m_pHead;
    while(pCurRect)
    {
		m_eIntersect = CheckMouseRect(X, Y, pCurRect);
        if (m_eIntersect != enNone)
   	    {
			if (m_eEditMode == emTransform)
    	    {
        	    switch(m_eIntersect)
	            {
    	        case enLeftBottom:
	            case enRightTop:
					Cursor = crSizeNESW;
    	            break;
	            case enLeftTop:
    	        case enRightBottom:
	            	Cursor = crSizeNWSE;
    	            break;
	            case enLeft:
    	        case enRight:
	               	Cursor = crSizeWE;
    	            break;
	            case enBottom:
    	        case enTop:
	               	Cursor = crSizeNS;
    	            break;
	            }
	        }
            else
            {
            	Cursor = crSizeAll;
            }
  	        if (!pCurRect->m_bSelected)
            {
   	            m_pSelected = pCurRect;
	    		pCurRect->m_bSelected = true;
				Invalidate();
            }
			m_iInterX = X;
       	    m_iInterY = Y;
           	break;
        }
    	else
        {
           	if ((pCurRect->m_bSelected) && (!pCurRect->m_bLocked))
	        {
		        m_pSelected = NULL;
	    		pCurRect->m_bSelected = false;
				Invalidate();
            }
        	Cursor = crArrow;
        }
	   	pCurRect = pCurRect->m_pNext;
    }
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::DrawRectangles()
{
	TSpriteRect* pCurRect = m_pHead;
    TRect Rect;
	m_Bitmap->Width = Width;
    m_Bitmap->Height = Height;
	TCanvas* pCanvas = Canvas;//m_Bitmap->Canvas;
/*	Rect.Left 	= 0;
    Rect.Right 	= Width;
    Rect.Top 	= 0;
    Rect.Bottom = Height;
	pCanvas->Brush->Style = bsSolid;
    pCanvas->Brush->Color = TColor(0x00000000);
    pCanvas->FillRect(Rect);*/

	int number = 1;
    while(pCurRect)
    {
        // draw points
        pCanvas->Brush->Style = bsSolid;
        if (pCurRect->m_bSelected)
        {
            if (pCurRect->m_bLocked)
	       	    pCanvas->Brush->Color = m_LockedPointColor;
            else
           	    pCanvas->Brush->Color = m_SelectedPointColor;
        }
        else
        {
			pCanvas->Brush->Color = m_UnselectedPointColor;
        }
        GetPointRect(pCurRect->m_iX, pCurRect->m_iY, Rect);
		pCanvas->FillRect(Rect);
        GetPointRect(pCurRect->m_iX + pCurRect->m_iWidth, pCurRect->m_iY, Rect);
		pCanvas->FillRect(Rect);
        GetPointRect(pCurRect->m_iX + pCurRect->m_iWidth, pCurRect->m_iY + pCurRect->m_iHeight, Rect);
		pCanvas->FillRect(Rect);
        GetPointRect(pCurRect->m_iX,  pCurRect->m_iY + pCurRect->m_iHeight, Rect);
		pCanvas->FillRect(Rect);

        // draw lines
        if (pCurRect->m_bSelected)
        {
            if (pCurRect->m_bLocked)
	        	pCanvas->Pen->Color = m_LockedLineColor;
            else
            	pCanvas->Pen->Color = m_SelectedLineColor;
        }
        else
        {
        	pCanvas->Pen->Color = m_UnselectedLineColor;
        }
        pCanvas->MoveTo(pCurRect->m_iX, pCurRect->m_iY);
		pCanvas->LineTo(pCurRect->m_iX + pCurRect->m_iWidth, pCurRect->m_iY);
		pCanvas->LineTo(pCurRect->m_iX + pCurRect->m_iWidth, pCurRect->m_iY + pCurRect->m_iHeight);
		pCanvas->LineTo(pCurRect->m_iX,  pCurRect->m_iY + pCurRect->m_iHeight);
        pCanvas->LineTo(pCurRect->m_iX, pCurRect->m_iY);

        String Number = number;
        pCanvas->Brush->Style = bsClear;
		pCanvas->Font->Color  = m_FrameNumberColor;
	    pCanvas->TextOutA(pCurRect->m_iX + 5, pCurRect->m_iY + 5, Number);

        number++;
    	pCurRect = pCurRect->m_pNext;
    }
/*	Rect.Left 	= 0;
    Rect.Right 	= Width;
    Rect.Top 	= 0;
    Rect.Bottom = Height;
    m_Bitmap->Transparent = true;
    m_Bitmap->TransparentColor = TColor(0x00000000);
//    Canvas->Draw(0, 0, m_Bitmap);
//    pBitmap->TransparentMode = tmAuto;
//    Form1->Canvas->Draw(50,50,pBitmap);

    Canvas->CopyRect(Rect, pCanvas, Rect);*/
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::GetPointRect(int iX, int iY, TRect& Rect, bool bUseZoom)
{
	Rect.Left   = iX - m_iPointSize * ((bUseZoom) ? Zoom : 1);
    Rect.Top    = iY - m_iPointSize * ((bUseZoom) ? Zoom : 1);
    Rect.Right  = iX + m_iPointSize * ((bUseZoom) ? Zoom : 1);
    Rect.Bottom = iY + m_iPointSize * ((bUseZoom) ? Zoom : 1);
}

//---------------------------------------------------------------------------
// Point size property control
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetPointSize(int iSize)
{
	m_iPointSize = iSize;
    Invalidate();
}
//---------------------------------------------------------------------------
int __fastcall TSpriteEdit::GetPointSize()
{
	return m_iPointSize;
}
//---------------------------------------------------------------------------
// Selected point color property
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetSelectedPointColor(TColor Color)
{
	m_SelectedPointColor = Color;
    Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TSpriteEdit::GetSelectedPointColor()
{
	return m_SelectedPointColor;
}
//---------------------------------------------------------------------------
// Unselected point color property
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetUnselectedPointColor(TColor Color)
{
	m_UnselectedPointColor = Color;
    Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TSpriteEdit::GetUnselectedPointColor()
{
	return m_UnselectedPointColor;
}
//---------------------------------------------------------------------------
// Selected line color property
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetSelectedLineColor(TColor Color)
{
	m_SelectedLineColor = Color;
    Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TSpriteEdit::GetSelectedLineColor()
{
	return m_SelectedLineColor;
}
//---------------------------------------------------------------------------
// Unselected line color property
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetUnselectedLineColor(TColor Color)
{
	m_UnselectedLineColor = Color;
    Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TSpriteEdit::GetUnselectedLineColor()
{
	return m_UnselectedLineColor;
}
//---------------------------------------------------------------------------
// Locked line color property
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetLockedLineColor(TColor Color)
{
	m_LockedLineColor = Color;
    Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TSpriteEdit::GetLockedLineColor()
{
	return m_LockedLineColor;
}
//---------------------------------------------------------------------------
// Locked point color property
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetLockedPointColor(TColor Color)
{
	m_LockedPointColor = Color;
    Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TSpriteEdit::GetLockedPointColor()
{
	return m_LockedPointColor;
}
//---------------------------------------------------------------------------
// Frame numbers color property
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetFrameNumberColor(TColor Color)
{
	m_FrameNumberColor = Color;
    Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TSpriteEdit::GetFrameNumberColor()
{
	return m_FrameNumberColor;
}
//---------------------------------------------------------------------------
// Edit mode property control
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetEditMode(enEditMode eMode)
{
	m_eEditMode = eMode;
}
//---------------------------------------------------------------------------
enEditMode __fastcall TSpriteEdit::GetEditMode()
{
	return m_eEditMode;
}
//---------------------------------------------------------------------------
// Line sense property control
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetLineSense(int iSense)
{
	m_MouseLineSens = iSense;
}
//---------------------------------------------------------------------------
int __fastcall TSpriteEdit::GetLineSense()
{
	return m_MouseLineSens;
}
//---------------------------------------------------------------------------
// Zoom property control
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetZoom(int iZoom)
{
	m_iZoom = iZoom;
    TSpriteRect* pRect = m_pHead;
    while(pRect)
    {
		pRect->SetZoom(iZoom);
    	pRect = pRect->m_pNext;
    }
    Invalidate();
}
//---------------------------------------------------------------------------
int __fastcall TSpriteEdit::GetZoom()
{
	return m_iZoom;
}
//---------------------------------------------------------------------------
// Lock event property control
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetLockEvent(TLockEvent pEvent)
{
	m_pLockEvent = pEvent;
}
//---------------------------------------------------------------------------
TLockEvent __fastcall TSpriteEdit::GetLockEvent()
{
	return m_pLockEvent;
}
//---------------------------------------------------------------------------
// Rectangle change event property control
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::SetChangeEvent(TRectChangeEvent pEvent)
{
	m_pChangeEvent = pEvent;
}
//---------------------------------------------------------------------------
TRectChangeEvent __fastcall TSpriteEdit::GetChangeEvent()
{
	return m_pChangeEvent;
}
//---------------------------------------------------------------------------
enMouseIntersect __fastcall TSpriteEdit::CheckMouseRect(int iX, int iY, TSpriteRect* pRect)
{
	// check points first
	TRect Rect;
    GetPointRect(pRect->m_iX, pRect->m_iY, Rect, true);
    if ((iX <= Rect.Right) && (iX >= Rect.Left) && (iY <= Rect.Bottom) && (iY >= Rect.Top))
    	return enLeftTop;
    GetPointRect(pRect->m_iX + pRect->m_iWidth, pRect->m_iY, Rect, true);
    if ((iX <= Rect.Right) && (iX >= Rect.Left) && (iY <= Rect.Bottom) && (iY >= Rect.Top))
    	return enRightTop;
    GetPointRect(pRect->m_iX + pRect->m_iWidth, pRect->m_iY + pRect->m_iHeight, Rect, true);
    if ((iX <= Rect.Right) && (iX >= Rect.Left) && (iY <= Rect.Bottom) && (iY >= Rect.Top))
    	return enRightBottom;
    GetPointRect(pRect->m_iX, pRect->m_iY + pRect->m_iHeight, Rect, true);
    if ((iX <= Rect.Right) && (iX >= Rect.Left) && (iY <= Rect.Bottom) && (iY >= Rect.Top))
    	return enLeftBottom;

    // check borders
    if ((iX <= (pRect->m_iX + m_MouseLineSens)) &&
    	(iX >= (pRect->m_iX - m_MouseLineSens)) &&
        (iY >= (pRect->m_iY)) &&
        (iY <= (pRect->m_iY + pRect->m_iHeight)))
        return enLeft;

    if ((iX <= (pRect->m_iX + pRect->m_iWidth + m_MouseLineSens)) &&
    	(iX >= (pRect->m_iX + pRect->m_iWidth - m_MouseLineSens)) &&
        (iY >= (pRect->m_iY)) &&
        (iY <= (pRect->m_iY + pRect->m_iHeight)))
        return enRight;

    if ((iX <= (pRect->m_iX + pRect->m_iWidth)) &&
    	(iX >= (pRect->m_iX)) &&
        (iY <= (pRect->m_iY + m_MouseLineSens)) &&
        (iY >= (pRect->m_iY - m_MouseLineSens)))
        return enTop;

    if ((iX <= (pRect->m_iX + pRect->m_iWidth)) &&
    	(iX >= (pRect->m_iX)) &&
        (iY <= (pRect->m_iY + pRect->m_iHeight + m_MouseLineSens)) &&
        (iY >= (pRect->m_iY + pRect->m_iHeight - m_MouseLineSens)))
        return enBottom;
	return enNone;
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::TransformRect(TSpriteRect* pRect, int iX, int iY)
{
	int iXDelta = iX - m_iInterX;
    int iYDelta = iY - m_iInterY;
    m_iInterX = iX;
    m_iInterY = iY;

    switch(m_eIntersect)
    {
    case enLeft:
		pRect->m_iX += iXDelta;
        pRect->m_iWidth -= iXDelta;
    	break;
    case enRight:
  		pRect->m_iWidth += iXDelta;
    	break;
    case enTop:
		pRect->m_iY += iYDelta;
   		pRect->m_iHeight -= iYDelta;
    	break;
    case enBottom:
  		pRect->m_iHeight += iYDelta;
    	break;
    case enLeftTop:
        pRect->m_iX += iXDelta;
        pRect->m_iWidth -= iXDelta;
        pRect->m_iY += iYDelta;
   		pRect->m_iHeight -= iYDelta;
    	break;
    case enRightTop:
        pRect->m_iWidth += iXDelta;
    	pRect->m_iY += iYDelta;
   		pRect->m_iHeight -= iYDelta;
    	break;
    case enLeftBottom:
        pRect->m_iX += iXDelta;
        pRect->m_iWidth -= iXDelta;
        pRect->m_iHeight += iYDelta;
        break;
    case enRightBottom:
        pRect->m_iWidth += iXDelta;
  		pRect->m_iHeight += iYDelta;
    	break;
	}
	bool bInvX, bInvY;
    pRect->Invert(bInvX, bInvY);
    if (bInvX && bInvY)
    {
		switch(m_eIntersect)
        {
        case enLeftTop:
            m_eIntersect = enRightBottom;
            return;
        case enRightBottom:
            m_eIntersect = enLeftTop;
            return;
        case enLeftBottom:
            m_eIntersect = enRightTop;
            return;
        case enRightTop:
            m_eIntersect = enLeftBottom;
            return;
        }
    }
    if (bInvX)
    {
		switch(m_eIntersect)
        {
        case enLeft:
            m_eIntersect = enRight;
            return;
        case enRight:
            m_eIntersect = enLeft;
            return;
        case enLeftTop:
            m_eIntersect = enRightTop;
            return;
        case enRightBottom:
            m_eIntersect = enLeftBottom;
            return;
        case enLeftBottom:
            m_eIntersect = enRightBottom;
            return;
        case enRightTop:
            m_eIntersect = enLeftTop;
            return;
        }
    }
    if (bInvY)
    {
		switch(m_eIntersect)
        {
        case enTop:
            m_eIntersect = enBottom;
            return;
        case enBottom:
            m_eIntersect = enTop;
            return;
        case enLeftTop:
            m_eIntersect = enLeftBottom;
            return;
        case enRightBottom:
            m_eIntersect = enRightTop;
            return;
        case enLeftBottom:
            m_eIntersect = enLeftTop;
            return;
        case enRightTop:
            m_eIntersect = enRightBottom;
            return;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TSpriteEdit::MoveRect(TSpriteRect* pRect, int iX, int iY)
{
	int iXDelta = iX - m_iInterX;
    int iYDelta = iY - m_iInterY;
//   	iXDelta = iXDelta / m_iZoom;
    m_iInterX = iX;//iXDelta * m_iZoom;
//  	iYDelta = iYDelta / m_iZoom;
    m_iInterY = iY;//iYDelta * m_iZoom;
    pRect->m_iX += iXDelta;
    pRect->m_iY += iYDelta;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

