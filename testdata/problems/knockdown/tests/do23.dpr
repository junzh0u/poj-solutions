uses
  Tools;

var
  i: Integer;

begin
  randseed := 23;
  RandomDeflector;
  for i := 1 to MAX_N do
    AddRandomDeflectedBomb(1.5);
  Done;
end.
