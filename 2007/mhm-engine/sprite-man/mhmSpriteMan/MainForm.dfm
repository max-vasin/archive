object EditorForm: TEditorForm
  Left = 241
  Top = 152
  Width = 860
  Height = 639
  Caption = 'SpriteMan (c) Mayhem projex 2006'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  OnCreate = OnCreate
  OnResize = OnResize
  OnShow = OnShow
  PixelsPerInch = 96
  TextHeight = 13
  object EditorView: TRegionEdit
    Left = 24
    Top = 8
    Width = 817
    Height = 561
    Anchors = [akLeft, akTop, akRight, akBottom]
    RE_GridWidth = 50
    RE_GridHeight = 50
    RE_GridColor = clBlack
    RE_OnDrawBackground = OnDrawEditor
  end
  object MainMenu: TMainMenu
    Left = 832
    Top = 552
    object File1: TMenuItem
      Caption = 'File'
      object Newsprite1: TMenuItem
        Caption = 'New sprite'
      end
      object Opensprite1: TMenuItem
        Caption = 'Open sprite'
      end
      object Savesprite1: TMenuItem
        Caption = 'Save sprite'
      end
    end
    object Edit1: TMenuItem
      Caption = 'Edit'
    end
    object Options1: TMenuItem
      Caption = 'Options'
      object Options: TMenuItem
        Caption = 'Preferences'
        OnClick = OptionsClick
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
    end
  end
end
