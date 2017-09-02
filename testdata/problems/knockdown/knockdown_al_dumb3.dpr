{$O+,Q-,R-}
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

var n:integer;
    p, l:array [1..MaxN] of integer;

function func (pp, ll:extended):extended;
var cur:extended;
    i:integer;
begin
  Result:=inf;
  for i:=1 to n do begin
    cur:=dist (pp, ll, p[i], l[i]);
    if cur<Result then Result:=cur
  end;
end;

function fl (i:integer):extended;
var t1, t2:extended;
begin
  t1:=sqrt (i);
  t2:=sqrt (t1);
  Result:=t1*sqrt (t2);
end;


var i:integer;
    np, cp, nl, cl, cf, nf, max, mp, ml, x, y:extended;
    start:cardinal;

begin
  reset (input, 'knockdown.in'); rewrite (output, 'knockdown.out');
  readr (n, 1, MaxN); readl;
  for i:=1 to n do begin
    readr (p[i], -MaxP, MaxP); readr (l[i], -MaxL, MaxL+1);
    readl;
  end;
  assert (eof);
  start:=GetTickCount; max:=0; mp:=p[1]; ml:=l[1];
  repeat
    cp:=random*random*180-90; cl:=random*360-180; cf:=func (cp, cl);
    //writeln ('---');
    for i:=1 to 90 do begin
      x:=20*(random*2-1)/i; y:=20*(random*2-1)/i;
      np:=cp+x; if np>90 then np:=180-np; if np<-90 then np:=-180-np;
      nl:=cl+y; if nl>180 then nl:=nl-360; if np<-180 then np:=nl+360;
      nf:=func (np, nl);
      if nf>cf then begin cp:=np; cl:=nl; cf:=nf end;
      //writeln (cf:0:20, ' ', cp:0:20,' ', cl:0:20);
      if cf>max then begin max:=cf;mp:=cp; ml:=cl end;
    end;
  until GetTickCount-start>800;
  i:=1; cp:=mp; cl:=ml; cf:=max;
  repeat
    x:=10*(random*2-1)/fl (i); y:=10*(random*2-1)/fl (i);
    np:=cp+x; if np>90 then np:=180-np; if np<-90 then np:=-180-np;
    nl:=cl+y; if nl>180 then nl:=nl-360; if np<-180 then np:=nl+360;
    nf:=func (np, nl);
    if nf>cf then begin cp:=np; cl:=nl; cf:=nf end;
    if cf>max then max:=cf;
    inc (i);
  until GetTickCount-start>2900;
  writeln (max);
end.
