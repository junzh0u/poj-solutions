const
	maxn = 500;
	MODULO = 1000000000;

var
	n: longint;
	s: string;
	cnt: array [1..maxn, 1..maxn] of int64;
	u: array [1..maxn, 1..maxn] of boolean;

function count(i, j: longint): int64;
var
	k: longint;
begin
	if not u[i][j] then begin
    	u[i][j] := true;
    	if i = j then begin
    		cnt[i][j] := 1
    	end else begin
    		if ((j - i) mod 2 <> 0) or (s[i] <> s[j]) then begin
    			cnt[i][j] := 0;
    		end else begin
    			cnt[i][j] := 0;
    			for k := i + 1 to j do begin
					cnt[i][j] := (cnt[i][j] + count(i + 1, k - 1) * count(k, j)) mod MODULO;
    			end;
    		end;
    	end;
	end;
	count := cnt[i][j];
end;

var
    i, j: longint;

begin
	reset(input, 'exploring.in');
	rewrite(output, 'exploring.out');

	readln(s);

	n := length(s);
    for i := 1 to n do
        for j := i to n do
            count(i, j);
	writeln(count(1, n));

	close(input);
	close(output);
end.