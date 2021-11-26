//---------------------------------------------------------------------------
// TSplineEditor component
// (c) Mayhem Projex 2007
// Coded: Max Vasin
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SplineEditor.h"
#pragma package(smart_init)

// C-Builder stuff
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TSplineEditor *)
{
	new TSplineEditor(NULL);
}
//---------------------------------------------------------------------------
namespace Splineeditor
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSplineEditor)};
		RegisterComponents("Mayhem projex", classes, 0);
	}
}

//###########################################################################
// Spline key implementation
//###########################################################################

// Spline key constructor
//---------------------------------------------------------------------------
__fastcall TSplineKey::TSplineKey(TSplineKeyChangeEvent OnChange)
{
    m_fPosition	= 0.0f;
    m_fValue	= 0.0f;
    m_pNextKey	= NULL;
    m_OnChange	= OnChange;
}

// Spline key properties control
//---------------------------------------------------------------------------
void __fastcall TSplineKey::SetValue(float fValue)
{
	m_fValue = fValue;
    if (m_OnChange)
    	(m_OnChange)(this);
}
//---------------------------------------------------------------------------
void __fastcall TSplineKey::SetPosition(float fPosition)
{
	m_fPosition = fPosition;
    if (m_OnChange)
    	(m_OnChange)(this);
}


//###########################################################################
// Spline editor component implementation
//###########################################################################

// Constructor
//---------------------------------------------------------------------------
__fastcall TSplineEditor::TSplineEditor(TComponent* Owner):TGraphicControl(Owner)
{
	m_pHead			= NULL;
	m_pTail			= NULL;
	m_nKeyCount		= 0;
	m_pBitmap		= new Graphics::TBitmap;
	m_Mouse[0]		= 0;
	m_Mouse[1]		= 0;
	m_ClipMouse[0]	= 0;
   	m_ClipMouse[1]	= 0;
	m_bDblClk		= false;
    m_pSelectedKey	= NULL;

    OnMouseMove		= MouseMoves;
	OnMouseDown		= MouseButtonDown;
  	OnMouseUp  		= MouseButtonUp;
	OnResize   		= DoResize;
	OnDblClick 		= MouseDblClk;

    m_PointColor	= clYellow;
    m_iPointSize	= 4;
    m_LineColor		= TColor(0x0000FF00);
    m_iLineWidth	= 2;
    m_BackColor		= clGray;
    m_GridColor		= clBlack;

	m_fMinRange		= 0.0f;
	m_fMaxRange		= 1.0f;
	m_fMinValue		= 0.0f;
	m_fMaxValue		= 1.0f;

    RecalcGraphics();
}

// Destructor
//---------------------------------------------------------------------------
__fastcall TSplineEditor::~TSplineEditor()
{
	ClearKeys();
}


//###########################################################################
// Spline editor keys operations
//###########################################################################

// Adds new key
//---------------------------------------------------------------------------
TSplineKey* __fastcall TSplineEditor::AddKey(float fPosition, float fValue)
{
	TSplineKey* pKey = new TSplineKey(&OnKeyChange);
    pKey->m_fPosition = fPosition;
    pKey->m_fValue    = fValue;
    if (pKey->m_fPosition < m_fMinRange)
		pKey->m_fPosition = m_fMinRange;
    if (pKey->m_fPosition > m_fMaxRange)
		pKey->m_fPosition = m_fMaxRange;
    if (pKey->m_fValue < m_fMinValue)
		pKey->m_fValue = m_fMinValue;
    if (pKey->m_fValue > m_fMaxValue)
		pKey->m_fValue = m_fMaxValue;
    AddKey(pKey);
	return pKey;
}


// Adds new key object to list
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::AddKey(TSplineKey* pKey)
{
	if (m_pTail == NULL)
    {
     	m_pHead = pKey;
        m_pTail = m_pHead;
        m_nKeyCount++;
        RecalcGraphics();
        return;
    }

    m_pTail->m_pNextKey = pKey;
    m_pTail = pKey;
    m_nKeyCount++;
	SortKeys();
    RecalcGraphics();
}

// Removes existing key object from list
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::RemoveKey(TSplineKey* pKey)
{
	TSplineKey* pCursor   = m_pHead;
	TSplineKey* pPrevious = NULL;
    while(pCursor != NULL)
    {
		if (pCursor == pKey)
        {
         	if (pCursor == m_pHead)
            	m_pHead = m_pHead->m_pNextKey;
            if (pCursor == m_pTail)
            	m_pTail = pPrevious;
            if (pPrevious)
	        	pPrevious->m_pNextKey = pCursor->m_pNextKey;
			delete pCursor;
            m_nKeyCount--;
            break;
        }
        pPrevious 	= pCursor;
        pCursor 	= pCursor->m_pNextKey;
    }
    RecalcGraphics();
}

// Clears all keys (destroy list)
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::ClearKeys()
{
	TSplineKey* pCursor   = m_pHead;
	TSplineKey* pPrevious = pCursor;
    while(pCursor != NULL)
    {
        pPrevious 	= pCursor;
        pCursor 	= pCursor->m_pNextKey;
        delete pPrevious;
        pPrevious   = NULL;
    }
    if (pPrevious != NULL)
    	delete pPrevious;
    m_nKeyCount = 0;
    m_pHead = NULL;
    m_pTail = NULL;
    RecalcGraphics();
}

// Sorting keys
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::SortKeys()
{
    bool bSwap = true;
    while(bSwap)
    {
		bSwap = false;
        TSplineKey* pCursor   		= m_pHead;
     	TSplineKey* pPrePrevious 	= NULL;
      	TSplineKey* pPrevious 		= NULL;
    	while(pCursor != NULL)
	    {
			pPrePrevious    = pPrevious;
            pPrevious 		= pCursor;
        	pCursor   		= pCursor->m_pNextKey;
            if (pCursor && pPrevious)
            {
             	if (pCursor->m_fPosition < pPrevious->m_fPosition)
                {
                	TSplineKey* pBuffer = pCursor->m_pNextKey;
                    if (pPrePrevious)
                    	pPrePrevious->m_pNextKey = pCursor;
                    pCursor->m_pNextKey   = pPrevious;
                    pPrevious->m_pNextKey = pBuffer;
					if (pPrevious == m_pHead)
                    	m_pHead = pCursor;
                    if (pCursor == m_pTail)
                    	m_pTail = pPrevious;
                    bSwap = true;
                }
            }
	    }
    }
}

//###########################################################################
// Spline editor graphics support methods
//###########################################################################

// Recalculates spline nodes graphics data
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::RecalcGraphics()
{
	float fRange  = m_fMaxRange - m_fMinRange;
    float fValue  = m_fMaxValue - m_fMinValue;
	TSplineKey* pCursor = m_pHead;
    while(pCursor != NULL)
    {
        pCursor->m_iX = (int)((pCursor->m_fPosition / fRange) * (float)Width);
		pCursor->m_iY = (int)((pCursor->m_fValue / fValue) * (float)Height);
		pCursor = pCursor->m_pNextKey;
    }
}

// Callback all keys changing
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::OnKeyChange(TSplineKey* pKey)
{
	RecalcGraphics();
    Paint();
}

//###########################################################################
// TSplineEditor component mouse tracking
//###########################################################################

// Handles mouse move
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::MouseMoves(TObject* pSender,
                         	   TShiftState Shift,
                           	   int X,
                           	   int Y)
{
   int iDeltaMouseX = X - m_Mouse[0];
    int iDeltaMouseY = Y - m_Mouse[1];

    // clips delta
    if ((m_Mouse[0] > 0) && (X < 0))
    	iDeltaMouseX = -m_Mouse[0];
    if ((m_Mouse[0] <= Width - 1) && (X > Width - 1))
    	iDeltaMouseX = Width - 1 - m_Mouse[0];
    if ((m_Mouse[1] > 0) && (Y < 0))
    	iDeltaMouseY = -m_Mouse[1];
    if ((m_Mouse[1] <= Height - 1) && (Y > Height - 1))
    	iDeltaMouseY = Height - 1 - m_Mouse[1];
    if ((m_Mouse[0] < 0) && (X > 0))
		iDeltaMouseX = X;
    if ((m_Mouse[0] > Width - 1) && (X <= Width - 1))
		iDeltaMouseX = Width - 1 - X;
    if ((m_Mouse[1] < 0) && (Y > 0))
		iDeltaMouseY = Y;
    if ((m_Mouse[1] > Height - 1) && (Y <= Height - 1))
		iDeltaMouseY = Height - 1 - Y;

    if (((m_Mouse[0] < 0) && (X < 0)) ||
		((m_Mouse[0] > Width - 1) && (X > Width - 1)))
    	iDeltaMouseX = 0;
    if (((m_Mouse[1] < 0) && (Y < 0)) ||
		((m_Mouse[1] > Height - 1) && (Y > Height - 1)))
    	iDeltaMouseY = 0;

    int ClipMousePrev[2];
	ClipMousePrev[0] = m_ClipMouse[0];
    ClipMousePrev[1] = m_ClipMouse[1];

    m_ClipMouse[0] += iDeltaMouseX;
    m_ClipMouse[1] += iDeltaMouseY;
    
    if (m_ClipMouse[0] < 0)
		m_ClipMouse[0] = 0;
    if (m_ClipMouse[0] > Width - 1)
		m_ClipMouse[0] = Width - 1;
    if (m_ClipMouse[1] < 0)
		m_ClipMouse[1] = 0;
    if (m_ClipMouse[1] > Height - 1)
		m_ClipMouse[1] = Height - 1;

    int iDeltaClipX = m_ClipMouse[0] - ClipMousePrev[0];
    int iDeltaClipY = m_ClipMouse[1] - ClipMousePrev[1];

    m_Mouse[0] = X;
    m_Mouse[1] = Y;

    if (m_pSelectedKey != NULL)
    {
	    float fXDiff = (float)iDeltaClipX / Width;
        float fYDiff = (float)iDeltaClipY / Height;
	    float fRange = m_fMaxRange - m_fMinRange;
   	    float fValue = m_fMaxRange - m_fMinRange;
     	m_pSelectedKey->m_fPosition += fXDiff * fRange;
        m_pSelectedKey->m_fValue    += fYDiff * fValue;
		SortKeys();
        RecalcGraphics();
       	Paint();
    }
}

// Handles mouse button down
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::MouseButtonDown(TObject* pSender,
  												TMouseButton Button,
				                                TShiftState Shift,
                				                int X,
                                				int Y)
{
	if (m_bDblClk)
	{
        m_bDblClk = false;
    	return;
    }

	m_Mouse[0] = X;
    m_Mouse[1] = Y;


	m_pSelectedKey = NULL;
	TSplineKey* pCursor = m_pHead;
    while(pCursor != NULL)
    {
		if ((X <= (pCursor->m_iX + m_iPointSize)) &&
        	(X >= (pCursor->m_iX - m_iPointSize)) &&
			(Y <= (pCursor->m_iY + m_iPointSize)) &&
            (Y >= (pCursor->m_iY - m_iPointSize)))
        {
            m_pSelectedKey  = pCursor;
            m_ClipMouse[0]  = pCursor->m_iX;
		   	m_ClipMouse[1]  = pCursor->m_iY;
        }
		pCursor = pCursor->m_pNextKey;
    }

    if ((Button == mbLeft) && (!m_pSelectedKey))
    {
    	float fRange = m_fMaxRange - m_fMinRange;
	    float fValue = m_fMaxValue - m_fMinValue;
   		float fXRel  = (float)X / (float)Width;
	    float fYRel  = (float)Y / (float)Height;
        m_pSelectedKey = AddKey(m_fMinRange + fXRel * fRange,
                                m_fMinValue + fYRel * fValue);
    	m_ClipMouse[0] = X;
       	m_ClipMouse[1] = Y;
	}
    if ((Button == mbRight) && (m_pSelectedKey))
	{
		RemoveKey(m_pSelectedKey);
        m_pSelectedKey = NULL;
    }
    Paint();
}

// Handles mouse button up
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::MouseButtonUp(TObject* pSender,
  											  TMouseButton Button,
				                              TShiftState Shift,
                				              int X,
                                			  int Y)
{
	m_Mouse[0] = X;
    m_Mouse[1] = Y;
    m_pSelectedKey = NULL;
    Paint();
}

// Handles mouse double click
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::MouseDblClk(TObject* pSender)
{
    m_bDblClk = true;
    Paint();
}

// Catches control size to repaint
//---------------------------------------------------------------------------
void __fastcall TSplineEditor::DoResize(TObject* Sender)
{
	RecalcGraphics();
	Paint();
}

//###########################################################################
// Spline editor painting
//###########################################################################
void __fastcall TSplineEditor::Paint()
{
	int iWidth  		= Width;
    int iHeight 		= Height;
    m_pBitmap->Width 	= iWidth;
    m_pBitmap->Height 	= iHeight;
    TCanvas* pCanvas 	= m_pBitmap->Canvas;

	// erase background with back color
	pCanvas->Brush->Color = m_BackColor;
    pCanvas->FillRect(TRect(0, 0, iWidth, iHeight));

    // draw grid

    // draw lines lines
    pCanvas->Brush->Style = bsSolid;
    pCanvas->Brush->Color = m_PointColor;
    pCanvas->Pen->Width   = m_iLineWidth;
    pCanvas->Pen->Color   = m_LineColor;

    // draw fixed line and point
    if (m_nKeyCount == 1)
    {
		pCanvas->MoveTo(0, m_pHead->m_iY);
        pCanvas->LineTo(Width, m_pHead->m_iY);
        pCanvas->FillRect(TRect(m_pHead->m_iX - m_iPointSize,
          						m_pHead->m_iY - m_iPointSize,
                                m_pHead->m_iX + m_iPointSize,
								m_pHead->m_iY + m_iPointSize));

    }

    if (m_nKeyCount >= 2)
    {
       	// draw lines first

        // left cut line
        pCanvas->MoveTo(m_pHead->m_iX, m_pHead->m_iY);
        pCanvas->LineTo(0, m_pHead->m_iY);

        // right cut line
        pCanvas->MoveTo(m_pTail->m_iX, m_pTail->m_iY);
        pCanvas->LineTo(Width, m_pTail->m_iY);


        // spline lines
        pCanvas->MoveTo(m_pHead->m_iX, m_pHead->m_iY);
    	TSplineKey* pCursor = m_pHead->m_pNextKey;
	    while(pCursor != NULL)
	    {
            pCanvas->LineTo(pCursor->m_iX, pCursor->m_iY);
			pCursor   = pCursor->m_pNextKey;
        }

        // spline points
        pCursor = m_pHead;
	    while(pCursor != NULL)
	    {
        	pCanvas->FillRect(TRect(pCursor->m_iX - m_iPointSize,
          							pCursor->m_iY - m_iPointSize,
                	                pCursor->m_iX + m_iPointSize,
									pCursor->m_iY + m_iPointSize));
			pCursor   = pCursor->m_pNextKey;
        }
    }

	// blit
    Canvas->Draw(0, 0, m_pBitmap);
}


//###########################################################################
// Spline editor properties control
//###########################################################################
void __fastcall TSplineEditor::SetPointColor(TColor Color)
{
	m_PointColor = Color;
    Paint();
}
void __fastcall TSplineEditor::SetLineColor(TColor Color)
{
	m_LineColor = Color;
    Paint();
}
void __fastcall TSplineEditor::SetBackColor(TColor Color)
{
	m_BackColor = Color;
    Paint();
}
void __fastcall TSplineEditor::SetGridColor(TColor Color)
{
	m_GridColor = Color;
    Paint();
}
void __fastcall TSplineEditor::SetPointSize(int iSize)
{
	m_iPointSize = iSize;
    Paint();
}
void __fastcall TSplineEditor::SetLineWidth(int iWidth)
{
	m_iLineWidth = iWidth;
    Paint();
}
void __fastcall TSplineEditor::SetMinRange(float fValue)
{
	m_fMinRange = fValue;
    RecalcGraphics();
    Paint();
}
void __fastcall TSplineEditor::SetMaxRange(float fValue)
{
	m_fMaxRange = fValue;
    RecalcGraphics();
    Paint();
}
void __fastcall TSplineEditor::SetMinValue(float fValue)
{
	m_fMinValue = fValue;
    RecalcGraphics();
    Paint();
}
void __fastcall TSplineEditor::SetMaxValue(float fValue)
{
	m_fMaxValue = fValue;
    RecalcGraphics();
    Paint();
}







