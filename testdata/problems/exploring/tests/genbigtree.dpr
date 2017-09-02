uses sysutils;

var
	i, n : integer;
	
begin
	n := strtoint(paramstr(1));
	randseed := strtoint(paramstr(2));
	write('Z');
	for i := 1 to n do begin
		write(chr(ord('A') + random(26)));
		write('Z');
	end;	
	writeln;
end.
