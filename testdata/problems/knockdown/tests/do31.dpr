// Dodecahedron
uses
  Tools;

var
  g: Double;
  i, j, k: Integer;

begin
  randseed := 31;
  g := (1 + Sqrt(5)) / 2;
  for i := -1 to 1 do
    if i <> 0 then
      for j := -1 to 1 do
        if j <> 0 then begin
          AddBomb(0, i / g, j * g);
          AddBomb(i / g, j * g, 0);
          AddBomb(j * g, 0, i / g);
          for k := -1 to 1 do
            if k <> 0 then
              AddBomb(i, j, k);
        end;
  Shuffle;
  Done;
end.
