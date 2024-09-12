unit uMain;

{$mode objfpc}{$H+}

interface

uses
   Classes, SysUtils, fphttpclient, DB, Forms, Controls, Graphics, Dialogs, StdCtrls, DBCtrls, ExtCtrls, ZConnection,
	 ZDataset, rxdbgrid;

type

	 { TForm1 }

   TForm1 = class(TForm)
			btnDesliga: TButton;
			btnPisca1: TButton;
			btnPisca2: TButton;
			btnLigado: TButton;
			DBNavigator1: TDBNavigator;
			dsCameras: TDataSource;
			cmpLigadoColor: TEdit;
			cmpPisca1Color: TEdit;
			cmpPisca3Color: TEdit;
			FPHTTPClient1: TFPHTTPClient;
			Memo1: TMemo;
			Panel1: TPanel;
			Panel2: TPanel;
			Panel3: TPanel;
			qryCamerasCOR: TWideStringField;
			qryCamerasMARCADO: TBooleanField;
			RxDBGrid1: TRxDBGrid;
			ZConnection1: TZConnection;
			qryCameras: TZQuery;
			qryCamerasDESCRICAO: TStringField;
			qryCamerasID_CAMERA: TLongintField;
			qryCamerasIP: TStringField;
			procedure btnDesligaClick(Sender: TObject);
			procedure btnPisca1Click(Sender: TObject);
			procedure btnPisca2Click(Sender: TObject);
			procedure btnLigadoClick(Sender: TObject);
			procedure FormShow(Sender: TObject);
   private
      var
        cor : String;

   public

   end;

var
   Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.btnDesligaClick(Sender: TObject);
var
  s : String;
begin
   try
      s := TFPCustomHTTPClient.SimpleGet('http://'+qryCamerasIP.AsString+'?led=0&R=255&G=0&B=0');
	    qryCameras.edit;
	    qryCamerasCOR.AsString:= '';
	    qryCameras.Post;
	    qryCameras.Refresh;
	 except
      ShowMessage('Camera nao encontrada');
	 end;

end;

procedure TForm1.btnPisca1Click(Sender: TObject);
var
  s : String;
begin
    try
   s := TFPCustomHTTPClient.SimpleGet('http://'+qryCamerasIP.AsString+'?led=1'+cmpPisca1Color.text);
   qryCameras.edit;
   qryCamerasCOR.AsString:= cmpPisca1Color.Text;
   qryCameras.Post;
   qryCameras.Refresh;
   except
      ShowMessage('Câmera não encontrada');
	 end;

end;

procedure TForm1.btnPisca2Click(Sender: TObject);
var
  s : String;
begin
   s := TFPCustomHTTPClient.SimpleGet('http://'+qryCamerasIP.AsString+'?led=2'+cmpPisca3Color.Text);
   qryCameras.edit;
   qryCamerasCOR.AsString:= cmpPisca3Color.Text;
   qryCameras.Post;
   qryCameras.Refresh;
end;

procedure TForm1.btnLigadoClick(Sender: TObject);
var
  s : String;
begin
   s := TFPCustomHTTPClient.SimpleGet('http://'+qryCamerasIP.AsString+'?led=3'+cmpLigadoColor.text);
   qryCameras.edit;
   qryCamerasCOR.AsString:= cmpLigadoColor.Text;
   qryCameras.Post;
   qryCameras.Refresh;
end;

procedure TForm1.FormShow(Sender: TObject);
begin
   qryCameras.Close;
   qryCameras.Open;
end;

end.

