//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("SpriteMan.res");
USEFORM("MainForm.cpp", EditorForm);
USEFORM("OptionsUnit.cpp", OptionsForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TEditorForm), &EditorForm);
                 Application->CreateForm(__classid(TOptionsForm), &OptionsForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
