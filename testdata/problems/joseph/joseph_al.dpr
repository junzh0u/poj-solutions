{$O-,Q+,R+}
uses SysUtils;
const MaxN=1000000000;
      MaxK=1000000000;

procedure readr (var x:integer; a, b:integer);
begin
  assert (not seekeoln); read (x); assert ((a<=x) and (x<=b));
end;

procedure readl;
var c:char;
begin
  read (c); assert (c=#13); read (c); assert (c=#10);
end;

var sum, p:int64;
    i, n, k, c, r:integer;

begin
  reset (input, 'joseph.in'); rewrite (output, 'joseph.out');
  readr (n, 1, MaxN); readr (k, 1, MaxK); readl; assert (eof);
  i:=n; sum:=0;
  repeat
    c:=k div i; r:=k mod i; p:=(i-r-1) div (c+1);
    inc (sum, (p+1)*(p*c+2*r) div 2);
    dec (i, p+1);
  until i<=0;
  assert (i=0);
  writeln (sum);
end.