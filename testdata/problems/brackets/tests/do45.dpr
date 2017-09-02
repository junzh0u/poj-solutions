var
	i: longint;
begin
	for i := 1 to 499 do begin
		write('a');
		case random(4) of
			0: write('-');
			1: write('+');
			2: write('*');
			3: write('/');
		end;
	end;
	write('a');
   	writeln;
end.