var
	k, n, r: int64;
	i: longint;
begin
	reset(input, 'joseph.in');
	rewrite(output, 'joseph.out');

	read(n, k);
	r := 0;
	for i := 1 to n do begin
		r := r + k mod i;
	end;
	writeln(r);

	close(input);
	close(output);
end.
