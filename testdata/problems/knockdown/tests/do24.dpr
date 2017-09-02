uses
  Tools;

var
  i: Integer;

begin
  randseed := 24;
  RandomDeflector;
  for i := 1 to MAX_N div 2 do
    AddRandomDeflectedBomb(3);
  dx := -dx;
  dy := -dy;
  dz := -dz;
  for i := 1 to MAX_N div 2 do
    AddRandomDeflectedBomb(3);
  Shuffle;
  Done;
end.
