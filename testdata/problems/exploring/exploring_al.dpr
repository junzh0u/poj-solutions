{$O-,Q+,R+}
uses SysUtils;
const MaxL=300;
      Modulo=1000000000;

var f:array [1..MaxL, 1..MaxL] of int64;
    s:string;

function rec (b, e:integer):int64;
var i:integer;
begin
  if f[b, e]<0 then begin
    f[b, e]:=0;
    for i:=b+1 to e do 
      if s[i]=s[b] then 
        f[b, e]:=(f[b, e]+rec (b+1, i-1)*rec (i, e)) mod Modulo;
  end;
  Result:=f[b, e]
end;

var i:integer;

begin
  reset (input, 'exploring.in'); rewrite (output, 'exploring.out');
  readln (s);
  assert ((length (s)>=1) and (length (s)<=MaxL));
  for i:=1 to length (s) do assert (s[i] in ['A'..'Z']);
  assert (eof);
  fillchar (f, sizeof (f), 255);
  for i:=1 to length (s) do f[i, i]:=1;
  writeln (rec (1, length (s)));
end.