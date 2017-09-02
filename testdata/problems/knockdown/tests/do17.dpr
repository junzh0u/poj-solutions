uses
  Tools;

var
  i, l: Integer;

begin
  randseed := 17;
  l := NextLambda;
  for i := 1 to MAX_N do
    AddRandomBombFixLambda(l);
  Done;
end.
