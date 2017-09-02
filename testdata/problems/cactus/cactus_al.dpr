{$O-,Q+,R+}
{$MAXSTACKSIZE 239171717}
uses SysUtils;
const MaxN=20000;
      MaxK=1000;
      MaxM=1000;
      MaxE=MaxM*(MaxK-1)*2;

procedure readr (var x:integer; a, b:integer);
begin
  assert (not seekeoln); read (x); assert ((a<=x) and (x<=b));
end;

procedure readl;
var c:char;
begin
  read (c); assert (c=#13); read (c); assert (c=#10);
end;


procedure no;
begin
  writeln (0); halt
end;


var b, e:array [1..MaxE] of integer;

procedure qsort (l, r:integer);
var i, j, xb, xe, y:integer;
begin
  if l>=r then exit;
  i:=l; j:=r; xb:=b[(l+r) shr 1]; xe:=e[(l+r) shr 1];
  repeat              
    while (b[i]<xb) or ((b[i]=xb) and (e[i]<xe)) do inc (i);
    while (xb<b[j]) or ((xb=b[j]) and (xe<e[j])) do dec (j);
    if i<=j then begin
      y:=b[i]; b[i]:=b[j]; b[j]:=y;
      y:=e[i]; e[i]:=e[j]; e[j]:=y;
      inc (i); dec (j);
    end;
  until i>j;
  qsort (l, j);
  qsort (i, r);
end;

var s, l, c, z, t:array [1..MaxN] of integer;
    f:array [1..MaxN] of boolean;
    cc, tc:integer;

function dfs (x, p:integer; var rc, rv:integer):integer;
var i, H, sum, trc, trv:integer;
begin
  //writeln (x);
  //writeln (x, ' ', p);
  sum:=0;
  f[x]:=true;
  inc (tc); t[x]:=tc; Result:=tc; rc:=0; rv:=0;
  for i:=s[x] to s[x]+l[x] do begin
    if not f[e[i]] then begin
      H:=dfs (e[i], x, trc, trv);
      inc (c[x], c[e[i]]);
      if (H>=t[x]) then begin
        if c[x]>1 then no;
        if trv=x then begin inc (cc); z[cc]:=trc end;
      end else begin
        inc (sum, c[x]);
        rc:=trc;
        rv:=trv;
      end;
      c[x]:=0;
      if H<Result then Result:=H;
    end else if (t[e[i]]<t[x]) and (e[i]<>p) then begin
      //writeln (e[i], ' ', t[e[i]], ' ', t[x]);
      inc (c[e[i]]); rv:=e[i]; rc:=2;
      if t[e[i]]<Result then Result:=t[e[i]];
    end;
  end;
  c[x]:=sum;
  if rv<>0 then inc (rc);
end;


const Base=10000;
      LongL=1600;
type Long=array [0..LongL] of integer;

procedure muls (var a:long; s:integer);
var i:integer;
    cy:int64;
begin
  cy:=0;
  for i:=1 to a[0]+1 do begin
    cy:=a[i]*s+cy;
    a[i]:=cy mod Base;
    cy:=cy div Base;
  end;
  while a[a[0]+1]>0 do inc (a[0]);
end;


procedure writel (var a:long);
var i:integer;
begin
  write (a[a[0]]);
  for i:=a[0]-1 downto 1 do write (format ('%.4d', [a[i]])); writeln;
end;


var i, j, u, v, n, m, k, ec, trc, trv:integer;
    a:long;


begin
  reset (input, 'cactus.in'); rewrite (output, 'cactus.out');
  readr (n, 1, MaxN); readr (m, 1, MaxM); readl;
  ec:=0; cc:=0;
  for i:=1 to m do begin
    readr (k, 1, MaxK); readr (u, 1, n);
    for j:=2 to k do begin
      readr (v, 1, n); assert (u<>v); //!!!
      inc (ec); b[ec]:=u; e[ec]:=v;
      inc (ec); b[ec]:=v; e[ec]:=u;
      u:=v;
    end;
    readl;
  end;
  assert (eof);
  if ec=0 then 
    if n=1 then begin writeln (1); halt end else no;
  qsort (1, ec);
  if b[1]<>1 then no;
  l[1]:=0; s[1]:=1;
  for i:=2 to ec do
    if b[i]=b[i-1] then begin
      assert (e[i]>e[i-1]);
      inc (l[b[i]]) 
    end else begin
      l[b[i]]:=0; s[b[i]]:=i;
    end;
  tc:=0; dfs (1, 0, trc, trv);
  for i:=1 to n do if not f[i] then no;
  a[0]:=1; a[1]:=1;
  for i:=1 to cc do muls (a, z[i]);
  writel (a);
end.