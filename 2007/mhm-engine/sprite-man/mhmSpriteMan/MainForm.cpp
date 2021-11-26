//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mhmTypes.h"
#include "mhmDefs.h"
#include "/Interfaces/mhmInterfaces.h"
#include "MainForm.h"
#include "OptionsUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RegionEdit"
#pragma resource "*.dfm"
TEditorForm *EditorForm;
//---------------------------------------------------------------------------
__fastcall TEditorForm::TEditorForm(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::OptionsClick(TObject *Sender)
{
    OptionsForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TEditorForm::OnCreate(TObject *Sender)
{
	TRegion* pRegion = EditorView->CreateRegion();
    pRegion->AddPoint(10, 10);
    pRegion->AddPoint(100, 10);
    pRegion->AddPoint(200, 200);
    pRegion->AddPoint(300, 150);
    pRegion->AddPoint(400, 50);
    pRegion->AddPoint(10, 200);
    pRegion->SetClosed(true);
}
//---------------------------------------------------------------------------


void __fastcall TEditorForm::OnDrawEditor(TObject *Sender)
{
    m_pEngine->Update();
	m_pEngine->Render();
}
//---------------------------------------------------------------------------

void __fastcall TEditorForm::OnShow(TObject *Sender)
{
    m_hEngineDLL = LoadLibrary("mhmEngine2D.dll");
    if (!m_hEngineDLL)
    {
        MessageBox(NULL, "Error", "Cant load engine DLL", MB_OK);
    	return;
    }


    GetInterfaceProc GetInterface = (GetInterfaceProc)GetProcAddress(m_hEngineDLL, "GetInterface");
    if (GetInterface == mmnull)
    {
		MessageBox(NULL, "Error", "Cant get interfaces proc from engine DLL", MB_OK);
		return;
    }

	m_pEngine = mmnull;
    (GetInterface)(MHM_INTERFACEID_ENGINE, (mmVoid**)&m_pEngine);
	char Path[MAX_PATH];
	GetModuleFileName(NULL, Path, MAX_PATH);
    for (int i = strlen(Path) - 1; i >= 0; i--)
    	if (Path[i] == '\\')
        {
         	Path[i] = 0;
            break;
        }
    strcat(Path, "\\mhm-2d.ini");
    m_pEngine->Initialize(EditorView->Handle, Path);
}
//---------------------------------------------------------------------------

void __fastcall TEditorForm::OnResize(TObject *Sender)
{
	if ((m_pEngine != NULL) && m_pEngine->IsInitialized())
    {
   //		MessageBox(NULL, "OK", "OK", MB_OK);
  		m_pEngine->Reset();
    }
}
//---------------------------------------------------------------------------

