// Icosahedron
uses
  Tools;

var
  g: Double;
  i, j: Integer;

begin
  randseed := 30;
  g := (1 + Sqrt(5)) / 2;
  for i := -1 to 1 do
    if i <> 0 then
      for j := -1 to 1 do
        if j <> 0 then begin
          AddBomb(0, i, j * g);
          AddBomb(i, j * g, 0);
          AddBomb(j * g, 0, i);
        end;
  Shuffle;
  Done;
end.
