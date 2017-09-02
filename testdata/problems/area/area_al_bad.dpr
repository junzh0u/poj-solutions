{$O-,Q+,R+}
uses SysUtils;
const MaxM=100;
      MaxXY=100;
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
  Result:=p1*q2-p2*q1
end;


var c:array ['A'..'Z'] of integer;
    ch:array [1..MaxM] of char;
    a, b, x, y:array [1..MaxM] of integer;
    d, e, p, q, pb, qb, pe, qe:array [1..MaxM*MaxM] of integer;
    cc:char;
    i, j, m, t, pc, pp, qq, pn, qn, ic:integer;
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
  pp:=-inf; qq:=1; ic:=0;
  repeat
    pn:=inf; qn:=1;
    for i:=1 to pc do 
      if (compare (pp, qq, p[i], q[i])<0) and (compare (p[i], q[i], pn, qn)<0) then begin
        pn:=p[i]; qn:=q[i]
      end;
    flag:=true;
    for i:=1 to m do
      if ch[a[i]]<>s[i] then begin flag:=false; break end;
    if flag then begin
      inc (ic); pb[ic]:=pp; qb[ic]:=qq; pe[ic]:=pn; qe[ic]:=qn;
    end;  
    if pn=inf then break;
    for i:=1 to pc do
      if compare (pn, qn, p[i], q[i])=0 then begin {change d[i], e[i] in reverse perm}
        a[b[d[i]]]:=e[i]; a[b[e[i]]]:=d[i];
        t:=b[d[i]]; b[d[i]]:=b[e[i]]; b[e[i]]:=t;
      end;
    writeln;
    pp:=pn; qq:=qn;
  until false;
  writeln (ic);
  for i:=1 to ic do begin
    if pb[i]=-inf then write ('* ') else write (pb[i]/qb[i]:0:20, ' ');
    if pe[i]=inf then write ('* ') else write (pe[i]/qe[i]:0:20, ' ');
  end;
end.