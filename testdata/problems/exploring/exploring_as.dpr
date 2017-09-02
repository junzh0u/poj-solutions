uses
	math;

const
	maxn = 500;
	MODULO = 1000000000;

var
	n: longint;
	s: string;
	cnt: array [1..maxn, 1..maxn] of extended;
	u: array [1..maxn, 1..maxn] of boolean;

function md(x, y: extended): extended;
begin
	md := (x - floor(x / y) * y);
end;

function count(i, j: longint): extended;
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
					cnt[i][j] := md(cnt[i][j] + count(i + 1, k - 1) * count(k, j), MODULO);
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
	writeln(count(1, n) :0 :0);

	close(input);
	close(output);
end.