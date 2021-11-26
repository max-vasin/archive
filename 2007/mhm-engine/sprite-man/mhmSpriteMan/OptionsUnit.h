//---------------------------------------------------------------------------

#ifndef OptionsUnitH
#define OptionsUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TOptionsForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *TextureDirectory;
        TSpeedButton *OpenTextureDirectory;
        TBitBtn *AcceptOptions;
        TBitBtn *DeclineOptions;
    TOpenDialog *OpenFileDialog;
    void __fastcall OnCreateForm(TObject *Sender);
    void __fastcall OpenTextureDirectoryClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TOptionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionsForm *OptionsForm;
//---------------------------------------------------------------------------
#endif
