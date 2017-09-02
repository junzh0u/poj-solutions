uses
    sysutils;

var
	tp, bp: longint;

// l: left possible chars, d: need to close brackets;
procedure gen(var l: longint; d, q: longint);
var
	r: longint;
begin
	if (l <= d + 2) or (q = 0) or (random(tp) = 0) then begin
		write(char(ord('a') + random(26)));
		dec(l);
		exit;
	end;

	if l > d + 3 then
	  	r := random(bp)
	else
		r := 1;
  	if r = 0 then begin
  		write('(');
  		dec(l);
  	end;
  	gen(l, d + ord(r = 0), q - 1);
  	if r = 0 then begin
  		write(')');
  		dec(l);
  	end;

  	if l > d + 3 then begin
  		if random(2) = 0 then exit;
      	case random(4) of
      		0: write('-');
      		1: write('+');
      		2: write('*');
      		3: write('/');
      	end;
      	dec(l);

    	if l > d + 3 then
    	  	r := random(bp)
    	else
    		r := 1;
      	if r = 0 then begin
      		write('(');
      		dec(l);
      	end;
      	gen(l, d + ord(r = 0), q - 1);
      	if r = 0 then begin
      		write(')');
      		dec(l);
      	end;
  	end;
end;

var
	l: longint;

begin
	randseed := 902734;
	l := 700;
	bp := 10;
	tp := 50;
	gen(l, 0, 20);
	writeln;
end.
