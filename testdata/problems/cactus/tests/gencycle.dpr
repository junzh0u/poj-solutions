uses sysutils;
var
	n, i : integer;
		
begin
	n := strtoint(paramstr(1));
	writeln(n, ' ', n);
	for i := 1 to n - 1 do
		writeln(i, ' ', i + 1);
	writeln(n, ' ', 1);	
end.
