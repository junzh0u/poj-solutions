{$R+,Q-,O+}
{$apptype CONSOLE}

uses Math;

const
	IN_FILE = 'hardwood.in';
	OUT_FILE = 'hardwood.out';

	MAX_N = 20;
    MAX_C = 1000;
	INFINITY = maxlongint;

type
	TRect = record
    	imin, jmin, imax, jmax : integer;
    end;

    TCharArray = array of char;

var
	m, n : integer;
	w : array[0..MAX_N+1,0..MAX_N+1] of char;
	col : array[1..MAX_N,1..MAX_N] of integer;
    count : array[char] of integer;
    colcount : array[0..MAX_C] of integer;
	answer : integer;

procedure ReadInput;
var
	i, j : integer;

begin
	fillchar(w, sizeof(w), '.');
	reset(input, IN_FILE);
	readln(m, n);
	for i := 1 to m do begin
		for j := 1 to n do
			read(w[i][j]);
		readln;
	end;
	close(input);
end;

procedure WriteOutput;
begin
	rewrite(output, OUT_FILE);
	writeln(answer);
	close(output);
end;

function GetColRect(c : integer) : TRect;
var
	i, j : integer;

begin
    result.imin := INFINITY;
    result.imax := -INFINITY;
    result.jmin := INFINITY;
    result.jmax := -INFINITY;
    for i := 1 to m do
        for j := 1 to n do
            if col[i,j] = c then begin
                result.imin := min(result.imin, i);
                result.jmin := min(result.jmin, j);
                result.imax := max(result.imax, i);
                result.jmax := max(result.jmax, j);
            end;
end;

function MakeRect(imin, jmin, imax, jmax : integer) : TRect;
begin
	result.imin := imin;
    result.imax := imax;
    result.jmin := jmin;
    result.jmax := jmax;
end;

procedure Solve;
var
	i, j : integer;
	queue : array[1..MAX_C] of integer;
	qhead, qtail : integer;
	ncol : integer;

	function Dequeue : integer;
	begin
		result := queue[qhead];
		inc(qhead);
	end;

	procedure Enqueue(value : integer);
	begin
		inc(qtail);
		queue[qtail] := value;
	end;

	function TryCut(const r : TRect; c : integer) : boolean;
    var
    	i, j : integer;
        ch : char;
        nc : integer;
        totalncount : integer;
        ncount : array[char] of integer;

	begin
    	result := false;

        fillchar(ncount, sizeof(ncount), 0);
        totalncount := 0;
        for i := r.imin to r.imax do
        	for j := r.jmin to r.jmax do
            	if col[i,j] = c then begin
                	inc(ncount[w[i,j]]);
                    inc(totalncount);
                end;

        if (totalncount = 0) or (totalncount = colcount[c]) then
        	exit;

        for ch := #0 to #255 do
        	if (ncount[ch] <> 0) and (ncount[ch] <> count[ch]) then
            	exit;

		nc := ncol;
		inc(ncol);

		for i := r.imin to r.imax do
        	for j := r.jmin to r.jmax do
            	if col[i,j] = c then
                	col[i,j] := nc;

        colcount[nc] := totalncount;
        dec(colcount[c], totalncount);

		Enqueue(c);
		Enqueue(nc);
        result := true;
	end;

	procedure Reduce(c : integer);
	var
    	r : TRect;
		i, j : integer;

	begin
        r := GetColRect(c);

		for i := r.imin to r.imax do
			for j := r.jmin to r.jmax do begin
				if TryCut(MakeRect(i, j, r.imax, r.jmax), c) or
				   TryCut(MakeRect(r.imin, j, i, r.jmax), c) or
				   TryCut(MakeRect(r.imin, r.jmin, i, j), c) or
				   TryCut(MakeRect(i, r.jmin, r.imax, j), c) then
                   	exit;
			end;

        inc(answer);
	end;

begin
    fillchar(col, sizeof(col), 0);
    
	fillchar(count, sizeof(count), 0);
    for i := 1 to m do
    	for j := 1 to n do
        	inc(count[w[i,j]]);

    fillchar(colcount, sizeof(colcount), 0);
    colcount[0] := m * n;
    
	queue[1] := 0;
	ncol := 1;
	qhead := 1;
	qtail := 1;
	answer := 0;
	while qtail >= qhead do
		Reduce(Dequeue());
end;

begin
	ReadInput;
	Solve;
	WriteOutput;
end.
