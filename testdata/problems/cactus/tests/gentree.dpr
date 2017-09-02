uses sysutils;
var
	n, i : integer;
			
begin
	n := strtoint(paramstr(1));
	randseed := strtoint(paramstr(2));
	writeln(n, ' ', n - 1);
	for i := 2 to n do begin
		writeln(i, ' ', random(i - 1) + 1);
	end;
end.
