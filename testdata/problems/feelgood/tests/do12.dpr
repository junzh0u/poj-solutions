const
	maxn = 100000;
var
	i, n: longint;
	a: array [1..maxn] of int64;
begin
	randseed := 19841984;

	n := 99999;
	for i := 1 to n do begin
		if i mod 2 = 1 then
			a[i] := n div 2 + 1
		else
			a[i] := 1;
	end;

	writeln(n);
	for i := 1 to n do begin
		write(a[i]);
		if i < n then
			write(' ');
	end;
	writeln;
end.
