//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include "RegionEdit.h"
//---------------------------------------------------------------------------
class TEditorForm : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *File1;
        TMenuItem *Edit1;
        TMenuItem *Help1;
        TMenuItem *Opensprite1;
        TMenuItem *Savesprite1;
        TMenuItem *Newsprite1;
        TMenuItem *Options1;
        TMenuItem *Options;
	TRegionEdit *EditorView;
        void __fastcall OptionsClick(TObject *Sender);
    void __fastcall OnCreate(TObject *Sender);
	void __fastcall OnDrawEditor(TObject *Sender);
	void __fastcall OnShow(TObject *Sender);
	void __fastcall OnResize(TObject *Sender);
private:	// User declarations
    HANDLE       m_hEngineDLL;
    clsIEngine*  m_pEngine;

public:		// User declarations
        __fastcall TEditorForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditorForm *EditorForm;
//---------------------------------------------------------------------------
#endif
