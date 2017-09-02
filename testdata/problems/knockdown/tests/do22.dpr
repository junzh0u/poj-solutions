uses
  Tools;

var
  i: Integer;

begin
  randseed := 22;
  RandomDeflector;
  for i := 1 to MAX_N do
    AddRandomDeflectedBomb(2);
  Done;
end.
