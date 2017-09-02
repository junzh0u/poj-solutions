// Randomly rotated Tetrahedron
uses
  Tools;

begin
  randseed := 37;
  RandomRotation;
  AddBombR(1, 1, 1);
  AddBombR(-1, -1, 1);
  AddBombR(-1, 1, -1);
  AddBombR(1, -1, -1);
  Shuffle;
  Done;
end.
