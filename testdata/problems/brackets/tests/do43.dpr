var
	i: longint;

procedure rgen(l: longint);
var
	r: longint;
begin
	r := random(2);
	if r = 0 then
		write('(');
	write('a');
	if l <> 0 then begin
      	case random(4) of
      		0: write('-');
      		1: write('+');
      		2: write('*');
      		3: write('/');
      	end;
      	rgen(l - 1);
    end;
    if r = 0 then
    	write(')');
end;

begin
	randseed := 92374024;
	rgen(300);
   	writeln;
end.