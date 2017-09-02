program e;
{$APPTYPE CONSOLE}
{$R+,Q+,S+,H+,O-}

uses
  Math, SysUtils;

Type
  Integer=LongInt;
  Real=Extended;

Const
  TaskID='exploring';
  InFile=TaskID+'.in';
  OutFile=TaskID+'.out';
  MaxLen=300;
  Modulo=1000000000;

Var
  S:String;
  Res:Array[1..MaxLen,1..MaxLen]Of Int64;

Procedure Load;
Begin
  ReSet(Input,InFile);
  ReadLn(S);
  Close(Input);
End;

Procedure Solve;
Var
  L,I,J,K:Integer;
Begin
  For L:=1 To Length(S) Do Begin
    For I:=1 To Length(S)-L+1 Do Begin
      J:=I+L-1;
      If (L=1) Then Res[I,J]:=1 Else Begin
        If S[I]<>S[J] Then Res[I,J]:=0 Else Begin
          For K:=I To J-2 Do If S[K]=S[I] Then
            Res[I,J]:=(Res[I,J]+Res[I,K]*Res[K+1,J-1]) Mod Modulo;
        End;
      End;
    End;
  End;
End;

Procedure Save;
Begin
  ReWrite(Output,OutFile);
  WriteLn(Res[1,Length(S)]);
  Close(Output);
End;

begin
  Load;
  Solve;
  Save;
end.
