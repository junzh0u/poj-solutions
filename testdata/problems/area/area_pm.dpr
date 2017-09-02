program a;
{$APPTYPE CONSOLE}
{$R+,Q+,S+,H+,O-}

uses
  Math, SysUtils;

Type
  Integer=LongInt;
  Real=Extended;
  TPoint=Record
    X,Y:Integer;
    Lab:Char;
  End;
  TSplit=Record
    P,Q:Integer;
    A,B:Integer;
  End;

Const
  TaskID='area';
  InFile=TaskID+'.in';
  OutFile=TaskID+'.out';
  MaxM=1500;
  MaxSplit=MaxM*(MaxM-1) Div 2+1;
  MaxRes=MaxSplit+1;
  Infinity=1e50;

Var
  M:Integer;
  Need:String;
  Cur:String;
  NDiff:Integer;
  Pnt:Array[1..MaxM]Of TPoint;
  Perm,At:Array[1..MaxM]Of Integer;
  NSplit:Integer;
  Split:Array[1..MaxSplit]Of TSplit;
  NRes:Integer;
  Res:Array[1..MaxRes]Of Record
    L,R:Real;
  End;

Procedure Load;
Var
  I:Integer;
Begin
  ReSet(Input,InFile);
  ReadLn(M);
  ReadLn(Need);
  For I:=1 To M Do With Pnt[I] Do Begin
    If Not SeekEOF Then;
    Read(Lab);
    Read(X,Y);
  End;
  Close(Input);
End;

Function Less(Const A,B:TPoint):Boolean;
Begin
  If (A.Y>B.Y) Then
    Less:=True
  Else If (A.Y<B.Y) Then
    Less:=False
  Else If (A.X<B.X) Then
    Less:=True
  Else If (A.X>B.X) Then
    Less:=False
  Else
    Less:=False;
End;

Procedure Swap(A,B:Integer);
Var
  Tmp:Integer;
Begin
  If Cur[A]<>Need[A] Then Dec(NDiff);
  If Cur[B]<>Need[B] Then Dec(NDiff);
  Tmp:=Perm[A];
  Perm[A]:=Perm[B];
  Perm[B]:=Tmp;
  At[Perm[A]]:=A;
  At[Perm[B]]:=B;
  Cur[A]:=Pnt[Perm[A]].Lab;
  Cur[B]:=Pnt[Perm[B]].Lab;
  If Cur[A]<>Need[A] Then Inc(NDiff);
  If Cur[B]<>Need[B] Then Inc(NDiff);
End;

Function CompareSplit(Const A,B:TSplit):Integer;
Var
  Z:Integer;
Begin
  Z:=A.P*B.Q-B.P*A.Q;
  If Z<0 Then CompareSplit:=-2 Else If Z>0 Then CompareSplit:=2 Else Begin
    If A.A=0 Then CompareSplit:=0 Else 
    If At[A.A]<At[B.A] Then
      CompareSplit:=-1
    Else If At[A.A]>At[B.A] Then
      CompareSplit:=1
    Else If At[A.B]>At[B.B] Then
      CompareSplit:=-1
    Else If At[A.B]<At[B.B] Then
      CompareSplit:=1
    Else
      CompareSplit:=0;
  End;
End;

Procedure QSortSplit(L,R:Integer);
Var
  I,J:Integer;
  X,Y:TSplit;
Begin
  I:=L;
  J:=R;
  X:=Split[L+Random(R-L+1)];
  While I<=J Do Begin
    While CompareSplit(Split[I],X)<0 Do Inc(I);
    While CompareSplit(X,Split[J])<0 Do Dec(J);
    If I<=J Then Begin
      Y:=Split[I];
      Split[I]:=Split[J];
      Split[J]:=Y;
      Inc(I);
      Dec(J);
    End;
  End;
  If I<R Then QSortSplit(I,R);
  If L<J Then QSortSplit(L,J);
End;

Function GetValue(Const S:TSplit):Real;
Begin
  If S.Q=0 Then GetValue:=Infinity Else GetValue:=S.P/S.Q;
End;

Procedure Rotate(L,R:Integer);
Var
  I:Integer;
Begin
  For I:=0 To (R-L+1) Div 2-1 Do
    Swap(L+I,R-I);
End;

Procedure WorkOut(ID1,ID2:Integer);
Var
  I:Integer;
  T,CMax:Integer;
Begin
//  WriteLn('***');
  For I:=ID1 To ID2 Do With Split[I] Do Begin
    If At[A]>At[B] Then Begin
      T:=A;
      A:=B;
      B:=T;
    End;
//    WriteLn(At[A],' ',At[B]);
  End;
  QSortSplit(ID1,ID2);
  CMax:=0;
//  WriteLn('---');
  For I:=ID1 To ID2 Do With Split[I] Do Begin
//    WriteLn(At[A],' ',At[B]);
    If At[A]>CMax Then Begin
      CMax:=At[B];
      Rotate(At[A],At[B]);
//      WriteLn('!');
    End;
  End;
End;

Procedure Solve;
Var
  I,J:Integer;
  Start:Integer;
Begin
  NDiff:=0;
  Cur:=Need;
  For I:=1 To M Do Begin
    Perm[I]:=I;
    At[I]:=I;
    Cur[I]:=Pnt[I].Lab;
    If Cur[I]<>Need[I] Then Inc(NDiff);
  End;
  For I:=1 To M Do
    For J:=M DownTo I+1 Do
      If Less(Pnt[Perm[J]],Pnt[Perm[J-1]]) Then Begin
        Swap(J,J-1);
      End;
  NSplit:=0;
  For I:=1 To M Do
    For J:=I+1 To M Do
      If Pnt[I].Y<>Pnt[J].Y Then Begin
        Inc(NSplit);
        With Split[NSplit] Do Begin
          P:=Pnt[J].X*(Pnt[I].Y-Pnt[J].Y)-Pnt[J].Y*(Pnt[I].X-Pnt[J].X);
          Q:=Pnt[I].Y-Pnt[J].Y;
          If Q<0 Then Begin
            P:=-P;
            Q:=-Q;
          End;
          A:=I;
          B:=J;
        End;
      End;
  Inc(NSplit);
  With Split[NSplit] Do Begin
    P:=1;
    Q:=0;
    A:=0;
    B:=0;
  End;
  QSortSplit(1,NSplit);
  NRes:=0;
  If NDiff=0 Then Begin
    Inc(NRes);
    With Res[NRes] Do Begin
      L:=-Infinity;
      R:=GetValue(Split[1]);
    End;
  End;
  Start:=1;
  For I:=1 To NSplit Do Begin
    If (I>1) And (CompareSplit(Split[I],Split[I-1])=2) Then Begin
      WorkOut(Start,I-1);
      If NDiff=0 Then Begin
        Inc(NRes);
        With Res[NRes] Do Begin
          L:=GetValue(Split[I-1]);
          R:=GetValue(Split[I]);
        End;
      End;
      Start:=I;
    End;
  End;
End;

Procedure Save;
Var
  I:Integer;
Begin
  ReWrite(Output,OutFile);
  WriteLn(NRes);
  For I:=1 To NRes Do Begin
    If I>1 Then Write(' ');
    If Res[I].L<-Infinity/2 Then Write('*') Else Write(Res[I].L:0:18);
    Write(' ');
    If Res[I].R>Infinity/2 Then Write('*') Else Write(Res[I].R:0:18);
  End;
  WriteLn;
  Close(Output);
End;

begin
  RandSeed:=543195731;
  Load;
  Solve;
  Save;
end.
