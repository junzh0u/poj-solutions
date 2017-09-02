uses
    sysutils;

var
	tp, bp, apmp: longint;

// l: left possible chars, d: need to close brackets;
procedure gen(var l: longint; d, q: longint; allowpm: boolean);
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
  	gen(l, d + ord(r = 0), q - 1, allowpm or (random(apmp) = 0));
  	if r = 0 then begin
  		write(')');
  		dec(l);
  	end;

  	while l > d + 3 do begin
  		if random(10) = 0 then exit;
  		if allowpm then begin
          	case random(4) of
          		0: write('-');
          		1: write('+');
          		2: write('*');
          		3: write('/');
          	end;
        end else begin
          	case random(2) of
          		0: write('*');
          		1: write('/');
          	end;
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
      	gen(l, d + ord(r = 0), q - 1, boolean(random(apmp) = 0));
      	if r = 0 then begin
      		write(')');
      		dec(l);
      	end;
  	end;
end;

var
	l: longint;

begin
	randseed := 9199111;
	l := 1000;
	bp := 2;
	tp := 20;
	apmp := 30;
	gen(l, 0, 10, true);
	writeln;
end.
