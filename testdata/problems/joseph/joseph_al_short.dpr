var sum, p:int64;
    n, k, c, r:integer;

begin
  reset (input, 'joseph.in'); rewrite (output, 'joseph.out');
  read (n, k); sum:=0;
  repeat
    c:=k div n; r:=k mod n; p:=(n-r-1) div (c+1);
    inc (sum, (p+1)*(p*c+2*r) shr 1);
    dec (n, p+1);
  until n=0;
  writeln (sum);
end.