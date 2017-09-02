var
	i: longint;

procedure rgen(l: longint);
var
	r: longint;
begin
	r := random(2);
	if r = 0 then
		write('(');
	if l <> 0 then begin
    	rgen(l - 1);
      	case random(4) of
      		0: write('-');
      		1: write('+');
      		2: write('*');
      		3: write('/');
      	end;
    end;
	write('a');
    if r = 0 then
    	write(')');
end;

begin
	randseed := 92374024;
	rgen(300);
   	writeln;
end.