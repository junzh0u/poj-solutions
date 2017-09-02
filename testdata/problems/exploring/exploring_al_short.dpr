const MaxL=300;
      Modulo=1000000000;

var f:array [1..MaxL, 0..MaxL] of int64;
    s:string;
    i, j, k:integer;

begin
  reset (input, 'exploring.in'); rewrite (output, 'exploring.out');
  readln (s);
  for i:=1 to length (s) do f[i, 1]:=1;
  for j:=2 to length (s) do
    for i:=1 to length (s)-j+1 do
      for k:=1 to j-1 do if s[i]=s[i+k] then 
        f[i, j]:=(f[i, j]+f[i+1, k-1]*f[i+k, j-k]) mod Modulo;
  writeln (f[1, length (s)]);
end.