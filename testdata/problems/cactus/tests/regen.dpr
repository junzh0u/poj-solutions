{$R+,Q+,I+,O-}
{$apptype CONSOLE}
{$assertions on}

const
	MAX_N = 20000;
	MAX_M = 2* 1000 * 1000;
	MAX_P = 1000;
	
type
	TEdge = record
		tail, head : integer;
		next : integer;
		used : boolean;
	end;
	
			
var
	n, m : integer;
	first : array[1..MAX_N] of integer;
	edges : array[1..MAX_M] of TEdge;
	deg : array[1..MAX_N] of integer;
	x, y : integer;

procedure AddEdge(head, tail : integer);
begin
	inc(m);
	edges[m].tail := tail;
	edges[m].head := head;
	edges[m].next := first[tail];
	first[tail] := m;

	inc(m);
	edges[m].tail := head;
	edges[m].head := tail;
	edges[m].next := first[head];
	first[head] := m;
end;

procedure UseEdge(i : integer);
begin
	edges[i].used := true;
	if odd(i) then
		edges[i+1].used := true
	else
		edges[i-1].used := true;
    dec(deg[edges[i].tail]);
    dec(deg[edges[i].head]);
end;

procedure Cleanup;
var
	i, j : integer;
	
begin
	for i := 1 to m do
		edges[i].used := false;
	for i := 1 to n do begin
		deg[i] := 0;
		j := first[i];
		while j <> 0 do begin
			inc(deg[i]);
			j := edges[j].next;
		end;
	end;		
end;

function GetUnusedEdge(i : integer) : integer;
var
	k : integer;

begin
	k := first[i];
	while k <> 0 do begin
		if not edges[k].used then begin
			result := k;
			exit;
		end;
		k := edges[k].next;
	end;
	result := 0;
	assert(false);
end;

var
	t, i, j, k, q, npaths, pnum : integer;
    plen : array[1..MAX_P] of integer;

begin
//	reset(input, 'z.pre');
	fillchar(first, sizeof(first), 0);
	fillchar(edges, sizeof(edges), 0);

	read(n, t);

	for i := 1 to t do begin
		read(x, y);
		AddEdge(x, y);
	end;

	npaths := 0;
	Cleanup;

	for i := 1 to n do
		while deg[i] > 0 do begin
			inc(npaths);
            plen[npaths] := 0;
			j := i;
			repeat
                inc(plen[npaths]);
                if plen[npaths] = 1000 then
                	break;
                if deg[j] = 0 then
                	break;
				k := GetUnusedEdge(j);
				UseEdge(k);
				j := edges[k].head;
			until false;
		end;

	writeln(n, ' ', npaths);

    Cleanup;

    pnum := 0;
	for i := 1 to n do
		while deg[i] > 0 do begin
			inc(pnum);
            write(plen[pnum]);
			j := i;
			for q := 1 to plen[pnum] do begin
        		write(' ', j);
                if q < plen[pnum] then begin
                    k := GetUnusedEdge(j);
                    UseEdge(k);
                    j := edges[k].head;
                end;    
			end;
        	writeln;
		end;
end.
