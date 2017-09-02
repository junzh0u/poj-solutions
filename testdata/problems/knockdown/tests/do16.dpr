uses
  Tools;

var
  i, p: Integer;

begin
  randseed := 1116;
  p := NextPhi;
  for i := 1 to MAX_N do
    AddRandomBombFixPhi(p);
  Done;
end.
