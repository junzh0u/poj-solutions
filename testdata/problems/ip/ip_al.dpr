{$O-,Q+,R+}
uses SysUtils;
const MaxN=1000;

procedure readr (var x:integer; a, b:integer);
begin
  assert (not seekeoln); read (x); assert ((a<=x) and (x<=b));
end;

procedure readl;
var c:char;
begin
  read (c); assert (c=#13); read (c); assert (c=#10);
end;

var s:string;
    p:integer;

function get:cardinal;
var x:string;
begin
  x:='';
  while s[p] in ['0'..'9'] do begin x:=x+s[p]; inc (p) end;
  assert (s[p]='.'); inc (p);
  Result:=strtoint (x); assert (Result<=255);
end;


function readip:cardinal;
begin
  readln (s); s:=s+'.'; p:=1;
  Result:=get shl 24+get shl 16 + get shl 8 + get;
  assert (p>length (s));
end;

procedure writeip (ip:cardinal);
var i:integer;
begin
  for i:=1 to 3 do begin write (ip shr 24, '.'); ip:=ip shl 8 end;
  writeln (ip shr 24);
end;

var i, n:integer;
    x, sip:cardinal;

begin
  reset (input, 'ip.in'); rewrite (output, 'ip.out');
  readr (n, 1, MaxN); readl; x:=0;
  sip:=readip; for i:=2 to n do x:=x or (sip xor readip);
  assert (eof);
  for i:=1 to 32 do x:=x or (x shr 1);
  writeip (sip and not x);
  writeip (not x);
end.