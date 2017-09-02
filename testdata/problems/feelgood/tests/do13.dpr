const
	maxn = 100000;
var
	i, n: longint;
	a: array [1..maxn] of int64;
begin
	randseed := 19841984;

	n := 100000;
	for i := 1 to n do begin
		if i mod 2 = 1 then
			a[i] := random(n)
		else
			a[i] := random(10);
	end;

	writeln(n);
	for i := 1 to n do begin
		write(a[i]);
		if i < n then
			write(' ');
	end;
	writeln;
end.
