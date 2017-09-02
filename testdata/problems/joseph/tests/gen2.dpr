var i, x, y:integer;
begin
  randomize;
  for i:=1 to 10 do begin
    x:=random (10000000)+1;
    y:=random (99)+1;
    writeln (x, ' ', x*y+random (3)-1);
  end;
end.