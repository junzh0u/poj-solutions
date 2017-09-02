uses
  Tools;

var
  i: Integer;

begin
  randseed := 25;
  RandomDeflector;
  for i := 1 to MAX_N div 2 do
    AddRandomDeflectedBomb(3);
  RandomDeflector;
  for i := 1 to MAX_N div 2 do
    AddRandomDeflectedBomb(3);
  Shuffle;
  Done;
end.
