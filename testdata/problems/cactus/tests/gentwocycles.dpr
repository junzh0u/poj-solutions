uses sysutils;
var
	n, m, i : integer;
		
begin
	n := strtoint(paramstr(1));
	m := strtoint(paramstr(2));
	writeln(n + m - 1, ' ', m + n);
	for i := 1 to m - 1 do
		writeln(i, ' ', i + 1);
	writeln(m, ' ', 1);	
	writeln(1, ' ', m + 1);
	for i := 1 to n - 2 do
		writeln(m + i, ' ', m + i + 1);
	writeln(m + n - 1, ' ', 1);	
end.
