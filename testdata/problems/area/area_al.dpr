{$O-,Q+,R+}
uses SysUtils;
const MaxM=100;
      MaxXY=100;
      MaxP=MaxM*(MaxM-1) shr 1 + 1;
      inf=1000000000;

procedure readr (var x:integer; a, b:integer);
begin
  assert (not seekeoln); read (x); assert ((a<=x) and (x<=b));
end;

procedure readl;
begin
  assert (eoln); assert (not eof); readln
end;

function compare (p1, q1, p2, q2:int64):int64;
begin
  Result:=p1*q2-p2*q1;
end;

var a, b, f:array [1..MaxM] of integer;

function less (p1, q1, d1, e1, p2, q2, d2, e2:int64):boolean;
var res:int64;
begin
  res:=compare (p1, q1, p2, q2);
  if res<0 then Result:=true else
  if res>0 then Result:=false else
  if f[d1]<f[d2] then Result:=true else
  if f[d2]<f[d1] then Result:=false else
  if f[e1]<f[e2] then Result:=true else
  Result:=false;
end;


var p, q, d, e:array [1..MaxP] of integer;


procedure qsort (l, r:integer);
var i, j, xp, xq, xd, xe, y:integer;
begin
  if l>=r then exit;
  i:=l; j:=r; xp:=p[(l+r) shr 1]; xq:=q[(l+r) shr 1]; xd:=d[(l+r) shr 1]; xe:=e[(l+r) shr 1];
  repeat
    while less (p[i], q[i], d[i], e[i], xp, xq, xd, xe) do inc (i);
    while less (xp, xq, xd, xe, p[j], q[j], d[j], e[j]) do dec (j);
    if i<=j then begin
      y:=p[i]; p[i]:=p[j]; p[j]:=y;
      y:=q[i]; q[i]:=q[j]; q[j]:=y;
      y:=d[i]; d[i]:=d[j]; d[j]:=y;
      y:=e[i]; e[i]:=e[j]; e[j]:=y;
      inc (i); dec (j);
    end;
  until i>j;
  qsort (l, j);
  qsort (i, r);
end;


var c:array ['A'..'Z'] of integer;
    ch:array [1..MaxM] of char;
    x, y:array [1..MaxM] of integer;
    pb, qb, pe, qe:array [1..MaxP] of integer;
    cc:char;
    i, j, m, t, pc, pp, qq, ic, tt:integer;
    s:string;
    flag:boolean;

begin
  reset (input, 'area.in'); rewrite (output, 'area.out');
  readr (m, 1, MaxM); readl;
  readln (s); assert (length (s)=m);
  for i:=1 to m do begin
    assert (s[i] in ['A'..'Z']);
    inc (c[s[i]]);
  end;
  pc:=0;
  for i:=1 to m do begin
    assert (not eoln); read (ch[i]); dec (c[ch[i]]);
    readr (x[i], -MaxXY, MaxXY);
    readr (y[i], 1, MaxXY);
    readl;
    for j:=1 to i-1 do begin
      assert ((x[i]<>x[j]) or (y[i]<>y[j]));
      if y[i]<>y[j] then begin
        inc (pc); {x[i]-y[i]*(x[j]-x[i])/(y[j]-y[i])}
        p[pc]:=x[i]*y[j]-y[i]*x[j]; q[pc]:=y[j]-y[i]; 
        if q[pc]<0 then begin p[pc]:=-p[pc]; q[pc]:=-q[pc] end;
        d[pc]:=i; e[pc]:=j;
      end;
    end;
  end;
  assert (eof); for cc:='A' to 'Z' do assert (c[cc]=0);
  for i:=1 to m do a[i]:=i;
  for i:=1 to m do
    for j:=i+1 to m do
      if (y[a[i]]<y[a[j]]) or ((y[a[i]]=y[a[j]]) and (x[a[i]]>x[a[j]])) then begin
        t:=a[i]; a[i]:=a[j]; a[j]:=t;
      end;
  for i:=1 to m do b[a[i]]:=i;
  f:=b;
  for i:=1 to pc do
    if f[d[i]]>f[e[i]] then begin tt:=d[i]; d[i]:=e[i]; e[i]:=tt end;
  qsort (1, pc);
  //for i:=1 to pc do writeln (p[i], ' ', q[i], ' ', d[i], ' ', e[i]);
  inc (pc); p[pc]:=inf; q[pc]:=1; d[pc]:=1; e[pc]:=1;
  pp:=-inf; qq:=1; ic:=0;
  for i:=1 to pc do begin
    if compare (pp, qq, p[i], q[i])<>0 then begin
      flag:=true;
      for j:=1 to m do
        if ch[a[j]]<>s[j] then begin flag:=false; break end;
      if flag then begin
        inc (ic); pb[ic]:=pp; qb[ic]:=qq; pe[ic]:=p[i]; qe[ic]:=q[i];
      end; 
    end;
    a[b[d[i]]]:=e[i]; a[b[e[i]]]:=d[i];
    t:=b[d[i]]; b[d[i]]:=b[e[i]]; b[e[i]]:=t;
    pp:=p[i]; qq:=q[i];
  end;
  writeln (ic);
  for i:=1 to ic do begin
    if pb[i]=-inf then write ('* ') else write (pb[i]/qb[i]:0:20, ' ');
    if pe[i]=inf then write ('* ') else write (pe[i]/qe[i]:0:20, ' ');
  end;
end.