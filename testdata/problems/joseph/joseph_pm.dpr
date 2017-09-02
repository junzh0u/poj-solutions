program j;
{$APPTYPE CONSOLE}
{$R+,Q+,S+,H+,O-}

uses
  Math, SysUtils;

Type
  Integer=LongInt;
  Real=Extended;

Const
  TaskID='joseph';
  InFile=TaskID+'.in';
  OutFile=TaskID+'.out';

Var
  N,K:Int64;
  Res:Int64;

Procedure Load;
Begin
  ReSet(Input,InFile);
  ReadLn(N,K);
  Close(Input);
End;

Procedure Solve;
Var
  Left,Right,Split:Int64;
  I,Q:Integer;
Begin
  Res:=K*N;
  Split:=Trunc(Sqrt(N+0.0));
  For Q:=0 To Split Do Begin
    Left:=K Div (Q+1)+1;
    If Q=0 Then Right:=N Else Right:=Min(N,K Div Q);
    If Left<=Right Then Begin
      Res:=Res-Q*(Left+Right)*(Right-Left+1) Div 2;
    End;
  End;
  For I:=1 To Min(N,Left-1) Do Begin
    Q:=K Div I;
    Res:=Res-Q*I;
  End;
End;

Procedure Save;
Begin
  ReWrite(Output,OutFile);
  WriteLn(Res);
  Close(Output);
End;

begin
  Load;
  Solve;
  Save;
end.
