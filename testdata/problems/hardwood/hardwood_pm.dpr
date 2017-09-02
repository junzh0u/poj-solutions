{$q+,r+,s+,o-}

uses
  sysutils, math;

type
  ar = array[1..20, 1..20] of char;

var
  n: integer;

function cut(a: ar; nnn: integer): integer;
var
  aa: ar;
  l, i, j: integer;
  b1, b2: boolean;
  s, li, lj: integer;
begin
  for l := 1 to n - 1 do begin
    b1 := false;
    b2 := true;
    for i := 1 to l do
      for j := 1 to n do
        if a[i, j] <> '.' then b2 := false;
    b1 := b1 or b2;
    b2 := true;
    for i := l + 1 to n do
      for j := 1 to n do
        if a[i, j] <> '.' then b2 := false;
    b1 := b1 or b2;
    b2 := false;
    for j := 1 to n do
      if (a[l, j] <> '.') and (a[l, j] = a[l + 1, j]) then b2 := true;
    b1 := b1 or b2;
    if not b1 then begin
      fillchar(aa, sizeof(aa), '.');
      for i := 1 to l do
        for j := 1 to n do aa[i, j] := a[i, j];
      s := cut(aa, 0);
      fillchar(aa, sizeof(aa), '.');
      for i := l + 1 to n do
        for j := 1 to n do aa[i, j] := a[i, j];
      s := s + cut(aa, 0);
      cut := s;
      exit;
    end;
  end;
  for li := 1 to n - 1 do
    for lj := 1 to n - 1 do begin
      b1 := false;
      b2 := true;
      for i := 1 to li do
        for j := 1 to lj do
          if a[i, j] <> '.' then b2 := false;
      b1 := b1 or b2;
      b2 := true;
      for i := 1 to n do
        for j := 1 to n do if (i > li) or (j > lj) then
          if a[i, j] <> '.' then b2 := false;
      b1 := b1 or b2;
      b2 := false;
      for j := 1 to lj do
        if (a[li, j] <> '.') and (a[li, j] = a[li + 1, j]) then b2 := true;
      for i := 1 to li do
        if (a[i, lj] <> '.') and (a[i, lj] = a[i, lj + 1]) then b2 := true;
      b1 := b1 or b2;
      if not b1 then begin
        fillchar(aa, sizeof(aa), '.');
        for i := 1 to li do
          for j := 1 to lj do aa[i, j] := a[i, j];
        s := cut(aa, 0);
        fillchar(aa, sizeof(aa), '.');
        for i := 1 to n do
          for j := 1 to n do if (i > li) or (j > lj) then aa[i, j] := a[i, j];
        s := s + cut(aa, 0);
        cut := s;
        exit;
      end;
    end;
    if (nnn < 3) then begin
      for i := 1 to n do
        for j := 1 to n do
          aa[i, j] := a[n - j + 1, i];
      cut := cut(aa, nnn + 1);
    end else begin
      cut := 1;
      {
      for i := 1 to n do begin
        for j := 1 to n do
          write(a[i, j]);
        writeln;
      end;
      writeln;
      }
    end;
end;

var
  m, i, j: integer;
  a: ar;

begin
  reset(input, 'hardwood.in');
  rewrite(output, 'hardwood.out');

  read(n, m);
  fillchar(a, sizeof(a), '.');
  for i := 1 to n do
    for j := 1 to m do
      if not seekeof then read(a[i, j]);

  n := max(n, m);

  writeln(cut(a, 0));

end.