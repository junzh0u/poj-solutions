uses
  Tools;

var
  i: Integer;

begin
  randseed := 20;
  RandomDeflector;
  for i := 1 to MAX_N do
    AddRandomDeflectedBomb(2.5);
  Done;
end.
