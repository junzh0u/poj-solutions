uses sysutils;

var
	i, k : integer;
	m, n : integer;
	x, y : array[1..1000000] of integer;
	u, v : integer;

function check(u, v : integer) : boolean;	
var
	i : integer;
	
begin
	result := false;
	if u = v then
		exit;
	for i := 1 to m do
		if (u = x[i]) and (v = y[i]) or (u = y[i]) and (v = x[i]) then
			exit;
	result := true;
end;

begin
	k := strtoint(paramstr(1));
	randseed := strtoint(paramstr(2));
	
	read(n, m);
	writeln(n, ' ', m + k);
	for i := 1 to m do begin
		read(x[i], y[i]);
		writeln(x[i], ' ', y[i]);
	end;

	for i := 1 to k do begin
		repeat
			u := random(n) + 1;
			v := random(n) + 1;
		until check(u, v);
		writeln(u, ' ', v);	
	end;	
end.
