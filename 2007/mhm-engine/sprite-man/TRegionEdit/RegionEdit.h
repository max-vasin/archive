//---------------------------------------------------------------------------

#ifndef RegionEditH
#define RegionEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
// Region point class
//---------------------------------------------------------------------------
class TRegion;
class TRegionPoint
{
private:
    int				m_iX;
    int 			m_iY;

public:
    TRegionPoint*	m_pNext;
    TRegion*		m_pRegion;

public:
	__fastcall TRegionPoint(TRegion* pRegion);
    __fastcall TRegionPoint(TRegion* pRegion, int X, int Y, TRegionPoint* pNext);
    int __fastcall GetX();
    void __fastcall SetX(int iX);
    int __fastcall GetY();
    void __fastcall SetY(int iY);

    __property int X = {read=GetX, write=SetX};
    __property int Y = {read=GetY, write=SetY};
};
//---------------------------------------------------------------------------
// Region class
//---------------------------------------------------------------------------
class TRegion
{
friend TRegionPoint;
friend class TRegionEdit;
protected:
	TRegion*		m_pNext;
    TRegionPoint*	m_pHead;
    TRegionPoint*	m_pTail;
    bool			m_bClosed;
    TRegionPoint	m_Pivot;
    TRegionPoint	m_Min;
    TRegionPoint	m_Max;

public:
	__fastcall TRegion();
    __fastcall ~TRegion();
    void __fastcall AddPoint(int X, int Y);
    void __fastcall SetClosed(bool bClosed);
    void __fastcall SetPivot(int X, int Y);

private:
	void __fastcall OnPointChanged(TRegionPoint* pPoint);
    void __fastcall RecalcBounds();
};

//---------------------------------------------------------------------------
// Component
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TOnPaintEvent)(TObject *Sender);
class PACKAGE TRegionEdit : public TCustomControl
{
private:
	bool 		  m_bShowGrid;
    int  		  m_iGridWidth;
    int  		  m_iGridHeight;
    TColor		  m_GridColor;
    TRegion* 	  m_pHead;
    TRegion*	  m_pTail;
    TRegionPoint* m_pSelectedPoint;
    TOnPaintEvent m_BackgroundDrawHandler;


protected:
	void __fastcall DrawGrid();
    void __fastcall DrawRegion(TRegion* pRegion);
    void __fastcall CheckRegionMouse(TRegion* pRegion, int X, int Y);

public:
	__fastcall TRegionEdit(TComponent* Owner);
    __fastcall ~TRegionEdit();
	// show grid property
    void __fastcall SetShowGrid(bool bShow);
    bool __fastcall GetShowGrid();
    // grid width property
	void __fastcall SetGridWidth(int iWidth);
    int  __fastcall GetGridWidth();
    // grid height property
    void __fastcall SetGridHeight(int iHeight);
    int  __fastcall GetGridHeight();
    // grid color property
    void __fastcall SetGridColor(TColor Color);
    TColor __fastcall GetGridColor();
    // background painting property
    void __fastcall SetDrawHandler(TOnPaintEvent value);
    TOnPaintEvent __fastcall GetDrawHandler();




    // control painting
   	void __fastcall Paint();

    // control mouse capture
    void __fastcall MouseMoves(TObject* pSender, TShiftState Shift, int X, int Y);

    // region control
    TRegion* __fastcall CreateRegion();
    void     __fastcall DeleteRegion(TRegion* pRegion);


__published:
    __property Anchors;
    __property DoubleBuffered;
	__property bool     RE_ShowGrid   = {read=GetShowGrid, write=SetShowGrid, default=false};
	__property int      RE_GridWidth  = {read=GetGridWidth, write=SetGridWidth, default=100};
	__property int      RE_GridHeight = {read=GetGridHeight, write=SetGridHeight, default=100};
    __property TColor   RE_GridColor  = {read=GetGridColor, write=SetGridColor, default=100};
    __property TOnPaintEvent RE_OnDrawBackground = {read=GetDrawHandler, write=SetDrawHandler, default=NULL};


};
//---------------------------------------------------------------------------
#endif
