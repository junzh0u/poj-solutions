program d;
{$APPTYPE CONSOLE}
{$R-,Q-,S-,H+,O-}

uses
  Math, SysUtils;

Type
  Integer=LongInt;
  Real=Extended;

Const
  TaskID='double';
  InFile=TaskID+'.in';
  OutFile=TaskID+'.out';
  Num=9;
  MaxH=4;
  NumState=5*5*5*5*5*5*5*5*5;

Var
  Seq:Array[1..Num,1..MaxH]Of Char;
  Prob:Array[0..NumState-1]Of Real;
  P5:Array[0..Num]Of Integer;
  Res:Real;

Procedure Load;
Var
  I,J:Integer;
  Ch:Char;
Begin
  ReSet(Input,InFile);
  For I:=1 To Num Do Begin
    For J:=1 To MaxH Do Begin
      If Not SeekEOF Then;
      Read(Ch);
      Seq[I,J]:=Ch;
      Read(Ch);
    End;
  End;
  Close(Input);
End;

Procedure Solve;
Var
  Total,Cnt,Cur,I,J,A,B:Integer;
  Am:Array[1..Num]Of Integer;
  Pr:Real;
Begin
  Prob[0]:=1;
  P5[0]:=1;
  For I:=1 To Num Do P5[I]:=P5[I-1]*5;
  For I:=1 To NumState-1 Do Begin
    Cur:=I;
    Total:=0;
    For J:=1 To Num Do Begin
      Am[J]:=Cur Mod 5;
      Inc(Total,Am[J]);
      Cur:=Cur Div 5;
    End;
    If Odd(Total) Then Continue;
    Cnt:=0;
    Pr:=0;
    For A:=1 To Num Do If Am[A]>0 Then
      For B:=A+1 To Num Do If Am[B]>0 Then
        If Seq[A,Am[A]]=Seq[B,Am[B]] Then Begin
          Inc(Cnt);
          Pr:=Pr+Prob[I-P5[A-1]-P5[B-1]];
        End;
    If Cnt=0 Then Prob[I]:=0 Else Prob[I]:=Pr/Cnt;
  End;
  Res:=Prob[NumState-1];
End;

Procedure Save;
Begin
  ReWrite(Output,OutFile);
  WriteLn(Res:0:18);
  Close(Output);
End;

begin
  Load;
  Solve;
  Save;
end.
