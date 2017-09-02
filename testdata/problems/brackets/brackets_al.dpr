{$O-,Q+,R+}
uses SysUtils;
const MaxLen=1000;

var curchar:char;
    cc:integer;

function nextchar:char;
begin
  Result:=curchar; read (curchar); inc (cc);
  assert (cc<=MaxLen+2);
end;


procedure init;
begin
  cc:=0; nextchar;
end;


type node=record
       l, r:integer;
       op:char;
     end;


var n:array [1..MaxLen] of node;
    nc:integer;


function create (c:char; x, y:integer):integer;
begin
  inc (nc); Result:=nc;
  with n[nc] do begin
    op:=c; l:=x; r:=y;
  end;
end;

function expr:integer; forward;

function muler:integer;
begin
  if curchar='(' then begin nextchar; Result:=expr; assert (nextchar=')') end else begin
    assert (curchar in ['a'..'z']);
    Result:=create (nextchar, 0, 0);
  end;
end;


function adder:integer;
var c:char;
begin
  Result:=muler; 
  while curchar in ['*', '/'] do begin
    c:=nextchar;
    Result:=create (c, Result, muler);
  end;
end;


function expr:integer;
var c:char;
begin
  Result:=adder; 
  while curchar in ['+', '-'] do begin
    c:=nextchar;
    Result:=create (c, Result, adder);
  end;
end;

const rop:array ['*'..'/'] of char=('/', '-', #0, '+', #0, '*');


procedure display (x:integer; p, rev:boolean);
var cp, cr:boolean;
begin
  with n[x] do
    if op in ['a'..'z'] then write (op) else begin
      cp:=op in ['*', '/']; cr:=(p=cp) and rev;
      if p and not cp then write ('(');
      display (l, cp, cr);
      if not cr then write (op) else write (rop[op]);
      display (r, cp, cr xor (op in ['-', '/']));
      if p and not cp then write (')');
    end;
end;


begin
  reset (input, 'brackets.in'); rewrite (output, 'brackets.out');
  init; nc:=0;
  display (expr, false, false); assert (nextchar=#13); assert (curchar=#10); assert (eof);
end.