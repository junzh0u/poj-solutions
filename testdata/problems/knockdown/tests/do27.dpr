uses
  Tools;

var
  i: Integer;

begin
  randseed := 27;
  RandomDeflector;
  for i := 1 to MAX_N div 3 do
    AddRandomDeflectedBomb(3.05);
  RandomDeflector;
  for i := 1 to MAX_N div 3 do
    AddRandomDeflectedBomb(3.05);
  RandomDeflector;
  for i := 1 to MAX_N - (2 * (MAX_N div 3)) do
    AddRandomDeflectedBomb(3.05);
  Shuffle;
  Done;
end.
