// Randomly rotated Octahedron
uses
  Tools;

var
  i: Integer;

begin
  randseed := 36;
  RandomRotation;
  for i := -1 to 1 do
    if i <> 0 then begin 
      AddBombR(i, 0, 0);
      AddBombR(0, i, 0);
      AddBombR(0, 0, i);
    end;
  Shuffle;
  Done;
end.
