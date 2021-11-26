//---------------------------------------------------------------------------
// TColorGradient component
// (c) Mayhem Projex 2007
// Coded: Max Vasin
//---------------------------------------------------------------------------

// Include needed files
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "ColorGradient.h"
#pragma package(smart_init)

// C-Builder needed stuff
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TColorGradient *)
{
	new TColorGradient(NULL);
}
namespace Colorgradient
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TColorGradient)};
		RegisterComponents("Mayhem projex", classes, 0);
	}
}

//###########################################################################
// TColorGradient key implementation
//###########################################################################

// Key constructor
//---------------------------------------------------------------------------
__fastcall TColorGradientKey::TColorGradientKey(TKeyChangeEvent OnChange)
{
  	m_fPosition = 0.0f;
	m_Color 	= TColor(0xFFFFFFFF);
    m_pNextKey  = NULL;
    m_OnChange	= OnChange;
}

// Color property setget
//---------------------------------------------------------------------------
TColor __fastcall TColorGradientKey::GetColor()
{
 	return m_Color;
}
//---------------------------------------------------------------------------
void __fastcall TColorGradientKey::SetColor(TColor Color)
{
 	m_Color = Color;
    if (m_OnChange)
    	(m_OnChange)(this);
}

// Position property setget
//---------------------------------------------------------------------------
float __fastcall TColorGradientKey::GetPosition()
{
 	return m_fPosition;
}
//---------------------------------------------------------------------------
void __fastcall TColorGradientKey::SetPosition(float Position)
{
	m_fPosition = Position;
    if (m_OnChange)
    	(m_OnChange)(this);
}

//###########################################################################
// TColorGradient component implementation
//###########################################################################

// Component constructor
//---------------------------------------------------------------------------
__fastcall TColorGradient::TColorGradient(TComponent* Owner):
                           TGraphicControl(Owner)
{
	m_pHead				= NULL;
	m_pTail 			= NULL;
    m_nKeyCount			= 0;
    m_pBitmap			= new Graphics::TBitmap();
    OnMouseMove 		= MouseMoves;
	OnMouseDown 		= MouseButtonDown;
  	OnMouseUp	 		= MouseButtonUp;
	OnResize			= DoResize;
	OnDblClick			= MouseDblClk;
    m_Mouse[0] 	 		= 0;
    m_Mouse[1] 	 		= 0;
    m_fMinRange	 		= 0.0f;
	m_fMaxRange	 		= 2.0f;
    m_RangeFont			= new TFont();
    m_iRangeTextHeight	= 0;
    m_iRangeMargin		= 0;
    m_pSelectedKey		= NULL;
    m_pColorDialog		= new TColorDialog(this);
    m_pColorDialog->Color = clWhite;
    m_bDblClk			= false;
    RecalcGraphics();
    Paint();
}

// Component destructor
//---------------------------------------------------------------------------
__fastcall TColorGradient::~TColorGradient()
{
	ClearKeys();
}

//###########################################################################
// TColor gradient component key control
//###########################################################################

// Adds new key to gradient list
//---------------------------------------------------------------------------
TColorGradientKey* __fastcall  TColorGradient::AddKey(float Position,
													  TColor Color)
{
	TColorGradientKey* pKey = new TColorGradientKey(&OnKeyChange);
    pKey->m_fPosition = Position;
    pKey->m_Color     = Color;
    AddKey(pKey);
	return pKey;
}

// Returns key count number
//---------------------------------------------------------------------------
int __fastcall  TColorGradient::GetKeyCount()
{
	return m_nKeyCount;
}

// Returns key by index
//---------------------------------------------------------------------------
TColorGradientKey* __fastcall TColorGradient::GetKey(int Index)
{
 	if (Index >= m_nKeyCount)
    	return NULL;
    TColorGradientKey* pCursor   = m_pHead;
	int nCounter = 0;
	while(pCursor != NULL)
    {
		if (nCounter == Index)
        	return pCursor;
     	pCursor = pCursor->m_pNextKey;
   		nCounter++;
    }
    return NULL;
}

//###########################################################################
// TColor gradient component internal
//###########################################################################

// Adds new key object to list
//---------------------------------------------------------------------------
void __fastcall TColorGradient::AddKey(TColorGradientKey* pKey)
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
void __fastcall TColorGradient::RemoveKey(TColorGradientKey* pKey)
{
	TColorGradientKey* pCursor   = m_pHead;
	TColorGradientKey* pPrevious = NULL;
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
void __fastcall TColorGradient::ClearKeys()
{
	TColorGradientKey* pCursor   = m_pHead;
	TColorGradientKey* pPrevious = pCursor;
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
    RecalcGraphics();
}

// Sorting keys
//---------------------------------------------------------------------------
void __fastcall TColorGradient::SortKeys()
{
    bool bSwap = true;
    while(bSwap)
    {
		bSwap = false;
        TColorGradientKey* pCursor   = m_pHead;
     	TColorGradientKey* pPrePrevious = NULL;
      	TColorGradientKey* pPrevious = NULL;
    	while(pCursor != NULL)
	    {
			pPrePrevious    = pPrevious;
            pPrevious 		= pCursor;
        	pCursor   		= pCursor->m_pNextKey;
            if (pCursor && pPrevious)
            {
             	if (pCursor->m_fPosition < pPrevious->m_fPosition)
                {
                	TColorGradientKey* pBuffer = pCursor->m_pNextKey;
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

// Returns gradient color to point
//---------------------------------------------------------------------------
TColor __fastcall TColorGradient::GetGradient(int x)
{
	float fPos = ((float)x / (float)Width) * (m_fMaxRange - m_fMinRange) + m_fMinRange;
    TColorGradientKey* pCursor   = m_pHead;
   	TColorGradientKey* pPrevious = NULL;
   	while(pCursor != NULL)
    {
		pPrevious = pCursor;
        pCursor = pCursor->m_pNextKey;
        // only one key or last key
        if (pCursor == NULL)
        	return pPrevious->m_Color;
		// exact position
        if (fPos == pPrevious->m_fPosition)
        	return pPrevious->m_Color;
        if (fPos == pCursor->m_fPosition)
			return pCursor->m_Color;

        // border keys
        if ((fPos < pPrevious->m_fPosition) && (pPrevious == m_pHead))
        	return pPrevious->m_Color;

        // interpolate
        if ((fPos > pPrevious->m_fPosition) &&
        	(fPos < pCursor->m_fPosition))
        {
			float fRange  = pCursor->m_fPosition - pPrevious->m_fPosition;
            float fRelPos = fPos - pPrevious->m_fPosition;
            float fK = fRelPos / fRange;
            unsigned char r  = GET_R(pPrevious->m_Color);
            unsigned char g  = GET_G(pPrevious->m_Color);
            unsigned char b  = GET_B(pPrevious->m_Color);
			unsigned char r1 = GET_R(pCursor->m_Color);
            unsigned char g1 = GET_G(pCursor->m_Color);
            unsigned char b1 = GET_B(pCursor->m_Color);

            float fRDiff = (float)r1 - (float)r;
            float fGDiff = (float)g1 - (float)g;
            float fBDiff = (float)b1 - (float)b;


			return TColor( MAKE_RGB( (unsigned char)((float)r + fRDiff * fK),
                                     (unsigned char)((float)g + fGDiff * fK),
                                     (unsigned char)((float)b + fBDiff * fK) ) );
        }
    }
    return clWhite;
}


// Handles each key change
//---------------------------------------------------------------------------
void __fastcall TColorGradient::OnKeyChange(TColorGradientKey* pKey)
{
 	SortKeys();
    RecalcGraphics();
}

//###########################################################################
// TColor gradient component painting
//###########################################################################
void __fastcall TColorGradient::Paint()
{
	int iWidth  = Width;
    int iHeight = Height;
    m_pBitmap->Width = iWidth;
    m_pBitmap->Height = iHeight;
	Canvas->CopyMode = cmSrcCopy;
    TCanvas* pCanvas = m_pBitmap->Canvas;
	for (int x = 0; x < iWidth; x++)
    	for (int y = 0; y < iHeight - m_iRangeBarHeight; y++)
        {
			pCanvas->Pixels[x][y] = GetGradient(x);
        }
	for (int x = 0; x < iWidth; x++)
    	for (int y = iHeight - m_iRangeBarHeight; y < iHeight; y++)
        {
			pCanvas->Pixels[x][y] = m_RangeColor;
        }

	// draw range
    pCanvas->Brush->Style = bsClear;
    String sText;
    sText = sText.FloatToStrF(m_fMinRange, AnsiString::sffFixed, 4, 2);
	int iTextHeight = pCanvas->TextHeight(sText);
    int iTextWidth  = pCanvas->TextWidth(sText);

    pCanvas->TextOutA(2, iHeight - iTextHeight, sText);

    sText = sText.FloatToStrF(m_fMaxRange, AnsiString::sffFixed, 4, 2);
    iTextHeight = pCanvas->TextHeight(sText);
    iTextWidth  = pCanvas->TextWidth(sText);

    pCanvas->TextOutA(iWidth - iTextWidth - 2, iHeight - iTextHeight, sText);


    // draw keys

    pCanvas->Brush->Style 		 = bsSolid;
    TColorGradientKey* pCursor   = m_pHead;
    TPoint Points[6];
    while(pCursor != NULL)
    {
	    pCanvas->Brush->Color = pCursor->Color;
        if (m_pSelectedKey == pCursor)
        	pCanvas->Pen->Color = clWhite;
        else
			pCanvas->Pen->Color = clBlack;
   	  	Points[0].x = pCursor->m_iImagePos - 4; Points[0].y = iHeight - 2;
		Points[1].x = pCursor->m_iImagePos + 4; Points[1].y = iHeight - 2;
   		Points[2].x = pCursor->m_iImagePos + 4; Points[2].y = iHeight - m_iRangeBarHeight;
   		Points[3].x = pCursor->m_iImagePos;  	Points[3].y = iHeight - m_iRangeBarHeight - 4;
   		Points[4].x = pCursor->m_iImagePos - 4; Points[4].y = iHeight - m_iRangeBarHeight;
		pCanvas->Polygon(Points, 4);

		pCursor   = pCursor->m_pNextKey;
    }

//    pCanvas->TextOutA(0, 0, String(m_Mouse[0]) + ":" + String(m_Mouse[1]));

	// blit
    Canvas->Draw(0, 0, m_pBitmap);
}


//###########################################################################
// TColorGradient component mouse tracking
//###########################################################################

// Handles mouse move
//---------------------------------------------------------------------------
void __fastcall TColorGradient::MouseMoves(TObject* pSender,
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
//  int iDeltaClipY = m_ClipMouse[1] - ClipMousePrev[1];

    m_Mouse[0] = X;
    m_Mouse[1] = Y;

    if (m_pSelectedKey != NULL)
    {
	    float fXDiff = (float)iDeltaClipX / Width;
	    float fRange = m_fMaxRange - m_fMinRange;
     	m_pSelectedKey->m_fPosition += fXDiff * fRange;
		SortKeys();
        RecalcGraphics();
    }
	Paint();
}

// Handles mouse button down
//---------------------------------------------------------------------------
void __fastcall TColorGradient::MouseButtonDown(TObject* pSender,
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
	m_pSelectedKey = NULL;

    TColorGradientKey* pCursor   = m_pHead;
    while(pCursor != NULL)
    {
		if ((X <= (pCursor->m_iImagePos + 4)) &&
        	(X >= (pCursor->m_iImagePos - 4)) &&
			(Y <= (Height - 2)) &&
            (Y >= (Height - m_iRangeBarHeight - 4)))
        {
            m_pSelectedKey  = pCursor;
            m_ClipMouse[0]  = pCursor->m_iImagePos;
		   	m_ClipMouse[1]  = Y;
        }
		pCursor = pCursor->m_pNextKey;
    }

    if (Button == mbLeft)
    {
    	if (m_pSelectedKey == NULL)
    	{
	    	float fPos = (float)X / Width;
			float fRange = m_fMaxRange - m_fMinRange;
    		AddKey(m_fMinRange + (fPos * fRange), m_pColorDialog->Color);
    	}
    }
    else
    {
        if (m_pSelectedKey != NULL)
		{
     		RemoveKey(m_pSelectedKey);
	        m_pSelectedKey = NULL;
        }
    }
	m_Mouse[0] = X;
    m_Mouse[1] = Y;

    Paint();
    
}

// Handles mouse button up
//---------------------------------------------------------------------------
void __fastcall TColorGradient::MouseButtonUp(TObject* pSender,
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
void __fastcall TColorGradient::MouseDblClk(TObject* pSender)
{
	TColorGradientKey* pCursor   = m_pHead;
    while(pCursor != NULL)
    {
		if ((m_Mouse[0] <= (pCursor->m_iImagePos + 4)) &&
        	(m_Mouse[0] >= (pCursor->m_iImagePos - 4)) &&
			(m_Mouse[1] <= (Height - 2)) &&
            (m_Mouse[1] >= (Height - m_iRangeBarHeight - 4)))
           break;
		pCursor = pCursor->m_pNextKey;
    }
	if (pCursor)
    {
     	m_pColorDialog->Color = pCursor->m_Color;
        if (m_pColorDialog->Execute())
        	pCursor->m_Color = m_pColorDialog->Color;
    }
    m_pSelectedKey = NULL;
    m_bDblClk = true;
    Paint();
}



// Catches control size to repaint
//---------------------------------------------------------------------------
void __fastcall TColorGradient::DoResize(TObject* Sender)
{
	RecalcGraphics();
	Paint();
}

// Recalculates graphics objects sizes, dimensions, etc
//---------------------------------------------------------------------------
void __fastcall TColorGradient::RecalcGraphics()
{
	String sFormatText = "0123456789.e+-";
    m_pBitmap->Canvas->Font = m_RangeFont;
    m_iRangeTextHeight = m_pBitmap->Canvas->TextHeight(sFormatText);
    m_iRangeBarHeight  = m_iRangeTextHeight + m_iRangeMargin;

	float fRange  = m_fMaxRange - m_fMinRange;
	TColorGradientKey* pCursor   = m_pHead;
    while(pCursor != NULL)
    {
		pCursor->m_iImagePos = (int)((pCursor->m_fPosition / fRange)* (float)Width);
		pCursor = pCursor->m_pNextKey;
    }

}

//###########################################################################
// TColorGradient properties control
//###########################################################################

// Range font property control
//---------------------------------------------------------------------------
void __fastcall TColorGradient::SetRangeFont(TFont* Font)
{
 	m_RangeFont->Assign(Font);
    RecalcGraphics();
	Paint();
}

// Range margin property control
//---------------------------------------------------------------------------
void __fastcall TColorGradient::SetRangeMargin(int iMargin)
{
 	m_iRangeMargin = iMargin;
    RecalcGraphics();
	Paint();
}

// Range bounds properties control
//---------------------------------------------------------------------------
void __fastcall TColorGradient::SetMinRange(float fValue)
{
 	m_fMinRange = fValue;
    RecalcGraphics();
	Paint();
}
//---------------------------------------------------------------------------
void __fastcall TColorGradient::SetMaxRange(float fValue)
{
 	m_fMaxRange = fValue;
    RecalcGraphics();
	Paint();
}
//---------------------------------------------------------------------------
void __fastcall TColorGradient::SetRangeColor(TColor Color)
{
 	m_RangeColor = Color;
	Paint();
}











