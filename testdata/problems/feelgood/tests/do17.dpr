const
	maxn = 100000;
var
	i, n: longint;
	a: array [1..maxn] of int64;
	q: longint;
begin
	randseed := 19841984;

	n := 100000;
	for i := 1 to n do begin  
		q := abs(i - (n div 2));
		a[i] := q;
	end;

	writeln(n);
	for i := 1 to n do begin
		write(a[i]);
		if i < n then
			write(' ');
	end;
	writeln;
end.
