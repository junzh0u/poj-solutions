uses
	math;
const
	s: string = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
var
	m, n: longint;
	saveb, b, c: array [1..30, 1..30] of longint;
	a: array [1..30, 1..30] of char;
	i, j, k: longint;
	x, y: longint;
	x1, y1, x2, y2: longint;
	tw: array [-900..900] of longint;
	oldcc, cc: longint;
	q: longint;

procedure fill(i, j, rc, cc: longint);
begin
	if (i < 1) or (i > m) or (j < 1) or (j > n) then exit;
	if b[i][j] <> 0 then exit;
	if c[i][j] <> rc then exit;
	b[i][j] := cc;
	fill(i + 1, j, rc, cc);
	fill(i - 1, j, rc, cc);
	fill(i, j - 1, rc, cc);
	fill(i, j + 1, rc, cc);
end;

begin
	m := 20;
	n := 19;
	randseed := 5574777;

	for i := 1 to m do begin
		for j := 1 to n do begin
			b[i][j] := 1;
		end;
	end;

	while true do begin
		saveb := b;
		case random(4) of
			0: begin
        		x := random(m) + 1;
        		y := random(n) + 1;
        		case random(4) of
        			0: begin
        				x1 := 1;
        				y1 := 1;
        				x2 := x;
        				y2 := y;
        			end;
        			1: begin
        				x1 := x;
        				y1 := 1;
        				x2 := m;
        				y2 := y;
        			end;
        			2: begin
        				x1 := 1;
        				y1 := y;
        				x2 := x;
        				y2 := n;
        			end;
        			3: begin
        				x1 := x;
        				y1 := y;
        				x2 := m;
        				y2 := n;
        			end;
        		end;
        	end;
        	1: begin
        		repeat
        			x1 := random(m) + 1;
        			x2 := random(m) + 1;
        			y1 := random(n) + 1;
        			y2 := random(n) + 1;
        		until (x1 <= x2) and (y1 <= y2) and (x2 <= x1 + 5) and (y2 <= y1 + 5);
        	end;
        	2: begin
        		repeat
        			x1 := random(m) + 1;
        			x2 := random(m) + 1;
        			y1 := random(n) + 1;
        			y2 := random(n) + 1;
        		until (x1 <= x2) and (y1 <= y2) and ((x1 = 1) or (x2 = m) or (y1 = 1) or (y2 = n));
        	end;
        	3: begin
        		repeat
        			x1 := random(m) + 1;
        			x2 := random(m) + 1;
        			y1 := random(n) + 1;
        			y2 := random(n) + 1;
        		until (x1 <= x2) and (y1 <= y2) and (x1 = x2) or (y1 = y2);
        	end;
        end;

        q := -100;
        if random(6) <> 0 then begin
    		for i := x1 to x2 do begin
    			for j := y1 to y2 do begin
    				b[i][j] := q;
    			end;
    		end;
    		dec(q);
        end else begin
    		for i := x1 to x2 do begin
    			for j := y1 to y2 do begin
    				b[i][j] := -b[i][j];
    			end;
    		end;
    	end;

		oldcc := cc;
		cc := 0;
		c := b;
		fillchar(b, sizeof(b), 0);
		for i := 1 to m do begin
			for j := 1 to n do begin
				if b[i][j] = 0 then begin
					inc(cc);
					fill(i, j, c[i][j], cc);
				end;
			end;
		end;
		if cc > 30 then break;
	end;
	b := saveb;

	for i := 1 to m do begin
		for j := 1 to n do begin
			a[i][j] := s[b[i][j]];
		end;
	end;

	writeln(m, ' ', n);
	for i := 1 to m do begin
		for j := 1 to n do begin
			write(a[i][j]);
		end;
		writeln;
	end;
end.