uses sysutils;

var
	i, n, k, m : integer;

	function map(i : integer) : integer;
	begin
		if i = n + 1 then
			result := 1
		else
			result := i;
	end;
		
begin
	n := strtoint(paramstr(1));
	if odd(n + 1) then begin
		k := (n + 1) div 2;
		m := 3 * k;
	end else begin
		k := (n + 1) div 2 - 2;
		m := 3 * k + 4;
	end;
	
	writeln(n, ' ', m);

	for i := 1 to k do begin
		writeln(map(2 * i - 1), ' ', map(2 * i));
		writeln(map(2 * i + 1), ' ', map(2 * i));
		writeln(map(2 * i - 1), ' ', map(2 * i + 1));
	end;

	if not odd(n + 1) then begin
		writeln(map(2 * k + 1), ' ', map(2 * k + 2));
		writeln(map(2 * k + 2), ' ', map(2 * k + 3));
		writeln(map(2 * k + 3), ' ', map(2 * k + 4));
		writeln(map(2 * k + 1), ' ', map(2 * k + 4));
	end;
end.
