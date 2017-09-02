uses sysutils;

var
	n, i : integer;
	
begin
	n := strtoint(paramstr(1));
	for i := 1 to n do
		write('AB');
	write('A');
	writeln;	
end.
