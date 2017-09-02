program b;
{$APPTYPE CONSOLE}
{$R+,Q+,S+,H+,O-}

uses
  Math, SysUtils;

Type
  Integer=LongInt;
  Real=Extended;

Const
  TaskID='brackets';
  InFile=TaskID+'.in';
  OutFile=TaskID+'.out';

Type
  PTree=^TTree;
  TTree=Record
    Op:Char;
    L,R:PTree;
  End;

Var
  S:String;
  Pos:Integer;
  Root:PTree;

Procedure Load;
Begin
  ReSet(Input,InFile);
  ReadLn(S);
  S:=S+'#';
  Close(Input);
End;

Function ParseExpr:PTree; Forward;

Function ParseMultiplier:PTree;
Var
  P:PTree;
Begin
  Case S[Pos] Of
    'a'..'z':Begin
      New(P);
      P^.Op:=S[Pos];
      Inc(Pos);
      P^.L:=Nil;
      P^.R:=Nil;
    End;
    '(':Begin
      Inc(Pos);
      P:=ParseExpr;
      Inc(Pos);
    End;
  End;
  Result:=P;
End;

Function ParseAddent:PTree;
Var
  P,PP:PTree;
Begin
  P:=ParseMultiplier;
  While S[Pos] In ['*','/'] Do Begin
    New(PP);
    PP^.L:=P;
    PP^.Op:=S[Pos];
    Inc(Pos);
    PP^.R:=ParseMultiplier;
    P:=PP;
  End;
  Result:=P;
End;

Function ParseExpr:PTree;
Var
  P,PP:PTree;
Begin
  P:=ParseAddent;
  While S[Pos] In ['-','+'] Do Begin
    New(PP);
    PP^.L:=P;
    PP^.Op:=S[Pos];
    Inc(Pos);
    PP^.R:=ParseAddent;
    P:=PP;
  End;
  Result:=P;
End;

Procedure WriteOutExpr(Root:PTree; First,Inverse:Boolean); Forward;

Procedure WriteOutMultiplier(Root:PTree);
Begin
  Case Root^.Op Of
    'a'..'z':Write(Root^.Op);
    Else Begin
      Write('(');
      WriteOutExpr(Root,True,False);
      Write(')');
    End;
  End;
End;

Procedure WriteOutAddent(Root:PTree; First,Inverse:Boolean);
Begin
  Case Root^.Op Of
    '*','/':Begin
      WriteOutAddent(Root^.L,First,Inverse);
      WriteOutAddent(Root^.R,False,Inverse Xor (Root^.Op='/'));
    End Else Begin
      If Not First Then Case Inverse Of
        False:Write('*');
        True:Write('/');
      End;
      WriteOutMultiplier(Root);
    End;
  End;
End;

Procedure WriteOutExpr(Root:PTree; First,Inverse:Boolean);
Begin
  Case Root^.Op Of
    '+','-':Begin
      WriteOutExpr(Root^.L,First,Inverse);
      WriteOutExpr(Root^.R,False,Inverse Xor (Root^.Op='-'));
    End Else Begin
      If Not First Then Case Inverse Of
        False:Write('+');
        True:Write('-');
      End;
      WriteOutAddent(Root,True,False);
    End;
  End;
End;

Procedure Solve;
Begin
  Pos:=1;
  Root:=ParseExpr;
End;

Procedure Save;
Begin
  ReWrite(Output,OutFile);
  WriteOutExpr(Root,True,False);
  WriteLn;
  Close(Output);
End;

begin
  Load;
  Solve;
  Save;
end.
