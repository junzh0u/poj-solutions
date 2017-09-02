uses
  Tools;

var
  i: Integer;

begin
  randseed := 14;
  for i := 1 to MAX_N do
    AddRandomBomb;
  Done;
end.
