//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RegionEdit.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// Region point implementation
//---------------------------------------------------------------------------
__fastcall TRegionPoint::TRegionPoint(TRegion* pRegion)
{
    m_pRegion = pRegion;
  	m_iX      = 0;
    m_iY      = 0;
    m_pNext   = NULL;
}
__fastcall TRegionPoint::TRegionPoint(TRegion* pRegion, int X, int Y, TRegionPoint* pNext)
{
    m_pRegion = pRegion;
	m_iX 	  = X;
    m_iY 	  = Y;
    m_pNext   = pNext;
}
int __fastcall TRegionPoint::GetX()
{
	return m_iX;
}
void __fastcall TRegionPoint::SetX(int iX)
{
    m_iX = iX;
    m_pRegion->OnPointChanged(this);
}
int __fastcall TRegionPoint::GetY()
{
   	return m_iY;
}
void __fastcall TRegionPoint::SetY(int iY)
{
  	m_iY = iY;
    m_pRegion->OnPointChanged(this);
}
//---------------------------------------------------------------------------
// Region implementation
//---------------------------------------------------------------------------
__fastcall TRegion::TRegion():
			m_Pivot(this, 0, 0, NULL),
            m_Min(this, 0, 0, NULL),
            m_Max(this, 0, 0, NULL)
{
	m_pHead = NULL;
    m_pTail = NULL;
    m_pNext = NULL;
}
//---------------------------------------------------------------------------
__fastcall TRegion::~TRegion()
{
	TRegionPoint* pCurrent = m_pHead;
    while(pCurrent)
    {
     	TRegionPoint* pPrev = pCurrent;
        pCurrent = pCurrent->m_pNext;
        delete pPrev;
    }
}
//---------------------------------------------------------------------------
void __fastcall TRegion::AddPoint(int X, int Y)
{
	if (m_pHead == NULL)
    {
     	m_pHead = new TRegionPoint(this, X, Y, NULL);
        m_pTail = m_pHead;
        return;
    }
    m_pTail->m_pNext = new TRegionPoint(this, X, Y, NULL);
    m_pTail = m_pTail->m_pNext;
}
//---------------------------------------------------------------------------
void __fastcall TRegion::SetClosed(bool bClosed)
{
	m_bClosed = bClosed;
}
//---------------------------------------------------------------------------
void __fastcall TRegion::SetPivot(int X, int Y)
{
    m_Pivot.X = X;
    m_Pivot.Y = Y;
}
//---------------------------------------------------------------------------
void __fastcall TRegion::OnPointChanged(TRegionPoint* pPoint)
{
	// if its one of our bounds - ignore it
	if ((pPoint == &m_Min) || (pPoint == &m_Max))
    	return;
    RecalcBounds();
}
//---------------------------------------------------------------------------
void __fastcall TRegion::RecalcBounds()
{
	TRegionPoint* pPoint = m_pHead;
    m_Min.X = 10000000;
    m_Min.Y = 10000000;
    m_Max.X = -10000000;
	m_Max.Y = -10000000;
    while(pPoint)
    {
		if (pPoint->X < m_Min.X)
        	m_Min.X = pPoint->X;
        if (pPoint->Y < m_Min.Y)
        	m_Min.X = pPoint->X;
        if (pPoint->X > m_Max.X)
        	m_Max.X = pPoint->X;
        if (pPoint->X > m_Max.X)
        	m_Max.X = pPoint->X;
    	pPoint = pPoint->m_pNext;
    }
}
//---------------------------------------------------------------------------
// Control implementation
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TRegionEdit *)
{
	new TRegionEdit(NULL);
}
//---------------------------------------------------------------------------
__fastcall TRegionEdit::TRegionEdit(TComponent* Owner)
	:TCustomControl(Owner)
{
	RE_ShowGrid   = false;
    RE_GridWidth  = 100;
    RE_GridHeight = 100;
	RE_GridColor  = TColor(0xFF000000);
    OnMouseMove   = this->MouseMoves;
    m_pHead		  = NULL;
    m_pTail		  = NULL;
	m_pSelectedPoint = NULL;
    m_BackgroundDrawHandler = NULL;
}
//---------------------------------------------------------------------------
__fastcall TRegionEdit::~TRegionEdit()
{
   	TRegion* pCurrent = m_pHead;
    while(pCurrent)
    {
     	TRegion* pPrev = pCurrent;
        pCurrent = pCurrent->m_pNext;
        delete pPrev;
    }
}
//---------------------------------------------------------------------------
namespace Regionedit
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TRegionEdit)};
		RegisterComponents("Mayhem", classes, 0);
	}
}
//---------------------------------------------------------------------------
// show grid prop control
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::SetShowGrid(bool bShow)
{
	m_bShowGrid = bShow;
    Invalidate();
}
//---------------------------------------------------------------------------
bool __fastcall TRegionEdit::GetShowGrid()
{
 	return m_bShowGrid;
}
//---------------------------------------------------------------------------
// Grid width prop control
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::SetGridWidth(int iWidth)
{
    if (iWidth < 10)
    {
     	MessageBox(NULL, "Too small grid range", "Invalid arg", MB_OK);
    	return;
    }
   	m_iGridWidth = iWidth;
    Invalidate();
}
//---------------------------------------------------------------------------
int __fastcall TRegionEdit::GetGridWidth()
{
	return m_iGridWidth;
}
//---------------------------------------------------------------------------
// Grid height prop control
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::SetGridHeight(int iHeight)
{
    if (iHeight < 10)
    {
     	MessageBox(NULL, "Too small grid range", "Invalid arg", MB_OK);
    	return;
    }
	m_iGridHeight = iHeight;
    Invalidate();
}
//---------------------------------------------------------------------------
int __fastcall TRegionEdit::GetGridHeight()
{
	return m_iGridHeight;
}
//---------------------------------------------------------------------------
// Grid color prop control
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::SetGridColor(TColor Color)
{
	m_GridColor = Color;
    Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TRegionEdit::GetGridColor()
{
	return m_GridColor;
}
//---------------------------------------------------------------------------
// Backgropund drawing control
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::SetDrawHandler(TOnPaintEvent value)
{
	m_BackgroundDrawHandler = value;
    Invalidate();
}
//---------------------------------------------------------------------------
TOnPaintEvent __fastcall TRegionEdit::GetDrawHandler()
{
	return m_BackgroundDrawHandler;
}
//---------------------------------------------------------------------------
// Control drawing
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::Paint()
{
//	Canvas->Va
 	if (m_BackgroundDrawHandler)
    	(m_BackgroundDrawHandler)(this);
    if (m_bShowGrid)
    	DrawGrid();
	TRegion* pRegion = m_pHead;
    while(pRegion)
    {
     	DrawRegion(pRegion);
        pRegion = pRegion->m_pNext;
    }
}
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::MouseMoves(TObject* pSender, TShiftState Shift, int X, int Y)
{
	if ((Shift.Contains(ssLeft)) && (m_pSelectedPoint != NULL))
    {
		m_pSelectedPoint->X = X;
		m_pSelectedPoint->Y = Y;
		Invalidate();
        return;
    }
  	TRegion* pRegion = m_pHead;
    while(pRegion)
    {
     	CheckRegionMouse(pRegion, X, Y);
        pRegion = pRegion->m_pNext;
    }
}
//---------------------------------------------------------------------------
// Draws grid
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::DrawGrid()
{
	Canvas->Pen->Color = m_GridColor;
	Canvas->Pen->Style = psDot;

    int iYCount = Height / m_iGridHeight;
    int iXCount = Width  / m_iGridWidth;

	// draw vertical lines
    for (int y = 1; y <= iYCount; y++)
    {
		int iYPos = y * m_iGridHeight;
        Canvas->MoveTo(0, iYPos);
        Canvas->LineTo(Width, iYPos);
    }
	// draw horizontal lines
    for (int x = 1; x <= iXCount; x++)
    {
		int iXPos = x * m_iGridWidth;
        Canvas->MoveTo(iXPos, 0);
        Canvas->LineTo(iXPos, Height);
    }
}
//---------------------------------------------------------------------------
// Draws region
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::DrawRegion(TRegion* pRegion)
{
	TRegionPoint* pPoint = pRegion->m_pHead;
    if (pPoint == NULL)
    	return;

    Canvas->Pen->Width = 1;
    Canvas->Pen->Style = psSolid;
    Canvas->Pen->Color = TColor(0x00FF0000);
    Canvas->MoveTo(pPoint->X, pPoint->Y);
    TRegionPoint* pFirst = pPoint;
//    pPoint = pPoint->m_pNext;
    TRect Rect;
    do
    {
	    Rect.Top    = pPoint->Y - 3;
    	Rect.Bottom = pPoint->Y + 3;
	    Rect.Left   = pPoint->X - 3;
	    Rect.Right  = pPoint->X + 3;

    	Canvas->Brush->Color = TColor(0x0000FF00);
        Canvas->Brush->Style = bsSolid;
	    Canvas->FillRect(Rect);
        if (pPoint != pFirst)
        	Canvas->LineTo(pPoint->X, pPoint->Y);
        pPoint = pPoint->m_pNext;
    }while(pPoint);
    if (pRegion->m_bClosed && (pPoint != pFirst))
    	Canvas->LineTo(pFirst->X, pFirst->Y);



}
//---------------------------------------------------------------------------
// Adds new region
//---------------------------------------------------------------------------
TRegion* __fastcall TRegionEdit::CreateRegion()
{
	if (m_pHead == NULL)
    {
     	m_pHead = new TRegion();
        m_pTail = m_pHead;
        return m_pTail;
    }
    m_pTail->m_pNext = new TRegion();
    m_pTail = m_pTail->m_pNext;
    return m_pTail;
}
//---------------------------------------------------------------------------
// Frees region
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::DeleteRegion(TRegion* pRegion)
{
   	if (pRegion == NULL)
    	return;
    TRegion* pCurrent, *pPrev;
    pCurrent = pPrev = m_pHead;
    while(pCurrent != pRegion)
    {
		pPrev = pCurrent;
        pCurrent = pCurrent->m_pNext;
    }
    pPrev->m_pNext = pCurrent->m_pNext;
    if (pCurrent == m_pTail)
    	m_pTail = pPrev;
    delete pCurrent;
}
//---------------------------------------------------------------------------
// Checks mouse within region
//---------------------------------------------------------------------------
void __fastcall TRegionEdit::CheckRegionMouse(TRegion* pRegion, int X, int Y)
{
	TRegionPoint* pPoint = pRegion->m_pHead;
	TRect Rect;
   	while(pPoint)
    {
    	if ((X < (pPoint->X + 3)) && (X > (pPoint->X - 3)) &&
        	(Y < (pPoint->Y + 3)) && (Y > (pPoint->Y - 3)))
        {
            if (pPoint == m_pSelectedPoint)
            	return;
            if (m_pSelectedPoint != NULL)
            {
            	Rect.Top    = m_pSelectedPoint->Y - 3;
		    	Rect.Bottom = m_pSelectedPoint->Y + 3;
			    Rect.Left   = m_pSelectedPoint->X - 3;
	    		Rect.Right  = m_pSelectedPoint->X + 3;
	            Canvas->Brush->Color = TColor(0x0000FF00);
		        Canvas->Brush->Style = bsSolid;
			    Canvas->FillRect(Rect);
            }
        	Rect.Top    = pPoint->Y - 3;
	    	Rect.Bottom = pPoint->Y + 3;
		    Rect.Left   = pPoint->X - 3;
	    	Rect.Right  = pPoint->X + 3;
            Canvas->Brush->Color = TColor(0x000000FF);
	        Canvas->Brush->Style = bsSolid;
		    Canvas->FillRect(Rect);
			m_pSelectedPoint = pPoint;
            Cursor = crSizeAll;
            return;
        }
		pPoint = pPoint->m_pNext;
    }
    if (m_pSelectedPoint != NULL)
    {
      	Rect.Top    = m_pSelectedPoint->Y - 3;
	   	Rect.Bottom = m_pSelectedPoint->Y + 3;
	    Rect.Left   = m_pSelectedPoint->X - 3;
		Rect.Right  = m_pSelectedPoint->X + 3;
	    Canvas->Brush->Color = TColor(0x0000FF00);
	    Canvas->Brush->Style = bsSolid;
		Canvas->FillRect(Rect);
		m_pSelectedPoint = NULL;
        Cursor = crArrow;
    }
}



