uses sysutils;

var
	i, j, n : integer;
	lo, hi : char;
	sym : array[1..300] of char;
	deg : array[1..300] of integer;
	children : array[1..300,1..300] of integer;

procedure traverse(i : integer);
var
	j : integer;
	
begin
	write(sym[i]);
	for j := 1 to deg[i] do begin
		traverse(children[i, j]);
		write(sym[i]);
	end;	
end;
	                                
begin
	n := strtoint(paramstr(1));
	lo := paramstr(2)[1];
	hi := paramstr(3)[1];
	randseed := strtoint(paramstr(4));

	for i := 1 to n do
		sym[i] := chr(ord(lo) + random(ord(hi) - ord(lo) + 1));

	fillchar(deg, sizeof(deg), 0);
	for i := 2 to n do begin
		j := 1 + random(i - 1);
		inc(deg[j]);
		children[j,deg[j]] := i;
	end;		
	
	traverse(1);
	writeln;
end.
