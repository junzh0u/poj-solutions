{$q+,r+,s+,o-}

uses
  sysutils, math;

var
  m1, m2, m3, m4, mm1, mm2, mm3, mm4, n1, n2, n3: integer;

function ok: boolean;
begin
  assert(m1 >= 0);
  assert(m2 >= 0);
  assert(m3 >= 0);
  assert(m4 >= 0);
  ok := (m1 + m2 + 2*m3 + 3*m4 >= n1) and
        (m1 + 2*m3 >= n2) and
        (m1 + m3 + 2*m4 >= n3);
end;

procedure renew;
begin
  assert(m1 >= 0);
  assert(m2 >= 0);
  assert(m3 >= 0);
  assert(m4 >= 0);
  assert(ok);
  if (m1*3+m2+m3*4+m4*5) < (mm1*3+mm2+mm3*4+mm4*5) then begin
    mm1 := m1;
    mm2 := m2;
    mm3 := m3;
    mm4 := m4;
  end;
end;

begin
  reset(input, 'guards.in');
  rewrite(output, 'guards.out');

  read(n1, n2, n3);

  mm1 := 1000;
  mm2 := 1000;
  mm3 := 1000;
  mm4 := 1000;

  //case1
  m2 := 0;
  for m1 := 0 to 1000 do begin
    m3 := 1000;
    for m4 := 0 to 1000 do begin
      while (m3 > 0) and ok do dec(m3);
      if not ok then inc(m3);
      renew;
    end;
  end;

  //case2
  m1 := 0;
  for m2 := 0 to 1000 do begin
    m3 := 1000;
    for m4 := 0 to 1000 do begin
      while (m3 > 0) and ok do dec(m3);
      if not ok then inc(m3);
      renew;
    end;
  end;


  writeln(mm1*3,' ',mm2,' ',mm3*4,' ',mm4*5);


end.