program f;
{$APPTYPE CONSOLE}
{$R+,Q+,S+,H+,O-}

uses
  Math, SysUtils;

Type
  Integer=LongInt;
  Real=Extended;

Const
  TaskID='feelgood';
  InFile=TaskID+'.in';
  OutFile=TaskID+'.out';
  MaxN=100000;
  MaxValue=1000000;

Var
  N:Integer;
  A,L,R:Array[1..MaxN]Of Integer;
  PSum:Array[0..MaxN]Of Int64;
  Max:Array[1..4*MaxValue]Of Integer;
  Res:Int64;
  ResL,ResR:Integer;

Procedure Load;
Var
  I:Integer;
Begin
  ReSet(Input,InFile);
  Read(N);
  For I:=1 To N Do Read(A[I]);
  Close(Input);
End;

Procedure TreeSet(A,Value,ID,L,R:Integer);
Var
  M:Integer;
Begin
  If Value>Max[ID] Then Max[ID]:=Value;
  If L=R Then Exit;
  M:=(L+R) Div 2;
  If A<=M Then TreeSet(A,Value,ID*2,L,M) Else TreeSet(A,Value,ID*2+1,M+1,R);
End;

Function TreeGet(S,E,ID,L,R:Integer):Integer;
Var
  M:Integer;
Begin
  If S>E Then Begin
    Result:=0;
    Exit;
  End;
  If (S=L) And (E=R) Then Begin
    Result:=Max[ID];
    Exit;
  End;
  M:=(L+R) Div 2;
  If E<=M Then Result:=TreeGet(S,E,ID*2,L,M) Else If S>M Then Result:=TreeGet(S,E,ID*2+1,M+1,R) Else
    Result:=Math.Max(TreeGet(S,M,ID*2,L,M),TreeGet(M+1,E,ID*2+1,M+1,R));
End;

Procedure Solve;
Var
  I:Integer;
  Cur:Int64;
Begin
  FillChar(Max,SizeOf(Max),0);
  For I:=1 To N Do Begin
    L[I]:=TreeGet(0,A[I]-1,1,0,MaxValue)+1;
    TreeSet(A[I],I,1,0,MaxValue);
  End;
  FillChar(Max,SizeOf(Max),0);
  For I:=N DownTo 1 Do Begin
    R[I]:=N-TreeGet(0,A[I]-1,1,0,MaxValue);
    TreeSet(A[I],N-I+1,1,0,MaxValue);
  End;
  PSum[0]:=0;
  For I:=1 To N Do PSum[I]:=PSum[I-1]+A[I];
  Res:=-1;
  For I:=1 To N Do Begin
    Cur:=Int64(A[I])*(PSum[R[I]]-PSum[L[I]-1]);
    If Cur>Res Then Begin
      Res:=Cur;
      ResL:=L[I];
      ResR:=R[I];
    End;
  End;
End;

Procedure Save;
Begin
  ReWrite(Output,OutFile);
  WriteLn(Res);
  WriteLn(ResL,' ',ResR);
  Close(Output);
End;

begin
  Load;
  Solve;
  Save;
end.
