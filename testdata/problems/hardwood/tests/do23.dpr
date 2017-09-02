uses
	math;
const
	s: string = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
var
	m, n: longint;
	b, c: array [1..30, 1..30] of longint;
	a: array [1..30, 1..30] of char;
	i, j, k: longint;
	x, y: longint;
	x1, y1, x2, y2: longint;
	tw: array [-900..900] of longint;
	cc: longint;
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
	n := 20;
	randseed := 9023740;

	for i := 1 to m do begin
		for j := 1 to n do begin
			b[i][j] := 1;
		end;
	end;

	for k := 1 to 10 do begin
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

		for i := x1 to x2 do begin
			for j := y1 to y2 do begin
				b[i][j] := -b[i][j];
			end;
		end;

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
		if cc > length(s) then break;
	end;

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