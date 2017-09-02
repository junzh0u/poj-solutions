uses sysutils;

var
	i, n, k, m : integer;

begin
	n := strtoint(paramstr(1));
	if odd(n) then begin
		k := n div 2;
		m := 3 * k;
	end else begin
		k := n div 2 - 2;
		m := 3 * k + 4;
	end;
	
	writeln(n, ' ', m);

	for i := 1 to k do begin
		writeln(2 * i - 1, ' ', 2 * i);
		writeln(2 * i + 1, ' ', 2 * i);
		writeln(2 * i - 1, ' ', 2 * i + 1);
	end;

	if not odd(n) then begin
		writeln(2 * k + 1, ' ', 2 * k + 2);
		writeln(2 * k + 2, ' ', 2 * k + 3);
		writeln(2 * k + 3, ' ', 2 * k + 4);
		writeln(2 * k + 1, ' ', 2 * k + 4);
	end;
end.
