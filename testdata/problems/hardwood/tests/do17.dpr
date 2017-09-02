uses
	math;
const
	s: string = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
var
	m, n: longint;
	a: array [1..30, 1..30] of char;
	i, j: longint;
begin
	m := 20;
	n := 20;

    for i := 1 to m do
    	for j := 1 to n do begin
    		a[i][j] := s[max(m + 1 - i, j)];
    	end;

	writeln(m, ' ', n);
	for i := 1 to m do begin
		for j := 1 to n do begin
			write(a[i][j]);
		end;
		writeln;
	end;
end.