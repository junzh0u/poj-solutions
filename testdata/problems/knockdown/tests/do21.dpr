uses
  Tools;

var
  i: Integer;

begin
  randseed := 21;
  RandomDeflector;
  for i := 1 to MAX_N do
    AddRandomDeflectedBomb(3.05);
  Done;
end.
