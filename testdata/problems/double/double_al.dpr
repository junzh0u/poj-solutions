{$O-,Q+,R+}
uses SysUtils;

const S=5;
      X=9;
      PC=S*S*S*S*S*S*S*S*S;

const CV:string='6789TJQKA';
      CS:string='SCDH';

var F:array [1..9, 1..4] of boolean;

procedure card (var v:integer);
var s:integer;
    c:char;
begin
  read (c); v:=pos (c, CV); assert (v in [1..9]);
  read (c); s:=pos (c, CS); assert (s in [1..4]);
  assert (not F[v, s]); F[v, s]:=true;
end;

var C:array [0..X-1, 1..S-1] of integer;
    H:array [0..X-1] of integer;
    V:array [0..PC-1] of extended;
    E:array [0..PC-1] of boolean;
        P:array [0..X] of integer;

function rec (N:integer):extended;
var i, j, cnt:integer;
begin
  if not E[N] then begin
    rec:=0; cnt:=0;
    for i:=0 to X-1 do
      for j:=i+1 to X-1 do
        if (H[i]>0) and (H[j]>0) and (C[i, H[i]]=C[j, H[j]]) then begin
          inc (cnt);
          dec (H[i]); dec (H[j]);
          Result:=Result+rec (N-P[i]-P[j]);
          inc (H[i]); inc (H[j]);
        end;
    if cnt=0 then V[N]:=0 else V[N]:=Result/cnt;
    E[N]:=true;
  end;
  rec:=V[N];
end;


var i, j:integer;
    cc:char;

begin
  p[0]:=1; for i:=1 to X do p[i]:=p[i-1]*S;
  reset (input, 'double.in'); rewrite (output, 'double.out');
  for i:=0 to X-1 do begin
    for j:=1 to S-1 do begin
      if j>1 then begin read (cc); assert (cc=#32) end;
      card (c[i, j]);
    end;
    read (cc); assert (cc=#13); read (cc); assert (cc=#10);
  end;
  assert (eof);
  for i:=0 to X-1 do H[i]:=S-1;
  E[0]:=true; V[0]:=1;
  writeln (rec (P[X]-1):0:20);
end.