{$apptype CONSOLE}
{$assertions ON}

uses Math;

const
	IN_FILE = 'area.in';
	OUT_FILE = 'area.out';
	MAX_N = 100;
	MAX_C = 100;
	MAX_CP = MAX_N * MAX_N div 2;
	INF = 1e20;
    EPS = 1e-15;

type
	real = extended;

	TChimney = record
		x, y : real;
		mark : char;
	end;		
	
	TCheckPoint = record
		x : real;
		i, j : integer;
	end;

	TSegment = record
		lo, hi : real;
	end;
			
var
	n : integer;
	marks : string;
	c : array[1..MAX_N] of TChimney;
	
	ncps : integer;
	cps : array[1..MAX_CP] of TCheckPoint;
	pcps : array[1..MAX_CP] of integer;
	
	nsegs : integer;
	segs : array[1..MAX_CP] of TSegment;
		
procedure ReadInput;
var
	i : integer;
	
begin
	reset(input, IN_FILE);
	read(n);
	assert(eoln);
	readln;
	assert((n >= 1) and (n <= MAX_N));
	readln(marks);
	assert(length(marks) = n);
	for i := 1 to n do begin
		read(c[i].mark);
		assert(not eoln);
		read(c[i].x);
		assert(abs(c[i].x) <= MAX_C);
		assert(not eoln);
		read(c[i].y);
		assert(abs(c[i].y) <= MAX_C);
		assert(eoln);
		readln;
	end;
	assert(eof);
	close(input);	
end;

procedure WriteOutput;
var
	i : integer;
	
begin
	rewrite(output, OUT_FILE);
	writeln(nsegs);
	for i := 1 to nsegs do begin
		if i > 1 then
			write(' ');
		if abs(segs[i].lo) > INF/2 then
			write('*')
		else
			write(segs[i].lo:0:6);
		write(' ');
		if abs(segs[i].hi) > INF/2 then
			write('*')
		else
			write(segs[i].hi:0:6);
	end;
	writeln;
	close(output);
end;

procedure Solve;

	procedure SortChimneys;
    var
        i, j : integer;
        tmp : TChimney;

	begin
		for i := 1 to n do
			for j := i + 1 to n do
				if (c[i].y < c[j].y) or ((c[i].y = c[j].y) and (c[i].x > c[j].x)) then begin
					tmp := c[i];
					c[i] := c[j];
					c[j] := tmp;
				end;	
	end;

	function IsLess(const lhs, rhs : TCheckPoint) : boolean;
	begin
		if lhs.x < rhs.x - EPS then begin
			result := true;
			exit;
		end;	
		if lhs.x > rhs.x + EPS then begin
			result := false;
			exit;
		end;
		if c[lhs.i].y < c[rhs.i].y - EPS then begin
			result := true;
			exit;
		end;	
		if c[lhs.i].y > c[rhs.i].y + EPS then begin
			result := false;
			exit;
		end;	
		if c[lhs.j].y < c[rhs.j].y - EPS then begin
			result := true;
			exit;
		end;	
		if c[lhs.j].y > c[rhs.j].y + EPS then begin
			result := false;
			exit;
		end;	
		result := false;
	end;
	
    procedure QSort(l, r : integer);
    var
        i, j : integer;
        x : TCheckPoint;
        t : integer;

    begin
        i := l;
        j := r;
        x := cps[pcps[l + random(r - l)]];
        while i <= j do begin
            while IsLess(cps[pcps[i]], x)  do
                inc(i);
            while IsLess(x, cps[pcps[j]]) do
                dec(j);
            if i <= j then begin
                t := pcps[i];
                pcps[i] := pcps[j];
                pcps[j] := t;
                inc(i);
                dec(j);
            end;
        end;
        if l < j then
            QSort(l, j);
        if i < r then
            QSort(i, r);
    end;

var
	state : array[1..MAX_N] of integer;
//	istate : array[1..MAX_N] of integer;
	match : integer;

	function CheckState : boolean;
    var
    	i : integer;
    begin
    	result := false;
        for i := 1 to n do
        	if marks[state[i]] <> c[i].mark then
            	exit;
        result := true;
    end;
    
    procedure ApplyCheckpoint(i : integer);
    var
    	u, v, tmp : integer;
    	
    begin
		u := cps[pcps[i]].i;
		v := cps[pcps[i]].j;
		
		if marks[state[u]] = c[u].mark then
			dec(match);
		if marks[state[v]] = c[v].mark then
			dec(match);
			
		tmp := state[u];
		state[u] := state[v];
		state[v] := tmp;
		
		if marks[state[u]] = c[u].mark then
			inc(match);
		if marks[state[v]] = c[v].mark then
			inc(match);
    end;
    
var
	i, j : integer;

begin
	SortChimneys;

	ncps := 0;
	for i := 1 to n do
		for j := i + 1 to n do
			if c[i].y <> c[j].y then begin
				inc(ncps);
				cps[ncps].x := c[j].x - (c[i].x - c[j].x) / (c[i].y - c[j].y) * c[j].y;
				cps[ncps].i := i;
				cps[ncps].j := j;
			end;

	for i := 1 to ncps do
		pcps[i] := i;

    if ncps > 0 then
		QSort(1, ncps);

	for i := 1 to n do
		state[i] := i;

	nsegs := 0;
	
	match := 0;
	for i := 1 to n do
		if marks[state[i]] = c[i].mark then
			inc(match);

	for i := 0 to ncps do begin

		if i > 0 then
			ApplyCheckpoint(i);
{
		for j := 1 to n do istate[state[j]] := j;
		write('match = ', match:6, ' -> ');
		for j := 1 to n do write(c[istate[j]].mark);
		writeln;
}
	
		if (match = n) and ((i = 0) or (i = ncps) or (abs(cps[pcps[i+1]].x - cps[pcps[i]].x) > EPS)) then begin
			inc(nsegs);
			if i = 0 then
				segs[nsegs].lo := -INF
			else
				segs[nsegs].lo := cps[pcps[i]].x;
			if i = ncps then
				segs[nsegs].hi := +INF
			else
				segs[nsegs].hi := cps[pcps[i+1]].x;
		end;
	end;
end;

begin
	ReadInput;
	Solve;
	WriteOutput;
end.
