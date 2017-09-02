{$R-,Q-}
{$apptype CONSOLE}
{$assertions on}

const
	IN_FILE = 'exploring.in';
	OUT_FILE = 'exploring.out';
	
	MAX_N = 500;
	MODULO = 1000000000;
	
var
	n : integer;
	s : string;
	answer : array [1..MAX_N, 1..MAX_N] of integer;

function GetAnswer(i, j: integer): integer;
var
	k: integer;
	tmp : int64;
	
begin
	if answer[i][j] = -1 then begin
    	if i = j then begin
    		answer[i][j] := 1
    	end else begin
    		if ((j - i) mod 2 <> 0) or (s[i] <> s[j]) then begin
    			answer[i][j] := 0;
    		end else begin
    			tmp := 0;
    			for k := i + 1 to j do
    				tmp := (tmp + int64(GetAnswer(i + 1, k - 1)) * int64(GetAnswer(k, j))) mod MODULO;
    			answer[i][j] := tmp;
    		end;
    	end;
	end;
	result := answer[i][j];
end;

procedure ReadInput;
var
	i : integer;
	
begin
	reset(input, IN_FILE);
	readln(s);
	assert(eof);
	n := length(s);
	assert((n >= 1) and (n <= MAX_N));
	for i := 1 to n do
		assert(s[i] in ['A'..'Z']);
	close(input);	
end;

procedure WriteOutput;
begin
	rewrite(output, OUT_FILE);
	writeln(GetAnswer(1, n));
	close(output);
end;

procedure Solve;
begin
	fillchar(answer, sizeof(answer), $ff);
end;

begin
	ReadInput;
	Solve;
	WriteOutput;
end.
