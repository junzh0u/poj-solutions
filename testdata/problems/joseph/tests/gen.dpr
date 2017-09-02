var i:integer;
begin
  randomize;
  for i:=1 to 5 do begin
    writeln (random (1000000000)+1, ' ', random (1000000000)+1);
  end;
end.