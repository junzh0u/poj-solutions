uses
  Tools;

var
  i: Integer;

begin
  randseed := 13;
  for i := 1 to MAX_N do
    AddRandomBomb;
  Done;
end.
