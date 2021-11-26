//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OptionsUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionsForm *OptionsForm;
//---------------------------------------------------------------------------
__fastcall TOptionsForm::TOptionsForm(TComponent* Owner)
        : TForm(Owner)
{
}

void __fastcall TOptionsForm::OnCreateForm(TObject *Sender)
{
//    OpenFileDialog->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TOptionsForm::OpenTextureDirectoryClick(TObject *Sender)
{
    OpenFileDialog->Execute();
}
//---------------------------------------------------------------------------

