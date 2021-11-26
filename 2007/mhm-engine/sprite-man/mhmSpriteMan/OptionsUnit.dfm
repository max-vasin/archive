object OptionsForm: TOptionsForm
  Left = 192
  Top = 114
  BorderStyle = bsToolWindow
  Caption = 'Options'
  ClientHeight = 106
  ClientWidth = 440
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = OnCreateForm
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 109
    Height = 13
    Caption = 'Textures directory:'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object OpenTextureDirectory: TSpeedButton
    Left = 416
    Top = 16
    Width = 17
    Height = 17
    Caption = '...'
    OnClick = OpenTextureDirectoryClick
  end
  object TextureDirectory: TEdit
    Left = 120
    Top = 16
    Width = 289
    Height = 17
    TabOrder = 0
    Text = 'TextureDirectory'
  end
  object AcceptOptions: TBitBtn
    Left = 240
    Top = 72
    Width = 89
    Height = 25
    Caption = 'Accept'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
  end
  object DeclineOptions: TBitBtn
    Left = 336
    Top = 72
    Width = 89
    Height = 25
    Caption = 'Decline'
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ModalResult = 2
    ParentFont = False
    TabOrder = 2
  end
  object OpenFileDialog: TOpenDialog
    Options = [ofHideReadOnly, ofPathMustExist, ofNoNetworkButton, ofEnableSizing]
    Left = 8
    Top = 72
  end
end
