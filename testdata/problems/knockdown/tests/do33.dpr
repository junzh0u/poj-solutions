// Randomly Rotated Cube (8 pts) + centers of its facets (6 pts)
uses
  Tools;

var
  i, j, k: Integer;

begin
  randseed := 33;
  RandomRotation;
  for i := -1 to 1 do
    if i <> 0 then begin
      AddBombR(i, 0, 0);
      AddBombR(0, i, 0);
      AddBombR(0, 0, i);
      for j := -1 to 1 do
        if j <> 0 then 
          for k := -1 to 1 do
            if k <> 0 then
              AddBombR(i, j, k);
    end;
  Shuffle;
  Done;
end.
