unit undmdados;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, mysql50conn, mysql55conn, sqldb, db, variants;

type

  { TDataModule1 }

  TDataModule1 = class(TDataModule)
    MySQL55Connection1: TMySQL55Connection;
    qryInc: TSQLQuery;
    qryIncleit_data: TDateTimeField;
    qryIncleit_id: TAutoIncField;
    qryIncleit_tipo: TStringField;
    qryIncleit_valor: TLongintField;
    SQLTransaction1: TSQLTransaction;
    procedure DataModuleCreate(Sender: TObject);
  private
    { private declarations }
  public
    procedure InsereItem(tipo: string; valor: LongInt);
    { public declarations }
  end;

var

  DataModule1: TDataModule1;

implementation

{$R *.lfm}

{ TDataModule1 }

procedure TDataModule1.DataModuleCreate(Sender: TObject);
begin
  MySQL55Connection1.Close();
  MySQL55Connection1.Open;
  qryInc.Open;;
end;

procedure TDataModule1.InsereItem(tipo: string; valor: LongInt);
var
  f:TextFile;
begin
  try
    if not (SQLTransaction1.Active ) then
    begin
      SQLTransaction1.StartTransaction;
    end;
    qryInc.Close;
    qryInc.Open;
    qryInc.Append;
    qryIncleit_valor.Value := valor;
    qryIncleit_tipo.AsString := tipo;
    qryInc.Post;
    qryInc.ApplyUpdates;
    SQLTransaction1.Commit;
  except
    on E: Exception do
    begin
      SQLTransaction1.Rollback;
      WriteLn(e.Message);
      AssignFile(f,'erro.log');
      if ( FileExists('erro.log') ) then
      begin
        Append(f);
      end
      else
      begin
        Rewrite(f);
      end;
      WriteLn(f,e.Message);
      CloseFile(f);
    end;
  end;


end;

end.
