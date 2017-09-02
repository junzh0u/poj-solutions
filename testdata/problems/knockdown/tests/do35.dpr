// Randomly rotated Cube (8 pts)
uses
  Tools;

var
  i, j, k: Integer;

begin
  randseed := 35;
  RandomRotation;
  for i := -1 to 1 do
    if i <> 0 then
      for j := -1 to 1 do
        if j <> 0 then
          for k := -1 to 1 do
            if k <> 0 then
              AddBombR(i, j, k);
  Shuffle;
  Done;
end.
