var
	i: longint;
begin
	randseed := 92374024;
	for i := 1 to 249 do
	begin
		write('a');
		case random(4) of
			0: write('-');
			1: write('+');
			2: write('*');
			3: write('/');
		end;
		write('(');
	end;
   	write('b+c');
   	for i := 1 to 249 do
   		write(')');
   	writeln;
end.