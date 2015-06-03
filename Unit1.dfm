object Form1: TForm1
  Left = 288
  Top = 134
  Width = 979
  Height = 563
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 23
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 971
    Height = 530
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'General'
      object versionLabel: TLabel
        Tag = 1
        Left = 312
        Top = 32
        Width = 110
        Height = 23
        Caption = 'versionLabel'
        OnClick = versionLabelClick
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'cevents'
      ImageIndex = 1
    end
    object TabSheet3: TTabSheet
      Caption = 'Help'
      ImageIndex = 2
    end
  end
  object HttpCli1: THttpCli
    LocalAddr = '0.0.0.0'
    ProxyPort = '80'
    Agent = 'Mozilla/4.0'
    Accept = 'image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, */*'
    NoCache = False
    ContentTypePost = 'application/x-www-form-urlencoded'
    MultiThreaded = False
    RequestVer = '1.0'
    FollowRelocation = True
    LocationChangeMaxCount = 5
    BandwidthLimit = 10000
    BandwidthSampling = 1000
    Options = []
    OnDocBegin = HttpCli1DocBegin
    OnDocEnd = HttpCli1DocEnd
    SocksAuthentication = socksNoAuthentication
    Left = 884
    Top = 50
  end
  object ElasticForm1: TElasticForm
    DesignScreenWidth = 1448
    DesignScreenHeight = 876
    DesignPixelsPerInch = 120
    DesignFormWidth = 979
    DesignFormHeight = 563
    DesignFormClientWidth = 971
    DesignFormClientHeight = 530
    DesignFormLeft = 288
    DesignFormTop = 134
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Arial'
    Font.Style = []
    Version = 700
    Left = 84
    Top = 258
  end
end
