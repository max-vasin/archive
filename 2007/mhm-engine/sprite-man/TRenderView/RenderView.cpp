//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RenderView.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TRenderView *)
{
	new TRenderView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TRenderView::TRenderView(TComponent* Owner)
	: TCustomPanel(Owner)
{
}
//---------------------------------------------------------------------------
namespace Renderview
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TRenderView)};
		RegisterComponents("Mayhem", classes, 0);
	}
}
//---------------------------------------------------------------------------
// Paint handler property control
//---------------------------------------------------------------------------
void __fastcall TRenderView::SetPaintHandler(TPaintHandler Handler)
{
 	m_PaintHandler = Handler;
}
TPaintHandler __fastcall TRenderView::GetPaintHandler()
{
 	return m_PaintHandler;
}
//---------------------------------------------------------------------------
// Control painting
//---------------------------------------------------------------------------
void __fastcall TRenderView::Paint()
{
	if (m_PaintHandler)
    {
    	(m_PaintHandler)(this);
    	return;
    }
	Canvas->Rectangle(0, 0, Width, Height);
}

