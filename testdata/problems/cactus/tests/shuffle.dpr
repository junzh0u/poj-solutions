uses sysutils;

const
	maxn = 20000;

var
	n, m, i, j, u, v : integer;
	p : array[1..maxn] of integer;
	used : array[1..maxn] of boolean;

begin
	randseed := strtoint(paramstr(1));

	read(n, m);

	fillchar(used, sizeof(used), false);
	for i := 1 to n do begin
		repeat
			j := 1 + random(n);
		until not used[j];
		used[j] := true;
		p[i] := j;
	end;

	writeln(n, ' ', m);
	for i := 1 to m do begin
		read(u, v);
		writeln(p[u], ' ', p[v]);
	end;
end.
