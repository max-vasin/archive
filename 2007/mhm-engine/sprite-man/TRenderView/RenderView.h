//---------------------------------------------------------------------------

#ifndef RenderViewH
#define RenderViewH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>

//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TPaintHandler)(TObject *Sender);
//---------------------------------------------------------------------------
class PACKAGE TRenderView : public TCustomPanel
{
private:
    TPaintHandler m_PaintHandler;

protected:
public:
	__fastcall TRenderView(TComponent* Owner);
	void __fastcall Paint();
	//----------------------------------------------------------------
    void __fastcall SetPaintHandler(TPaintHandler value);
    TPaintHandler __fastcall GetPaintHandler();


__published:
    __property Anchors;
    __property Cursor;
    __property DockSite;
    __property DragCursor;
   	__property TPaintHandler OnRender = {read=GetPaintHandler, write=SetPaintHandler, default=NULL};
};
//---------------------------------------------------------------------------
#endif
 