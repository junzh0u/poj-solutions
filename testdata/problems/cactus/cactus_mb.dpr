{$R+,Q+,I+}
{$apptype CONSOLE}
{$assertions on}

uses
    sysutils;

const
	IN_FILE = 'cactus.in';
	OUT_FILE = 'cactus.out';
	
	MAX_N = 20000;
	MAX_M = 4 * MAX_N;
	MAX_H = 10000;
	
type
	TEdge = record
		head, tail : integer;
		next : integer;
		onCycle : boolean;
	end;
	
	TColor = (_white, _grey, _black);
			
var
	n, m : integer;
	first : array[1..MAX_N] of integer;
	edges : array[1..MAX_M] of TEdge;
	col : array[1..MAX_N] of TColor;
	parentEdge : array[1..MAX_N] of integer;

	answer : array[1..MAX_H] of integer;
	ansLen : integer;

procedure WriteYes;
var
	i : integer;
	
begin
	rewrite(output, OUT_FILE);
	write(answer[ansLen]);
	i := ansLen - 1;
	while i >= 1 do begin
		write((answer[i] div 1000) mod 10, (answer[i] div 100) mod 10, (answer[i] div 10) mod 10, answer[i] mod 10);
        dec(i);
	end;
	close(output);
	halt;
end;

procedure Multiply(x : integer);
var
	i, carry, y : integer;

begin
	carry := 0;
	i := 0;
	repeat
		inc(i);
		y := answer[i] * x + carry;
		answer[i] := y mod 10000;
		carry := y div 10000;
	until (i > ansLen) and (carry = 0);
	ansLen := i;
	if answer[ansLen] = 0 then
		dec(ansLen);
end;

procedure WriteNo;
begin
	rewrite(output, OUT_FILE);
	writeln(0);	
	close(output);
	halt;
end;

procedure SetCycle(i : integer);
begin
	if edges[i].onCycle then
		WriteNo;	
	edges[i].onCycle := true;
	if odd(i) then
		edges[i+1].onCycle := true
	else
		edges[i-1].onCycle := true;	
end;

procedure AddEdge(head, tail : integer);
begin
	inc(m);
	if m > 4 * n then
		WriteNo;
	edges[m].head := head;
	edges[m].tail := tail;
	edges[m].next := first[tail];
	edges[m].onCycle := false;
	first[tail] := m;
end;
		
procedure ReadInput;
var
	i, j, k, q, prev, cur : integer;
	
begin
	reset(input, IN_FILE);
	
	read(n);
	assert((n >= 1) and (n <= MAX_N));
	assert(not eoln);
	read(q);
	assert((q >= 0) and (q <= 1000));
	assert(eoln);
	readln;
	
	m := 0;
	fillchar(first, sizeof(first), 0);

	for i := 1 to q do begin
		read(k);
		assert((k >= 2) and (k <= 1000));
		prev := 0;
		for j := 1 to k do begin
			assert(not eoln);
			read(cur);	
			if prev <> 0 then begin
				AddEdge(prev, cur);
				AddEdge(cur, prev);
			end;
			prev := cur;	
		end;
		assert(eoln);
		readln;
	end;	
	
	assert(eof);
	
	close(input);	
end;

procedure DFS(i, p : integer);
var
	j, k, q : integer;
	cycleLen : integer;

begin
	parentEdge[i] := p;
	col[i] := _grey;
	
	j := first[i];
	while j <> 0 do begin
		k := edges[j].head;

        if ((p = 0) or (k <> edges[p].tail)) and (col[k] <> _black) then begin
            if col[k] = _grey then begin
            	SetCycle(j);
                q := i;
                cycleLen := 2;
                repeat
                	if edges[parentEdge[q]].onCycle then
                        WriteNo;
                    SetCycle(parentEdge[q]);
                    q := edges[parentEdge[q]].tail;
                    inc(cycleLen);
                until q = k;
                Multiply(cycleLen);
            end;

            if col[k] = _white then
                DFS(k, j);
		end;

		j := edges[j].next;
	end;
	
	col[i] := _black;
end;

procedure Solve;
var	
	i : integer;
	
begin
	fillchar(answer, sizeof(answer), 0);
	answer[1] := 1;
	ansLen := 1;

	for i := 1 to n do begin
		col[i] := _white;
		parentEdge[i] := 0;
	end;
	
	DFS(1, 0);
	
	for i := 1 to n do
		if col[i] <> _black then
			WriteNo;
			
	WriteYes;
end;

begin
	ReadInput;
	Solve;
end.
