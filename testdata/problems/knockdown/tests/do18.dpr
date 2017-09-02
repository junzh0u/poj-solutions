uses
  Tools;

var
  i, l: Integer;

begin
  randseed := 18;
  l := NextLambda;
  for i := 1 to MAX_N div 2 do
    AddRandomBombFixLambda(l);
  for i := 1 to MAX_N div 2 do
    AddRandomBombFixLambda((l + 360) mod 360 - 180);
  Shuffle;
  Done;
end.
