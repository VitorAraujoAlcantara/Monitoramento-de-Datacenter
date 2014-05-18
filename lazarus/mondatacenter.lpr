program mondatacenter;

{$mode objfpc}{$H+}

uses
  synaser, {$IFDEF UNIX} {$IFDEF UseCThreads}
  cthreads, {$ENDIF} {$ENDIF}
  Classes,
  SysUtils,
  dateutils,
  laz_synapse,
  undmdados,
  CustApp { you can add units after this };

type
  { monitora_servidor }
  monitora_servidor = class(TCustomApplication)
  protected
    retorno: string;
    ultimaLeitura: TDateTime;
    ultimaTentativaLigarAr: TDateTime;
    contadorTempo: TDateTime;
    gravaRegistro: boolean;
    arquivo: string;
    procedure DoRun; override;
  public
    constructor Create(TheOwner: TComponent); override;
    destructor Destroy; override;
    procedure WriteHelp; virtual;
    procedure PiscaMonitor;
    procedure EnviaCmd(c: string);
    procedure CriaArquivo(fileName: string; Text: string);
    procedure EscreveArquivo(Text: string);
  end;

const
  OP_PISCA = 'p';
  OP_TEMPERATURA = 't';
  OP_LUMINOSIDADE = 'l';
  OP_PORTA = 'q';
  OP_PRESENCA = 'r';
  OP_LIGA_TV = 'o';
  OP_LIGA_LUZ = '1';
  OP_DESLIGA_LUZ = '0';
  OP_LUZ = 's';

  MSG_LIGANDO_LUS =  '.:: Presença detectada - Ligando luz! ::.';
  MSG_DESLIGANDO_LUS = '.:: Entrando em modo de economida. Desligando luz! ::.';
  MSG_LIGANDO_AR = '.:: Tentando ligar arcondicionado! ::.';

  FILE_NAME_LIGA_AR = 'LIGA_AR';
  FILE_NAME_DESLIGA_LUS = 'DESLIGA_LUS';
  FILE_NAME_LIGA_LUS = 'LIGA_LUS';
  FILE_NAME_TEMPERATURA = 'TEMPERATURA';
  FILE_NAME_LUMINOSIDADE = 'LUMINOSIDADE';
  FILE_NAME_PORTA  = 'PORTA';
  FILE_NAME_PRESENCA = 'PRESENCA';
  FILE_NAME_LUS = 'LUS';

  FILE_NAME_RET_TEMPERATURA = 'RET_TEMPERATURA';
  FILE_NAME_RET_LUS = 'RET_LUS';
  FILE_NAME_RET_PRESENCA = 'RET_PRESENCA';
  FILE_NAME_RET_LUMINOSIDADE = 'RET_LUMINOSIDADE';
  FILE_NAME_RET_PORTA = 'RET_PORTA';

  { monitora_servidor }

  procedure monitora_servidor.DoRun;
  var
    lus: boolean;
    ultimaLeituraPresenca: TDateTime;
    porta: string;
  begin
    DataModule1 := TDataModule1.Create(nil);

    ultimaLeituraPresenca := -1;
    ultimaTentativaLigarAr := now;

    if HasOption('a', 'automatic') then
    begin
      gravaRegistro := False;
      ultimaLeitura := 0;
      contadorTempo := now;
      while (True) do
      begin
        if (IncSecond(now, 3) > ultimaLeitura) then
        begin
          gravaRegistro := True;
          ultimaLeitura := now;
        end;
        if (FileExists(FILE_NAME_LIGA_AR)) then
        begin
          EnviaCmd(OP_LIGA_TV);
          DeleteFile(FILE_NAME_LIGA_AR);
        end;

        if (FileExists(FILE_NAME_DESLIGA_LUS)) then
        begin
          EnviaCmd(OP_DESLIGA_LUZ);
          DeleteFile(FILE_NAME_DESLIGA_LUS);
        end;

        if (FileExists(FILE_NAME_LIGA_LUS)) then
        begin
          EnviaCmd(OP_LIGA_LUZ);
          DeleteFile(FILE_NAME_LIGA_LUS);
        end;

        if (FileExists(FILE_NAME_TEMPERATURA)) then
        begin
          EnviaCmd(OP_TEMPERATURA);
          DeleteFile(FILE_NAME_TEMPERATURA);
          CriaArquivo(FILE_NAME_RET_TEMPERATURA, retorno);
        end;

        if (FileExists(FILE_NAME_LUMINOSIDADE)) then
        begin
          EnviaCmd(OP_LUMINOSIDADE);
          DeleteFile(FILE_NAME_LUMINOSIDADE);
          CriaArquivo(FILE_NAME_RET_LUMINOSIDADE, retorno);
        end;

        if (FileExists(FILE_NAME_PORTA)) then
        begin
          EnviaCmd(OP_PORTA);
          DeleteFile(FILE_NAME_PORTA);
          CriaArquivo(FILE_NAME_RET_PORTA, retorno);
        end;

        if (FileExists(FILE_NAME_PRESENCA)) then
        begin
          EnviaCmd(OP_PRESENCA);
          DeleteFile(FILE_NAME_PRESENCA);
          CriaArquivo(FILE_NAME_RET_PRESENCA, retorno);
        end;

        if (FileExists(FILE_NAME_LUS)) then
        begin
          EnviaCmd(OP_LUZ);
          DeleteFile(FILE_NAME_LUS);
          CriaArquivo(FILE_NAME_RET_LUS, retorno);
        end;

        EnviaCmd(OP_PORTA);
        porta := retorno;
        EnviaCmd(OP_TEMPERATURA);

        // A cada 7 minutos chega a temperatura e se for o caso liga o ar condicionado
        if (IncMInute(ultimaTentativaLigarAr, 7) < now) then
        begin
          EnviaCmd(OP_TEMPERATURA);
          sleep(100);
          if (StrToIntDef(Copy(retorno, 1, 4), 0) > 250) and (Pos('1', porta) > 0) then
          begin
            WriteLn(FormatDateTime('hh:mm:ss', now) +
              MSG_LIGANDO_AR);
            EnviaCmd(OP_LIGA_TV);
          end;
          ultimaTentativaLigarAr := now;
        end;

        EnviaCmd(OP_LUZ);
        lus := Pos('1', retorno) > 0;

        EnviaCmd(OP_PRESENCA);
        if (not lus or (IncMinute(ultimaLeituraPresenca, 15) < now)) then
        begin
          EnviaCmd(OP_PRESENCA);
          sleep(100);
          if (Pos('1', retorno) > 0) then
          begin
            EscreveArquivo(MSG_LIGANDO_LUS);
            EnviaCmd(OP_LIGA_LUZ);
          end
          else
          begin
            if (lus) then
            begin
              EscreveArquivo(
                MSG_DESLIGANDO_LUS);
              EnviaCmd(OP_DESLIGA_LUZ);
            end;
          end;
          ultimaLeituraPresenca := now;
        end
        else
        begin
          if (Pos('1', retorno) > 0) then
          begin
            ultimaLeituraPresenca := Now;
          end;
        end;


        EnviaCmd(OP_LUMINOSIDADE);
        Sleep(100);
        gravaRegistro := False;
      end;
    end;

    // parse parameters
    if HasOption('h', 'help') then
    begin
      WriteHelp;
      Terminate;
      Exit;
    end;

    if HasOption(OP_PISCA, 'Pisca') then
    begin
      PiscaMonitor;
      Terminate;
      Exit;
    end;

    if (HasOption(OP_DESLIGA_LUZ, 'Desliga')) then
    begin
      EnviaCmd(OP_DESLIGA_LUZ);
      Terminate;
      Exit;
    end;

    if (HasOption(OP_LIGA_LUZ, 'Liga')) then
    begin
      EnviaCmd(OP_LIGA_LUZ);
      Terminate;
      Exit;
    end;

    if (HasOption(OP_LIGA_TV, 'Liga')) then
    begin
      EnviaCmd(OP_LIGA_TV);
      Terminate;
      Exit;
    end;

    if (HasOption(OP_LUMINOSIDADE, 'Liga')) then
    begin
      EnviaCmd(OP_LUMINOSIDADE);
      Terminate;
      Exit;
    end;

    if (HasOption(OP_LUZ, 'Liga')) then
    begin
      EnviaCmd(OP_LUZ);
      Terminate;
      Exit;
    end;

    if (HasOption(OP_PORTA, 'Liga')) then
    begin
      EnviaCmd(OP_PORTA);
      Terminate;
      Exit;
    end;

    if (HasOption(OP_PRESENCA, 'Liga')) then
    begin
      EnviaCmd(OP_PRESENCA);
      Terminate;
      Exit;
    end;

    if (HasOption(OP_TEMPERATURA, 'Liga')) then
    begin
      EnviaCmd(OP_TEMPERATURA);
      Terminate;
      Exit;
    end;

    { add your program here }

    // stop program loop
    DataModule1.Free;
    Terminate;
  end;



  constructor monitora_servidor.Create(TheOwner: TComponent);
  begin
    inherited Create(TheOwner);
    StopOnException := True;
  end;

  destructor monitora_servidor.Destroy;
  begin
    inherited Destroy;
  end;

  procedure monitora_servidor.WriteHelp;
  begin
    { add your help code here }
    writeln('Usage: ', ExeName, ' -h');
  end;

  procedure monitora_servidor.PiscaMonitor;
  var
    ser: TBlockSerial;
  begin
    WriteLn('Piscando');
    ser := TBlockSerial.Create;
    ser.CloseSocket;
    ;
    sleep(30);
    ser.Connect('//dev//ttyS0');
    sleep(30);

    if ser.LastError = 2 then
      WriteLn('Erro ao conectar')
    else
      ser.Config(9600, 8, 'N', 0, False, False);
    ser.Flush;
    ser.SendString('p'#13#10);
    ser.CloseSocket;
    ser.Free;

  end;

  procedure monitora_servidor.EnviaCmd(c: string);
  var
    ser: TBlockSerial;
  begin
    ser := TBlockSerial.Create;
    try
      ser.CloseSocket;
      sleep(30);
      ser.Connect('//dev//ttyS0');
      sleep(30);

      if ser.LastError = 2 then
        WriteLn('Erro ao conectar')
      else
        ser.Config(9600, 8, 'N', 0, False, False);

      ser.SendString(c + #13#10);
      sleep(60);
      retorno := ser.Recvstring(100);
      if (gravaRegistro) then
      begin
        DataModule1.InsereItem(c, StrToIntDef(copy(retorno, 1, 4), 0));
      end;

      EscreveArquivo(retorno);

      WriteLn(retorno);
      ser.CloseSocket;
    finally
      ser.Free;
    end;
  end;

  procedure monitora_servidor.CriaArquivo(fileName: string; Text: string);
  var
    f: TextFile;
  begin
    try
      AssignFile(f, fileName);
      Rewrite(f);
      WriteLn(f, Text);
    finally
      CloseFile(f);
    end;
  end;

  procedure monitora_servidor.EscreveArquivo(Text: string);
  var
    f: TextFile;
  begin
    try
      AssignFile(f, FormatDateTime('yyyymmdd', now) + '_retorno.txt');
      if (FileExists(FormatDateTime('yyyymmdd', now) + '_retorno.txt')) then
      begin
        Append(f);
      end
      else
      begin
        Rewrite(f);
      end;
      WriteLn(f, FormatDateTime('hh:mm:ss', now) + ' ', Text);
    finally
      closefile(f);
    end;
  end;

var
  Application: monitora_servidor;

{$R *.res}

begin
  Application := monitora_servidor.Create(nil);
  Application.Run;
  Application.Free;
end.
