uses sysutils;

var
	s : string;
	
begin
	while not eof do begin
		readln(s);
		s := trim(s);
		write(s);
	end;
	writeln;	
end.
