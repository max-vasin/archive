//---------------------------------------------------------------------------
// TSplineEditor component
// (c) Mayhem Projex 2007
// Coded: Max Vasin
//---------------------------------------------------------------------------
#ifndef SplineEditorH
#define SplineEditorH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>

// Spline key structure
//--------------------------------------------------
class TSplineKey;
typedef void __fastcall (__closure *TSplineKeyChangeEvent)(TSplineKey* Sender);
class TSplineKey
{
friend class TSplineEditor;
private:
	float					m_fPosition;
    float					m_fValue;
    TSplineKey*				m_pNextKey;
    TSplineKeyChangeEvent	m_OnChange;

    int						m_iX;
    int						m_iY;

private:
    void   __fastcall SetValue(float fValue);
    void   __fastcall SetPosition(float Position);

public:
    __property float  Value 	= {read=m_fPosition,	write=SetValue};
    __property float  Position 	= {read=m_fValue,	 	write=SetPosition};

protected:
    __fastcall TSplineKey(TSplineKeyChangeEvent OnChange);
};

// Spline editor control
//---------------------------------------------------------------------------
class PACKAGE TSplineEditor : public TGraphicControl
{
private:
	TSplineKey*			m_pHead;
   	TSplineKey*			m_pTail;
    int					m_nKeyCount;
    Graphics::TBitmap*	m_pBitmap;
    int					m_Mouse[2];
    int					m_ClipMouse[2];
    bool				m_bDblClk;
    TSplineKey*			m_pSelectedKey;

    // properties storages
    TColor				m_PointColor;
    int					m_iPointSize;
    TColor				m_LineColor;
    int					m_iLineWidth;
    TColor				m_BackColor;
    TColor				m_GridColor;

    float				m_fMinRange;
    float				m_fMaxRange;
    float				m_fMinValue;
    float 				m_fMaxValue;


protected:

__published:
  __property Anchors;
  __property TColor PointColor = {read=m_PointColor, write=SetPointColor};
  __property TColor LineColor  = {read=m_LineColor,  write=SetLineColor};
  __property TColor BackColor  = {read=m_BackColor,  write=SetBackColor};
  __property TColor GridColor  = {read=m_GridColor,  write=SetGridColor};
  __property int    PointSize  = {read=m_iPointSize, write=SetPointSize};
  __property int    LineWidth  = {read=m_iLineWidth, write=SetLineWidth};
  __property float  MinRange   = {read=m_fMinRange,  write=SetMinRange};
  __property float  MaxRange   = {read=m_fMaxRange,  write=SetMaxRange};
  __property float  MinValue   = {read=m_fMinValue,  write=SetMinValue};
  __property float  MaxValue   = {read=m_fMaxValue,  write=SetMaxValue};

public:
	__fastcall TSplineEditor(TComponent* Owner);
  	__fastcall ~TSplineEditor();

    TSplineKey* __fastcall AddKey(float fPosition, float fValue);

    // control painting
	void __fastcall Invalidate()	{};
    void __fastcall Update()		{Paint();}
    void __fastcall Paint();

private:
	void __fastcall AddKey(TSplineKey* pKey);
  	void __fastcall RemoveKey(TSplineKey* pKey);
    void __fastcall ClearKeys();
    void __fastcall SortKeys();
    void __fastcall RecalcGraphics();

    // keys change callback
    void __fastcall OnKeyChange(TSplineKey* pKey);

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

    // size tracking
	void __fastcall DoResize(TObject* Sender);

    // properties sets
    void __fastcall SetPointColor(TColor Color);
    void __fastcall SetLineColor(TColor Color);
    void __fastcall SetBackColor(TColor Color);
    void __fastcall SetGridColor(TColor Color);
    void __fastcall SetPointSize(int iSize);
    void __fastcall SetLineWidth(int iWidth);
    void __fastcall SetMinRange(float fValue);
    void __fastcall SetMaxRange(float fValue);
    void __fastcall SetMinValue(float fValue);
    void __fastcall SetMaxValue(float fValue);
};
//---------------------------------------------------------------------------
#endif
 