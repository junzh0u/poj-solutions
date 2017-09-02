uses
  Tools;

var
  i, j: Integer;
  p: array[1..4] of Integer;
  l: array[1..5] of Integer;

begin
  randseed := 29;
  AddRandomBomb;
  p[1] := phi[1];
  l[1] := lambda[1];
  for i := 2 to 4 do begin
    AddRandomBombFixLambda(l[1]);
    p[i] := phi[n];
  end;
  for i := 2 to 5 do begin
    AddRandomBombFixPhi(p[1]);
    l[i] := lambda[n];
  end;
  for i := 2 to 4 do
    for j := 2 to 5 do
      AddBomb(p[i], l[j]);
  Shuffle;
  Done;
end.
