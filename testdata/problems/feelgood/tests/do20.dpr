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
		if i mod 2 = 0 then 
			a[i] := 1000000
		else if abs(i - n div 2) > n div 4 then
			a[i] := 19 + random(40)
		else	
			a[i] := 40 + random(40);
	end;

	writeln(n);
	for i := 1 to n do begin
		write(a[i]);
		if i < n then
			write(' ');
	end;
	writeln;
end.
