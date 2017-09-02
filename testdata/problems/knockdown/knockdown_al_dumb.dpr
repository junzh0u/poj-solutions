{$O-,Q+,R+}
uses SysUtils, Math, Windows;
const MaxN=20;
      MaxP=89;
      MaxL=179;
      inf=1e1000;

procedure readr (var x:integer; a, b:integer);
begin
  assert (not seekeoln); read (x); assert ((a<=x) and (x<=b));
end;

procedure readl;
var c:char;
begin
  read (c); assert (c=#13); read (c); assert (c=#10);
end;


procedure xyz (p, l:extended; var x, y, z:extended);
begin
  p:=p/180*pi; l:=l/180*pi;
  x:=cos (l)*cos (p);
  y:=sin (l)*cos (p);
  z:=sin (p);
end;


function dist (p1, l1, p2, l2:extended):extended;
var scal, x1, y1, x2, y2, z1, z2:extended;
begin
  xyz (p1, l1, x1, y1, z1);
  xyz (p2, l2, x2, y2, z2);
  scal:=x1*x2+y1*y2+z1*z2;
  if scal>1 then Result:=0 else if scal<-1 then Result:=pi else
  Result:=arccos (scal);
  if Result<0 then Result:=Result+pi;
end;


var i, n:integer;
    p, l:array [1..MaxN] of integer;
    start:cardinal;
    pp, ll, cur, max, min:extended;

begin
  reset (input, 'knockdown.in'); rewrite (output, 'knockdown.out');
  readr (n, 1, MaxN); readl;
  for i:=1 to n do begin
    readr (p[i], -MaxP, MaxP); readr (l[i], -MaxL, MaxL+1);
    readl;
  end;
  assert (eof);
  start:=GetTickCount;
  max:=0;
  repeat
    pp:=random*random*90; ll:=random*360-180;
    if random<0.5 then pp:=-pp;
    min:=inf;
    for i:=1 to n do begin
      cur:=dist (p[i], l[i], pp, ll);
      if cur<min then min:=cur;
    end;
    if min>max then max:=min;
  until GetTickCount-start>2900;
  writeln (max:0:20);
end.
