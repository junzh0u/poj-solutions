const
	maxn = 100000;
var
	i, n: longint;
	a: array [1..maxn] of int64;
	q: longint;
	l, r, k: longint;
begin
	randseed := 19841984;

	n := 100000;
	l := n div 2;
	r := n div 2 + 1;
	k := 1;
	for i := 1 to n do begin  
		if i mod 2 = 0 then begin
			a[l] := k;
			dec(l);
		end else begin
			a[r] := k;
			inc(r);
		end;
		inc(k);
	end;

	writeln(n);
	for i := 1 to n do begin
		write(a[i]);
		if i < n then
			write(' ');
	end;
	writeln;
end.
