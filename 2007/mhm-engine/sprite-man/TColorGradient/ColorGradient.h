//---------------------------------------------------------------------------
// TColorGradient component
// (c) Mayhem Projex 2007
// Coded: Max Vasin
//---------------------------------------------------------------------------
#ifndef ColorGradientH
#define ColorGradientH

#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>

// Macros
//--------------------------------------------------
#define GET_R(C) (C) & 0x000000FF
#define GET_G(C) ((C) & 0x0000FF00) >> 8
#define GET_B(C) ((C) & 0x00FF0000) >> 16
#define MAKE_RGB(r, g, b) ( (((b) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((r) & 0xFF) )


// Color key structure
//--------------------------------------------------
class TColorGradientKey;
typedef void __fastcall (__closure *TKeyChangeEvent)(TColorGradientKey* Sender);
class TColorGradientKey
{
friend class TColorGradient;
private:
	float				m_fPosition;
    TColor  			m_Color;
    TColorGradientKey*	m_pNextKey;
    TKeyChangeEvent		m_OnChange;
    int					m_iImagePos;

private:
	TColor __fastcall GetColor();
    void   __fastcall SetColor(TColor Color);
    float  __fastcall GetPosition();
    void   __fastcall SetPosition(float Position);

public:
    __property TColor Color 	= {read=GetColor, 		write=SetColor};
    __property float  Position 	= {read=GetPosition, 	write=SetPosition};

protected:
    __fastcall TColorGradientKey(TKeyChangeEvent OnChange);
};

// Component class
//--------------------------------------------------
class PACKAGE TColorGradient : public TGraphicControl
{
// iternal members
private:
	TColorGradientKey*	m_pHead;
   	TColorGradientKey*	m_pTail;
    int					m_nKeyCount;
    Graphics::TBitmap*	m_pBitmap;
    int					m_Mouse[2];
    int					m_ClipMouse[2];
    TColorDialog*		m_pColorDialog;
    bool				m_bDblClk;

	// properties storages
    float				m_fMinRange;
    float 				m_fMaxRange;
    TFont*				m_RangeFont;
    int					m_iRangeTextHeight;
    int					m_iRangeMargin;
    TColor				m_RangeColor;

    // state
    int					m_iRangeBarHeight;
    TColorGradientKey*	m_pSelectedKey;
	int					m_iSelectOffset;    



// properties
__published:
  __property Anchors;
  __property TFont* RangeFont = {read=m_RangeFont, write=SetRangeFont};
  __property int RangeMargin = {read=m_iRangeMargin, write=SetRangeMargin};
  __property float RangeMin = {read=m_fMinRange, write=SetMinRange};
  __property float RangeMax = {read=m_fMaxRange, write=SetMaxRange};
  __property TColor RangeColor = {read=m_RangeColor, write=SetRangeColor};

// public methods
public:
	__fastcall TColorGradient(TComponent* Owner);
    __fastcall ~TColorGradient();

    // key program control
    TColorGradientKey* __fastcall AddKey(float Position, TColor Color);
    void __fastcall RemoveKey(TColorGradientKey* pKey);
    int  __fastcall GetKeyCount();
    TColorGradientKey* __fastcall GetKey(int Index);

    // control painting
	void __fastcall Invalidate()	{};
    void __fastcall Update()		{Paint();}
    void __fastcall Paint();

private:
	void __fastcall AddKey(TColorGradientKey* pKey);
    void __fastcall ClearKeys();
    void __fastcall SortKeys();
    TColor __fastcall GetGradient(int X);

    // keys change callback
    void __fastcall OnKeyChange(TColorGradientKey* pKey);

    // mouse tracking
	void __fastcall MouseMoves(TObject* pSender,
                               TShiftState Shift,
                               int X,
                               int Y);
    void __fastcall MouseButtonDown(TObject* pSender,
    								TMouseButton Button,
                                    TShiftState Shift,
                                    int X,
                                    int Y);
    void __fastcall MouseButtonUp(TObject* pSender,
    							  TMouseButton Button,
                                  TShiftState Shift,
                                  int X,
                                  int Y);
	void __fastcall MouseDblClk(TObject* pSender);
	void __fastcall DoResize(TObject* Sender);

    // recalculates drawing objects parameters
    void __fastcall RecalcGraphics();

	// properties control
	void __fastcall SetRangeFont(TFont* Font);
   	void __fastcall SetRangeMargin(int Margin);
   	void __fastcall SetMinRange(float fValue);
   	void __fastcall SetMaxRange(float fValue);
   	void __fastcall SetRangeColor(TColor Color);


};

#endif  // ColorGradientH
 