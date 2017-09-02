uses sysutils;

var
	n, i : integer;
	
begin
	n := strtoint(paramstr(1));
	
	writeln(n, ' ', n - 1);
	for i := 1 to n - 1 do
		writeln(1, ' ', i + 1);
end.
