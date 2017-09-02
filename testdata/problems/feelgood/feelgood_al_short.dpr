uses Math;
const MaxN=100000;
      MaxV=1000000;
      Size=131072;

var m, s:array [1..2*Size-1] of int64;
    i, x, y, z, l, r, ml, mr:integer;
    sum, mx, n:int64;

begin
  reset (input, 'feelgood.in'); rewrite (output, 'feelgood.out');
  read (n);
  for i:=Size+1 to Size+n do read (m[i]);
  m[Size]:=-1; m[Size+n+1]:=-1;
  for i:=Size to Size+n+1 do s[i]:=m[i];
  for i:=Size-1 downto 1 do begin
    s[i]:=s[2*i]+s[2*i+1];
    m[i]:=min (m[2*i], m[2*i+1]);
  end;
  mx:=-1;
  for i:=1 to n do begin
    x:=Size+i; y:=x;
    repeat
      if odd (y) then y:=y shr 1 else dec (y)
    until m[y]<m[x];
    while y<Size do begin
      y:=y*2; if m[y+1]<m[x] then inc (y)
    end;
    z:=x;
    repeat
      if odd (z) then inc (z) else z:=z shr 1
    until m[z]<m[x];
    while z<Size do begin
      z:=z*2; if m[z]>=m[x] then inc (z)
    end;
    inc (y); dec (z); l:=y-Size; r:=z-Size;
    sum:=0;
    repeat
      if odd (y) then begin inc (sum, s[y]); inc (y) end;
      if not odd (z) then begin inc (sum, s[z]); dec (z) end;
      y:=y shr 1; z:=z shr 1;
    until y>z;
    if sum*s[x]>mx then begin
      mx:=sum*s[x]; ml:=l; mr:=r;
    end;
  end;
  writeln (mx);
  writeln (ml, ' ', mr);
end.              