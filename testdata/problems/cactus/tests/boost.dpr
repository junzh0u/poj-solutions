uses sysutils;

var
	mins, maxs : integer;
	i, j : integer;
	n, m : integer;
	totaln, totalm : integer;
	x, y : array[1..1000000] of integer;
	cp : integer;
	
	sz, st : array[1..20000] of integer;
	
	parent : array[1..20000] of integer;
	renum : array[1..20000] of integer;
	
function getroot(x : integer) : integer;
begin
	if parent[x] <> x then
		parent[x] := getroot(parent[x]);
	result := parent[x];	
end;	

procedure unite(x, y : integer);
begin
	if random < 0.5 then
		parent[x] := y
	else
		parent[y] := x;
end;

function generate(i : integer) : integer;
begin
	result := st[i] + random(sz[i]);
end;

procedure initdsu;
begin
	for i := 1 to 20000 do
		parent[i] := i;
end;
	
begin
	mins := strtoint(paramstr(1));
	maxs := strtoint(paramstr(2));
	cp := strtoint(paramstr(3));
	randseed := strtoint(paramstr(4));

	read(n, m);
	for i := 1 to m do
		read(x[i], y[i]);
		
	initdsu;
			
	for i := 1 to n do begin
		repeat
			sz[i] := mins + random(maxs - mins + 1);
		until sz[i] <> 2;	
		if i = 1 then
			st[i] := 1
		else
			st[i] := st[i-1] + sz[i-1];	
	end;	
	
	for i := 1 to m do begin
		x[i] := generate(x[i]);
		y[i] := generate(y[i]);
		if random(100) < cp then
			unite(x[i], y[i]);
	end;		

	totaln := 0;
	for i := 1 to st[n] + sz[n] - 1 do
		if parent[i] = i then begin
			inc(totaln);
			renum[i] := totaln;
		end;	

	totalm := 0;
	for i := 1 to m do
		if getroot(x[i]) <> getroot(y[i]) then
			inc(totalm);
			
	for i := 1 to n do
		if sz[i] > 1 then
			inc(totalm, sz[i]);
							
	writeln(totaln, ' ', totalm);

	for i := 1 to m do
		if getroot(x[i]) <> getroot(y[i]) then begin
			writeln(renum[getroot(x[i])], ' ', renum[getroot(y[i])]);
		end;
			
	for i := 1 to n do
		if sz[i] > 1 then begin
			for j := 0 to sz[i] - 2 do
				writeln(renum[getroot(st[i] + j)], ' ', renum[getroot(st[i] + j + 1)]);
			writeln(renum[getroot(st[i] + sz[i] - 1)], ' ', renum[getroot(st[i])]);
			writeln;
		end;	
end.
