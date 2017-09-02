{q+,r+,s+,o-}

uses
  sysutils, math;

procedure writelnip(q: integer);
begin
  write(q shr 24, '.');
  write(q shr 16 and 255, '.');
  write(q shr 8 and 255, '.');
  writeln(q and 255);
end;

var
  s, ss: string;
  k, n, q, qq, m, i, j: integer;

begin
  reset(input, 'ip.in');
  rewrite(output, 'ip.out');

  readln(n);
  for i := 1 to n do begin
    readln(s);
    s := s + '.';
    q := 0;
    for j := 1 to 4 do begin
      k := pos('.', s);
      q := q shl 8 + strtoint(copy(s, 1, k - 1));
      s := copy(s, k + 1, 1000);
    end;

    if i = 1 then begin
      qq := q;
      m := -1;
    end else begin
      while (q and m <> qq and m) do m := m and (m - 1);
    end;
  end;

  writelnip(qq and m);
  writelnip(m);
end.                          