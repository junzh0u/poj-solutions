uses sysutils;

var
	n, i : integer;
	ch : char;
	
begin
	n := strtoint(paramstr(1));
	ch := paramstr(2)[1];
	
	for i := 1 to n do
		write(ch);
	writeln;	
end.
