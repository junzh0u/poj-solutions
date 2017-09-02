uses sysutils;

var
	n, k, m, i, j : integer;
	
begin
	n := strtoint(paramstr(1));
	k := strtoint(paramstr(2));
        m := (n - 1) div k;
	n := k * m + 1;
	writeln(n, ' ', (k + 1) * m);
	for i := 0 to m - 1 do begin
		writeln(1, ' ', k * i + 2);
		for j := 1 to k - 1 do 
			writeln(k * i + j + 1, ' ', k * i + j + 2);
		writeln(k * i + k + 1, ' ', 1);
	end;
end.
